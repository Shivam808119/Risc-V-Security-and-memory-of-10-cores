# Ibex: Memory and Security Study

A small, production-quality 32-bit RISC-V core from lowRISC, the CPU inside the OpenTitan root of trust. It is the best-documented security core of the ten.

Upstream: https://github.com/lowRISC/ibex, docs: https://ibex-core.readthedocs.io

## Key facts

| Item | Value |
|------|-------|
| ISA | RV32IMC with optional B and others [verify] |
| Pipeline | 2 stages, with an optional third writeback stage [verify] |
| Security parameter | `SecureIbex` enables a bundle of countermeasures [src] |
| Alerts | `alert_major_internal_o`, `alert_major_bus_o`, `alert_minor_o` [src] |
| Runtime controls | `cpuctrl` CSR bits such as `data_ind_timing`, `dummy_instr_en`, `icache_enable` [src] |
| Used in | OpenTitan [src: OpenTitan interface docs in search results] |

## Why it exists

Ibex began as a small efficient core (Zero-riscy lineage) [verify] and was extended to be a trustworthy component for secure silicon. Its documentation explains every hardware countermeasure and how to turn it on, which makes it ideal for studying side-channel defenses.

## Study goals

1. Learn each countermeasure and what threat it targets.
2. Understand the memory interfaces, optional ICache, and bus integrity checks.
3. See how a runtime switch (data-independent timing) trades speed for safety.
4. Understand the limits the Ibex docs themselves state.

## Hands-on checklist

- [ ] Clone upstream, build the simple system, run a test.
- [ ] Read the Security Features page end to end and make a table: feature, threat, parameter, cost.
- [ ] Run `docs/labs/timing_lab.c` with `data_ind_timing` off and then on, and compare.
- [ ] Enable `dummy_instr_en`, vary `dummy_instr_mask`, and plot cycle count variation across runs.
- [ ] Enable and disable `icache_enable` and compare timing variation.
- [ ] Study the lockstep configuration and list what is and is not duplicated.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.

## To verify before citing

- Pipeline stage options and ICache default size and associativity [verify].
- PMP/ePMP details, since the security page only links to a separate page [src].
