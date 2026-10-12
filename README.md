# RISC-V Core Study: Memory and Security of 10 Open Cores

A study kit for ten open RISC-V cores. For each core it answers four questions:

1. Which memory system does it use?
2. Why was it built that way?
3. How is it different from the other nine?
4. Which security features does it have, and how exposed is it to side channels?

It also explains how side-channel attacks work and how to protect a core of your own.

## The 10 cores

| # | Folder | Core | Class | One-line memory story | One-line security story |
|---|--------|------|-------|-----------------------|-------------------------|
| 1 | `cores/serv` | SERV | Bit-serial RV32I, smallest possible | No cache, narrow RAM-based register file | None built in |
| 2 | `cores/picorv32` | PicoRV32 | Size-optimized RV32IMC, multi-cycle | Simple valid/ready bus, no cache | None built in, no privilege model |
| 3 | `cores/ibex` | Ibex | 2-stage RV32IMC(B), used in OpenTitan | Optional ICache, bus ECC option | Most security features of the ten (SecureIbex) |
| 4 | `cores/vega` | VEGA (C-DAC India) | Family: RV32 to quad-core RV64 OoO | 8 KB to 32 KB L1, L2 on multi-core parts | Not documented in public sources |
| 5 | `cores/cv32e40p` | CV32E40P | 4-stage RV32IMFC (ex-RI5CY) | OBI bus, no cache | Almost none: M-mode only, no PMP |
| 6 | `cores/rocket` | Rocket | In-order RV64GC application core | L1 I/D, TLB, TileLink, L2 | Privilege modes and PMP, no side-channel defenses |
| 7 | `cores/veer-eh1` | VeeR EH1 (ex-SweRV) | Superscalar in-order RV32 | ICCM/DCCM tightly coupled memories | ECC on memories, no side-channel defenses documented |
| 8 | `cores/vexriscv` | VexRiscv | Plugin-based RV32, FPGA friendly | Optional I$/D$ plugins, many bus bridges | PMP and MMU plugins |
| 9 | `cores/sonicboom` | SonicBOOM | Out-of-order superscalar RV64GC | Speculative loads, LAQ/SAQ/SDQ | Spectre-v1 reproduced on BOOM, research mitigations |
| 10 | `cores/shakti` | Shakti | Family from IIT Madras | Configurable caches, TIM, AXI4 | T-class tagged memory, F-class fault tolerance |

## Folder layout

```
riscv-core-study/
  README.md                      <- you are here
  docs/
    side-channel-guide.md        <- how attacks work, how to defend, RISC-V specifics
    comparison.md                <- memory and security tables for all 10 cores
    labs/timing_lab.c            <- small cycle-counter experiment you can run on any core
  cores/<core>/
    README.md                    <- overview, key facts, hands-on checklist
    MEMORY.md                    <- which memory, why, how it differs
    SECURITY.md                  <- features present, side-channel exposure, hardening ideas
  scripts/create_repos.sh        <- turns each cores/<core> into its own GitHub repo
```

## Publishing as 10 separate repos

Each folder under `cores/` is self-contained. The script copies the shared guide and lab into each one and pushes it as its own repository (needs the GitHub CLI, logged in with `gh auth login`):

```bash
cd riscv-core-study
chmod +x scripts/create_repos.sh
./scripts/create_repos.sh YOUR_GITHUB_USERNAME public
```

This creates `riscv-serv-study`, `riscv-picorv32-study`, and so on.

## Confidence tags

Facts in these notes carry a tag:

- **[src]**: checked against the project's own page or a paper while preparing this kit.
- **[verify]**: from general knowledge or reasoning. Confirm against the core's manual before you cite it.
- **[analysis]**: a conclusion drawn from the architecture, not something the project documents.

Several details could not be confirmed (VeeR EH1 pipeline and cache specifics, Rocket cache sizes, SonicBOOM predictor details, VEGA security). They are tagged [verify] and each core's README lists what to look up.

## Suggested study order

Go from simplest to most complex, so each core adds one new idea:

1. SERV: what is the minimum a CPU needs?
2. PicoRV32: multi-cycle design, the native bus, coprocessor interface.
3. VexRiscv: how a plugin architecture builds many CPUs from one codebase.
4. CV32E40P: a classic 4-stage pipeline with a clean memory interface.
5. Ibex: caches, bus integrity, and the first real security features.
6. VeeR EH1: tightly coupled memories and superscalar issue.
7. Rocket: caches, TLB, PMP, TileLink, a Linux-capable design.
8. VEGA: how one national program spans microcontroller to out-of-order.
9. Shakti: a family that also explores tagged memory and fault tolerance.
10. SonicBOOM: out-of-order execution and why speculation leaks.

Read `docs/side-channel-guide.md` before core 5 and again after core 10.
