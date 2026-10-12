/*
 * timing_lab.c : measure whether timing depends on secret data.
 *
 * Runs on any RISC-V core that provides the cycle counter (rdcycle).
 * Build bare-metal with your core's toolchain, e.g. riscv32-unknown-elf-gcc.
 * Read the results from the `results` array with a debugger, or print them
 * through your platform's UART (replace record() with your own output).
 *
 * Experiments:
 *   1. Early-exit compare vs constant-time compare, mismatch at first vs last byte.
 *   2. Multiply by zero vs multiply by a large value.
 *   3. Divide by a small vs large divisor.
 *   4. Taken vs not-taken branch.
 *
 * What to look for: if two runs that differ only in secret-like data take a
 * different number of cycles, that code path leaks through timing on this core.
 * On a core with a data-independent-timing mode, turn it on and repeat.
 */
#include <stdint.h>

#define N 16
#define REPS 64

static inline uint32_t cycles(void)
{
    uint32_t c;
    __asm__ volatile("rdcycle %0" : "=r"(c));
    return c;
}

volatile uint32_t results[8];   /* inspect with a debugger or print it */

static void record(int slot, uint32_t value) { results[slot] = value; }

/* Leaky: returns as soon as a byte differs. */
static int leaky_compare(const uint8_t *a, const uint8_t *b, int n)
{
    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

/* Constant time: always touches every byte, no data-dependent branch. */
static int ct_compare(const uint8_t *a, const uint8_t *b, int n)
{
    uint8_t diff = 0;
    for (int i = 0; i < n; i++)
        diff |= (uint8_t)(a[i] ^ b[i]);
    return diff == 0;
}

/* Average cycles over REPS runs. */
#define MEASURE(slot, expr)                         \
    do {                                            \
        uint32_t start = cycles();                  \
        for (int r = 0; r < REPS; r++) { expr; }    \
        record(slot, (cycles() - start) / REPS);    \
    } while (0)

int main(void)
{
    uint8_t secret[N], guess_first_wrong[N], guess_last_wrong[N];
    volatile int sink = 0;

    for (int i = 0; i < N; i++) {
        secret[i] = (uint8_t)(i * 7 + 3);
        guess_first_wrong[i] = secret[i];
        guess_last_wrong[i]  = secret[i];
    }
    guess_first_wrong[0]     ^= 0xFF;   /* mismatch at first byte */
    guess_last_wrong[N - 1]  ^= 0xFF;   /* mismatch at last byte  */

    /* 1. Compare timing. Leaky versions differ, constant-time versions should not. */
    MEASURE(0, sink += leaky_compare(secret, guess_first_wrong, N));
    MEASURE(1, sink += leaky_compare(secret, guess_last_wrong,  N));
    MEASURE(2, sink += ct_compare(secret, guess_first_wrong, N));
    MEASURE(3, sink += ct_compare(secret, guess_last_wrong,  N));

    /* 2. Multiply: does a zero operand finish early? */
    volatile uint32_t zero = 0, big = 0x9E3779B9u, x = 12345u;
    MEASURE(4, sink += (int)(x * zero));
    MEASURE(5, sink += (int)(x * big));

    /* 3. Divide: does a small divisor finish early? */
    volatile uint32_t d_small = 3, d_big = 0x7FFFFFFFu, n = 0x12345678u;
    MEASURE(6, sink += (int)(n / d_small));
    MEASURE(7, sink += (int)(n / d_big));

    (void)sink;
    /* Compare results[0] vs [1], [2] vs [3], [4] vs [5], [6] vs [7]. */
    for (;;) { }
}
