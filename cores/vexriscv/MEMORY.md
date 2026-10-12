# VexRiscv: Memory

## What it uses

All of this is plugin-selected [src].

| Part | Options |
|------|---------|
| Instruction fetch | `IBusSimplePlugin` (no cache, with bridges to AXI4, Avalon, Wishbone) or `IBusCachedPlugin` (instruction cache) |
| Data access | `DBusSimplePlugin` (no cache) or `DBusCachedPlugin` (data cache) |
| Address translation | `MmuPlugin` (page-based), or `StaticMemoryTranslatorPlugin` (fixed regions, no paging) |
| Protection | `PmpPlugin`, `PmpPluginNapot` |
| FPU memory dependency | `FpuPlugin` requires the data cache |
| Bus protocols | AXI4, Avalon, Wishbone [src] |

## Why it is built this way

- **A "simple" and a "cached" plugin for each side** because FPGA designs differ. A design with fast block RAM wants no cache (smaller, deterministic). A design with slow external DRAM wants a cache.
- **Static translator versus MMU** lets you pay for virtual memory only if you need Linux.
- **Plugins** mean the memory system is chosen, not fixed. The core becomes a family.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| PicoRV32 | PicoRV32 has one fixed multi-cycle design. VexRiscv builds a pipelined core with whichever memory plugins you pick |
| SERV | SERV minimizes area at one design point. VexRiscv spans a range of points |
| Rocket, Shakti | They provide a fixed, full memory hierarchy. VexRiscv can match that with the cached and MMU plugins, but also scales down |
| Ibex, CV32E40P | Fixed interface, optional cache (Ibex) or none (CV32E40P). VexRiscv exposes the choice as a plugin list |

## Questions to answer while studying

1. What is the cycle difference for a load with `DBusSimplePlugin` versus `DBusCachedPlugin`?
2. How does `IBusSimplePlugin` bridge to Wishbone versus AXI4?
3. What does the Linux configuration add in memory-system area?

## To verify

- Default cache geometry in the example configs [verify].
