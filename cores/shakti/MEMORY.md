# Shakti: Memory

## What it uses

| Part | Design |
|------|--------|
| C-class caches | Configurable: sets, ways, banks, fill buffer size, ECC, and tightly integrated memory (TIM) [src] |
| I-class caches | Non-blocking caches [src] |
| Tightly coupled memory | 256 KB on RIMO and Risecreek [src] |
| Translation | C-class has an MMU, runs Linux and seL4. RIMO and Risecreek support Sv39 under privilege spec v1.10. Configurable TLB sizes, ASID width, supervisor mode [src] |
| Protection | Configurable PMP size [src] |
| Branch predictor | Configurable history size, BHT, BTB, RAS [src] |
| Bus | AXI4 and AXI4-Lite in the uncore, with bridges for different data widths and clocks. Which bus the C-class core itself uses was not stated [src] |
| E-class memory | E-arty35T has 128 KB RAM. Cache details not stated [src] |
| T-class | Tags attached to memory to catch spatial and temporal memory errors [src] |
| F-class | ECC for critical memory and redundant bus fabrics [src] |

## Why it is built this way

- **Everything configurable** because the project is a platform, and each user wants a different area, performance, and reliability tradeoff.
- **TIM alongside caches** for deterministic latency in embedded use, similar in spirit to VeeR's ICCM/DCCM.
- **Tagged memory (T-class)** because memory-safety bugs are a major source of vulnerabilities and hardware tags can detect them cheaply [src: described as mitigating both spatial and temporal memory attacks].
- **ECC and redundancy (F-class)** for safety-critical use.

## How it differs from the other nine

| Compared with | Difference |
|---------------|------------|
| Rocket | Similar scope (in-order Linux-capable). Rocket is Chisel/TileLink, Shakti is Bluespec/AXI4 |
| VEGA | Another Indian family. Shakti is configurable and research-driven, VEGA is a set of fixed parts |
| VeeR EH1 | Both offer tightly integrated memory. Shakti pairs it with configurable caches |
| Ibex | Both explore integrity and lockstep. Shakti spreads these across class variants, Ibex bundles them under one parameter |
| All others | Shakti is the only one of the ten with a tagged-memory variant (T-class) |

## Questions to answer while studying

1. How are tags stored (a separate tag memory, widened lines) and cached?
2. What is the cost in area and bandwidth of tagging?
3. How does TIM get arbitrated against the cache path?

## To verify

- Tag storage design in Shakti-T [verify, search results pointed to the Shakti-T paper but I did not read it].
