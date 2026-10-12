# SERV: Memory and Security Study

The smallest RISC-V core: a bit-serial CPU that processes one bit per clock cycle.

Upstream: https://github.com/olofk/serv, docs: https://serv.readthedocs.io

## Key facts

| Item | Value |
|------|-------|
| ISA | RV32I with Zifencei. Optional C, M interface, Zicsr [src] |
| Datapath | Bit-serial, one bit per clock [src] |
| Smallest area | 125 LUT on Artix-7, 198 LUT on iCE40, 2.1 kGE in CMOS, excluding register file [src] |
| CSR / interrupts | `WITH_CSR` enables Zicsr, timer interrupt, exceptions. Only a small CSR subset [src] |
| M extension | `MDU` interface only. The multiply/divide unit must be attached externally [src] |
| Reset | `RESET_STRATEGY` MINI by default, NONE removes reset logic [src] |
| License | ISC [src] |

## Why it exists

SERV answers one question: how small can a standards-compliant CPU be? Doing one bit per cycle means a 1-bit ALU, tiny datapath wiring, and a narrow register file. It is used where area and power matter more than speed, for example many cores on one FPGA or tiny control tasks in an ASIC.

## Study goals

1. Understand why bit-serial arithmetic works (carry kept in a flip-flop between cycles).
2. Understand how the register file shrinks to a narrow RAM.
3. See what happens to memory access when everything is serial.
4. Identify what leaks (and what cannot) in a core with no cache and no speculation.

## Hands-on checklist

- [ ] Clone the upstream repo and follow its getting-started flow (FuseSoC based).
- [ ] Simulate the provided hello-world on Verilator.
- [ ] Read `serv_top`, `serv_rf_top`, `serv_rf_ram`, `serv_rf_ram_if` and sketch the data flow.
- [ ] Change `RF_WIDTH` between 2 and 32 and record cycle count and area for each.
- [ ] Run `docs/labs/timing_lab.c`. Compare the multiply/divide numbers once you attach an MDU.
- [ ] Write one page: "What is the cheapest thing SERV gives up compared with PicoRV32?"

## Files in this repo

- `MEMORY.md`: bus, register file, why it is built this way, how it differs.
- `SECURITY.md`: what protection exists (none built in), what leaks, how to harden.
- `docs/side-channel-guide.md`: shared background on side channels.
- `docs/labs/timing_lab.c`: cycle-counter experiment.

## To verify before citing

- Which bus protocol the instruction and data buses follow in your integration (the datasheet describes signals, the SoC wrapper uses Wishbone-style handshakes) [verify].
- Exact cycle counts per instruction for the version you use [verify].
