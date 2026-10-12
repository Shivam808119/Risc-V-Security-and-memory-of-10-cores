# Comparison: Memory and Security of the 10 Cores

Tags: **[src]** checked against a project page or paper, **[verify]** from general knowledge, **[analysis]** reasoned from the architecture.

## Memory

| Core | Bus / memory interface | Caches | Local memory | Register file | Address translation | Protection |
|------|------------------------|--------|--------------|---------------|---------------------|------------|
| SERV | Separate instruction and data buses (address, cycle, ack, plus byte select and write enable on data) [src] | None [src: none described] | Can share one SRAM for code, data, and registers [src] | RAM-based, 2 to 32 bits wide (`RF_WIDTH`) [src] | None | None |
| PicoRV32 | Native valid/ready bus, AXI4-Lite and Wishbone wrappers, optional look-ahead signals [src] | None | None | Single-port (small) or dual-port (fast), RV32E option [src] | None | None, custom IRQ scheme [src] |
| Ibex | Separate instruction and data ports (simple request/grant style) [verify] | Optional instruction cache, with ECC option [src: security page] | None | Flip-flop, latch, or FPGA variants [verify] | None [verify] | PMP / ePMP [verify], bus integrity via `MemECC` [src] |
| VEGA | AHB / AXI4, plus ACE on larger parts [src] | None stated (ET1031), 8 KB I + 8 KB D (AS1061), 32 KB + 32 KB (AS1161/2161/4161) [src] | Not stated | Not stated | Optional MMU on ET1031, Linux on AS4161 [src] | Not documented |
| CV32E40P | OBI v1.2 for instruction and data [src] | None described [src] | None | Standard, optional FPU registers [verify] | None | M-mode only, no PMP [src] |
| Rocket | TileLink, with AXI4 / AHB / APB converters [src] | L1 I$ and D$ per tile, L2 banks behind the system bus [src] | Scratchpad option [verify] | Standard | Page-table walker and TLB [src], Sv39 [verify] | PMP [verify] |
| VeeR EH1 | AXI4 or AHB-Lite [src: repo topics] | Instruction cache option [verify] | ICCM and DCCM, size set in `veer.config` [src] | Standard | None [verify] | ECC on memories [verify] |
| VexRiscv | Wishbone, AXI4, Avalon bridges [src] | Optional I-cache and D-cache plugins [src] | None | Plugin | `MmuPlugin`, `StaticMemoryTranslatorPlugin` [src] | `PmpPlugin`, `PmpPluginNapot` [src] |
| SonicBOOM | TileLink as a drop-in for Rocket's tile [src] | L1 I/D [verify], load queue, store address queue, store data queue [src] | Not stated | Unified physical register file, renamed [src] | Page-based virtual memory [src] | PMP [verify] |
| Shakti | AXI4 / AXI4-Lite uncore [src] | C-class: configurable sets, ways, banks, fill buffer, ECC, TIM. I-class: non-blocking caches [src] | Tightly integrated memory (256 KB on RIMO) [src] | Standard | Sv39 on RIMO/Risecreek [src] | Configurable PMP size [src], T-class tagged memory [src] |

## Security features

| Core | Privilege / isolation | Timing defenses | Fault / integrity | Power-analysis defenses | Speculation exposure |
|------|-----------------------|-----------------|-------------------|-------------------------|----------------------|
| SERV | None | None designed. Bit-serial execution is regular [analysis] | None | None | None, no speculation [analysis] |
| PicoRV32 | None | None. Multiply, divide, and shifts are variable cycle [src] | Traps on illegal instruction and bus error [src] | None | None, no speculation [analysis] |
| Ibex | M/U, PMP/ePMP [verify] | Data-independent timing bit [src] | Lockstep, register-file ECC, ICache ECC, bus integrity, hardened PC, shadow CSRs [src] | Dummy instruction insertion [src] | Branch prediction is optional and experimental [src], tiny window [analysis] |
| VEGA | Not documented | Not documented | Not documented | Not documented | OoO variants (16-stage) are in the exposed class [analysis] |
| CV32E40P | M-mode only, no PMP [src] | None | Memory requests cannot be retracted (OBI) [src] | None | In-order, little speculation [analysis]. Use CV32E40S for a security-oriented sibling [verify] |
| Rocket | M/S/U, PMP [verify] | None | None | None | In-order with predictors. Fetch-side predictor effects possible [analysis] |
| VeeR EH1 | M-mode [verify] | None documented | ECC on ICCM/DCCM [verify] | None documented | Superscalar in-order with branch predictor, limited window [analysis] |
| VexRiscv | M/S/U via plugins, MMU, PMP [src] | None | None | None | Depends on configuration, simple configs do not speculate loads [analysis] |
| SonicBOOM | M/S/U, virtual memory [src] | None | None | None | Spectre v1 reproduced on BOOM, speculation-buffer mitigation studied [src: CARRV19] |
| Shakti | S/U and PMP options [src] | Side-channel resistance listed as research area [src] | F-class: DMR/TMR, lockstep, ECC, redundant fabrics [src]. T-class: tagged memory against spatial and temporal memory attacks [src] | Not documented | C-class in-order. I-class is out-of-order [src] |

## Reading the tables

- **Fewer features can mean less to attack.** SERV and PicoRV32 have no cache and no speculation, so whole attack families do not apply. They also have no isolation, so any code that runs can read everything.
- **Isolation is not side-channel defense.** PMP, MMU, and privilege modes stop direct reads. They do nothing about timing and cache leakage between two pieces of code that are both allowed to run.
- **Ibex is the one to study for countermeasures,** because its security page lists each defense and how it works. **SonicBOOM is the one to study for speculation,** because the attack and a mitigation are both published.
- **The biggest gaps in public documentation are VEGA security, VeeR EH1 security, and Rocket cache parameters.** Fill these from the primary manuals.
