# SonicBOOM: Memory and Security Study

SonicBOOM is the third generation of BOOM, the Berkeley Out-of-Order Machine: a superscalar, out-of-order RV64GC core written in Chisel. It is the most speculation-heavy core of the ten and the standard open platform for studying Spectre-class problems.

Upstream: https://github.com/riscv-boom/riscv-boom, docs: https://docs.boom-core.org

## Key facts

| Item | Value |
|------|-------|
| ISA | RV64GC with the privileged ISA and page-based virtual memory [src] |
| Execution | Out-of-order, inspired by MIPS R10k and Alpha 21264, unified physical register file with explicit renaming [src] |
| Pipeline | Conceptually 10 stages, implemented as 7: Fetch, Decode/Rename, Rename/Dispatch, Issue/Register Read, Execute, Memory, Writeback. Commit is asynchronous [src] |
| Speculation | Every instruction carries a branch tag, on mispredict dependent instructions are killed and rename state is restored [src] |
| Integration | Drop-in replacement for the Rocket tile (`BoomTile`) in Rocket Chip and Chipyard [src] |
| Branch predictor | TAGE-based in SonicBOOM [verify] |
| Paper | "SonicBOOM: The 3rd Generation Berkeley Out-of-Order Machine", CARRV 2020 [verify, I could not open the paper] |

## Why it exists

To give architects an open, realistic, high-performance out-of-order core to modify, so research on prediction, memory, and security can be done on something close to real silicon.

## Study goals

1. Understand rename, issue queues, ROB, and branch tags.
2. Understand the load/store unit and why loads can execute speculatively.
3. Understand why this makes Spectre-style leaks possible, using the published BOOM work.
4. Understand how mitigations such as a speculation buffer change the design.

## Hands-on checklist

- [ ] Set up Chipyard, build a BOOM config, simulate with Verilator or FireSim.
- [ ] Walk one instruction through the 7 pipeline stages using a waveform.
- [ ] Locate the load address queue, store address queue, store data queue in the source.
- [ ] Read the CARRV19 paper (linked in `SECURITY.md`) and summarize the attack steps and the mitigation.
- [ ] Compare the same benchmark on Rocket versus BOOM to see what out-of-order buys.

## Files in this repo

- `MEMORY.md`, `SECURITY.md`, `docs/side-channel-guide.md`, `docs/labs/timing_lab.c`.

## To verify before citing

- Predictor design (TAGE, loop predictor, BTB, RAS), ROB size, queue depths, L1 geometry, TLB and PMP details. The pages I could read did not give them [verify].
