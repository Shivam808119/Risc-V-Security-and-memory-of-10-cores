# VexRiscv: Memory and Security Study

An FPGA-friendly 32-bit RISC-V CPU written in SpinalHDL, built almost entirely from plugins. The same codebase produces anything from a 500-LUT microcontroller to a Linux-capable core.

Upstream: https://github.com/SpinalHDL/VexRiscv (MIT license) [src]

## Key facts

| Item | Value |
|------|-------|
| ISA | RV32I with optional M, A, F/D, C [src] |
| Pipeline | 2 to 5+ stages: fetch, decode, execute, memory, writeback, with optional bypass or interlock per stage [src] |
| Architecture | Almost everything is a plugin, including PC manager, register file, hazard logic [src] |
| Buses | AXI4, Avalon, Wishbone ready, with bridges in `IBusSimplePlugin` [src] |
| Software | Linux compatible (in simulation, no hardware SoC listed on the page), Zephyr, FreeRTOS port [src] |
| Sample sizes | Small RV32I: about 504 LUTs, 243 MHz, 0.52 DMIPS/MHz. Full RV32IM: 1418 LUTs, 216 MHz, 1.21 DMIPS/MHz. Linux-balanced RV32IMA: 2883 LUTs, 180 MHz on Artix-7 [src] |

## Why it exists

To let an FPGA designer choose exactly the features and pipeline depth they need, and to explore CPU design as software: plugins that insert stages, hazards, and instructions.

## Plugin families (from the project)

- Fetch: `IBusSimplePlugin`, `IBusCachedPlugin`
- Data: `DBusSimplePlugin`, `DBusCachedPlugin`
- Execute: `IntAluPlugin`, `LightShifterPlugin`, `FullBarrelShifterPlugin`, `BranchPlugin`
- Multiply/divide: `MulPlugin`, `DivPlugin`, `MulDivIterativePlugin`
- System: `CsrPlugin`, `MmuPlugin`, `StaticMemoryTranslatorPlugin`, `PmpPlugin`, `PmpPluginNapot`
- Floating point and crypto: `FpuPlugin` (requires data cache), `AesPlugin`
- Debug: `DebugPlugin`, `EmbeddedRiscvJtag` [src]

## Study goals

1. Understand how plugins add pipeline stages and share services.
2. See how cached versus simple bus plugins change memory behavior.
3. Understand the MMU and PMP plugins as optional security building blocks.
4. Compare configurations of the same core on size and leak surface.

## Hands-on checklist

- [ ] Clone upstream, generate Verilog for the smallest and the Linux-capable configuration.
- [ ] Compare the two with respect to bus, cache, MMU, PMP.
- [ ] Run `docs/labs/timing_lab.c` on both configs, with and without `MulPlugin` vs `MulDivIterativePlugin`.
- [ ] Add `PmpPlugin` to a config and write a test that traps on a protected region.
- [ ] Check what `AesPlugin` provides and how its timing behaves.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.
