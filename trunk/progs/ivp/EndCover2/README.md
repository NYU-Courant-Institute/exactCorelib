# EndCover

Rigorous reachable-set enclosures for ODE initial value problems, implementing
the End Cover algorithm of Zhang and Yap.

Given an autonomous system `x' = f(x)`, an initial box `B0`, a time horizon `T`
and a tolerance `eps`, `endcover` returns a finite set of boxes `C` that
provably satisfies

```
End(B0, T)  ⊆  ⋃ C  ⊆  End(B0, T) + [-eps, eps]^n
```

In words: the union of the boxes contains every endpoint `x(T)` of a trajectory
starting in `B0`, and overshoots that true set by at most `eps` in each
coordinate. Everything is computed with validated interval arithmetic (CAPD and
filib), so the output is a guaranteed bound, not an approximation.

## Requirements

- A C++17 compiler. Developed with `g++` 13 under Cygwin; GCC or Clang on
  Linux, macOS and WSL work just as well.
- [CAPD](http://capd.ii.uj.edu.pl/), which provides the rigorous
  Taylor/Lohner integrator. Its interval backend (filib) is bundled with it.

CAPD is the only external dependency.

## Installation

Installation is two ordered steps: **first install CAPD, then build
`endcover`** (CAPD must exist before the compiler can find its headers and
library).

### Step 1 — install CAPD

If you don't already have it, CAPD builds once with CMake:

```sh
git clone https://github.com/CAPDGroup/CAPD.git
cd CAPD && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/capd
make -j4 && make install
```

That leaves a `capd-config` helper under `$HOME/capd/bin`. Sanity-check it with

```sh
capd-config --cflags --libs
```

which should print include and link flags ending in `-lcapd -lfilib`. filib
(the interval backend) is bundled with CAPD, so there is nothing else to
install.

This repository already ships a CAPD build, and the Makefile points at it by
default (`CAPD_CONFIG = ../../bin/capd-config`), so within this tree you can
skip straight to Step 2.

### Step 2 — build endcover

From this directory (`build/examples/EndCover_algo`):

```sh
make            # compiles endcover.exe, then runs the default example
make compile    # just compile, don't run
```

If your CAPD lives elsewhere, point the build at its `capd-config` and drop the
in-tree include hints:

```sh
make CAPD_CONFIG=$HOME/capd/bin/capd-config INCLUDE_PATH= FILIB_OVERRIDE=
```

> On Windows this workspace is built from a **Cygwin login shell**
> (`bash -l`); `capd-config` resolves the CAPD paths there. The plain Windows
> Git-Bash environment is not set up for the CAPD toolchain.

## Usage

The easiest way to run is through the Makefile: you can either use a built-in
example, or feed a parameter file.

```sh
make eg                       # the built-in Volterra example
make eg-lorenz                # a named example (3D)
make eg-lorenz eps=0.05 T=2   # …with overrides
make run-eg7Lorenz            # read a shared example file (see below)
make run-eg8Rossler iflag=4   # …with an override
```

`make run-egNAME` includes the parameter file `examples/egNAME.mk`, which ships
in this folder (so the tool is self-contained). Point elsewhere with
`EXAMPLES_DIR=...`, or name a file directly:

```sh
make run FILE=examples/eg2Vanderpol.mk
```

Those `.mk` files describe the system in centered form (`cen`, `wid`) plus
`var`, `ff`, `eps`, `order`, `T`, `iflag`, `stepB`, `stepA`, `debug`; the
Makefile converts `(cen, wid)` into `[cen-wid, cen+wid]` bounds. (If a file
happens to carry extra `mode` / `method` fields, they are simply ignored.)

Every override may be passed on the command line, e.g.
`make run-eg7Lorenz iflag=4 T=2`; command-line values win over the file.

### Calling the executable directly

```
./endcover.exe  iflag stepB stepA n  <vars...> <funs...>  eps order T debug tubedegree  <lo hi ...>
```

For example, the Volterra predator–prey system on `[0.9,1.1] × [2.9,3.1]`,
integrated to `T = 1` with `eps = 0.1`, printing time + hull + box count
(`tubedegree = -1` means "auto"):

```sh
./endcover.exe 2 0 0 2 x y "2*x-2*x*y" "-y+x*y" 0.1 20 1 0 -1  0.9 1.1 2.9 3.1
```

| argument | meaning |
|----------|---------|
| `iflag` | output level (progressive, see below) |
| `stepB` | StepB method: `0` C^r-Lohner, `1` Lohner + logNorm, `2` Direct + logNorm, `3` Direct, `4` SIAM with logNorm StepB plus endpoint tightening, `5` Boundary-compatible endpoint refine, `6` SIAM/Boundary endpoint hybrid |
| `stepA` | StepA method: `0` adaptive, `1` fixed |
| `n` | number of state variables |
| `<vars>` | the `n` variable names, e.g. `x y` |
| `<funs>` | the `n` components of `f`, quoted, e.g. `"2*x-2*x*y" "-y+x*y"` |
| `eps` | cover tolerance |
| `order` | Taylor order (20 is a sensible default) |
| `T` | time horizon |
| `debug` | `1` prints admissibility diagnostics, `0` stays quiet |
| `tubedegree` | Taylor-tube degree; `-1` = auto (= `order-1`). Used only by the Taylor tube (see below). |
| `<lo hi ...>` | bounds of `B0`, one `lo hi` pair per variable |

### Output levels (`iflag`)

`iflag` only controls *what is reported* — it never changes the computed cover.
It is **progressive**: level `k` includes everything from all smaller levels.

| iflag | adds |
|-------|------|
| `0+` | reserved for a future interactive shell — prints nothing on its own |
| `1+` | prints the total runtime (`time(ms)=`), then `Hull(T)=`, the minimal axis-aligned hull of the time-`T` cover, and its max width `wmax=` |
| `2+` | prints `E0Boxes=`, the number of initial sub-boxes in the space cover of `B0` |
| `3+` | writes `E0.txt` and `E1.txt` — `E1.txt` is **only** the time-`T` cover, `E0.txt` the matching initial sub-boxes (same order, one box per line) |
| `4+` | additionally writes `output/E_0.txt` and `output/E_1.txt` for plotting (see below) |

The two plotting files (written into the `output/` subfolder, created on demand)
hold a validated propagation *tube*:

- `E_0.txt` — the initial sub-boxes `E0`, and (in 2D) the 4 corner points of `B0`.
- `E_1.txt` — the time-`T` cover, then each `E0` sub-box's rigorous image at
  `t = 0.1/0.4/0.7` (those `<= T`), and (in 2D) the corner points and their
  propagation. Every image is a C^r-Lohner time-`t` map, so the tube is
  guaranteed too.

All box files use the plain `Box i: [lo, hi] x [lo, hi] ...` text format (one
box per line), so any plotting script — MATLAB, Python/matplotlib, gnuplot —
can read them directly by parsing the `[lo, hi]` intervals. The plotting files
to draw are `output/E_1.txt` (the tube) and `output/E_0.txt` (the initial
boxes).

### Notes

`stepB` `4` keeps the SIAM-style `Extend`/`Refine` scaffold, uses the stronger
local logNorm StepB, and adds endpoint Lohner tightening plus
longest-dimension splitting. `5` reproduces the fast Boundary-method
endpoint-refine behaviour: it uses the whole-horizon validated Lohner image to
decide acceptance/splitting, without importing the Boundary-method transform
machinery. `6` first runs the SIAM scaffold and then applies the Boundary
endpoint refine as a verified final-time completion/splitting step.

#### Tube method

Inside `Refine`, each stage is tightened by a *tube* around the centre
trajectory. Three tubes are available, selected with `CAPD_TUBE_METHOD` (the
Makefile exposes this as `tube=...`):

- `euler` (default) — the first-order Euler tube of the paper (Section 5.2).
- `rk2` — an experimental midpoint-RK2 centre; the local error is bounded by
  Taylor coefficients on the current full enclosure.
- `taylor` — the **Taylor tube of degree `p`** (`TaylorTube-new.h`), the
  degree-`p` generalisation of the Euler tube (`p = 1`). It marches the centre
  with a degree-`tubedegree` Taylor polynomial plus a rigorous remainder, so it
  stays a validated enclosure while admitting much larger steps (fewer
  bisections). The degree defaults to `order-1`, which lets the tube *inherit*
  the Taylor coefficients StepB already computes; set it explicitly with
  `tubedegree`.

```sh
make eg tube=taylor                  # Taylor tube, degree = order-1 (best default)
make eg tube=taylor tubedegree=5     # Taylor tube, degree 5
```

All three tubes only ever tighten by intersection with the already-validated
scaffold, so switching tube changes the *cost* (bisection count / time), not the
soundness of the cover.

Write `f` in CAPD's expression syntax: multiplication is explicit (`2*x`, not
`2x`), and `^`, `/` and parentheses behave as expected (`"x*y-8*z/3"`,
`"(x+y)^2"`).

