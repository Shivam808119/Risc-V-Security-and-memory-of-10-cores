# VEGA: Security

## Features built in

The public sources found while preparing this kit (a family overview article) do not describe security features for any VEGA variant [src]. That does not mean none exist. It means they are not documented where I could read them. Check the C-DAC datasheets for: PMP, privilege modes, ECC on caches, secure boot, and any side-channel hardening.

## Side-channel exposure (architecture-class reasoning)

None of this is a statement about VEGA documentation. It is what the *class* of microarchitecture implies [analysis].

| Variant class | Likely exposure |
|---------------|-----------------|
| ET1031 (3-stage in-order, RV32IM) | Timing from variable-latency multiply/divide if implemented iteratively. Little or no speculation |
| AS1061 (6-stage in-order, 8 KB + 8 KB L1) | Cache timing channels (Prime+Probe, Flush+Reload where memory is shared). Predictor effects if a branch predictor exists |
| AS1161, AS2161, AS4161 (16-stage out-of-order, L1 + L2) | Same as above, plus the full speculative execution family. A study of commercial out-of-order RISC-V silicon found Spectre-class attacks work there [src: CISPA publication page, covers SiFive P550 and T-Head C910/C920, not VEGA] |

## How to protect a VEGA-based system

1. **Get the facts first.** Request the security section of each datasheet and the list of CSRs controlling any mitigations.
2. **On the in-order parts:** use constant-time code, partition or flush caches between domains, and avoid shared memory between trust domains.
3. **On the out-of-order parts:** add speculation barriers after bounds checks where the toolchain supports them, flush predictors and caches on privilege switches, and follow the mitigations used for other RISC-V out-of-order parts, including those proposed in the CISPA work.
4. **Isolation:** use whatever PMP/MMU support the variant provides to separate secrets.
5. **Test it:** run `docs/labs/timing_lab.c`, then read the shared guide for cache and speculation test methods.

## Experiments

- [ ] Run the timing lab on whichever VEGA board or simulator you can get.
- [ ] Build a table of documented versus undocumented security features per variant.
- [ ] If you get RTL access, check whether predictors are shared across privilege levels.

## Compare with others

VEGA is the family with the *least public security documentation* of the ten. Ibex is the opposite.
