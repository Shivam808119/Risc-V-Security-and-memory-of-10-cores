# CV32E40P: Security

## Features built in

Very few. The user manual intro states [src]:

- **Machine mode only.** U-mode is no longer supported.
- **No PMP.** It was removed. Most of the old RTL for these removed features was kept but is not maintained.
- **OBI address stability.** Once a memory request is issued it cannot be retracted and the address cannot change.
- Trap handling in direct and vectored modes.

It has no data-independent timing mode, no dummy instruction insertion, no ECC, and no lockstep in the core itself.

## Side-channel exposure

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache | None from the core | No cache [src] |
| Speculation | Low | 4-stage in-order pipeline [src], no deep speculation [analysis] |
| Timing | Present | Multiply and divide latency and branch timing are not hardened. Hardware loops and post-increment access change timing patterns [analysis] |
| Power / EM | Present | No countermeasures |
| Fault injection | Present | No lockstep, ECC, or hardened PC |
| Direct memory reads | Total | No PMP, single privilege level |

## The sibling to look at

OpenHW's **CV32E40S** (documented at https://cv32e40s-user-manual.readthedocs.io) is the security-oriented variant from the same family [src: manual exists]. From general knowledge it adds PMP/ePMP, user mode, and security hardening features such as data-independent timing and dummy instructions [verify]. If you need security, evaluate that core instead of hardening CV32E40P.

## How to protect a CV32E40P design anyway

1. **Move isolation to the system.** Put an address filter or a memory protection unit on the OBI buses.
2. **Constant-time software** with the lab to confirm.
3. **Fixed-latency multiplier/divider** if your build allows a choice.
4. **Redundancy outside the core** for physical attackers: run two cores in lockstep at the SoC level, add glitch detectors, ECC SRAM.
5. **Do not rely on the core to separate trusted and untrusted code.** It cannot.

## Experiments

- [ ] Run the timing lab and record branch and multiply timing.
- [ ] Compare against the Ibex results with `data_ind_timing` on.
- [ ] Try building the core with the FPU on and see which instructions have operand-dependent latency.

## Compare with others

CV32E40P is the "performance and verification first, security later" point. Ibex is the "security built in" point from the same size class.
