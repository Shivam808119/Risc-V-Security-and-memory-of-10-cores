# VEGA (C-DAC): Memory and Security Study

VEGA is the family of indigenous RISC-V processors from C-DAC, India, under the Digital India RISC-V (DIR-V) program. Unlike the other nine, VEGA is a family spanning a 32-bit microcontroller up to a quad-core out-of-order 64-bit part.

Project site: https://vegaprocessors.in

> Confidence note: the core table below comes from one news article that summarizes the family [src: https://www.cnx-software.com/2022/02/04/india-goes-risc-v-with-vega-processors/]. Numbers may have changed in later releases. Check the C-DAC site and datasheets.

## Variants

| Variant | ISA | Cores | Pipeline | Execution | Caches | Notes |
|---------|-----|-------|----------|-----------|--------|-------|
| ET1031 | RV32IM | 1 | 3-stage | In-order | Not stated | AHB / AXI4, optional MMU and debug. Basis of THEJAS32 SoC |
| AS1061 | RV64IMAFDC | 1 | 6-stage | In-order | 8 KB I + 8 KB D | FPU, AHB / AXI4. Basis of THEJAS64 SoC |
| AS1161 | RV64IMAFDC | 1 | 16-stage | Out-of-order | 32 KB I + 32 KB D | FPU, AHB / AXI4 / ACE |
| AS2161 | RV64IMAFDC | 2 | 16-stage | Out-of-order | 32 KB + 32 KB, 512 KB L2 | FPU, AHB / AXI4 / ACE |
| AS4161 | RV64IMAFD(C) | 4 | 16-stage (listed 13 to 16) | Out-of-order | 32 KB + 32 KB, 1024 KB L2 | AXI4 / ACE, Linux capable, storage and networking [src] |

The article lists AS4161 as RV64IMAFDC in one place and RV64IMAFD in another [src].

## Why it exists

A national program to have a domestically designed processor family, giving India a sovereign option from embedded control (ET1031) to server-class parts (AS4161), plus SoCs such as THEJAS32 and THEJAS64 built on them.

## Study goals

1. See how a single program scales across in-order and out-of-order, 32 and 64 bits.
2. Compare memory subsystem growth across variants (L1 only, L1 plus L2, coherent ACE).
3. Work out the security implications of each variant, since the family spans both "no speculation" and "deep out-of-order."

## Hands-on checklist

- [ ] Visit the VEGA site, collect the latest datasheets for each variant.
- [ ] Check which variants have public RTL or FPGA images (the site lists hardware challenges and boards).
- [ ] Run a bare-metal program on an ET1031/THEJAS32 board or simulator if available, and run `docs/labs/timing_lab.c`.
- [ ] Fill in the "Not stated" cells in the table above from datasheets.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.

## To verify before citing

- Everything about cache organization, associativity, MMU details, and security features. None were documented in the sources found [verify].
- If you meant a different "Vega" core, tell me and I will redo this folder.
