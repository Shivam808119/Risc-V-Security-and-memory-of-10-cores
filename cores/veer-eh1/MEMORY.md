# VeeR EH1: Memory

## What it uses

| Part | Design |
|------|--------|
| ICCM (instruction closely coupled memory) | Optional, enabled with `-set=iccm_enable` in `veer.config` [src] |
| DCCM (data closely coupled memory) | Optional, size set with e.g. `-dccm_size=64`, can be removed with `-unset=dccm_enable` [src] |
| Instruction cache | Optional instruction cache with ECC or parity protection [verify] |
| External bus | AXI4 or AHB-Lite [src] |
| Interrupt controller | PIC with a memory map generated for the configured size [src] |
| Virtual memory / PMP | None in EH1 [verify] |

## Why it is built this way

- **Tightly coupled memory gives deterministic latency.** A load from DCCM takes a fixed, short time because there is no tag lookup, no miss, no replacement. Embedded real-time and storage firmware care about worst-case timing, not just average [analysis].
- **ECC on local memories** matters because a storage controller must not silently corrupt data [verify].
- **Configurable via `veer.config`** so a customer can drop what they do not need, which is the point of a licensable IP core.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| Ibex | Ibex has an optional ICache with ECC and no tightly coupled memory in the core. VeeR leans on ICCM/DCCM |
| Rocket, SonicBOOM | They use caches and TLBs for DRAM-backed software. VeeR assumes much of the working set sits in local memory |
| CV32E40P | CV32E40P expects external TCM. VeeR integrates ICCM/DCCM into the core configuration |
| Shakti C-class | Shakti offers TIM (tightly integrated memory) as one option among configurable caches. For VeeR it is the main route |
| PicoRV32, SERV | They have no local memory block. Memory is external and unmanaged |

## Questions to answer while studying

1. What is the load-to-use latency from DCCM versus through the AXI bus?
2. Can external masters (DMA) access ICCM/DCCM, and how is that arbitrated?
3. How are ICCM/DCCM errors reported and handled?

## To verify

- Instruction cache presence, size, and protection scheme [verify].
- ICCM/DCCM maximum sizes and ECC type [verify].
