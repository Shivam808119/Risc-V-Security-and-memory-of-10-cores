# SERV: Security

## Features built in

The SERV datasheet does not describe any security features [src]. There are no privilege modes beyond what the small CSR subset provides, no PMP, no memory protection, no integrity checks, and no side-channel countermeasures.

## Side-channel exposure

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache | None from the core | No cache [src] |
| Speculation (Spectre-style) | None | No speculation and no predictors [analysis] |
| Branch predictor | None | No predictor [analysis] |
| Timing | Low for ALU operations, higher for shifts and anything external | Bit-serial execution takes a number of cycles set by the instruction, not by operand values. Shifts, memory access latency, and any attached MDU can still vary [analysis, verify cycle behavior] |
| Power / EM | Present | One bit per cycle means data-dependent switching is tiny per cycle but not absent. No masking or noise [analysis] |
| Fault injection | Present | No lockstep, ECC, or hardened PC |
| Direct memory access by any code | Everything is readable | No PMP or privilege separation |

## How to protect a SERV-based design

1. **Keep secrets out of the core's reach.** Put key operations in a dedicated peripheral (AES, hash block) with its own isolation, and let SERV only command it.
2. **Add a memory protection layer in the interconnect** since the core has none: an address filter on the data bus that blocks access to secret regions based on a mode signal.
3. **Constant-time software.** Avoid secret-dependent branches and early exits. Use `docs/labs/timing_lab.c` to confirm timing is flat.
4. **If an MDU is attached, choose a fixed-latency one** so multiply/divide do not leak operand size.
5. **For physical attackers:** add a clock/voltage monitor, redundant registers on critical state, and software-level masking. SERV itself will not help you here.

## Experiments

- [ ] Run the timing lab and record whether shifts of different amounts take different cycle counts.
- [ ] Attach two different MDUs (iterative and fixed-latency) and compare multiply timing.
- [ ] Write a short "what an attacker with code execution can read" list for your SoC.

## Compare with others

SERV and PicoRV32 are both "nothing to attack, nothing to stop you." Ibex is the opposite end: see its `SECURITY.md`.
