# VEGA: Memory

## What it uses (by variant)

| Variant | Bus | L1 caches | L2 | Translation |
|---------|-----|-----------|----|-------------|
| ET1031 | AHB / AXI4 | Not stated | None | Optional MMU [src] |
| AS1061 | AHB / AXI4 | 8 KB I + 8 KB D [src] | None | Not stated |
| AS1161 | AHB / AXI4 / ACE | 32 KB I + 32 KB D [src] | None | Not stated |
| AS2161 | AHB / AXI4 / ACE | 32 KB + 32 KB per core [src] | 512 KB [src] | Not stated |
| AS4161 | AXI4 / ACE | 32 KB + 32 KB per core [src] | 1024 KB [src] | Linux capable, so virtual memory [src] |

## Why it is built this way

- **Tiered by market.** The 3-stage ET1031 behaves like a microcontroller core: small, simple memory, AHB/AXI. The 64-bit parts move to caches and, for multi-core, a coherent bus.
- **ACE appears from AS1161 up.** ACE is the AXI coherency extension, which is what you need when several cores share cached data. That fits the dual- and quad-core parts [analysis].
- **L2 appears only in multi-core parts** because shared lower-level cache matters when several cores contend for memory [analysis].

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| Rocket, SonicBOOM | Comparable to the 64-bit VEGA parts: in-order and out-of-order, L1 and L2. VEGA uses AMBA buses (AHB/AXI4/ACE) rather than TileLink |
| Shakti | Another Indian family. Shakti's differentiators are configurability and research variants. VEGA is a productized ladder of fixed parts |
| Ibex, CV32E40P, VeeR | Single-class 32-bit cores. VEGA ET1031 sits in that range, but VEGA also covers 64-bit |
| SERV, PicoRV32 | Minimal-area cores. No VEGA part targets that corner |

## Questions to answer while studying

1. How does the 3-stage ET1031 handle memory compared with CV32E40P?
2. What coherence protocol and cache line size do the ACE parts use?
3. Are caches write-back or write-through, and how are they flushed?

## To verify

- Everything in "Not stated" cells [verify].