The named example targets are:

```sh
make eg-volterra      make eg-lorenz      # 3D
make eg-vanderpol     make eg-rossler     # 3D
make eg-asymptote
```

These accept the usual overrides, e.g. `make eg-lorenz eps=0.05 T=2`.

For reference, a few runs on this machine:

| system      | eps  | T | boxes | hull width |
|-------------|------|---|-------|------------|
| Volterra    | 0.1  | 1 | 16    | 0.130      |
| Volterra    | 0.03 | 1 | 256   | 0.127      |
| Lorenz (3D) | 0.1  | 1 | 8     | 0.079      |

## How it works

The solver keeps a queue of boxes, initially just `B0`. Each box is propagated
to time `T` by a *scaffold* — a multi-stage validated time-stepping structure
that, following the paper's refine-before-extend discipline, alternates
`Extend` (advance one step via StepA/StepB) and `Refine` (tighten the
enclosures, or signal a split when the tolerance is out of reach). A box that
cannot be made `eps`-small as a whole is subdivided and its pieces go back on
the queue; otherwise its time-`T` image is added to the cover.

```
EndCover(B0, eps, T):
    C ← ∅;  Q ← {B0}
    while Q not empty:
        B   ← Q.pop()
        olB ← image of B at time T          # one scaffold, 0 → T
        C.add(olB)
        if B had to be shrunk to stay eps-small:
            Q.add(the 2^n sub-boxes of B)
    return C
```

The code follows the structure of the paper; each header points to the relevant
section.

| file | contents |
|------|----------|
| `calD-calQ-new.h` | scaffold data structures, interval helpers, logNorm, Euler-tube step |
| `stepAB-new.h`    | StepA and StepB |
| `Extend-new.h`    | the Extend subroutine |
| `Refine-new.h`    | Refine, Bisect, EulerTube, RK2Tube |
| `TaylorTube-new.h`| the Taylor tube of degree `p` (generalises the Euler tube) |
| `EndCover-new.h`  | the outer EndCover queue |
| `endcover.cpp`    | command-line front end |

## References

- B. Zhang and C. Yap. *End Cover for Initial Value Problem: Complete Validated
  Algorithm with Complexity Analysis.*
- B. Zhang and C. Yap. *Taylor Tube Method for Validated IVP.* — the Taylor tube
  of degree `p`, its step-size bound (Lemma 2) and enclosure theorem (Theorem 1),
  implemented in `TaylorTube-new.h`.
