# PicoRV32: Memory and Security Study

A size-optimized, multi-cycle RV32IMC core with a simple native memory interface, designed to be used as a small auxiliary processor in FPGA designs.

Upstream: https://github.com/YosysHQ/picorv32

## Key facts

| Item | Value |
|------|-------|
| ISA | RV32IMC, configurable as RV32E, RV32I, RV32IC, RV32IM, RV32IMC [src] |
| Variants | `picorv32` (native), `picorv32_axi` (AXI4-Lite), `picorv32_wb` (Wishbone) [src] |
| Average CPI | About 4. Example: ALU and `jal` 3, load/store 5, taken branch 5, `jalr` 6, shifts 4 to 14 [src] |
| Size | 750 to 2000 LUTs, 250 to 450 MHz on Xilinx 7-series [src] |
| Coprocessor interface | PCPI, for multiply/divide and custom instructions [src] |
| Interrupts | Custom 32-input scheme using `custom0` opcodes, not the RISC-V privileged spec [src] |
| Stated goal | "Optimized for size and fmax, not performance" [src] |

## Why it exists

It targets FPGA designs that need a small controller or sequencer without a full application core. Multi-cycle execution lets the design reuse the ALU and register-file ports, which saves area and gives a high clock rate.

## Study goals

1. Understand the native `valid`/`ready` memory interface and when the look-ahead signals help.
2. Understand PCPI and how multiplier, divider, and custom instructions plug in.
3. Understand the cost of multi-cycle execution (CPI about 4) against area.
4. See how a non-standard interrupt scheme works and why it is simpler.

## Hands-on checklist

- [ ] Clone upstream, run the testbench with Icarus or Verilator.
- [ ] Trace one load and one store at the native interface, noting `mem_valid`, `mem_ready`, `mem_wstrb`.
- [ ] Toggle `ENABLE_MUL`, `ENABLE_FAST_MUL`, `ENABLE_DIV`, `BARREL_SHIFTER` and record area versus cycles.
- [ ] Run `docs/labs/timing_lab.c`. Compare multiply and shift timing across those parameter settings.
- [ ] Wrap the core with the AXI4-Lite and Wishbone variants and compare the signals.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.

## To verify before citing

- Release-specific parameter defaults [verify].
