# Shakti: Memory and Security Study

Shakti is the family of open-source RISC-V processors from IIT Madras (India). Its cores are written in Bluespec SystemVerilog and range from an IoT microcontroller to proposed server-class designs, plus research variants for security and fault tolerance.

Project site: https://shakti.org.in, overview: https://en.wikipedia.org/wiki/SHAKTI_(microprocessor)

## Processor classes

| Class | Target | Notes |
|-------|--------|-------|
| E | Low-power microcontroller, IoT, motor control | 3-stage in-order, under 200 MHz on silicon, runs FreeRTOS, Zephyr, eChronos [src] |
| C | Mid-range embedded and Linux-class | In-order, MMU, runs Linux and seL4, 200 to 800 MHz. Pipeline is described as 6-stage on one page and 5-stage on others [src] |
| I | Compute, mobile, storage, networking | Out-of-order, multithreaded, aggressive branch prediction, 1.5 to 2.5 GHz [src] |
| M | Multi-core | Up to 8 cores mixing C and I [src] |
| S | Desktop and enterprise server | Superscalar, multithreaded, 2 to 16 cores [src] |
| H | HPC and analytics | Up to 128 cores with accelerators [src] |
| T | Security | Hardware support against memory-based attacks [src] |
| F | Fault tolerance | DMR/TMR, lockstep, ECC, redundant buses [src] |

Silicon and boards [src]: **RIMO** and **Risecreek** (C-class, 64-bit, RV64IMAFD, privilege spec v1.10 with Sv39, branch predictor with RAS, 256 KB tightly coupled memory), **E-arty35T** (32-bit, 128 KB RAM), **C-arty100T** (64-bit, 128 MB DDR3).

C-class benchmark data from an IIT Madras showcase deck [src]: 5-stage in-order, 1.72 DMIPS/MHz, boots Linux, RV32 about 4.5K LUTs and RV64 about 6.5K LUTs on Artix-7.

## Why it exists

India's open, indigenous processor program, designed to cover embedded to server use cases and to give researchers a platform to explore security and reliability.

## Study goals

1. Understand a Bluespec-based design and its configurability (caches, TLB, PMP, branch predictor sizes).
2. Compare E-class and C-class memory systems.
3. Understand what T-class (tagged memory) and F-class (fault tolerance) add.
4. See how Shakti treats side-channel resistance: it is listed as a research area, not a shipped feature [src].

## Hands-on checklist

- [ ] Clone the Shakti repositories from the project site (C-class, E-class).
- [ ] Build C-class on an FPGA or simulate. Change cache sets and ways, branch predictor sizes.
- [ ] Run `docs/labs/timing_lab.c` on E-class and C-class and compare.
- [ ] Read the Shakti-T material and sketch how a tag travels with data through the pipeline.
- [ ] Read about F-class lockstep and compare with Ibex lockstep.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.

## To verify before citing

- Default cache geometry, MMU, PMP details for each class on the release you use [verify].
- Which classes have public RTL. Several (I, M, S, H) are described as roadmap [verify].
