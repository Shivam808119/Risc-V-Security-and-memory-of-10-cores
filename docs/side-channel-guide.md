# Side-Channel Security for RISC-V Cores

## 1. What a side channel is

A side channel leaks a secret through something the program was never meant to reveal: how long it ran, which cache lines it touched, how much power it drew. The attacker never breaks the cipher or the access rules. They measure a physical or microarchitectural effect that depends on the secret.

Three ingredients are always present:

1. **A secret-dependent behavior.** A branch taken only for some key bits, a table lookup indexed by a secret, a multiplier that finishes early for small operands.
2. **A shared or observable resource.** A cache, a predictor, a bus, a power rail.
3. **A way to measure it.** A cycle counter, a probe, an oscilloscope.

## 2. The main families and how each works

| Family | What leaks | How the attack works | Needs physical access? |
|--------|-----------|----------------------|------------------------|
| Timing | Execution time | Secret-dependent branches, early-out multiply/divide, variable-latency shifts. Attacker times many runs and correlates. | No |
| Cache | Which cache lines were used | Prime+Probe: fill a cache set, let the victim run, time which lines were evicted. Flush+Reload: flush a shared line, time whether the victim reloaded it. Secret-indexed lookups (AES S-boxes) leak the index. | No |
| Branch predictor | Branch history | Predictor state (BTB, pattern history) is shared. Attacker trains or probes it to learn victim branch outcomes, or to steer victim speculation. | No |
| Speculative execution | Data read on a wrong path | Spectre v1: train a predictor so a bounds check is bypassed speculatively, the out-of-bounds load leaves a cache footprint, the attacker reads that footprint. Other variants abuse indirect-branch and return predictors. | No |
| Power / EM | Switching activity | Simple power analysis reads one trace. Differential or correlation power analysis uses many traces and a power model (Hamming weight or distance) to recover key bytes. | Yes |
| Fault injection | Not a leak but an active attack | Voltage or clock glitching, laser, or EM pulses corrupt a computation. Differential fault analysis of the faulty output reveals the key. Also used to skip security checks. | Yes |

Note that speculative attacks need an out-of-order or at least speculating core. A core that never executes instructions past an unresolved branch has no wrong-path loads to leak.

## 3. Defenses, by layer

### Hardware (the core designer's job)

| Threat | Mitigation | How it works | Typical cost |
|--------|-----------|--------------|--------------|
| Timing | **Data-independent timing (DIT)** | Branches take the same time taken or not, remove early-out for multiply/divide, fix shift latency. Execution time no longer reveals operand values. | Some performance |
| Cache | Cache partitioning (way partitioning, page coloring) | Attacker and victim cannot evict each other's lines. | Capacity, complexity |
| Cache | Randomized index mapping, flushing on context switch | Attacker cannot build a reliable eviction set, or finds nothing left to probe. | Performance on switch |
| Cache | **No cache** or tightly coupled memory with fixed latency | Nothing shared to probe. Memory timing is constant. | Speed |
| Speculation | No speculation past unresolved branches | In-order, no wrong-path loads. | Performance |
| Speculation | **Speculation buffer / InvisiSpec / SafeSpec / taint tracking** | Speculative cache refills are held in a buffer or tracked, and are written to the real cache only when the load is safe. | Area, 2% to 22% performance depending on design [src: CARRV19 paper] |
| Predictors | Flush or domain-tag predictor state on privilege change | Attacker cannot train victim predictions. | Performance |
| Power | **Dummy instruction insertion**, random delays, clock jitter | Misaligns traces and adds noise so averaging gets harder. | Power, performance |
| Power | Masking, dual-rail logic | Splits the secret into random shares or balances switching so power no longer tracks data. | Large area |
| Faults | ECC on memories and register file, parity | Detects or corrects bit flips. | Area |
| Faults | **Dual-core lockstep**, redundancy | Two copies compute the same thing, mismatch raises an alert. | About 2x area |
| Faults | Hardened PC, shadow CSRs, bus integrity checks | Detects control-flow or register corruption. | Small |
| Isolation | PMP / ePMP, privilege modes | Limits which code can read which memory. Does not stop side channels but stops direct reads. | Small |

### Software

- **Constant-time coding.** No branches on secrets, no array indices from secrets, no early-exit comparisons. Use `|=` accumulation instead of `memcmp`.
- **Bit-slicing** for ciphers, so no secret-indexed table lookups.
- **Masking** the secret in software against power analysis.
- **Fences and barriers** after bounds checks, and speculative-load hardening in compilers.
- **Flush caches** when switching between mutually distrusting code.

### System

- Separate privilege domains with PMP or an MMU, keep keys in a small trusted component, and use a hardware root of trust (OpenTitan is the open example, and it uses Ibex).

## 4. RISC-V specifics

- **Zkt** is the RISC-V extension that promises data-independent execution latency for a defined set of instructions (bitwise logic, shifts, rotates, add/sub, multiply, and similar). Software can rely on those instructions not leaking operand values through timing. See the [scalar crypto specification](https://docs.riscv.org/reference/isa/unpriv/scalar-crypto.html).
- **Zkr** provides an entropy source for key generation.
- **PMP and Smepmp (ePMP)** restrict physical memory access, including from machine mode in the ePMP case.
- There is **no standard speculation barrier instruction** in the base ISA. A 2024 study of commercial out-of-order RISC-V silicon (SiFive P550, T-Head C910/C920) found them vulnerable to Spectre-class attacks and had to use instructions that happen to halt speculation as a stopgap [src: [CISPA publication page](https://cispa.de/de/research/publications/213214-spectre-on-risc-v-silicon-attacks-and-defenses-on-commercial-out-of-order-processors)].

## 5. Protecting your own core: a working checklist

1. **Write the threat model first.** Remote software attacker only? Co-resident attacker on the same core? Physical attacker with a probe? The answer decides everything below.
2. **Pick a microarchitecture that leaks less by default.** In-order, no speculation past branches, no shared caches, tightly coupled memory with fixed latency. Simplicity is the cheapest side-channel defense.
3. **Make timing data-independent.** Fix branch latency, remove multiply/divide early-outs, fix shift latency, provide a runtime switch (Ibex does this with a `data_ind_timing` bit).
4. **If you have a cache, decide how to partition or randomize it,** or make it optional and disabled for secret-handling code.
5. **If you speculate, contain it.** Use a speculation buffer or taint tracking, and provide a real fence for software.
6. **Add physical defenses only if physical access is in scope.** Dummy instructions, lockstep, ECC, hardened PC, and bus integrity.
7. **Provide isolation.** PMP regions for secrets, machine/user separation.
8. **Verify, do not assume.** Run constant-time test harnesses, measure with the lab in `labs/timing_lab.c`, use statistical leakage tests (TVLA) on power traces from an FPGA board, and check information flow in RTL.
9. **Document what you did not protect.** Every design leaks something. Say what.

## 6. Where the 10 cores sit on this map

See `comparison.md`, and each core's `SECURITY.md`. In short: SERV and PicoRV32 have no defenses but also very little to attack (no caches, no speculation). Ibex carries the broadest hardware countermeasure set. Rocket, VexRiscv, VeeR, and Shakti sit in the middle with isolation and memory protection. SonicBOOM is the research platform for the speculation problem.
