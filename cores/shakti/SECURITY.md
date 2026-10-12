# Shakti: Security

## What exists

| Feature | Class | Detail |
|---------|-------|--------|
| Tagged memory | T-class | Hardware support against memory-based attacks. One framework to mitigate both spatial and temporal memory attacks [src] |
| Fault tolerance | F-class | Dual and triple modular redundancy, temporal redundancy for permanent fault detection, lockstep cores, fault localization, ECC for critical memory, redundant bus fabrics [src] |
| SafeRV | Work in progress | Developed with Thales, includes a criticality-aware network-on-chip and a triple lock-step pipeline [src] |
| PMP / privilege | C-class | Configurable PMP size, supervisor mode support [src] |
| Side-channel resistance | Research area | Listed as research, not a shipped feature [src] |

## Important distinction

Tagged memory protects against **memory-safety bugs** (buffer overflow, use-after-free). It does **not** by itself stop side channels. A tag check stops a wrong access from succeeding. Timing, cache, and power leaks happen with perfectly legal accesses. Treat T-class as a different protection category from the countermeasures in `docs/side-channel-guide.md`.

## Side-channel exposure

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache | Yes on C-class | Configurable but standard caches, no partitioning described [analysis] |
| Speculation | C-class limited, I-class exposed | C-class is in-order [src]. I-class is out-of-order with aggressive branch prediction [src], so the architecture class is exposed [analysis] |
| Branch predictor | Present | BHT, BTB, RAS exist and are shared [src, analysis] |
| Timing | Present | No data-independent timing mode described |
| Power / EM | Present | No countermeasures described |
| Faults | Strongest in F-class | DMR/TMR, lockstep, ECC [src] |

## How to protect a Shakti-based design

1. **Use T-class** when memory-safety bugs are the main worry.
2. **Use F-class features** (lockstep, ECC) for fault injection and soft errors.
3. **For side channels, add the standard measures yourself** on C-class: flush caches and predictors on domain switches, constant-time software, and PMP for isolation. Shakti lists this as research, so expect to build rather than configure.
4. **Use TIM for secret data** where deterministic latency is available, which avoids the cache channel.
5. **Check which variants are really available** before planning around them.

## Experiments

- [ ] Run the timing lab on E-class and C-class.
- [ ] Compare configured cache size effect on timing variation.
- [ ] Sketch how a tag-check failure would be reported and handled.
- [ ] Compare F-class lockstep with Ibex lockstep: what is duplicated in each?

## Compare with others

Shakti is the family that spans *memory-safety* (T) and *reliability* (F) security research. Ibex has the broadest shipped hardware countermeasures. SonicBOOM has the clearest speculation research. Together they cover the map.
