# 模版測試狀態

產生於 2026-09-16，最後更新 2026-09-27（補 51 個本地 stress test、修 13 個 bug、排版 26 → 25 頁；見文末）。
資料來源：`content.tex`、`MAP.tsv`、`RESULT.md`、`stress/`。怎麼跑見 `verify/README.md`。

> **2026-09-16 修正**：`run.py` 原本用 `checker <in> <jury> <ours>` 呼叫官方 checker，
> 但 testlib 的順序是 `<in> <ours> <jury>`。對於只驗證選手輸出（不比對標準答案）的
> special judge，這等於每次都在驗證標準答案，**必然 AC**。修正後全部重跑：
> 只有 `sqrt_of_formal_power_series` 真的翻成 WA（見下），其餘 47 列維持 AC。

| 欄位 | 意思 |
|---|---|
| **Library Checker** | `run.py` 實測：跑完該題全部測資，用官方 checker 判定 |
| **本地對拍** | 沒有 LC 對應題、或 LC 測不到的介面，改用暴力窮舉／參考實作對拍 |
| **單檔編譯** | `g++ -std=c++20 -fsyntax-only`，只給六行 macro 契約（幾何章另給章節 prelude）。「需要外部 X」不是缺陷，是這份模版照設計由使用者或前面的模版提供 X |
| **書上標註** | `content.tex` 裡 `% test by` 的原始註記 |


## Basic

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `1_Basic/readchar.cpp` | — 無對應題 | 400 萬 byte、跨 2^16 buffer 邊界 | ✅ | — |
| `1_Basic/black_magic.cpp` | — 無對應題 | — | ✅ | — |
| `1_Basic/Pragma.cpp` | — 無對應題 | — | 片段，非獨立編譯單元 | — |
| `1_Basic/sanitize_fixer.cpp` | — 無對應題 | — | ✅ | — |

