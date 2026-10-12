# CV32E40P: Memory and Security Study

A 4-stage, in-order, 32-bit RISC-V core from OpenHW Group, derived from the PULP project's RI5CY. It is a verified, industrial-style embedded core with optional FPU and DSP-oriented extensions.

Upstream: https://github.com/openhwgroup/cv32e40p, manual: https://docs.openhwfoundation.org/projects/cv32e40p-user-manual/en/latest/index.html

## Key facts

| Item | Value |
|------|-------|
| Pipeline | 4-stage, in-order [src] |
| ISA | RV32I base. Always on: C, M, Zicntr, Zicsr, Zifencei. Optional F (`FPU`), Zfinx (`ZFINX`) [src] |
| Custom extensions | Xcv (`COREV_PULP`) and Xcvelw (`COREV_CLUSTER`): hardware loops, post-increment load/store, SIMD [src] |
| Memory interface | OBI v1.2 for instruction and data [src] |
| Privilege | Machine mode only, U-mode no longer supported [src] |
| PMP | Not supported, removed [src] |
| Lineage | OR10N, then RI5CY (2016), maintained by PULP until February 2020, then OpenHW Group [src] |

## Why it exists

A well-verified, area-efficient core for embedded and DSP-like workloads, with the Xpulp extensions improving tight loops, and OpenHW's verification process making it industrially usable.

## Study goals

1. Learn the OBI protocol and how it differs from AXI and the PicoRV32 interface.
2. Understand why hardware loops and post-increment accesses help memory-bound code.
3. Understand what was *removed* (PMP, U-mode, atomics) and what that means for security.
4. Compare with CV32E40S, the security-oriented sibling [verify].

## Hands-on checklist

- [ ] Clone upstream, run the provided testbench.
- [ ] Trace an OBI instruction fetch and a data load, noting request, grant, and response timing.
- [ ] Enable `COREV_PULP`, compile a loop using hardware loops, compare cycle counts.
- [ ] Run `docs/labs/timing_lab.c`. Compare against Ibex with data-independent timing on.
- [ ] Read the CV32E40S manual and list the security features CV32E40P lacks.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.

## To verify before citing

- Instruction prefetch buffer behavior [verify].
- CV32E40S feature list [verify].
