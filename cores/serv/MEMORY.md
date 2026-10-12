# SERV: Memory

## What it uses

| Part | Design |
|------|--------|
| Instruction / data access | Separate instruction and data buses. Each has address, cycle, and acknowledge signals. The data bus adds byte select and write enable [src] |
| Caches | None |
| Register file | RAM-based by default (`serv_rf_ram` behind `serv_rf_ram_if`). Width set by `RF_WIDTH` of 2, 4, 8, 16, or 32 bits [src] |
| Alternative layouts | `serv_top` accepts your own register file, such as a shift-register design, or a single combined SRAM holding code, data, and registers [src] |
| Virtual memory / protection | None |

## Why it is built this way

- **Bit-serial needs only a bit or two of register data per cycle.** A 2-bit-wide RAM can feed it, so the register file becomes a small memory instead of 32x32 flip-flops. That is where most of a conventional small core's area goes.
- **No cache** because it would be larger than the CPU itself, and the core is already slow enough that memory latency is hidden by serial execution.
- **One shared SRAM** is possible because the core is so slow that port contention rarely matters. This minimizes total system area.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| PicoRV32 | Both skip caches. PicoRV32 keeps 32 bits wide and spends cycles per instruction. SERV narrows the datapath itself, so its register file can be tiny RAM |
| Ibex, CV32E40P | These expose full-width buses and optional caches for throughput. SERV optimizes area only |
| VexRiscv | VexRiscv can be built small or large by plugins. SERV is fixed at one design point, the smallest |
| Rocket, SonicBOOM, Shakti, VEGA | Application-class cores with TLBs and multi-level caches. SERV has no translation at all |

## Questions to answer while studying

1. How many cycles does a load take versus an add? Why?
2. What does changing `RF_WIDTH` do to cycle count and to LUT/RAM usage?
3. What would you add first if you needed 10x the performance: a cache, a wider datapath, or a faster bus?

## To verify

- Bus protocol naming and exact signal timing for your SoC wrapper [verify].
