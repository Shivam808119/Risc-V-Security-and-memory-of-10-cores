# PicoRV32: Memory

## What it uses

| Part | Design |
|------|--------|
| Main interface | One native memory interface: `mem_valid`, `mem_instr`, `mem_ready`, `mem_addr`, `mem_wdata`, `mem_wstrb`, `mem_rdata`. Reads have `mem_wstrb` = 0. Writes use byte strobes for 8, 16, or 32 bits [src] |
| Look-ahead option | `mem_la_*` signals announce the next transfer one cycle early. They are combinational, which can hurt timing closure [src] |
| Wrappers | AXI4-Lite master (`picorv32_axi`), Wishbone master (`picorv32_wb`), and an adapter module [src] |
| Caches | None |
| Register file | Dual-port (faster) or single-port (smaller). RV32E halves the register count. On FPGAs with dedicated memory these options may not save area [src] |
| Coprocessor | PCPI interface for multiply, divide, and custom instructions. No response in 16 cycles raises illegal instruction [src] |
| Virtual memory / protection | None |

## Why it is built this way

- **One shared interface for fetch and data.** The core is multi-cycle, so it never needs both at once. `mem_instr` tells the system which kind of access it is. This saves pins and logic.
- **Valid/ready handshake** is the simplest protocol that tolerates any memory latency. Wrappers adapt it to the buses real SoCs use.
- **No cache** because the target is FPGA block RAM or small on-chip memory with 1-cycle access.
- **PCPI** keeps the core small: features like multiply are separate modules you include only if needed.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| SERV | SERV is narrower and smaller still. PicoRV32 is 32 bits wide, much faster per instruction, and has a richer memory interface |
| Ibex, CV32E40P | These have separate instruction and data ports so both can be active in the same cycle. PicoRV32 has one |
| VexRiscv | VexRiscv builds an I-bus and D-bus from plugins with optional caches. PicoRV32 is one fixed design with parameters |
| Rocket, SonicBOOM | Application-class memory systems with TLB and caches. PicoRV32 has neither |

## Questions to answer while studying

1. How many cycles does the memory handshake add if `mem_ready` is delayed?
2. When do look-ahead signals pay off, and what do they cost?
3. If you added a tiny instruction cache, which part of the interface would change?

## To verify

- Behavior of the look-ahead interface on your simulator version [verify].
