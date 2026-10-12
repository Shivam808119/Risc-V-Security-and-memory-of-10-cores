# Rocket: Memory

## What it uses

| Part | Design |
|------|--------|
| L1 caches | Instruction and data cache inside each RocketTile [src]. Size and associativity configurable, default values [verify] |
| Address translation | Page-table walker in the tile [src], TLB, Sv39 virtual memory [verify] |
| Local memory | Scratchpad option instead of cache [verify] |
| System fabric | Tiles connect to the `SystemBus`, which reaches L2 cache banks. L2 connects to the `MemoryBus` and then to the DRAM controller through a TileLink-to-AXI converter [src] |
| Peripherals | `PeripheryBus`, `ControlBus` for MMIO, `FrontendBus` for DMA devices [src] |
| Accelerators | RoCC coprocessor interface [src] |
| Protocol | TileLink inside the SoC, converters to AXI4, AHB-lite, APB at the edges [src] |

## Why it is built this way

- **Caches and a TLB** because Rocket targets Linux-class software running from DRAM, where latency would otherwise dominate.
- **TileLink** gives a coherent, cache-aware, multi-master protocol designed for the Rocket/BOOM ecosystem, with the ability to bridge to industry buses at the boundary.
- **Diplomacy** lets components negotiate parameters automatically so adding a cache level or master does not mean hand-wiring the bus.
- **RoCC** gives accelerators a direct, low-overhead path to the core.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| SERV, PicoRV32, CV32E40P, Ibex | These are microcontroller-class with no TLB and mostly no data cache. Rocket has a complete virtual-memory hierarchy |
| SonicBOOM | Same memory system infrastructure, but SonicBOOM issues loads out of order and speculatively, which changes the leak surface |
| VexRiscv | VexRiscv builds optional caches and MMU from plugins for FPGA. Rocket's memory hierarchy is the SoC-scale reference |
| Shakti | Shakti C-class is similar in ambition (Linux-capable, in-order) but built in Bluespec with AXI4 uncore |
| VeeR EH1 | VeeR favors tightly coupled memories for deterministic access. Rocket favors caches and L2 |

## Questions to answer while studying

1. Where does a data cache miss travel through the system (D$, PTW if needed, system bus, L2, memory bus, DRAM)?
2. What does the TileLink-to-AXI converter add in latency?
3. What changes if you replace the D$ with a scratchpad?

## To verify

- Default cache geometry and TLB sizes in the config you build [verify].
