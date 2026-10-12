# CV32E40P: Memory

## What it uses

| Part | Design |
|------|--------|
| Interfaces | Separate instruction fetch and data load/store interfaces, both OBI v1.2. Once issued, a request cannot be retracted and the address cannot change [src] |
| Caches | None described in the core [src] |
| Instruction prefetch | A small prefetch buffer in the fetch stage [verify] |
| Register file | Standard integer register file. Optional separate FP registers when `FPU` is on unless `ZFINX` shares the integer file [verify] |
| Memory protection | None. PMP removed [src] |
| Virtual memory | None |
| DSP-oriented access | Post-increment loads/stores and hardware loops via Xcv [src] |

## Why it is built this way

- **OBI** is a lightweight request/grant protocol that is easy to bridge to AXI or AHB and keeps the core interface simple.
- **No cache** because the target systems use tightly coupled SRAM where latency is short and deterministic. Predictable timing is valuable in embedded control.
- **Post-increment accesses and hardware loops** reduce instruction count in loops that stream through memory, which is exactly the memory-bound pattern in signal processing.
- **PMP and U-mode removed** to cut area and verification scope for the intended use.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| Ibex | Similar separate ports. Ibex adds optional ICache, PMP, ECC, and integrity checks. CV32E40P adds DSP-style memory access instructions instead |
| PicoRV32 | One shared multi-cycle interface versus two overlappable ports on a pipeline |
| VeeR EH1 | VeeR uses tightly coupled memories with ECC. CV32E40P expects external TCM with no ECC built in |
| VexRiscv | Configurable plugin cores. CV32E40P is a fixed design with a few parameters |

## Questions to answer while studying

1. How does OBI back-pressure work, and how does the prefetch buffer hide it?
2. How many cycles do hardware loops save on a streaming loop?
3. What does a core without a cache or PMP push onto the system designer?

## To verify

- Prefetch buffer depth [verify].
