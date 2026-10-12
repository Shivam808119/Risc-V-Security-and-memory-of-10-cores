# VexRiscv: Security

## Features built in

Security-relevant plugins listed by the project [src]:

- `MmuPlugin` and `CsrPlugin` for page-based memory and privilege modes. Linux compatibility implies supervisor and user separation [src: Linux compatible].
- `PmpPlugin` and `PmpPluginNapot` for physical memory protection.
- `AesPlugin` for AES acceleration.

The project page does not list data-independent timing, dummy instruction insertion, lockstep, ECC, or side-channel countermeasures [src]. Whether any of these exist is a question for the plugin source.

## Side-channel exposure (depends on configuration)

| Channel | Exposure | Reasoning |
|---------|----------|-----------|
| Cache | Only if cached plugins are included | `IBusSimplePlugin` and `DBusSimplePlugin` have no cache to probe [analysis] |
| Speculation | Low in simple configs | Short in-order pipeline, no loads past unresolved branches in typical configs [analysis, verify] |
| Timing | Present, depends on plugin | `MulPlugin` versus `MulDivIterativePlugin` and `LightShifterPlugin` versus `FullBarrelShifterPlugin` give different latency behavior, some operand-dependent [analysis] |
| Table-based AES leakage | Reduced | A hardware AES instruction avoids secret-indexed table lookups that leak through cache [analysis, verify AesPlugin timing] |
| Power / EM | Present | No countermeasures documented |
| Faults | Present | No integrity checks documented |

## How to protect a VexRiscv design

1. **Pick plugins for predictability.** Choose the full barrel shifter and a fixed-latency multiplier for crypto code, and prefer simple bus plugins for secret handling to avoid cache effects.
2. **Use `PmpPlugin`** to carve out protected regions and the MMU plugin if you run untrusted code.
3. **Use `AesPlugin` instead of software AES** if the leak you worry about is cache timing from table lookups, and check that the plugin itself has no data-dependent timing.
4. **Write a plugin for what is missing.** The architecture makes this the natural place to prototype data-independent timing, dummy instruction insertion, or a hardened PC, which Ibex documents as separate features. This is the best hands-on exercise in this kit.
5. **Physical attackers:** no help in the base plugins, add ECC and redundancy yourself.

## Experiments

- [ ] Build three configs (min, full, Linux) and run the timing lab on each.
- [ ] Write a plugin that makes branch timing constant, then measure.
- [ ] Compare the PMP plugin's region count and granularity to Ibex PMP.

## Compare with others

VexRiscv is the best place to *build* your own countermeasure and see its cost, because every feature is a plugin. Ibex is the best place to *read* how a finished set works.
