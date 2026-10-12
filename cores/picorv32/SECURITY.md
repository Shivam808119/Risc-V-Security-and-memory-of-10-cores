# PicoRV32: Security

## Features built in

The project page describes no security model [src]. What it does provide:

- Illegal-instruction and misaligned-access trapping, controlled by `CATCH_ILLINSN` and `CATCH_MISALIGN` [src].
- Bus-error interrupt (IRQ 2), and a halt if an illegal instruction or bus error occurs with its interrupt disabled [src].
- Interrupts use a custom scheme rather than the RISC-V privileged spec, and there are no privilege levels, PMP, or MMU [src].

## Side-channel exposure

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache | None from the core | No cache |
| Speculation | None | No speculation or predictor [analysis] |
| Timing | Yes | Multiplier takes 40 cycles, MULH variants 72, divider 40, shifts 4 to 14 cycles depending on settings [src]. Those values come from fixed algorithms, but shift cost depends on shift amount, and the fast multiplier changes the cost entirely, so latency is configuration-dependent and in some settings operand-dependent [analysis] |
| Power / EM | Present | No masking, no noise |
| Fault injection | Present | No integrity checks on PC, registers, or bus |
| Direct memory access | Total | No isolation, any code reads everything |

## How to protect a PicoRV32-based design

1. **Choose the multiplier deliberately.** `ENABLE_FAST_MUL` gives constant single-cycle multiply, which removes operand-dependent multiply timing. Verify with the lab.
2. **Use `BARREL_SHIFTER`** for fixed one-cycle shifts if shift amounts are secret-dependent.
3. **Write constant-time software** and verify with `docs/labs/timing_lab.c`.
4. **Isolate in hardware outside the core.** Add an address-filter on the memory bus so a mode signal controls access to secret regions, since the core has no PMP.
5. **Put crypto in a coprocessor via PCPI or a peripheral** that has constant latency, and keep key material out of CPU registers where possible.
6. **For physical attackers,** add redundancy around the PC and bus, and brown-out and glitch detection.

## Experiments

- [ ] Measure shift latency for amounts 1, 8, 31 with and without `BARREL_SHIFTER`.
- [ ] Measure multiply with `ENABLE_MUL` versus `ENABLE_FAST_MUL`.
- [ ] Write a PCPI divider with fixed latency and compare.

## Compare with others

Same family of exposure as SERV, but with a configurable multiplier and shifter that decide whether timing leaks.
