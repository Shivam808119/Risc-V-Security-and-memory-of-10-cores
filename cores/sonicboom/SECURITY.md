# SonicBOOM: Security

## What the published work shows

A CARRV 2019 paper, [Replicating and Mitigating Spectre Attacks on an Open Source RISC-V Microarchitecture](https://boom-core.org/docs/replicating_mitigating_spectre_carrv19.pdf) [src]:

- **Reproduced Spectre v1** (bounds-check bypass) on BOOM running on FireSim. Only v1 was covered, not v2 [src].
- **Why it worked:** the attack has three steps: train the branch predictor, trigger misspeculation past a bounds check so a secret-dependent load runs, then recover the secret by timing cached versus uncached lines. BOOM allowed this through set-associative BTB and GShare predictors that can be trained, out-of-order issue with branch kill masks that squash wrong-path work while cache side effects remain, an L1 data cache, and a custom L1 flush the attacker used to prepare the timing measurement [src].
- **Mitigation: Speculation Buffer.** Refills from misspeculated loads leave cache side effects. The fix treats the data cache as architectural state: speculative refills are held in the existing line-fill buffers and written to the cache only after the load reaches a "point of no return" tracked by a new ROB pointer [src].
- **Cost reported:** about 2% Dhrystone change (2176 to 2216 DPS), preliminary TSMC 45nm area about 3% higher, some microbenchmarks 11% to 40% slower [src].

Note this paper studies BOOM at the time of the paper, not necessarily the SonicBOOM (v3) design, so the exact predictor structures may differ [verify].

Other approaches the paper compares [src]: InvisiSpec (reported about 22% performance loss), SafeSpec (shadow structures for D$, I$, TLB), DAWG (cache partitioning), StealthMem, CATalyst, context-sensitive fencing, and compiler-inserted fencing.

## Related evidence on real silicon

A 2024 study of commercial out-of-order RISC-V chips (SiFive P550, T-Head C910/C920) found Spectre-class attacks working, and noted the ISA lacks a dedicated speculation barrier [src: [CISPA page](https://cispa.de/de/research/publications/213214-spectre-on-risc-v-silicon-attacks-and-defenses-on-commercial-out-of-order-processors)]. Those chips are not BOOM, but the architecture class is the same.

## Features built in

No side-channel countermeasures in the baseline design as far as the sources show. It provides the privileged ISA and virtual memory [src] and, from general knowledge, PMP [verify].

## Side-channel exposure

| Channel | Exposure |
|---------|----------|
| Speculation (Spectre-style) | **High.** Demonstrated on BOOM [src] |
| Cache | High, shared L1 and lower levels, no partitioning in baseline [analysis] |
| Branch predictor | High, trainable predictors [src] |
| Timing | Present, variable-latency units and cache effects |
| Power / EM | No countermeasures |
| Faults | No countermeasures |

## How to protect a BOOM-class design

1. **Contain speculative cache effects:** speculation buffer, InvisiSpec- or SafeSpec-style shadow structures, or taint tracking that blocks dependent loads.
2. **Software barriers** after bounds checks in security-critical code, though the ISA gives no standard speculation barrier, so you may need to define one in your core.
3. **Flush or tag predictors** on privilege-level switches.
4. **Partition or randomize caches** (DAWG-style partitioning, randomized mapping) when attackers and victims share a cache.
5. **Provide a flush instruction for caches** (BOOM's custom flush is also what made the attack easier, so restrict it to privileged code).
6. **Measure the cost.** Every mitigation above trades performance. The paper's table shows the range.

## Experiments

- [ ] Reproduce the *defensive* result: run the public boom-attacks-style test on a config you own in simulation, then enable a mitigation and confirm the leak disappears.
- [ ] Measure IPC cost of delaying speculative loads until non-speculative.
- [ ] Compare against Rocket: the same test should show no leak because loads do not reach the cache past an unresolved branch [analysis].

## Compare with others

SonicBOOM is where side-channel *attack* research lives. Ibex is where side-channel *defense features* are documented. A good study ends with both.
