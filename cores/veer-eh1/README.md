# VeeR EH1 (formerly SweRV EH1): Memory and Security Study

A 32-bit, in-order, superscalar RISC-V core released by Western Digital and now hosted by CHIPS Alliance as "Cores-VeeR-EH1". It targets embedded storage controllers and similar deterministic workloads.

Upstream: https://github.com/chipsalliance/Cores-VeeR-EH1

> Confidence note: the repo landing page I could read confirms the configurable ICCM/DCCM, AXI4/AHB-Lite support, debug, and the PIC, but not pipeline depth or cache details. Those items below are tagged [verify]. Read `docs/RISC-V_VeeR_EH1_PRM.pdf` in the repo to confirm them.

## Key facts

| Item | Value |
|------|-------|
| ISA | RV32IMC [verify] |
| Pipeline | 9-stage, 2-way superscalar, in-order, single-threaded [verify] |
| Local memory | ICCM and DCCM, enabled and sized via `veer.config` (example: `-dccm_size=64`, `-set=iccm_enable`) [src] |
| Buses | AXI4 (default in simulation) or AHB-Lite (`default_ahb`) [src] |
| Other blocks | Programmable interrupt controller with generated memory map, debug module and DMI [src] |
| Config tool | `veer.config` generates the RTL to match chosen options [src] |
| Family | EH2 adds a second thread, EL2 is a smaller sibling [verify] |

## Why it exists

Storage controllers and other embedded SoCs want a core with predictable performance and fast, deterministic access to local memory. Tightly coupled memories give both.

## Study goals

1. Understand ICCM/DCCM and why they replace caches in some designs.
2. Understand how a configuration tool shapes RTL.
3. Understand 2-way in-order issue and how it differs from out-of-order.
4. Compare the memory approach with Ibex's cache-with-ECC and Rocket's full hierarchy.

## Hands-on checklist

- [ ] Clone upstream, run `veer.config` with default settings and look at what it generates.
- [ ] Rebuild with and without ICCM and DCCM, compare a benchmark.
- [ ] Switch between AXI4 and AHB-Lite builds and compare the interface wrappers.
- [ ] Run `docs/labs/timing_lab.c` and see whether timing is flat with code and data in ICCM/DCCM.
- [ ] Read the PRM and fill in every [verify] cell.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.
