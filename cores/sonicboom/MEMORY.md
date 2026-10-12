# SonicBOOM: Memory

## What it uses

| Part | Design |
|------|--------|
| Load/store unit | Three queues: Load Address Queue (LAQ), Store Address Queue (SAQ), Store Data Queue (SDQ) [src] |
| Load behavior | Loads are sent to memory once their address is in the LAQ, which means before older branches may have resolved: speculative [src, analysis] |
| Store behavior | Stores are sent to memory at commit, after address and data are in SAQ and SDQ [src] |
| L1 caches | Instruction and data caches, non-blocking, same Rocket-derived infrastructure [verify] |
| Translation | Page-based virtual memory [src], TLB and page-table walker [verify] |
| Bus | TileLink via the Rocket Chip tile interface [src] |
| Register file | Unified physical register file with renaming [src] |

## Why it is built this way

- **Loads issue early** because out-of-order cores earn their performance by overlapping memory latency. Waiting for every earlier branch would remove most of the benefit.
- **Stores wait for commit** because a store cannot be undone once it reaches memory. Loads can be squashed, stores cannot.
- **Branch tags** make recovery cheap: kill everything tagged with a mispredicted branch and restore the rename table.
- **Reuse Rocket's memory system** so effort goes into the out-of-order engine, not a new cache hierarchy.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| Rocket | Same cache and bus infrastructure. Rocket executes loads in order after branches resolve. SonicBOOM runs loads speculatively |
| VEGA AS1161/2161/4161 | Also 64-bit out-of-order with L1 and L2. VEGA uses AMBA buses. SonicBOOM uses TileLink |
| Shakti I-class | Also out-of-order. Shakti is a family with security research variants, SonicBOOM is the open platform where speculation leaks are studied |
| All in-order cores | Loads on wrong paths do not reach the cache. Here they can |

## Questions to answer while studying

1. Exactly when does a load's cache refill happen relative to branch resolution?
2. Which structure holds a speculative refill, and what happens to it on a squash?
3. What would change if refills were buffered until the load is non-speculative (the idea in the CARRV19 mitigation)?

## To verify

- LAQ/SAQ/SDQ depths, MSHR count, L1 sizes and ways [verify].
