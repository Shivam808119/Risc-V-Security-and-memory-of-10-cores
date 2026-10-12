# Ibex: Memory

## What it uses

| Part | Design |
|------|--------|
| Instruction / data interfaces | Separate instruction and data ports, request/grant style (OBI-like) [verify] |
| Instruction cache | Optional. When present, can be protected with ECC on data and tag banks and with address-bound tweaks [src: security page] |
| Bus integrity | `MemECC` parameter checks incoming instruction and data bus data against supplied checkbits. A bad load has its register write suppressed and raises the bus major alert [src] |
| Register file | Flip-flop, latch, and FPGA variants [verify]. With SecureIbex a second instance stores ECC for the shadow core [src] |
| Memory protection | PMP, and ePMP (Smepmp) [verify]. The security page links to a separate PMP page [src] |
| Virtual memory | None [verify] |

## Why it is built this way

- **Separate ports** let fetch and data access overlap in a short pipeline.
- **Optional ICache** exists because secure chips often run code from slow flash. The cache hides that latency, but it also adds timing variation, so the docs tell you to disable it or analyze the code when you need data-independent timing [src].
- **Bus integrity and cache ECC** exist because a physical attacker can flip bits on a bus or in SRAM. The core assumes memory is not fully trustworthy.
- **Tweak infection in the ICache** ties stored data to its address so moving or replaying cache contents is detected [src].

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| CV32E40P | Both have separate ports. CV32E40P has no cache and no PMP, Ibex has optional ICache, PMP, and integrity checking |
| PicoRV32, SERV | Those have no integrity features at all |
| VeeR EH1 | VeeR relies on tightly coupled memories with ECC for deterministic latency. Ibex goes through an optional cache with ECC |
| Rocket, SonicBOOM | They have full L1 data caches and TLBs. Ibex has no data cache and no address translation |
| Shakti F-class | Both explore lockstep and ECC. Shakti pushes fault tolerance as a family variant, Ibex bundles it in SecureIbex |

## Questions to answer while studying

1. Which memory structures are *not* duplicated in lockstep (the docs say register file and ICache RAMs are not), and how are they protected instead [src]?
2. How does the ICache tweak differ from the usual cache scrambling idea?
3. What does the core do on a bus integrity error, and which software handler would you write?

## To verify

- ICache default configuration and line size [verify].