## Graph

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `2_Graph/EBCC.cpp` | `graph/two_edge_connected_components` **AC** 21 筆 · 0.116s/5s (2%) | — | 需要外部 `adj` | test by caido, Lib-Checker Two-Edge-Connected Components |
| `2_Graph/VBCC.cpp` | `graph/biconnected_components` **AC** 22 筆 · 0.518s/5s (10%) | — | 需要外部 `adj` | test by caido, Lib-Checker Biconnected Components |
| `2_Graph/SCC.cpp` | `graph/scc` **AC** 12 筆 · 0.316s/5s (6%)<br>`other/two_sat` **AC** 18 筆 · 0.317s/5s (6%) | 30 000 組隨機有向圖 vs 可達性閉包（含反向拓撲序編號、scc_adj） | 需要外部 `adj` | test by caido, Lib-Checker Strongly Connected Components |
| `2_Graph/2SAT.cpp` | `other/two_sat` **AC** 18 筆 · 0.317s/5s (6%) | 20 000 組 vs 2^n 窮舉（either / set_value / at_most_one） | 需要外部 `adj` | needs SCC; test by Lib-Checker Two SAT, local brute force (either/set_value/at_most_one) |
| `2_Graph/MinimumMeanCycle.cpp` | — 無對應題 | 20 000 組 n≤8 vs 枚舉簡單環（**修 bug**：Karp 少算 k=n−1） | 需要外部 `N` | test by TIOJ 1934 |
| `2_Graph/Virtual_Tree.cpp` | — 無對應題 | 150 000 筆查詢 n≤40 vs 暴力（**修 bug**：`reset(st[0])`） | 需要外部 `N` | test by luogu P2495 |
| `2_Graph/Maximum_Clique_Dyn.cpp` | — 無對應題 | 6 000 組 n≤60，答案 + `sol[]` 驗證 | 需要外部 `N` | test by TIOJ 1978, CF 101221 I (World Finals' problem) |
| `2_Graph/MinimumSteinerTree.cpp` | `graph/minimum_steiner_tree` **AC** 24 筆 · 0.114s/5s (2%) | 16 000 組隨機圖 vs 枚舉 Steiner 點集 + Prim，含 `vcst` 與 W=1e9 兩種 | 需要外部 `N`、`T`、`INF` | test by luogu P6192, Lib-Checker Minimum Steiner Tree, local brute force |
| `2_Graph/Dominator_Tree.cpp` | `graph/dominatortree` **AC** 13 筆 · 0.115s/5s (2%) | — | 需要外部 `N` | test by CF 100513 L, Lib-Checker Dominator Tree |
| `2_Graph/Minimum_Arborescence_fast.cpp` | `graph/directedmst` **AC** 12 筆 · 0.122s/5s (2%) | — | 需要外部 `min_heap` | test by luogu P4716, Lib-Checker Directed MST |
| `2_Graph/Vizing.cpp` | — 無對應題 | 4 000 組 n≤104，合法著色且 ≤Δ+1 色 | ✅ | test by CF 101933 G |
| `2_Graph/Minimum_Clique_Cover.cpp` | — 無對應題 | 3 000 組 n≤14 vs 子集 DP（**修 UB**：改 `unsigned`） | 需要外部 `N` | test by TIOJ 1472 |
| `2_Graph/NumberofMaximalClique.cpp` | — 無對應題 | 4 000 組 n≤15 精確 + Moon–Moser n≤120（上限 1001） | 需要外部 `N` | test by POJ 2989 |

## Data Structure

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `3_Data_Structure/min_heap.cpp` | `graph/directedmst` **AC** 12 筆 · 0.122s/5s (2%) | — | ✅ | test by Lib-Checker Directed MST (through Minimum Arborescence) |
| `3_Data_Structure/BIT_kth.cpp` | `data_structure/ordered_set` **AC** 37 筆 · 0.268s/10s (3%) | — | 需要外部 `N` | test by CSES 1076, Lib-Checker Ordered Set |
| `3_Data_Structure/IntervalContainer.cpp` | — 無對應題 | — | ✅ | test by kactl stress test |
| `3_Data_Structure/Heavy_light_Decomposition.cpp` | `tree/vertex_add_path_sum` **AC** 19 筆 · 0.468s/5s (9%) | — | 需要外部 `N` | test by CSES Path Queries II, Lib-Checker Vertex Add Path Sum |
| `3_Data_Structure/Centroid_Decomposition.cpp` | `tree/frequency_table_of_tree_distance` 無法驅動 | 3 000 棵樹重用同一物件，28 萬次操作（**修 bug**：多測資沒清） | 需要外部 `N` | test by TIOJ 1171 |
| `3_Data_Structure/Treap.cpp` | `data_structure/ordered_set#treap` **AC** 37 筆 · 0.817s/10s (8%) | — | ✅ | test by Lib-Checker Ordered Set |
| `3_Data_Structure/LiChaoST.cpp` | `data_structure/line_add_get_min#lichao` 無法驅動 | 200 組 vs 窮舉（在其整數定義域 [0, n) 上） | 需要外部 `INF` | test by Library Checker Line Add Get Min |
| `3_Data_Structure/link_cut_tree.cpp` | `tree/dynamic_tree_vertex_add_path_sum` **AC** 15 筆 · 0.515s/5s (10%) | — | ✅ | test by luogu P3690, Lib-Checker Dynamic Tree Vertex Add Path Sum |
| `3_Data_Structure/KDTree.cpp` | — 無對應題 | 3 000 組 n≤2000，座標 ±1e9（**修 bug**：初值 `LLONG_MAX`） | 需要外部 `maxn` | — |

## Flow/Matching

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `4_Flow_Matching/Bipartite_Matching.cpp` | `graph/bipartitematching` **AC** 44 筆 · 0.516s/5s (10%) | `cover()`：30 000 組 vs 2^(L+R) 窮舉 | 需要外部 `N` | test by Lib-Checker Matching on Bipartite Graph; cover() by local brute force |
| `4_Flow_Matching/Kuhn_Munkres.cpp` | `graph/assignment` **AC** 14 筆 · 0.165s/5s (3%) | 20 000 組 vs 窮舉（14 919 組為矩形 n < m） | 需要外部 `N` | test by CF 101239 C (World Finals' problem), Lib-Checker Assignment Problem |
| `4_Flow_Matching/MincostMaxflow.cpp` | `graph/assignment#mcmf` **AC** 14 筆 · 1.417s/5s (28%) | 4 000 組隨機圖 vs SPFA 參考（2 000 組含負權、走 setpi） | 需要外部 `N` | test by Lib-Checker Assignment Problem, local stress vs SPFA reference |
| `4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp` | `graph/general_matching` **AC** 12 筆 · 0.008s/5s (0%) | — | ✅ | test by Library Checker Matching on General Graph |
| `4_Flow_Matching/Maximum_Weight_Matching.cpp` | `graph/general_weighted_matching` **AC** 24 筆 · 0.336s/5s (7%) | — | 需要外部 `INF` | test by Library Checker General Weighted Matching |
| `4_Flow_Matching/SW-mincut.cpp` | — 無對應題 | 30 000 組 n≤11 vs 2^n 枚舉 | ✅ | — |
| `4_Flow_Matching/BoundedFlow.cpp` | — 無對應題 | 30 000 組 vs 枚舉整數流（可行/最大/最小）（`init` 補清匯點） | 需要外部 `N` | Maximum boundedflow test by LOJ 116; Minimum boundedflow test by LOJ 117; Dinic test by NTUJ 184 |
| `4_Flow_Matching/Gomory_Hu_tree.cpp` | — 無對應題 | 6 000 組 n≤8，全點對 min cut（**修越界**） | 需要外部 `MaxFlow` | test by LOJ 2042 |
| `4_Flow_Matching/MinCostCirculation.cpp` | `graph/min_cost_b_flow` 無法驅動 | 23 000 組（含負環）vs 枚舉 / cycle-cancelling | 需要外部 `N` | test by uoj 487 |

## String

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `5_String/KMP.cpp` | — 無對應題 | 200 000 組 σ≤3（空 pattern 不可） | 需要外部 `MAXN` | — |
| `5_String/Z-value.cpp` | `string/zalgorithm` **AC** 29 筆 · 0.040s/5s (1%) | — | 需要外部 `SIZE` | test by Lib-Checker Z Algorithm |
| `5_String/Manacher.cpp` | `string/enumerate_palindromes` **AC** 24 筆 · 0.068s/5s (1%) | — | 需要外部 `SIZE` | test by Lib-Checker Enumerate Palindromes |
| `5_String/SA_LCP_becaido.cpp` | `string/suffixarray#becaido` **AC** 50 筆 · 0.167s/5s (3%) | — | ✅ | test by caido, Lib-Checker Suffix Array |
| `5_String/SAIS-C++20.cpp` | `string/suffixarray` **AC** 50 筆 · 0.181s/5s (4%) | σ≤3 窮舉 + 隨機 n≤3000，共 30 252 組 | ✅ | test by Lib-Checker Suffix Array |
| `5_String/Aho-Corasick_Automatan.cpp` | `string/aho_corasick` **AC** 72 筆 · 0.320s/5s (6%) | — | 需要外部 `len` | test by CF 102511 G (World Finals' problem), Lib-Checker Aho-Corasick |
| `5_String/Smallest_Rotation.cpp` | — 無對應題 | 79 523 組（σ=3 n≤9 窮舉 + 週期串） | ✅ | test by CSES Minimal Rotation |
| `5_String/De_Bruijn_sequence.cpp` | — 無對應題 | 416 組 (C,N,K)，每個長 N 字只出現一次 | ✅ | test by CF 102001 C |
| `5_String/exSAM.cpp` | `string/number_of_substrings` **AC** 24 筆 · 0.065s/5s (1%) | — | 需要外部 `N` | test by CF 616 C, Lib-Checker Number of Substrings |
| `5_String/PalTree.cpp` | `string/eertree` **AC** 24 筆 · 0.171s/5s (3%) | — | ✅ | test by APIO 2014 palindrome, Lib-Checker Eertree |
| `5_String/MainLorentz.cpp` | `string/runenumerate` **AC** 24 筆 · 0.165s/5s (3%) | — | 需要外部 `kN` | test by Lib-Checker Run Enumerate |

## Math

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `6_Math/ax+by=gcd.cpp` | — 無對應題 | 204 萬組含負數與 ±1e18 | ✅ | exgcd test by NTUJ 110 |
| `6_Math/floor_ceil.cpp` | — 無對應題 | 101 萬組含 INT 邊界 | ✅ | — |
| `6_Math/floor_enumeration.cpp` | `number_theory/enumerate_quotients` **AC** 26 筆 · 0.121s/5s (2%) | — | 片段，非獨立編譯單元 | test by Lib-Checker Enumerate Quotients |
| `6_Math/ModMin.cpp` | `number_theory/min_of_mod_of_linear` **AC** 17 筆 · 0.265s/10s (3%) | 暴力對拍 8 508 045 組（m ≤ 90） | ✅ | test by Lib-Checker Min of Mod of Linear |
| `6_Math/Gaussian_gcd.cpp` | `number_theory/gcd_of_gaussian_integers` **AC** 12 筆 · 0.065s/5s (1%) | — | 需要外部 `cpx` | test by Lib-Checker GCD of Gaussian Integers |
| `6_Math/Miller_Rabin.cpp` | `number_theory/factorize` **AC** 31 筆 · 1.167s/10s (12%)<br>`number_theory/primality_test` **AC** 12 筆 · 0.265s/5s (5%) | — | ✅ | test by NTUJ 1237, Lib-Checker Primality Test |
| `6_Math/Linear_Equations.cpp` | `linear_algebra/system_of_linear_equations` **AC** 27 筆 · 0.515s/5s (10%) | 3 000 組隨機有理數系統，驗 A·sol = b 且每個 basis 在核空間 | 需要外部 `MAXN` 與域型別 `T` | test by Lib-Checker System of Linear Equations (modint), local brute force (fraction) |
| `6_Math/Pollard_Rho.cpp` | `number_theory/factorize` **AC** 31 筆 · 1.167s/10s (12%) | — | 需要外部 `prime` | test by Lib-Checker Factorize |
| `6_Math/Simplex_Algorithm.cpp` | — 無對應題 | 11 889 組 vs 頂點枚舉（最佳/無界/不可行）+ 274 組對偶 | ✅ | — |
| `6_Math/chineseRemainder.cpp` | — 無對應題 | 424 575 組含非互質、無解、lcm≈1e18（**修溢位**） | 需要外部 `exgcd` | — |
| `6_Math/fac_no_p.cpp` | `enumerative_combinatorics/binomial_coefficient` 無法驅動 | 44 960 組 vs 直接計算 | 需要外部 `MAXP` | test by luogu P4720 |
| `6_Math/QuadraticResidue.cpp` | `number_theory/sqrt_mod` **AC** 13 筆 · 0.065s/10s (1%) | — | ✅ | test by Lib-Checker Sqrt Mod |
| `6_Math/PiCount.cpp` | `number_theory/counting_primes` **AC** 31 筆 · 0.033s/5s (1%) | — | ✅ | test by luogu P7884, Lib-Checker Counting Primes |
| `6_Math/DiscreteLog.cpp` | `number_theory/discrete_logarithm_mod` **AC** 20 筆 · 0.265s/10s (3%) | 暴力對拍 9 045 050 組（m ≤ 300） | 需要外部 `fpow` | test by Lib-Checker Discrete Logarithm |
| `6_Math/Berlekamp-Massey.cpp` | `other/find_linear_recurrence` **AC** 18 筆 · 0.164s/5s (3%) | — | ✅ | test by Lib-Checker Find Linear Recurrence |

## Polynomial

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `7_Polynomial/Fast_Fourier_Transform.cpp` | `convolution/convolution_mod_1000000007` **AC** 48 筆 · 0.369s/10s (4%) | `conv()`：500 組 vs 樸素卷積，最大誤差 1.9e-9 | ✅ | test by Lib-Checker Convolution mod 1e9+7 (through FFT Mod) |
| `7_Polynomial/Fast_Fourier_Transform_Mod.cpp` | `convolution/convolution_mod_1000000007` **AC** 48 筆 · 0.369s/10s (4%) | — | 需要外部 `cplx` | test by Lib-Checker Convolution mod 1e9+7 |
| `7_Polynomial/NTT_stdabs.cpp` | `convolution/convolution_mod` **AC** 53 筆 · 0.379s/5s (8%) | — | 需要外部 `mod`/`G`/`N` 和 `add`/`sub`/`mul`/`Pow`（`Pow` 的指數要 `ll`，`PolyPow` 會傳 k）；可用的 (mod, G, 2^k) 見下面的 NTT Primes 表 | std_abs 原檔 + macro 契約；test by Lib-Checker Convolution |
| `7_Polynomial/Fast_Walsh_Transform.cpp` | — 無對應題 | `subset_convolution`：2 000 組隨機 vs 樸素 $O(4^L)$，L 隨機 1..5、共用同一組全域，全程 ASan + UBSan | ✅ | test by luogu P6097, local brute force |
| `7_Polynomial/Operation_stdabs.cpp` | `polynomial/inv_of_formal_power_series` **AC** 25 筆 · 0.581s/10s (6%)<br>`polynomial/exp_of_formal_power_series` **AC** 26 筆 · 2.077s/10s (21%)<br>`polynomial/log_of_formal_power_series` **AC** 25 筆 · 0.974s/10s (10%)<br>`polynomial/pow_of_formal_power_series` **AC** 37 筆 · 2.779s/10s (28%)<br>`polynomial/sqrt_of_formal_power_series` **AC** 35 筆 · 1.826s/10s (18%)<br>`polynomial/polynomial_taylor_shift` **AC** 38 筆 · 0.478s/5s (10%)<br>`polynomial/shift_of_sampling_points_of_polynomial` **AC** 32 筆 · 0.893s/5s (18%) | `Divide`（$bq+r=a$）500 組、`Evaluate` vs Horner 300 組、`Interpolate` 來回 300 組、`TaylorShift` vs 直接代入 300 組、`SamplingShift` vs Lagrange 200 組、`power_proj` vs $O(n^2m)$ 樸素 250 組 | 需要外部 NTT、`QuadraticResidue`；`TaylorShift`/`SamplingShift` 要 `fac[]`/`ifac[]` | std_abs 原檔 + macro 契約 |
| `7_Polynomial/FastLinearRecursion_stdabs.cpp` | `other/kth_term_of_linearly_recurrent_sequence` **TLE** 19.442s/10s (194%)，答案正確 | BM → FastLinearRecursion 重播整段前綴，300 組 | 需要外部 `Divide`、`Mul` | `c` 是 **0-based**（BM 回傳 1-based，串接要平移）；慢在 `Divide` 對固定的 `C` 重算 `Inverse` 約 120 次 |
| `7_Polynomial/Value_Poly.cpp` | — 無對應題 | 431 189 組 vs 前綴和表 | 需要外部 `mint` | — |

### 支線（不在 `content.tex`，不進 PDF）

我們自己改寫的自由函式版，留著當備用——`kth_term` 那題它 AC 而主線 TLE。
六列 `#ours` 保護它不腐爛。

| 模版 | Library Checker | 本地對拍 |
|---|---|---|
| `7_Polynomial/Number_Theory_Transform.cpp` | `convolution/convolution_mod#ours` **AC** 53 筆 · 0.574s/5s (11%) | — |
| `7_Polynomial/Polynomial_Operation.cpp` | `inv#ours` **AC** 25 筆 (6%)<br>`exp#ours` **AC** 26 筆 (20%)<br>`pow#ours` **AC** 37 筆 (28%)<br>`sqrt#ours` **AC** 35 筆 (12%)<br>`kth_term#ours` **AC** 20 筆 · 9.740s/10s (97%) | `LinearRecursion`（經 BM）600 組、`PowerProj` 400 組、`Evaluate`/`Interpolate` 400 組、`Divide` 600 組 |

## Geometry

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `8_Geometry/Default_code.cpp` | — 無對應題 | projection/reflection/linearTransformation 30 萬組 vs complex | 片段，非獨立編譯單元 | — |
| `8_Geometry/PointSegDist.cpp` | — 無對應題 | 400 000 組 vs long double 投影 | ✅ | test by aizu oj CGL_2_D |
| `8_Geometry/Heart.cpp` | `geo/minimum_enclosing_circle` **AC** 25 筆 · 0.033s/5s (1%) | 294 534 個三角形（外心/內心/重心/垂心性質） | ✅ | circenter test by Lib-Checker Minimum Enclosing Circle |
| `8_Geometry/point_in_circle.cpp` | — 無對應題 | 383 207 組，exact `__int128` 參考（p 須 CCW） | ✅ | — |
| `8_Geometry/Convex_hull.cpp` | `geo/static_convex_hull` **AC** 25 筆 · 0.168s/5s (3%) | — | ✅ | test by Zerojudge b398, Lib-Checker Static Convex Hull |
| `8_Geometry/PointInConvex.cpp` | — 無對應題 | 400 萬筆查詢（不可有共線頂點） | ✅ | test by CF 104114 B, CF 101242 J (World Finals' problem) |
| `8_Geometry/TangentPointToHull.cpp` | — 無對應題 | 461 307 筆查詢 | 需要外部 `cyc_tsearch` | test by CF 104114 B, CF 101242 J (World Finals' problem) |
| `8_Geometry/Intersection_of_line_and_convex.cpp` | — 無對應題 | 558 997 條線，含過頂點/貼邊 | 需要外部 `cyc_tsearch` | test by aizu CGL_4_C, probably not enough |
| `8_Geometry/minMaxEnclosingRectangle.cpp` | — 無對應題 | 2 989 組（**修 bug**：acos NaN 丟掉最大值，改封閉式） | 需要外部 `hull` | test by UVA 819 |
| `8_Geometry/Vector_in_poly.cpp` | — 無對應題 | 928 200 組含凹角/180° | ✅ | test by qoj 5479 |
| `8_Geometry/PolyUnion.cpp` | — 無對應題 | 20 000 組 vs 精確 slab 分解（含重複多邊形） | ✅ | test by CF 101673 A |
| `8_Geometry/Polar_Angle_Sort.cpp` | `geo/sort_points_by_argument` **AC** 21 筆 · 0.117s/5s (2%) | — | ✅ | test by NTUJ 2270, Lib-Checker Sort Points by Argument |
| `8_Geometry/Half_plane_intersection.cpp` | — 無對應題 | 25 000 組 vs O(n³) 暴力（含空/零面積） | 需要外部 `cmp` | test by qoj 2162 (World Finals' problem) |
| `8_Geometry/HPIGeneralLine.cpp` | — 無對應題 | 25 000 組 vs O(n³) 暴力 | 需要外部 `cmp` | — |
| `8_Geometry/rotatingSweepLine.cpp` | — 無對應題 | 2 173 組一般位置（前提：無三點共線） | 需要外部 `cmp` | — |
| `8_Geometry/Minimum_Enclosing_Circle.cpp` | `geo/minimum_enclosing_circle` **AC** 25 筆 · 0.033s/5s (1%) | — | 需要外部 `circenter` | test by TIOJ 1093, Lib-Checker Minimum Enclosing Circle |
| `8_Geometry/Intersection_of_two_circles.cpp` | — 無對應題 | 398 738 組含精確相切（同圓回傳 NaN） | ✅ | test by TIOJ 1503 |
| `8_Geometry/Intersection_of_polygon_and_circle.cpp` | — 無對應題 | 99 364 組 vs kactl（**修 NaN**：改 atan2） | ✅ | test by HDU 2892 |
| `8_Geometry/Intersection_of_line_and_circle.cpp` | — 無對應題 | 299 352 組 | ✅ | test by kactl stress-tests |
| `8_Geometry/Tangent_line_of_two_circles.cpp` | — 無對應題 | 300 000 組 | ✅ | — |
| `8_Geometry/CircleCover.cpp` | — 無對應題 | 1 500 組 vs 數值積分（修未定義 `pi`） | 需要外部 `CCinter` | test by TIOJ 1503 |
| `8_Geometry/3Dpoint.cpp` | — 無對應題 | 200 000 組 | ✅ | test by HDU 3662 |
| `8_Geometry/Convexhull3D.cpp` | — 無對應題 | 5 765 個凸包 vs O(n⁴) 暴力（**修編譯錯誤**） | 需要外部 `Point` | test by HDU 3662, Ptz. Summer 2023 olmrgcsi And His Friends' Contest K |
| `8_Geometry/DelaunayTriangulation_dq.cpp` | — 無對應題 | 2 500 組 exact 驗空圓性質（需外部 `seg_strict_intersect`） | 需要外部 `N` | test by Zerojudge b370, TIOJ 1310 |
| `8_Geometry/Triangulation_Vonoroi.cpp` | — 無對應題 | 1 500 組 / 27 396 個 cell | 需要外部 `tool` | test by CF 104555 J, NPSC 2018 territory |
| `8_Geometry/Minkowski_Sum.cpp` | — 無對應題 | 9 634 組 vs 所有和的凸包（hull 不可退化） | 需要外部 `hull` | test by Zerojudge b398 |
| `8_Geometry/ClosestPair.cpp` | `geo/closest_pair` **AC** 29 筆 · 0.467s/5s (9%) | — | ✅ | test by Lib-Checker Closest Pair |

## Else

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `9_Else/cyc_tsearch.cpp` | — 無對應題 | 100 000 組 | ✅ | test by local brute force |
| `9_Else/Mos_Algorithm_With_modification.cpp` | — 無對應題 | 3 000 組 6 萬筆查詢（**原本編不過** + `R = -1`） | 需要外部 `blk` | — |
| `9_Else/Mos_Algorithm_On_Tree.cpp` | — 無對應題 | 3 000 棵樹 6 萬筆查詢（**原本編不過** + `R = -1`） | 需要外部 `LCA` | — |
| `9_Else/HilbertCurve.cpp` | — 無對應題 | n≤2^10 窮舉 + 2^30 隨機 | ✅ | — |
| `9_Else/DynamicConvexTrick.cpp` | `data_structure/line_add_get_min` **AC** 15 筆 · 0.167s/5s (3%) | — | ✅ | test by TIOJ 1921, Lib-Checker Line Add Get Min |
| `9_Else/All_LCS.cpp` | `string/prefix_substring_lcs` **AC** 11 筆 · 0.115s/5s (2%) | — | ✅ | test by Lib-Checker Prefix-Substring LCS |
| `9_Else/DLX.cpp` | — 無對應題 | — | 需要外部 `NN` | test by TIOJ 1333, 1381 |
| `9_Else/MatroidIntersection.cpp` | — 無對應題 | 3 000 組（圖擬陣 ∩ 分割擬陣）vs 2^m 窮舉 | ✅ | test by local brute force (graphic x colourful) |
| `9_Else/AdaptiveSimpson.cpp` | — 無對應題 | 40 000 個積分 | ✅ | test by CF 101193J, 100553D |
| `9_Else/simulated_annealing.cpp` | — 無對應題 | — | 片段，非獨立編譯單元 | — |
| `9_Else/tree_hash.cpp` | `tree/rooted_tree_isomorphism_classification` **AC** 17 筆 · 0.166s/5s (3%) | — | 需要外部 `G`、`N` | test by CSES Tree Isomorphism I, Lib-Checker Rooted Tree Isomorphism Classification |
| `9_Else/BinarySearchOnFraction.cpp` | `number_theory/rational_approximation` **AC** 28 筆 · 0.117s/5s (2%) | — | ✅ | test by ABC 333 G after adjusted to int128, Lib-Checker Rational Approximation |
| `9_Else/1d1d.cpp` | — 無對應題 | 20 000×2 組 vs O(n²)（**修 bug**：convex 版 deque 用錯端） | 需要外部 `N` | — |
| `9_Else/smawk.cpp` | `convolution/min_plus_convolution_convex_arbitrary#smawk` **AC** 41 筆 | 30 000 組完全單調矩陣求每列最小值 vs 暴力（Monge 型 + min-plus 斜帶） | ✅ | test by Lib-Checker Min Plus Convolution (Convex and Arbitrary) |
| `9_Else/min_plus_convolution.cpp` | `convolution/min_plus_convolution_convex_arbitrary` **AC** 41 筆 · 0.119s/5s (2%) | 15 000 組 vs 暴力與 SMAWK 交叉比對（在 SMAWK 的對拍裡） | 需要外部 `INF` | test by Library Checker Min Plus Convolution (Convex and Arbitrary) |
| `9_Else/BitsetLCS.cpp` | — 無對應題 | 10 000 組 vs O(nm) DP（需自備 bitset 型別） | 需要外部 `cin` | — |

---

## 總計

- 收錄模版：**111**
- 有 Library Checker AC 的：**50**
- 有本地對拍的：**66**（`verify/stress/`，54 個測試；舊的幾個在 `drivers/` 之外另有紀錄）
- 兩種自動測試都沒有的：**8**（readchar、black_magic、Pragma、sanitize_fixer、IntervalContainer、Default_code、DLX、simulated_annealing；多半是片段或純工具檔）

## 2026-09-27：stress test + 排版

- `verify/stress/`：kactl 式對拍（純 `main` + `assert`）。`stress/run.sh` 跑全部；預設 ASan+UBSan+`_GLIBCXX_DEBUG`。
- **harness 修正**：`geo_prelude.h` 原本寫死 master 的 `Default_code.cpp`，對其他 checkout 測試時幾何章只換了模版、沒換 prelude。harness 搬進 codebook repo 後路徑都是 repo 內相對路徑，這個問題不再存在。
- 後補的對拍：`Math/Pollard_Rho`（24 萬組）、`Basic/readchar`（400 萬 byte 跨 2^16 邊界）、`Geometry/Default_code`（projection/reflection/linearTransformation）；後兩者以植入錯誤確認會 FAIL。
- 對拍抓到 13 個 bug，已合併（PR #4）；上表標 **修** 的就是。
- PDF：listings 一行 58 欄，超過就折行。原本 123 行會折，現在 0 行；另外把放得下的「header + 單一敘述」接回一行（純空白、hash 不變，獨立 commit）。**26 → 25 頁，最後一頁是滿的。**
- 書裡仍缺、刻意沒補（不加新行）：`seg_strict_intersect`（Delaunay 要用）、`cmp`（HPI 兩份 + rotatingSweepLine，Polar_Angle_Sort 被註解掉）、`abs(pll)`（minMaxEnclosingRectangle）、BitsetLCS 需要的 bitset 型別。
