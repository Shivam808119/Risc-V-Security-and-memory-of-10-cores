# Rocket: Security

## Features built in

The pages I could read did not describe security features [src: Rocket Chip README and Chipyard page covered structure only]. From general knowledge, a standard Rocket configuration provides [verify]:

- Machine, supervisor, and user privilege modes.
- Physical Memory Protection (PMP).
- Virtual memory with a TLB and page-table walker (the PTW is confirmed in the tile description [src]).

Rocket is widely used as a platform for security research (for example, enclave frameworks built on PMP) [verify].

It has no data-independent timing mode, no dummy instruction insertion, no lockstep, and no ECC-based hardening in the default configuration, as far as the sources show.

## Side-channel exposure

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache (Prime+Probe, Flush+Reload) | Yes | Shared L1 and L2, no partitioning or randomization in the baseline [analysis] |
| Branch predictor state | Yes | The front-end predictors are shared between privilege levels unless flushed [analysis, verify] |
| Speculation (Spectre) | Limited | In-order design: instructions after an unresolved branch are fetched but generally do not reach memory access before the branch resolves, so the classic bounds-check-bypass load does not happen the way it does on out-of-order cores [analysis, verify] |
| Timing | Present | Multiply/divide latency and cache hit/miss variation are not hardened |
| Power / EM | Present | No countermeasures |
| Faults | Present | No lockstep or ECC in the baseline |

## How to protect a Rocket-based design

1. **Use PMP and privilege modes to isolate secrets** from untrusted code, which is the intended use of those features.
2. **Defend the cache.** Options: flush L1 on context switch, restrict shared memory between domains, or partition the L2 by ways. Add a way-partition or index-randomization in the generator if you need hardware support.
3. **Flush or tag predictor state on privilege change.**
4. **Make timing data-independent** for crypto code (Zkt-style instruction guarantees and constant-time code), or move crypto to a fixed-latency RoCC accelerator.
5. **For physical attackers,** none of the baseline helps: add ECC and redundancy via the generator.
6. **Test** with the timing lab, then repeat with cache disabled to see how much comes from the cache.

## Experiments

- [ ] Run the timing lab with a D$ of 4 KB, 16 KB, 64 KB.
- [ ] Write a Prime+Probe-style timing measurement of cache set contention as a defensive test, not as an attack on anyone else's system.
- [ ] Check the generated Verilog for how predictor tables are indexed (shared across privilege levels?).

## Compare with others

Rocket is the "standard features, no side-channel hardening" reference point. SonicBOOM uses the same infrastructure but adds out-of-order speculation. Ibex is the opposite philosophy: small core, many hardening features.
