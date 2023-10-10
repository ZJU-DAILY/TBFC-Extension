# TBFC

This repository contains the source code for our paper: **Efficient Temporal Butterfly Counting and Enumeration on Temporal Bipartite Graphs**.

# Requirements

+ cmake (>3.15)
+ g++ (supports c++17)
+ GNU Make

# Datasets

All the networks can be downloaded from [KONECT](http://konect.cc/).

| Datasets | link |
| :----: | :----: |
|WQ|<http://konect.cc/networks/edit-bnwikisource/>|
|WN|<http://konect.cc/networks/edit-itwikinews/>|
|SO|<http://konect.cc/networks/stackexchange-stackoverflow/>|
|CU|<http://konect.cc/networks/citeulike-ti/>|
|BS|<http://konect.cc/networks/bibsonomy-2ti/>|
|TW|<http://konect.cc/networks/munmun_twitterex_ut/>|
|AM|<http://konect.cc/networks/amazon-ratings/>|
|ER|<http://konect.cc/networks/edit-ruwiktionary/>|
|EP|<http://konect.cc/networks/epinions-rating/>|
|LF|<http://konect.cc/networks/lastfm_band/>|
|WT|<http://konect.cc/networks/edit-enwiktionary/>|

# Usage

1. Download datasets.
2. Compile project with `make build` and `cd bin`.
3. Reformat datasets with `./reformat <data_path>`, where `<data_path>` is the path for input dataset, e.g., `./reformat ../data/WQ`.
4. Run the specific algorithm with `./main <argv[1]> <argv[2]> <argv[3]> <...>`, such as `./main ../data/WQ 3456000 3`. The specific parameters are as follows. Note that the `output` folder needs to be created in advance to save the instances.
```
argv[1] = data_path
argv[2] = delta
argv[3] = algorithm :
         -10 : TBE (not print instance, for experiments)
         -11 : TBE (print instance)
         -20 : TBE$^+$ (not print instance, for experiments)
         -21 : TBE$^+$ (print instance)
          1 : TBC
          2 : TBC$^+$
          3 : TBC$^{++}$
          4 : STBC
          5 : STBC$^+$
          61 : ApproxTBC
          62 : ApproxTBC$^+$
          63 : ApproxTBC$^{++}$
          71 : sGrappTBC
          72 : sGrappTBC$^+$
          73 : sGrappTBC$^{++}$

(optional)
algorithm 4
argv[4] = |Window| (default: 10000)
argv[5] = |Stride|/|window| (default: 0.05)
alporithm 5
argv[4] = |Window| (default: 10000)
argv[5] = |Stride|/|window| (default: 0.05)
argv[6] = |Thread| (default: 8)
algorithms 61, 62, 63
argv[4] = sampling probability (default: 0.8)
algorithms 71, 72, 73
algorithms 71, 72, 73
argv[4] = unique timestamps per window (default: 1000)
argv[5:10] = approximation exponent theta_i (default: 1.05)
```