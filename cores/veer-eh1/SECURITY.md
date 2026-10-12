# VeeR EH1: Security

## Features built in

The repository page I could read has a security-policy link but no description of security features, and mentions no PMP or side-channel countermeasures [src]. From general knowledge [verify]:

- ECC on ICCM and DCCM, and ECC or parity on the instruction cache.
- Machine mode only in EH1. A later sibling (EL2) added PMP and user mode [verify].

No data-independent timing mode, dummy instruction insertion, or lockstep are documented for EH1.

## Side-channel exposure

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache | Low if code and data live in ICCM/DCCM | Tightly coupled memories have fixed latency and no replacement state, so there is no cache to probe [analysis]. Exposed if the instruction cache is enabled and shared with secret-dependent code paths |
| Speculation | Limited | Superscalar in-order with a branch predictor [verify]. The speculation window is short and loads do not execute past unresolved branches in a classic in-order design [analysis, verify] |
| Timing | Present | No hardening of multiply/divide or branch timing is documented |
| Power / EM | Present | No countermeasures documented |
| Faults | Partial | ECC on memories [verify]. No lockstep or hardened PC |
| Direct memory reads | Total | No PMP or privilege separation in EH1 [verify] |

## How to protect a VeeR EH1 design

1. **Keep secret-handling code and data in ICCM/DCCM,** and disable the instruction cache for them if it is present. This gives deterministic access time.
2. **Constant-time software**, and verify with the lab.
3. **Add system-level isolation** (a bus-side address filter) because the core has no PMP.
4. **Add redundancy for physical attackers** at SoC level, since the core has none.
5. **Consider EL2 or another core with PMP** if you need privilege separation [verify].

## Experiments

- [ ] Run the timing lab with code and data in ICCM/DCCM versus in external memory through the cache.
- [ ] Measure whether the instruction-cache hit/miss difference is visible in cycle counts.
- [ ] Inject a single-bit error in simulation into DCCM and observe the ECC response.

## Compare with others

VeeR's approach to timing predictability is architectural (tightly coupled memory), whereas Ibex's is a runtime mode (data-independent timing). Both are valid ways to reduce the timing channel. Combining them is a good design exercise.
