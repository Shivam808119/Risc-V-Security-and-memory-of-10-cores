# Ibex: Security

Everything below in the feature table is from the Ibex Security Features page [src: https://ibex-core.readthedocs.io/en/latest/03_reference/security.html].

## Features and how each works

| Feature | Enable | How it works | Threat addressed |
|---------|--------|--------------|------------------|
| Data-independent timing | `data_ind_timing` bit in `cpuctrl` | Branches take the same time taken or not. Early completion for multiply by zero/one and divide by zero is removed | Timing side channels |
| Dummy instruction insertion | `dummy_instr_en` bit, rate by `dummy_instr_mask` | Random dummy instructions (multiply/divide operations with no effect) at intervals set by an LFSR, reseedable via the `secureseed` CSR. Mask 000 gives one dummy every 0 to 4 real instructions, up to 111 for every 0 to 32. Needs a multiplier | Power analysis, timing, fault injection |
| Dual-core lockstep | Configuration option | A shadow core runs on delayed inputs, outputs are compared. Mismatch raises an internal major alert | Faults, glitching |
| Bus integrity checking | `MemECC` | Instruction and data bus data checked against checkbits. Bad data raises the bus major alert, suppresses a load's register write | Bus tampering, bit flips |
| Register file ECC | SecureIbex | A second register file instance stores ECC, checked by the shadow core. Detection only | Register faults |
| ICache ECC | SecureIbex | ECC on data and tag banks, error raises minor alert | Cache faults |
| ICache tweak infection | SecureIbex | A tweak derived from the address is XORed in before write and removed on read, so contents are bound to location | Cache tampering, replay |
| Hardened PC | not stated | Checks the fetch-stage PC equals the previous PC plus the right increment, disabled after branches and reset | Control-flow faults |
| Shadow CSRs | not stated, not used with SecureIbex | Complemented copies of `mstatus`, `mtvec`, `cpuctrl`, `pmpcfg`, `pmpaddr` are checked continuously | CSR corruption |

Alert outputs: `alert_major_internal_o` (internal unrecoverable), `alert_major_bus_o` (bad incoming bus data), `alert_minor_o` (recoverable) [src].

## Documented caveats

The docs state their own limits [src]:

- Unaligned data accesses can generate multiple bus accesses, so timing varies.
- Instruction cache hits and misses vary in latency. Disable the cache (`icache_enable`) or analyze the code.
- Branch prediction is experimental and should be off when relying on data-independent timing.
- Lockstep does not duplicate the register file or ICache RAMs.

## Side-channel summary

| Channel | Status |
|---------|--------|
| Timing | Defended by DIT when the caveats above are respected |
| Cache | Exposed if ICache is on and timing matters. Switch it off for secret code |
| Speculation | Small exposure, the pipeline is short and branch prediction is optional [analysis] |
| Power | Partial: dummy instructions raise the cost of attacks, no masking or dual-rail [analysis] |
| Faults | Strongest of the ten: lockstep, ECC, hardened PC, shadow CSRs, bus integrity |

## What you can do beyond Ibex's own features

1. Add software masking for secret-dependent ALU operations if power analysis is in scope.
2. Keep keys in a peripheral with its own protections (as OpenTitan does).
3. Use ePMP to lock secret regions even against machine mode [verify].
4. Always test: run the timing lab with `data_ind_timing` on and off, and collect power traces on an FPGA for TVLA.

## Experiments

- [ ] Repeat `docs/labs/timing_lab.c` with `data_ind_timing` off, then on.
- [ ] Sweep `dummy_instr_mask` and record cycle-count variation across 1000 runs.
- [ ] Enable `MemECC` and inject a bad checkbit in simulation to see the alert.
