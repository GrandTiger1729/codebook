# 模版測試狀態

產生於 2026-09-16，最後更新 2026-10-01（本地對拍共 77 個、修 14 個 bug；見文末）。
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
| `1_Basic/SIMD.cpp` | — 無對應題 | 2 萬組隨機陣列與區間（各種對齊與尾端）vs 純量迴圈；註解列出的 intrinsic 都實際呼叫，並在已知輸入上驗證註解寫的語意（set 順序、半邊內位移、alignr、前綴技巧等）；cnt_lt 約 3.5 倍快（-O2） | ✅ | 2026-11-17 加入（GCC vector extension + AVX2 intrinsic 速查） |

## Graph

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `2_Graph/EBCC.cpp` | `graph/two_edge_connected_components` **AC** 21 筆 · 0.116s/5s (2%) | 20 000 組多重圖（含重邊、自環）vs 刪邊暴力 | 需要外部 `adj` | test by caido, Lib-Checker Two-Edge-Connected Components |
| `2_Graph/VBCC.cpp` | `graph/biconnected_components` **AC** 22 筆 · 0.518s/5s (10%) | 20 000 組多重圖（含重邊、自環）vs kactl `bicomps`；直接呼叫 `tarjan` == `work()` | 需要外部 `adj` | test by caido, Lib-Checker Biconnected Components |
| `2_Graph/SCC.cpp` | `graph/scc` **AC** 12 筆 · 0.316s/5s (6%)<br>`other/two_sat` **AC** 18 筆 · 0.317s/5s (6%) | 30 000 組隨機有向圖 vs 可達性閉包（含反向拓撲序編號、scc_adj） | 需要外部 `adj` | test by caido, Lib-Checker Strongly Connected Components |
| `2_Graph/2SAT.cpp` | `other/two_sat` **AC** 18 筆 · 0.317s/5s (6%) | 20 000 組 n ≤ 10 vs 2^n 窮舉（either / set_value / at_most_one，14 101 組可滿足），並驗 `ans` | 需要外部 `adj` | needs SCC; test by Lib-Checker Two SAT, local brute force (either/set_value/at_most_one) |
| `2_Graph/EulerTrail.cpp` | `graph/eulerian_trail_directed` **AC** 27 筆 · 0.114s/5s (2%)<br>`graph/eulerian_trail_undirected` **AC** 27 筆 · 0.115s/5s (2%) | — | ✅ | 2026-11-17 加入（迭代版 Hierholzer；度數檢查與起點由呼叫端負責） |
| `2_Graph/MinimumMeanCycle.cpp` | — 無對應題 | 20 200 組：n≤8 vs 枚舉簡單環，n ≤ 300、權重 ≤ 1e9 vs 恰 q 邊閉走最小值（**修 bug**：Karp 少算 k=n−1） | 需要外部 `N`；權重須為正 | test by TIOJ 1934 |
| `2_Graph/Virtual_Tree.cpp` | — 無對應題 | 150 000 筆查詢 n≤40 vs 暴力（**修 bug**：`reset(st[0])`） | 需要外部 `N` | test by luogu P2495 |
| `2_Graph/Maximum_Clique_Dyn.cpp` | — 無對應題 | 6 000 組 n≤60，答案 + `sol[]` 驗證 | 需要外部 `N` | test by TIOJ 1978, CF 101221 I (World Finals' problem) |
| `2_Graph/MinimumSteinerTree.cpp` | `graph/minimum_steiner_tree` **AC** 24 筆 · 0.114s/5s (2%) | 16 000 組多重圖 n ≤ 8、T ≤ 5 vs 枚舉點集 + Prim（8 047 組有 `vcst`、5 411 組 W=1e9、3 486 組不連通），並驗 `ans` 是連通所有 terminal 的樹 | 需要外部 `N`、`T`、`INF` | test by luogu P6192, Lib-Checker Minimum Steiner Tree, local brute force |
| `2_Graph/Dominator_Tree.cpp` | `graph/dominatortree` **AC** 13 筆 · 0.115s/5s (2%) | — | 需要外部 `N` | test by CF 100513 L, Lib-Checker Dominator Tree |
| `2_Graph/Minimum_Arborescence_fast.cpp` | `graph/directedmst` **AC** 12 筆 · 0.122s/5s (2%) | 3 萬張 n ≤ 6 隨機多重圖 vs 暴力（16 933 張無解、5 117 張 n = 1）：有解 iff 回傳 n − 1 條邊，且是以 root 為根的樹、權重最小 | 需要外部 `min_heap` | test by luogu P4716, Lib-Checker Directed MST |
| `2_Graph/ManhattanMST.cpp` | `geo/manhattanmst` **AC** 16 筆 · 0.314s/5s (6%) | 20 000 組 n ≤ 200（座標到 ±1e9、大量重點）+ Kruskal vs O(n²) Prim；每條邊權正確、至多 4n 條 | ✅ | 2026-11-17 加入（kactl 演算法；取代 `9_Else` 裡沒收錄、不能編譯的舊版） |
| `2_Graph/BipolarOrientation.cpp` | — 無對應題 | 20 萬張隨機多重圖 n ≤ 60，其中 93 582 張加 (s, t) 後點雙連通：結果是排列、s 第一 t 最後、其他點前後都有鄰居 | ✅ | 2026-11-17 加入（ckiseki 的 linked-list st-numbering）, test by CF1916F |
| `2_Graph/Vizing.cpp` | — 無對應題 | 4 000 組 n≤104，合法著色且 ≤Δ+1 色；另 4 000 組二分圖用 `bip = 1`，≤Δ 色 | ✅ | test by CF 101933 G |
| `2_Graph/Minimum_Clique_Cover.cpp` | — 無對應題 | 3 000 組 n≤14 vs 子集 DP（**修 UB**：改 `unsigned`） | 需要外部 `N` | test by TIOJ 1472 |
| `2_Graph/NumberofMaximalClique.cpp` | — 無對應題 | 4 000 組 n≤15 精確 + Moon–Moser n≤45 精確 | 需要外部 `N` | test by POJ 2989 |
| `2_Graph/C3C4.cpp` | `graph/enumerate_triangles` **AC** 17 筆 · 0.164s/5s (3%) | 30 000 張隨機圖 vs 暴力（247 994 個三角形各回報恰一次、973 727 個四環）+ 星狀圖加團 n=2·10⁵ 的複雜度檢查 | 需要外部 `N`；先呼叫 `build(n)` | 2026-09-30 加入；`C3(n, f)` 對每個三角形呼叫 `f(a, b, c)`，`C4(n)` 回傳四環數 |

## Data Structure

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `3_Data_Structure/min_heap.cpp` | `graph/directedmst` **AC** 12 筆 · 0.122s/5s (2%) | leftist tree：3 000 組共 42.7 萬次 push/pop/join/add_lazy vs `multiset`，並檢查左偏性質 | ✅ | test by Lib-Checker Directed MST (through Minimum Arborescence) |
| `3_Data_Structure/BIT_kth.cpp` | `data_structure/ordered_set` **AC** 37 筆 · 0.268s/10s (3%) | — | 需要外部 `N` | test by CSES 1076, Lib-Checker Ordered Set |
| `3_Data_Structure/IntervalContainer.cpp`（不在書中） | — 無對應題 | — | ✅ | test by kactl stress test |
| `3_Data_Structure/Heavy_light_Decomposition.cpp`（不在書中） | `tree/vertex_add_path_sum` **AC** 19 筆 · 0.468s/5s (9%) | — | 需要外部 `N` | test by CSES Path Queries II, Lib-Checker Vertex Add Path Sum |
| `3_Data_Structure/Centroid_Decomposition.cpp` | `tree/frequency_table_of_tree_distance` 無法驅動 | 3 000 棵樹重用同一物件，28 萬次操作（**修 bug**：多測資沒清） | 需要外部 `N` | test by TIOJ 1171 |
| `3_Data_Structure/Treap.cpp` | `data_structure/ordered_set#treap` **AC** 37 筆 · 0.766s/10s (8%) | — | ✅ | test by Lib-Checker Ordered Set；2026-11-22 `rand()` → `mt19937` |
| `3_Data_Structure/LiChaoST.cpp`（不在書中） | `data_structure/line_add_get_min#lichao` 無法驅動 | 3 000 組共 976 772 次查詢 vs 窮舉 | 需要外部 `INF` | test by Library Checker Line Add Get Min |
| `3_Data_Structure/link_cut_tree.cpp` | `tree/dynamic_tree_vertex_add_path_sum` **AC** 15 筆 · 0.33s/5s (7%)（指標版 0.56s） | 3 000 個森林 n ≤ 40、88 萬次操作 vs 暴力：link / cut（含 10 萬次切非邊，須為 no-op）/ change / query / conn，chroot 後的 lca 與 get_root（**修 bug**：舊版 `cut` 判斷 `size != 5`，幾乎不切；切相連但不相鄰的點會切錯邊；切完沒 `pull`） | 需要外部 `N` | 2026-11-17 改成陣列版 `struct LCT`（1-base，0 是 nil） |
| `3_Data_Structure/KDTree.cpp` | — 無對應題 | 3 000 組 n≤2000，座標 ±1e9（**修 bug**：初值 `LLONG_MAX`） | 需要外部 `maxn` | — |
| `3_Data_Structure/StaticTopTree.cpp` | `tree/point_set_tree_path_composite_sum_fixed_root` **AC** 26 筆 · 0.465s/10s (5%) | 3 000 棵樹 n ≤ 5000（隨機、路徑、星、毛毛蟲）、55 萬次單點更新後 vs 暴力重算；高度 ≤ 6 log n + 6（實測最大 34） | 需要外部 `Path`、`Point` 與五個 DP 函式；`query_subtree`、`query_reroot` 另有 4 000 棵樹 n ≤ 400 各約 31 萬次 vs 暴力（含單點更新） | 2026-11-17 加入（ABC351G 解說的建法） |

## Flow/Matching

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `4_Flow_Matching/Bipartite_Matching.cpp` | `graph/bipartitematching` **AC** 44 筆 · 0.516s/5s (10%) | 30 000 組 L, R ≤ 6（含重邊）：`matching()` vs bitmask DP；`cover()` vs 2^(L+R) 窮舉 | 需要外部 `N` | test by Lib-Checker Matching on Bipartite Graph; cover() by local brute force |
| `4_Flow_Matching/Kuhn_Munkres.cpp` | `graph/assignment` **AC** 14 筆 · 0.165s/5s (3%) | 20 000 組 n ≤ m ≤ 7 vs 窮舉（12 545 組矩形 n < m、6 666 組 perfect 模式含負權） | 需要外部 `N` | test by CF 101239 C (World Finals' problem), Lib-Checker Assignment Problem |
| `4_Flow_Matching/MincostMaxflow.cpp` | `graph/assignment#mcmf` **AC** 14 筆 · 1.417s/5s (28%) | 4 000 組 n ≤ 8、m ≤ 20 vs SPFA 最短路增廣（1 545 組負權、走 setpi；同一物件重複 `init`）（**修 bug**：`par[s]` 沒重設，重用物件會沿舊邊回溯）；負環：Flow Models 的「先把負邊流滿」歸約，20 000 組 vs 窮舉所有整數流（任意流量 / 最大流兩種） | 需要外部 `N` | test by Lib-Checker Assignment Problem, local stress vs SPFA reference |
| `4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp` | `graph/general_matching` **AC** 12 筆 · 0.008s/5s (0%) | — | ✅ | test by Library Checker Matching on General Graph |
| `4_Flow_Matching/Maximum_Weight_Matching.cpp` | `graph/general_weighted_matching` **AC** 24 筆 · 0.336s/5s (7%) | — | 需要外部 `INF` | test by Library Checker General Weighted Matching |
| `4_Flow_Matching/SW-mincut.cpp` | — 無對應題 | 30 000 組 n≤11 vs 2^n 枚舉 | ✅ | — |
| `4_Flow_Matching/BoundedFlow.cpp` | — 無對應題 | 30 000 組 vs 枚舉整數流（可行/最大/最小，流量可為負）（`init` 補清匯點只是清理，不影響答案） | 需要外部 `N`、`INF` | Maximum boundedflow test by LOJ 116; Minimum boundedflow test by LOJ 117; Dinic test by NTUJ 184; local brute force (flows of either sign)；2026-11-25 縮短（80 → 67 行，行為與速度不變） |
| `4_Flow_Matching/Gomory_Hu_tree.cpp` | — 無對應題 | 6 000 組 n≤8，全點對 min cut（**修越界**） | 需要外部 `MaxFlow` | test by LOJ 2042 |
| `4_Flow_Matching/MinCostCirculation.cpp` | `graph/min_cost_b_flow` 無法驅動 | 23 000 組（含負環）vs 枚舉 / cycle-cancelling | 需要外部 `N` | test by uoj 487 |

## String

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `5_String/KMP.cpp` | — 無對應題 | 200 000 組 σ≤3（空 pattern 不可） | 需要外部 `MAXN` | — |
| `5_String/Z-value.cpp` | `string/zalgorithm` **AC** 29 筆 · 0.040s/5s (1%) | — | 需要外部 `SIZE` | test by Lib-Checker Z Algorithm |
| `5_String/RollingHash.cpp` | — 無對應題 | 400 個字串（σ ≤ 3）的所有等長子字串對（220 萬對）：雜湊相等 ⇔ 子字串相等；`string` 與 `vector<int>` 一致 | ✅ | 2026-11-17 加入（mod 2^61−1、隨機底數） |
| `5_String/Manacher.cpp` | `string/enumerate_palindromes` **AC** 24 筆 · 0.068s/5s (1%) | — | 需要外部 `SIZE` | test by Lib-Checker Enumerate Palindromes |
| `5_String/SA_LCP_becaido.cpp` | `string/suffixarray#becaido` **AC** 50 筆 · 0.167s/5s (3%) | — | ✅ | test by caido, Lib-Checker Suffix Array |
| `5_String/SAIS-C++20.cpp` | `string/suffixarray` **AC** 50 筆 · 0.181s/5s (4%) | σ≤3 窮舉 + 隨機 n≤3000，共 30 252 組 | ✅ | 2026-11-22 書中改用 SAIS.cpp（註解掉） |
| `5_String/SAIS.cpp` | `string/suffixarray#sais` **AC** 50 筆 · 0.064s/5s (1%) | σ≤3 窮舉 + 隨機 n≤3000，共 30 252 組；另 10 萬組 vs 暴力排序 | ✅ | 2026-11-22 改寫：教科書式 SA-IS（S/L 型、LMS、induce、命名遞迴），不用 C++20 ranges；n = 5e6 速度與 SAIS-C++20 相同 |
| `5_String/LCP_Array.cpp` | `string/number_of_substrings#lcp` **AC** 24 筆 · 0.065s/5s (1%) | 30 000 組 n≤2000（含週期串）vs 直接比相鄰後綴 | ✅ | SA 由 SAIS 產生；2026-11-17 從 SA_LCP_becaido 拆出（倍增 SA 刪掉） |
| `5_String/Aho-Corasick_Automaton.cpp` | `string/aho_corasick` **AC** 72 筆 · 0.369s/5s (7%) | `fail[]` vs 最長後綴節點、沿 `to[]` 累加 `cnt[]` vs 樸素出現次數、`count()` vs 每個節點字串的樸素出現次數，100 000 組（含 `reset()` 重用） | 需要外部 `N`、`C`；字元以 `'a'` 為基底；根是 0 | std_abs 原檔 + macro 契約（2026-09-30 取代舊版，檔名改正為 Automaton）；`cnt[]` 在 `build_fail()` 後是「以該節點結尾的 pattern 數」；另加 `count(s)` 回傳每個節點字串在 `s` 裡的出現次數（取代舊版的 `ord[]`/`solve()`） |
| `5_String/MinimumRotation.cpp` | — 無對應題 | 79 523 組（σ=3 n≤9 窮舉 + 週期串） | ✅ | test by CSES Minimal Rotation |
| `5_String/LyndonFactorization.cpp` | `string/lyndon_factorization` **AC** 23 筆 · 0.032s/5s (1%) | 79 524 組（σ=3 n≤9 窮舉 + 週期串），依定義檢查：接起來是 s、每段嚴格小於自己的真後綴、各段不遞增 | ✅ | std_abs 原檔 + macro 契約（Duval，2026-09-30 加入） |
| `5_String/De_Bruijn_sequence.cpp` | — 無對應題 | 416 組 (C,N,K)，每個長 N 字只出現一次 | ✅ | test by CF 102001 C |
| `5_String/exSAM.cpp` | `string/number_of_substrings` **AC** 24 筆 · 0.065s/5s (1%) | — | 需要外部 `N` | test by CF 616 C, Lib-Checker Number of Substrings |
| `5_String/PalTree.cpp` | `string/eertree` **AC** 24 筆 · 0.171s/5s (3%) | `process()`：3 000 組最小回文分割 vs O(n³) 暴力（另驗 `cnt`）、3 000 組 CF 906E（driver 直接走 `diff`/`slink`）vs 區間暴力（含套用反轉驗證）、n = 5e5 計時 | ✅ | test by APIO 2014 palindrome, Lib-Checker Eertree；2026-11-25 加 series link 與 `process()` |
| `5_String/MainLorentz.cpp` | `string/runenumerate` **AC** 24 筆 · 0.165s/5s (3%) | — | 需要外部 `kN` | test by Lib-Checker Run Enumerate |

## Math

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `6_Math/exgcd.cpp` | — 無對應題 | 204 萬組含負數與 ±1e18；`a >= 0, b > 0` 時 `|x| <= b / g` | ✅ | test by NTUJ 110 (old version), local brute force |
| `6_Math/floor_ceil.cpp` | — 無對應題 | 101 萬組含 INT 邊界 | ✅ | — |
| `6_Math/floor_enumeration.cpp` | `number_theory/enumerate_quotients` **AC** 26 筆 · 0.121s/5s (2%) | — | 片段，非獨立編譯單元 | test by Lib-Checker Enumerate Quotients |
| `6_Math/ModMin.cpp` | `number_theory/min_of_mod_of_linear` **AC** 17 筆 · 0.265s/10s (3%) | 1 734 355 組 vs 暴力（m ≤ 60 窮舉，88 406 組無解） | ✅ | test by Lib-Checker Min of Mod of Linear |
| `6_Math/Gaussian_gcd.cpp` | `number_theory/gcd_of_gaussian_integers` **AC** 12 筆 · 0.065s/5s (1%) | — | 需要外部 `cpx` | test by Lib-Checker GCD of Gaussian Integers |
| `6_Math/NimProduct.cpp` | `number_theory/nim_product_64` **AC** 10 筆 · 0.365s/10s (4%) | Turning Corners 32×32 的 SG 暴力表（Tartan）、2 萬組 64 位元隨機值的體公理（交換、結合、對 xor 分配、a^(2^64−1) = 1） | ✅ | 2026-11-17 加入 |
| `6_Math/Miller_Rabin.cpp` | `number_theory/factorize` **AC** 31 筆 · 1.167s/10s (12%)<br>`number_theory/primality_test` **AC** 12 筆 · 0.265s/5s (5%) | 經 Pollard_Rho 的對拍（每個因數都要通過 `prime()`） | ✅ | test by NTUJ 1237, Lib-Checker Primality Test |
| `6_Math/Linear_Equations.cpp` | `linear_algebra/system_of_linear_equations` **AC** 27 筆 · 0.515s/5s (10%) | 有理數、modint 各 3 000 組，驗 A·sol = b 且 basis 在核空間（1 239 + 1 322 組無解） | 需要外部 `MAXN` 與域型別 `T` | test by Lib-Checker System of Linear Equations (modint), local brute force (fraction) |
| `6_Math/DeterminantAnyMod.cpp` | `linear_algebra/matrix_det_arbitrary_mod` **AC** 20 筆 · 0.165s/5s (3%)<br>`linear_algebra/matrix_det` **AC** 25 筆 · 0.165s/5s (3%) | — | ✅ | 2026-11-17 收進書（原 `Determinant.cpp` 改寫：回傳行列式而非交換次數，m 任意） |
| `6_Math/CharPoly.cpp`（不在書中） | `linear_algebra/characteristic_polynomial` **AC** 28 筆 · 0.265s/10s (3%) | — | ✅ | 2026-11-17 加入（Hessenberg，O(n³)）；2026-11-21 拿出書（ICPC 少用：$A^k$ 用 BM、$\det(A+xB)$ 用插值） |
| `6_Math/XorBasis.cpp` | — 無對應題 | 3 000 組、55 093 個區間查詢 vs 枚舉子集：每個前綴一份 basis 的區間最大 xor（含起始值 x）、整個 span 的第 k 小 | ✅ | 2026-11-17 加入（插入時間戳記版） |
| `6_Math/Pollard_Rho.cpp` | `number_theory/factorize` **AC** 31 筆 · 0.069s/10s (1%)（2026-11-17 改 kactl 式批次 gcd，原本 1.167s） | 24 萬組：n ≤ 2·10⁵ 窮舉 vs 試除法、2ᵏ·p、隨機至 10¹⁸、兩個約 10⁹ 質數之積、質數平方與立方、Carmichael 數、連續呼叫累加 | 需要外部 `prime` | test by Lib-Checker Factorize |
| `6_Math/Simplex_Algorithm.cpp` | — 無對應題 | 12 000 組 vs 頂點枚舉（最佳/無界/不可行）+ 275 組對偶 | 需要外部 `MAXN`、`MAXM`、`INF` | 2026-11-18 改寫（見最後一節） |
| `6_Math/chineseRemainder.cpp` | — 無對應題 | 724 575 組含非互質、無解、lcm 到 4e18（大 gcd、單一模數近 4e18）（**修溢位**） | 需要外部 `exgcd` | — |
| `6_Math/fac_no_p.cpp` | `enumerative_combinatorics/binomial_coefficient` 無法驅動 | 44 960 組 vs 直接計算 | 需要外部 `MAXP` | test by luogu P4720 |
| `6_Math/QuadraticResidue.cpp` | `number_theory/sqrt_mod` **AC** 13 筆 · 0.064s/10s (1%) | 289 050 組（p < 2000 每個 a 窮舉 + 大質數植入解） | ✅ | test by Lib-Checker Sqrt Mod, local brute force |
| `6_Math/PiCount.cpp` | `number_theory/counting_primes` **AC** 31 筆 · 0.033s/5s (1%) | — | ✅ | test by luogu P7884, Lib-Checker Counting Primes |
| `6_Math/DiscreteLog.cpp` | `number_theory/discrete_logarithm_mod` **AC** 20 筆 · 0.264s/10s (3%) | 585 220 組（m ≤ 120 窮舉 + 植入解到 1e9；443 296 組無解） | ✅ | test by Lib-Checker Discrete Logarithm |
| `6_Math/Berlekamp-Massey.cpp` | `other/find_linear_recurrence` **AC** 18 筆 · 0.214s/5s (4%) | 經 FastLinearRecursion 的對拍（BM 的結果要能重播整段數列）；與舊版比 200 000 組任意數列（含大量 0）：長度相同、新版都能重播 | ✅ | 照 kactl 改成 `int` + `add`/`sub`/`mul`/`Pow`（2026-11-21）；test by Lib-Checker Find Linear Recurrence |

## Polynomial

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `7_Polynomial/Fast_Fourier_Transform.cpp` | `convolution/convolution_mod_1000000007` **AC** 48 筆 · 0.369s/10s (4%) | `conv()`：500 組 vs 樸素卷積，最大誤差 1.5e-8（\|a_i\| ≤ 1e3）、1.1e-5（\|a_i\| ≤ 3e4） | ✅ | test by Lib-Checker Convolution mod 1e9+7 (through FFT Mod) |
| `7_Polynomial/Fast_Fourier_Transform_Mod.cpp` | `convolution/convolution_mod_1000000007` **AC** 48 筆 · 0.369s/10s (4%) | — | 需要外部 `cplx` | test by Lib-Checker Convolution mod 1e9+7 |
| `7_Polynomial/Number_Theory_Transform.cpp` | `convolution/convolution_mod` **AC** 53 筆 · 0.379s/5s (8%) | — | 需要外部 `MOD`/`G`/`N` 和 `add`/`sub`/`mul`/`Pow`；可用的 (mod, G, 2^k) 見下面的 NTT Primes 表 | std_abs 原檔 + macro 契約；test by Lib-Checker Convolution |
| `7_Polynomial/Fast_Walsh_Transform.cpp` | — 無對應題 | 書上三行 OR/AND/XOR 各留一行產生三份（`.pre.sh`）：各驗封閉式、逆變換、卷積定理；`subset_convolution` 3 000 組 mod 1e9+9 vs 樸素 $O(4^L)$，L = 0..6、值到 MOD − 1；L = 18 抽查 | 需要外部 `MOD`、`add`、`sub`、`mul` | test by luogu P6097, local brute force |
| `7_Polynomial/Polynomial_Operation.cpp` | `polynomial/inv_of_formal_power_series` **AC** 25 筆 · 0.418s/10s (4%)<br>`polynomial/exp_of_formal_power_series` **AC** 26 筆 · 1.619s/10s (16%)<br>`polynomial/log_of_formal_power_series` **AC** 25 筆 · 0.668s/10s (7%)<br>`polynomial/pow_of_formal_power_series` **AC** 37 筆 · 2.227s/10s (22%)<br>`polynomial/sqrt_of_formal_power_series` **AC** 35 筆 · 1.120s/10s (11%)<br>`polynomial/polynomial_taylor_shift` **AC** 38 筆 · 0.268s/5s (5%)<br>`polynomial/shift_of_sampling_points_of_polynomial` **AC** 32 筆 · 0.668s/5s (13%)<br>`polynomial/compositional_inverse_of_formal_power_series` **AC** 23 筆 · 0.215s/10s (2%)<br>`polynomial/compositional_inverse_of_formal_power_series_large` **AC** 28 筆 · 3.674s/10s (37%) | `Divide`（$bq+r=a$）500 組、`Evaluate` vs Horner 300 組、`Interpolate` 來回 300 組、`TaylorShift` vs 直接代入 300 組、`SamplingShift` vs Lagrange 200 組、`power_proj` vs $O(n^2m)$ 樸素 250 組、`PolyPow` vs 連乘 3 000 組（含 $k = 0$、$a_0 = 0$）、`Sqrt` 平方回去 1 073 組、`CompInv` $f(g(x)) = x$ 樸素合成 20 000 組（$n$ = 1..40） | 需要外部 NTT、`QuadraticResidue`；`PolyPow` 要 `Pow(int, ll)`；`TaylorShift`/`SamplingShift` 要 `fac[]`/`ifac[]` | 從 std_abs 改短（2026-11-20） |
| `7_Polynomial/FastLinearRecursion.cpp` | `other/kth_term_of_linearly_recurrent_sequence` **AC** 20 筆 · 3.573s/10s (36%) | BM → BostanMori 重播整段前綴，290 組（d = 1..20）共 9 166 項 | 需要外部 `Mul`、`ntt`（`Polynomial_Operation`、`Number_Theory_Transform`） | Bostan–Mori（2026-09-28 取代 std_abs 的 Kitamasa，原本 TLE 19.442s）；`c` 是 **0-based**，BM 的輸出可直接餵入 |
| `7_Polynomial/Lagrange_Interpolation.cpp` | — 無對應題 | 500 000 次求值 vs Horner（$d \le 300$；$x$ 含 $0..d$、近 $p$、到 $10^{18}$、負數；平移起點 $y_i = f(a+i)$ 查 $x - a$）、62 031 個 $\sum i^k$ vs 直接累加；$d = 10^6$ 0.013s | 需要外部 `MOD`/`add`/`sub`/`mul`、`ifac[]` 到 $d$ | 2026-11-22 取代 Value Polynomial；test by CF 622F（使用者送出 AC） |
| `7_Polynomial/Value_Poly.cpp`（不在書中） | — 無對應題 | 431 189 組 vs 前綴和表 | 需要外部 `mint` | — |

## Geometry

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `8_Geometry/Default_code.cpp` | — 無對應題 | projection/reflection/linearTransformation 30 萬組 vs complex | 片段，非獨立編譯單元 | — |
| `8_Geometry/PointSegDist.cpp` | — 無對應題 | 400 000 組 vs long double 投影 | ✅ | test by aizu oj CGL_2_D |
| `8_Geometry/Heart.cpp` | `geo/minimum_enclosing_circle` **AC** 25 筆 · 0.033s/5s (1%) | 294 534 個三角形（外心/內心/重心/垂心性質） | ✅ | circenter test by Lib-Checker Minimum Enclosing Circle |
| `8_Geometry/point_in_circle.cpp` | — 無對應題 | 686 947 組，exact `__int128` 參考，含 |coord| ≤ 1e9 與恰好共圓（p 須 CCW） | ✅ | — |
| `8_Geometry/Convex_hull.cpp`（不在書中） | `geo/static_convex_hull` **AC** 25 筆 · 0.168s/5s (3%) | — | ✅ | test by Zerojudge b398, Lib-Checker Static Convex Hull；2026-11-24 拿出書（版面），Minkowski Sum、minMaxEnclosingRectangle 仍呼叫 `hull()` |
| `8_Geometry/FarthestPair.cpp` | `geo/furthest_pair` **AC** 46 筆 · 0.366s/5s (7%) | 29 975 個凸包（座標到 1e9、含共線）vs O(n²) | 需要外部 `pll` primitives | 2026-11-17 加入（旋轉卡尺）；注意 `hull()` 對「全部同一點」會回傳重複點，driver 先去重 |
| `8_Geometry/PointInConvex.cpp` | — 無對應題 | 1 988 萬筆查詢（不可有共線頂點），含 |coord| ≤ 1e9、凸包到 95 786 點 | ✅ | test by CF 104114 B, CF 101242 J (World Finals' problem) |
| `8_Geometry/PointInPolygon.cpp` | — 無對應題 | 131 萬筆查詢，隨機星形簡單多邊形（兩種方向），含 |coord| ≤ 1e9、n 到 1e5，vs exact 繞數與邊上判斷 | 需要外部 `pll` primitives | 2026-11-17 加入（射線法，回傳 1 / 0 / −1） |
| `8_Geometry/TangentPointToHull.cpp` | — 無對應題 | 537 397 筆查詢，座標到 ±1e9；其中 21.5 萬筆 p 在頂點、8.9 萬筆在邊上（2026-11-17 起可在凸包上） | 需要外部 `cyc_tsearch` | test by CF 104114 B, CF 101242 J (World Finals' problem) |
| `8_Geometry/Intersection_of_line_and_convex.cpp` | — 無對應題 | 558 997 條線，含過頂點/貼邊 | 需要外部 `cyc_tsearch` | test by aizu CGL_4_C, probably not enough |
| `8_Geometry/minMaxEnclosingRectangle.cpp` | — 無對應題 | 2 989 組（**修 bug**：acos NaN 丟掉最大值，改封閉式） | 需要外部 `hull` | test by UVA 819 |
| `8_Geometry/Vector_in_poly.cpp` | — 無對應題 | 1 832 940 組含凹角/180°，其中 904 740 組 |coord| ≤ 1e9 | ✅ | test by qoj 5479 |
| `8_Geometry/PolyUnion.cpp` | — 無對應題 | 20 000 組 vs 精確 slab 分解（含重複多邊形） | ✅ | test by CF 101673 A |
| `8_Geometry/Polar_Angle_Sort.cpp` | `geo/sort_points_by_argument` **AC** 21 筆 · 0.117s/5s (2%) | — | ✅ | test by NTUJ 2270, Lib-Checker Sort Points by Argument |
| `8_Geometry/Half_plane_intersection.cpp` | — 無對應題 | 56 016 組 vs exact 有理數暴力（含空/零面積），n 到 1e5 | 需要外部 `cmp`；書中 double 前置只到 |coord| ≈ 3.3e7（1e9 會把近平行線當平行），整數前置到 1e9 | test by qoj 2162 (World Finals' problem) |
| `8_Geometry/HPIGeneralLine.cpp` | — 無對應題 | 56 012 組 vs exact 有理數暴力，格點直線到 1e9、n 到 1e5 | 需要外部 `cmp`；書中 double 前置只到 ≈ 3.3e7；`cov()` 直接比較 `__int128`（2026-11-22 修：原本 `sign(ll)` 會截斷） | — |
| `8_Geometry/rotatingSweepLine.cpp` | — 無對應題 | 2 173 組一般位置（前提：無三點共線） | 需要外部 `cmp` | — |
| `8_Geometry/Minimum_Enclosing_Circle.cpp` | `geo/minimum_enclosing_circle` **AC** 25 筆 · 0.033s/5s (1%) | — | 需要外部 `circenter` | test by TIOJ 1093, Lib-Checker Minimum Enclosing Circle |
| `8_Geometry/Intersection_of_two_circles.cpp` | — 無對應題 | 398 738 組含精確相切（同圓回傳 NaN） | ✅ | test by TIOJ 1503；2026-11-25 函式 `CCinter` 改名 `circleInter` |
| `8_Geometry/Intersection_of_polygon_and_circle.cpp` | — 無對應題 | 99 364 組 vs kactl（**修 NaN**：改 atan2） | ✅ | test by HDU 2892 |
| `8_Geometry/Intersection_of_line_and_circle.cpp` | — 無對應題 | 299 352 組 | ✅ | test by kactl stress-tests |
| `8_Geometry/Tangent_line_of_two_circles.cpp` | — 無對應題 | 300 000 組 | ✅ | — |
| `8_Geometry/CircleCover.cpp` | — 無對應題 | 1 500 組 vs 數值積分（修未定義 `pi`） | 需要外部 `circleInter` | test by TIOJ 1503；2026-11-24 改寫成函式 `circleCover(vector<Cir>)` 回傳 `a[k]`（58 → 36 行） |
| `8_Geometry/3Dpoint.cpp` | — 無對應題 | 200 000 組 | ✅ | test by HDU 3662 |
| `8_Geometry/Convexhull3D.cpp` | — 無對應題 | 8 693 個凸包：|coord| ≤ 65 000 vs O(n⁴) 暴力，n ≤ 2000 驗結構（**修編譯錯誤**） | 需要外部 `Point`；double、eps 1e-8，|coord| ≤ 6.5e4 才保證精確（1e6 會錯） | test by HDU 3662, Ptz. Summer 2023 olmrgcsi And His Friends' Contest K；2026-11-24 縮短（66 → 59 行，介面不變） |
| `8_Geometry/DelaunayTriangulation_dq.cpp` | — 無對應題 | 5 012 組 exact 驗空圓性質，n 到 1e5、近共圓（需外部 `seg_strict_intersect`） | 需要外部 `N`；書中 double 前置只到 |coord| ≈ 3.3e7，整數前置到 1e9 | test by Zerojudge b370, TIOJ 1310 |
| `8_Geometry/Triangulation_Vonoroi.cpp` | — 無對應題 | 1 902 組 / 239 202 個 cell vs 全部中垂線裁切，n 到 1e5 | 需要外部 `tool`；書中 double 前置只到 |coord| ≈ 3.3e7，整數前置到 1e9 | test by CF 104555 J, NPSC 2018 territory |
| `8_Geometry/Minkowski_Sum.cpp` | — 無對應題 | 9 735 組 vs 所有和的凸包（hull 不可退化），含 101 組 |coord| ≤ 7.5e8、1e5 點 | 需要外部 `hull`；輸入 |coord| ≤ 759 250 124（內部 `hull(C)` 的 cross 會溢位） | test by Zerojudge b398 |
| `8_Geometry/ClosestPair.cpp` | `geo/closest_pair` **AC** 29 筆 · 0.467s/5s (9%) | — | ✅ | test by Lib-Checker Closest Pair |

## Else

| 模版 | Library Checker | 本地對拍 | 單檔編譯 | 書上標註 |
|---|---|---|---|---|
| `9_Else/cyc_tsearch.cpp` | — 無對應題 | 100 000 組 | ✅ | test by local brute force |
| `9_Else/Mos_Algorithm_With_modification.cpp` | — 無對應題 | 3 000 組 6 萬筆查詢（**原本編不過** + `R = -1`） | 需要外部 `blk` | — |
| `9_Else/Mos_Algorithm_On_Tree.cpp` | — 無對應題 | 3 000 棵樹 6 萬筆查詢（**原本編不過** + `R = -1`） | 需要外部 `LCA` | — |
| `9_Else/HilbertCurve.cpp` | — 無對應題 | n≤2^10 窮舉 + 2^30 隨機 | ✅ | — |
| `9_Else/Josephus.cpp` | — 無對應題 | n ≤ 300、k ≤ 400 vs 模擬；n ≤ 1e7 vs O(n) 遞迴；n 到 1e18（k ≤ 1000）vs 獨立實作與 k = 2 公式，共 123 300 組 | k > n 時遞迴深 n：n ≈ 3e5 會爆棧 | 2026-11-17 加入 |
| `9_Else/DynamicConvexTrick.cpp` | `data_structure/line_add_get_min` **AC** 15 筆 · 0.167s/5s (3%) | — | ✅ | test by TIOJ 1921, Lib-Checker Line Add Get Min |
| `9_Else/All_LCS.cpp` | `string/prefix_substring_lcs` **AC** 11 筆 · 0.115s/5s (2%) | — | ✅ | test by Lib-Checker Prefix-Substring LCS |
| `9_Else/DLX.cpp`（不在書中） | — 無對應題 | — | 需要外部 `NN` | test by TIOJ 1333, 1381 |
| `9_Else/MatroidIntersection.cpp` | — 無對應題 | 3 000 組（圖擬陣 ∩ 分割擬陣，n ≤ 7、m ≤ 12）vs 2^m 窮舉 | ✅ | test by local brute force (graphic x colourful) |
| `9_Else/AdaptiveSimpson.cpp` | — 無對應題 | 40 000 個積分 | ✅ | test by CF 101193J, 100553D |
| `9_Else/simulated_annealing.cpp` | — 無對應題 | — | 片段，非獨立編譯單元 | — |
| `9_Else/tree_hash.cpp` | `tree/rooted_tree_isomorphism_classification` **AC** 17 筆 · 0.166s/5s (3%) | — | 需要外部 `G`、`N` | test by CSES Tree Isomorphism I, Lib-Checker Rooted Tree Isomorphism Classification |
| `9_Else/BinarySearchOnFraction.cpp` | `number_theory/rational_approximation` **AC** 28 筆 · 0.123s/5s (2%) | 4 680 組 N ≤ 30 窮舉 + 10 萬組 N ≤ 1e18（Farey 相鄰檢查） | ✅ | test by ABC 333 G after adjusted to int128, Lib-Checker Rational Approximation, local brute force |
| `9_Else/1d1d.cpp` | — 無對應題 | 區段 deque 版：convex、concave 各 20 000 組 vs O(n²)，n ≤ 400、大量平手 | 需要外部 `N`、`w` | — |
| `9_Else/smawk.cpp` | `convolution/min_plus_convolution_convex_arbitrary#smawk` **AC** 41 筆 | 30 000 組完全單調矩陣求每列最小值 vs 暴力（Monge 型 + min-plus 斜帶） | ✅ | test by Lib-Checker Min Plus Convolution (Convex and Arbitrary) |
| `9_Else/min_plus_convolution.cpp` | `convolution/min_plus_convolution_convex_arbitrary` **AC** 41 筆 · 0.119s/5s (2%) | 15 000 組 vs 暴力與 SMAWK 交叉比對（在 SMAWK 的對拍裡） | 需要外部 `INF` | test by Library Checker Min Plus Convolution (Convex and Arbitrary) |
| `9_Else/BitsetLCS.cpp` | — 無對應題 | 10 000 組 vs O(nm) DP（需自備 bitset 型別） | 需要外部 `cin` | — |

---

## 總計

- 收錄模版：**114**（上面各章主表中在書裡的列；不含 `typo_check.sh`、`stress.sh`、`Primes`、`misc.py`）
- 有 Library Checker AC 的：**54**
- 有本地對拍的：**88**（主表「本地對拍」欄有內容的列；`verify/stress/` 共 92 個測試）
- 兩種自動測試都沒有的：**4**（black_magic、Pragma、sanitize_fixer、simulated_annealing；都是片段或工具檔）

## 2026-09-27：stress test + 排版

- `verify/stress/`：kactl 式對拍（純 `main` + `assert`）。`stress/run.sh` 跑全部；預設 ASan+UBSan+`_GLIBCXX_DEBUG`。
- **harness 修正**：`geo_prelude.h` 原本寫死 master 的 `Default_code.cpp`，對其他 checkout 測試時幾何章只換了模版、沒換 prelude。harness 搬進 codebook repo 後路徑都是 repo 內相對路徑，這個問題不再存在。
- 後補的對拍：`Math/Pollard_Rho`（24 萬組）、`Basic/readchar`（400 萬 byte 跨 2^16 邊界）、`Geometry/Default_code`（projection/reflection/linearTransformation）；後兩者以植入錯誤確認會 FAIL。
- 對拍抓到 13 個 bug，已合併（PR #4）；上表標 **修** 的就是。
- PDF：listings 一行 58 欄，超過就折行。原本 123 行會折，現在 0 行；另外把放得下的「header + 單一敘述」接回一行（純空白、hash 不變，獨立 commit）。**26 → 25 頁，最後一頁是滿的。**
- 書裡仍缺、刻意沒補（不加新行）：`seg_strict_intersect`（Delaunay 要用）、`abs(pll)`（minMaxEnclosingRectangle）、BitsetLCS 需要的 bitset 型別。

## 2026-09-28：補回對拍檔

- 表中 15 個模版的「本地對拍」原本只有紀錄、沒有檔案，這次補成 `verify/stress/` 裡的測試，表中數字改成實際輸出：ModMin、Linear_Equations、DiscreteLog、LiChaoST、FFT、FWT、Operation_stdabs、FastLinearRecursion_stdabs、Polynomial_Operation、2SAT、MinimumSteinerTree、Bipartite_Matching、Kuhn_Munkres、MincostMaxflow、MatroidIntersection。
- **修 bug**：`MincostMaxflow` 的 `path()` 沒有重設 `par[s]`，同一物件 `init()` 後重用會沿舊邊回溯（算錯或無窮迴圈）。
- 補記先前已有檔案但表中沒寫的對拍（EBCC、VBCC、min_heap、Pollard_Rho、1d1d 等），`（不在書中）` 標註不在 `content.tex` 的列，總計重算。
- `FastLinearRecursion_stdabs.cpp` 換成 Bostan–Mori，改名 `FastLinearRecursion.cpp`、函式 `BostanMori`（參數不變）：kth_term 從 **TLE** 19.442s 變成 **AC** 3.573s；每步共用 Q 的 NTT，只做 4 次轉換。
- `Berlekamp-Massey.cpp` 改回傳 **0-based** 的 `c`（刪掉前面補的 `c[0]`），可以直接餵給 `FastLinearRecursion`；支線 `LinearRecursion` 仍是 1-based。

## 2026-09-30：AC 自動機換成 std_abs

- `Aho-Corasick_Automatan.cpp` 換成 std_abs 的 `AC`（套 macro 契約），檔名改正為 `Aho-Corasick_Automaton.cpp`：需要 `N`、`C`，根是 0，字元以 `'a'` 為基底，`insert` / `build_fail` / `reset`。Library Checker **AC** 0.369s，速度和舊版相同（同一批輸入逐一執行，最大測資兩者都約 0.16s）。
- `cnt[]` 在 `build_fail()` 後沿 fail 累加成「以該節點結尾的 pattern 數」，走 `to[]` 加總就是總出現次數（std_abs 註解掉的 `match` 縮成一行註解）。
- 在 std_abs 原檔之外加了 `ord`（BFS 順序）和 `count(s)`：回傳 `occ[v]` = 節點 `v` 的字串在 `s` 裡的出現次數，pattern 的 `v` 就是 `insert()` 的回傳值；對應舊版的 `ord[]` + `solve()`。
- 新增本地對拍 100 000 組（fail、match、count）。
- 之後依使用者修改：`reset()` 移到最前面並一併清空 `ord`（`build_fail()` 不再清），所以兩次 `build_fail()` 之間要 `reset()`（`cnt` 本來就不能重複累加）。

## 2026-09-30：加入 Lyndon Factorization

- 新增 `5_String/LyndonFactorization.cpp`（std_abs 的 `duval`，只把 `sz(s)` 換成 `s.size()`）。Library Checker **AC** 0.032s，本地對拍 79 524 組，PDF 仍是 25 頁。
- `Smallest_Rotation.cpp` 改名 `MinimumRotation.cpp`，函式 `mcp` 改名 `min_rotation`（只改名字）。

## 2026-09-30：數學章補充（依 review 的決定）

- **新增理論**（公式都先用暴力程式驗過小範圍）：`Theorem.tex` 加 BEST、Tutte–Berge、Dilworth/Mirsky、Graph facts（Hall、Gallai、Dirac/Ore）、Burnside（項鍊、手環）、Labeled graph counts、Hook length、Fibonacci、Generalized Euler、Landau、Misère Nim、Sum of squares；`numbers.tex` 加第一類 Stirling、Bell、Narayana。
- **新增程式**：`2_Graph/C3C4.cpp`（三角形列舉、四環計數，O(M√M)）；`1_Basic/stress.sh`（對拍迴圈）。
- **移除**：Cramer's rule、Spherical Coordinate、Rotation Matrix（幾何章已有對應程式）。
- **修正**：Erdős–Gallai 的求和下標、Du's sieve 的 `f(i)` 與 `N/i`、Tutte's matrix 的定義、Kirchhoff 的用詞、Spherical cap 的 θ 改成 `cos θ = (r−h)/r`、Lagrange multiplier 改寫成一般形式。Erdős–Gallai、Gale–Ryser、Fulkerson–Chen–Anstee、Landau 合併成 Degree sequences 一條。
- 備選四項（Narayana、Generalized Euler、Landau、Misère Nim）在拿掉 Pell 之後都放得下，全部加入。HPI Alternative Form 等隊伍確認後再決定，沒有動。
- Pell equation（定理條目和 `misc.py` 的 `pell(D)`）加了之後又拿掉。PDF 25 頁。

## 2026-10-01：數學章後續修改（PR #19 內，上一節沒記到的）

- **內容修正**：Min-Max inclusion–exclusion 的求和改成非空子集 $\emptyset \ne T \subseteq S$；Bernoulli 統一成 $B_1 = +\tfrac12$ 一種慣例（遞迴式改為 $\sum_{j\le m}\binom{m+1}{j}B_j = m+1$，對所有 $m \ge 0$ 成立；EGF 改為 $x/(1-e^{-x})$）。原本的遞迴式和 EGF 只在 $B_1=-\tfrac12$ 時成立，遞迴式在 $m=0$ 也不成立。
- **使用者改寫並補充**（都已查證或驗證）：BEST 加上「從 $s$ 出發的乘 $d(s)$」；Tutte matrix 註明錯誤率 $\le n/p$、只會低估；Dirac/Ore 加 Palmer $O(n^2)$ 構造；Pick 加 $B=\sum\gcd$ 與有洞版本；Hook length 加 $2\times n$ 為 Catalan；Pisano 註明純週期；Misère Nim 標題註明規則；導數表加 $\int x\sin ax$。
- **排版**：Degree sequences 有上下限的求和用 `\displaystyle`；skew lines 改成左右兩欄列點並保留在書中；Bell 的 EGF 寫成 $\exp(e^x-1)$；`numbers.tex` 各條公式改成列點，Eulerian、Narayana 改寫；為了放下列點，Newton's method 的 $F(x)$ 改成行內。PDF 仍是 25 頁且沒有餘裕。

## 2026-11-17：待辦清單

- **移出 PDF（檔案保留，content.tex 註解掉）**：`KDTree`、`BIT_kth`、`Heavy_light_Decomposition`、倍增 SA（`SA_LCP_becaido`）、`RollingHash`、`FarthestPair`；LCP 拆成 `5_String/LCP_Array.cpp`，SA 改由 SAIS 提供。
- **TangentPointToHull**：p 可以在凸包上（共線時當作 p 往凸包外移一點來決勝負）。
- **MincostMaxflow**：Flow Models 加「有負環的最小費用流」（先把負邊流滿），本地窮舉驗證。
- **Pollard rho**：kactl 式批次 gcd，Factorize 1.167s → 0.069s。
- **Link cut tree**：改陣列版（0.56s → 0.33s），順便修了 `cut` 的 bug（見表）。
- **新增程式**：Manhattan MST、Bipolar Orientation、Static Top Tree、SIMD、Nim Product、Determinant Any Mod、Characteristic Polynomial、Xor Basis（都有 LC 或本地對拍）。
- **對照 ckiseki 補的缺口**：Point In Polygon、Josephus（另寫了 Euler Trail、Rolling Hash、Farthest Pair，太基本，只留檔案；Xor Basis 同）。
- **理論部分**（Game Theory、Linear Algebra、Polynomial Facts、String and Automaton Facts、Theorem 圖論補充）不在這次合併，之後重寫再加。

## 2026-11-18：模板改寫與修 bug

- **QuadraticResidue**：Cipolla 重寫，拿掉 Jacobi（Euler 判別法），37 → 21 行。
- **PiCount**：只整理排版（early continue、合併宣告），演算法不變；n = 1e13 0.79s → 0.66s，與舊版逐一比對答案相同。
- **DiscreteLog**：BSGS 重寫成一個函式，用 gcd 判斷 y 在不在循環上，不再需要外部 `fpow`，27 → 19 行。
- **Simplex**：改成跟 System of Linear Equations 一樣的 struct；無解回傳 `-INF`、無界回傳 `INF`（原本都回 -1，跟最佳值 -1 撞），2000×200 隨機 LP 9.4s → 0.17s（Dantzig 規則，pivot 數 44 617 → 937；遇到退化 pivot 改用 Bland 規則防循環，Beale 的例子在對拍裡）。
- **BoundedFlow**（**修 bug**）：`solve(s, t)` 只加 t→s 的 INF 邊，所以只找得到流量 ≥ 0 的可行流；可行流全是負流量時（s 有入邊或 t 有出邊）誤報無解，最小流卻又可能回傳負數。改成 t→s、s→t 各一條 INF 邊；無解改回傳 `-INF`（-1 現在可能是答案）。
- **BoundedFlow** 的 `init` 補清匯點（09-27 那次）：重新檢查後只是清理，殘留的邊都從匯點出發，Dinic 不會走到，不影響答案。
- **BinarySearchOnFraction**：倍增改成標準寫法（原本減半階段失敗一次就停，少走的步數要多兩輪補）；lo 固定為 false 側、hi 為 true 側，輪流移動，拿掉 swap 和 L/H 旗標；每輪 k = 1 就是上一輪失敗的那個分數，不再呼叫 pred；加 pred 範例。N = 1e18 時 pred 次數（平均）：均勻 159 → 125、golden 174 → 172、X = 1 514 → 118。
- **ManhattanMST**：排序比較式改成直接比 x + y（原本是 kactl 的移項寫法，等價）。
- **StaticTopTree**：補一段用法註解（g 是子節點、要定義哪些函式、答案在哪、修改後呼叫 update）。
- 標題拿掉 `*`（程式已不是實際送過的版本）：QuadraticResidue、DiscreteLog、BoundedFlow（Dinic 保留）、BinarySearchOnFraction。

## 2026-11-18：頁首

- 隊名移到右上角（regional 規定），頁碼改到左上角。

## 2026-11-19

- **Fast_Walsh_Transform**：`FOR (j, i, j + (L >> 1) - 1)` 改成 `i + (L >> 1) - 1`。原本不是 bug（`FOR` 展開後 `j` 先初始化成 `i` 才算上界），只是容易看錯；FWT 與 Minimum_Clique_Cover 的對拍都過。
- **ManhattanMST**：「then Kruskal」從開頭註解移到 `return edges;` 那行，不重複。程式碼不變。
- **exgcd**：`ax+by=gcd.cpp` 改名 `exgcd.cpp`，遞迴改成 structured binding、拿掉 `pll` typedef 與「最小非負 x」的註解食譜（exgcd 屬太基礎）；註解只寫 CRT 依賴的條件與界 `|x| <= b / gcd`，對拍加驗這個界。標題去掉 `*`。exgcd、CRT 對拍都過。
- **exgcd / chineseRemainder**：exgcd 改成 `ll exgcd(a, b, &x, &y)` 回傳 gcd，CRT 不再另外呼叫 `gcd`、也不先除 g（兩組輸入的 Euclid 商序列相同，x 一樣，界仍是 `|x| <= m2 / g`）。未收錄的 `ax=b%n.cpp` 跟著改介面，輸出與舊版相同。對拍都過。
- **exgcd**：底部加一行解 `ax = c (mod m)` 的用法：`x * (c / g) + k * (m / g)`。
- **chineseRemainder**：函式名 `solve` 改 `crt`。
- **fac_no_p**：STATUS 補上外部需求 `mpow(a, e, m)`（模板沒定義，對拍自備）。
- **chineseRemainder**：對拍補上 lcm 接近宣稱上限 4e18 的情形（原本模數只到 1e9）；新舊版都過。
- **對拍加大輸入**：12 個只靠本地對拍的模板改測比賽規模（幾何 |coord| 到 1e9、Josephus n 到 1e18、MinimumMeanCycle 權重 1e9），暴力參考全改 exact。發現：Minkowski_Sum 輸入 |coord| > 759 250 124 會在內部 `hull(C)` 溢位（加註解）；HPI / Delaunay / Voronoi 在書中 double 前置下只到 ≈ 3.3e7；HPIGeneralLine 用整數前置時 `sign(ll)` 截斷 `__int128`；Convexhull3D 只到 6.5e4；Josephus 在 k > n 時遞迴深 n。模板程式碼未改。
- **content.tex**：chineseRemainder 移到 exgcd 正下方。
- **排版改橫式三欄**：A4 landscape、`multicols*{3}`、欄距 6pt、頁首距紙邊 15pt；目錄縮成第一欄裡的兩小欄（去掉 Contents 標題）；最後一頁不平衡分欄。Theorem 的微積分表改兩欄；Nim Product 移到 Berlekamp Massey 後（修 Theorem 的怪間距）；readchar 拿出書。`check_layout.py` 上限改 56 欄——目前 52 行超過，定稿前再重排（CI 的排版檢查會先失敗）。目前 26 頁。
- **listings 字寬**：`basewidth=0.55em`（listings 預設 0.6em 是給 Courier 的，Ubuntu Mono 本身 0.5em，原本字距被撐開）。每欄 56 → 61 字，原本 52 行折行全部消失，回到 25 頁；`check_layout.py` 上限改 61，CI 排版檢查恢復通過。

## 2026-11-19（二）

- **Convexhull3D 對拍**：`assert` 內 `edges[{e.S, e.F}]` 的逗號在 CI 的單參數 `assert` 巨集下被拆成兩個參數，編譯失敗；外層加括號。

## 2026-11-20

- **刪掉 NTT 支線**：`Number_Theory_Transform.cpp`、`Polynomial_Operation.cpp`（不在 `content.tex`，已由 `NTT_stdabs`／`Operation_stdabs`／`FastLinearRecursion` 取代），連同 `#ours` 驅動程式、MAP 六列和對拍。
- **Operation_stdabs 縮短**：225 → 203 行，30 000 組小輸入的輸出跟舊版逐位元組相同，LC 7 題 AC、時間不變；`Sqrt` 併掉 `SqrtImpl`、迭代改 $q \gets (q + a/q)/2$（每輪少一次 `Mul`，1.526s → 1.120s）；`Integral` 併進 `Ln`；`Inverse`/`Exp`/`Divide`/`Evaluate`/`Interpolate` 小幅合併；各函式結尾的 hash 重算（原本已經跟程式碼對不上）。
- **PolyPow 修 bug**：$k = 0$ 且 $a_0 = 0$ 時讀到 `b` 的範圍外（`b` 只有 $n - m$ 項），改成 $k = 0$ 直接回傳 1；對拍補上這個情形，舊版會失敗。
- **Evaluate**：結果改回放在新的 `y`，不覆寫輸入的 `x`（每個變數只有一個用途）。
- **SamplingShift 改回獨立版本**：`TaylorShift` 不再帶 `fall` 參數；只需要 SamplingShift 時不必連 TaylorShift 一起抄。
- **新增 CompositionalInverse**（`CompInv`，16 行）：Lagrange 反演，LC 兩題 AC（large 3.674s/10s）；仍是 25 頁。
- **`mod` 改名 `MOD`**（照隊上習慣）：`NTT_stdabs`、`Operation_stdabs`、`CompositionalInverse` 和 `verify/mod_helpers.h`、相關驅動程式與對拍；註解裡數學意義的 mod 不動。
- **檔名去掉 `_stdabs`**：`NTT_stdabs.cpp` → `Number_Theory_Transform.cpp`、`Operation_stdabs.cpp` → `Polynomial_Operation.cpp`（跟同章 `Fast_Fourier_Transform.cpp` 一致）；`CompInv` 併到 `Polynomial_Operation.cpp` 最後，不再單獨成檔，對拍也併進去。
- **註解搬家**：`Pow` 指數要 `ll` 的說明從 NTT 檔頭移到 `PolyPow` 那一行（只有它會傳 `ll`）；NTT 檔頭縮成一行。`fac[]`/`ifac[]` 的說明本來就在 `TaylorShift`。
- **`power_proj`／`CompInv` 註明 $N \ge 4|f|$**：`power_proj` 把 $|f|$ 補到 $2^k$ 後做長度 $2 \cdot 2^k$ 的乘法，NTT 長度到 $4 \cdot 2^k$；$N$ 是 2 的冪，所以 $N \ge 4|f|$ 剛好等價。其他函式只要 $N \ge 2|f|$。

## 2026-11-21

- **Berlekamp–Massey 改寫**：原本是 template `T`，要自己寫支援除法的 `mint`（書裡沒有），而傳 `vector<int>` 會靜靜算錯（整數除法、不取模）。改照 kactl，吃 `vector<int>`、用 NTT 那套 `MOD`/`add`/`sub`/`mul`/`Pow`，回傳 0-based `c` 可以直接餵 `BostanMori`。23 → 22 行；LC AC；對拍的 `mi` 結構拿掉。
- **DeterminantAnyMod**：註解補上 $m \le 3 \cdot 10^9$（`(m - q) * a[j][k]` 接近 $m^2$）。
- **Characteristic Polynomial 拿出書**（`content.tex` 註解掉，檔案與 LC 驗證保留）：ICPC 少用；$A^k$、$u^\mathsf{T}A^kv$ 用 Berlekamp–Massey + BostanMori，$\det(A + xB)$ 用 $n+1$ 個點的 `det` 再插值。

## 2026-11-22
- **HPIGeneralLine 修 bug**：`cov()` 把 `__int128` 傳給 `sign(ll)`，超過 $2^{63}$ 會截斷（|coord| 到 1e9 時近平行線判錯，該空的回傳 4 條）。改成直接比較 `__int128` 的正負，不多佔行。對拍拿掉原本補的 `sign(__int128)`（它把 bug 遮住了），舊版現在會失敗、新版 56 012 組通過。
- **DelaunayTriangulation_dq**：`while (gao(0) || gao(1));` 改成 `{}`，空迴圈主體看得出來是故意的。
- **Value Polynomial → Lagrange Interpolation**：舊版要自己寫支援比較與轉整數的 `mint`、外加 `inegfac[]`；換成 16 行的 `Lagrange(y, x)`（$f(0..d)$ 求 $f(x)$，$O(d)$，`int` + `add`/`sub`/`mul` + `ifac`，跟多項式章一致）。舊檔留著、從書中註解掉。
- **Lagrange**：`x %= MOD` 改成 `x = (x % MOD + MOD) % MOD`，負的 $x$ 也安全（平移起點時呼叫 `Lagrange(y, x - a)` 可能是負數）。
- **Lagrange Interpolation**：使用者在 CF 622F（$\sum_{i=1}^n i^k$，$k \le 10^6$）送出 AC，記進 content.tex 與 STATUS。
- **拿掉所有標題的 `*`**（72 個，含註解掉的）：`*` 原本表示「真的在 OJ 送過」，現在書裡每個模板都有 LC 或對拍，測試紀錄在 content.tex 的 `test by` 註解和這份表裡，`*` 只剩雜訊。
- **SAIS.cpp 改寫**：原本是全域陣列版（含 LCP，沒用到），換成教科書式 SA-IS：輸入尾端唯一最小 0、其餘在 $[1, K)$，回傳 `sa[0] = n - 1`，和 SAIS-C++20 介面相同。型別陣列命名 `ty`（`S` 是巨集）。LC AC 0.064s（C++20 版 0.066s），n = 5e6 兩者 0.448s 相同。還沒放進書。
- **SAIS.cpp**：`same()` 只比字元、不比 S/L 型（兩段都在同一個位移結束於 LMS 時，型別由字元和後一格的型別決定，字元相同就推得型別相同）。少一行；對拍與 LC 照樣通過。
- **書中 SAIS C++20 → SAIS**：SAIS-C++20 註解掉（檔案、driver、對拍保留）。LCP Array 的 LC 驅動（Number of Substrings，AC 0.064s）和對拍改用 SAIS.cpp。
- **Tips for Generating Functions** 移到 Polynomial 章（`7_Polynomial/Generating_function.tex`，接在 Newton's Method 後）。刪掉基本性質（和、OGF 乘積與 $A^k$、$A(rx)$、EGF $A^k$、$(1+x)^n$）；修 EGF 定義 $x_i \to x^i$、$xA(x) \Rightarrow na_{n-1}$（原寫 $na_n$ 是錯的）；加 $\ln(1\pm x)$ 展開、$-\ln(1-x)$ 的 $k$ 階導數、$(\ln A)' = A'/A$、$(-\ln(1-x))^k/k!$ 是第一類 Stirling 的 EGF。
- **Tips for Generating Functions**：加單位根濾波 $\sum_{m \mid n} a_nx^n = \frac1m\sum_j A(\omega^jx)$（取代 $A(rx)$ 的用途）。
- **Tips for Generating Functions**：單位根濾波註明 mod $p$ 需要 $m \mid p - 1$（998244353 下 $m = 3$ 不行）。
- **理論文字行距** 9.5pt → 10.5pt（字仍是 8pt；`codebook.tex` 的 `\footnotesize` 改成 `\fontsize{8}{10.5}`，程式碼列表自己設 `\footnotesize` 不受影響）。成本約 0.63 欄，第 25 頁用到 0.65/3 欄。比較過 `\small`（9pt，1.26 欄且 5 行溢出）。
- **Treap**：merge 的 `rand()` 換成 `mt19937`（Windows 的 `RAND_MAX` = 32767，子樹超過 32768 個節點時機率偏掉，樹會退化）。保留依大小隨機的 merge（不存 priority），複製節點（可持久化）時仍然平衡，加一行註解說明。LC AC 0.766s。
- **Adaptive Simpson**：`make_simpson`（4 行）換成 deduction guide（2 行），用 `Simpson S{f};` 建構；guide 是給 C++17 用的（C++20 不需要）。
- **EBCC / SCC / VBCC**：出棧的 `while (1)` 五行改成兩行 `for (int x = -1; x != u; st.pop_back())`。
- **Aho-Corasick**：fail link 直接取 `to[fail[v]][i]`，不再往上跳 `while`（少 2 行，LC 0.369 → 0.266s）。
- **Treap**：`up()` 寫成一行。
- **LCP Array**：`len = 0; continue;` 寫成一行。
- **Miller Rabin**：用 `__builtin_ctzll` 取 $n-1$ 的 2 的次方，取代 `(n-1) & (1-n)` 的寫法。
- **PalTree**：建構子改呼叫 `clear()`、拿掉多餘的 `inline`、`count()` 改用下標迴圈（少 4 行）；`St` 加註解說明 0、1 號節點是根（len 0、-1）。
- **NumberofMaximalClique**：pivot 改用 Tomita（在 `some ∪ none` 中選 `some` 鄰居最多的點），最壞 $O(3^{n/3})$；稠密隨機圖 n=80, p=0.9 從 10.5s 變 0.91s。拿掉 `cnt > 1000` 的剪枝（POJ 2989 專用，計數會被截斷）。加註解：時間與答案成正比（約 7e7 個/秒），Moon–Moser n=48 約 0.6s。
- **Miller Rabin**：拿掉 `prime()` 包裝（還原上面三筆）；Pollard Rho 照舊需要自己寫 `prime()`，測試的 driver 恢復自帶的版本。
- **Miller Rabin**：加一行註解：回傳 0 代表 n 一定是合數；n 是質數 ⇔ 表中每個 base a 都通過，且 n ≥ 2（`Miller_Rabin(a, 1)` 會回傳 1）。
- **Polar Angle Sort** 放回書中（HPI、HPI Alternative Form、rotatingSweepLine 要用 `cmp`）；`is_neg` 改寫成 `sign(k.Y) ? sign(k.Y) < 0 : sign(k.X) < 0` 才塞得進 61 欄。
- **Convexhull3D**：`n <= 2` 改 `n <= 3`（n = 3 時 `P[3]` 越界）；註解改成「需要 4 點不共面，否則 UB」（原本寫 WA，實際是 `*end()`）。
- **Black Magic**：`__float128_t` 不存在，改 `__float128`。
- **Pragma**：最後一行補 `;`，加註解「in main: flush denormals to 0」與「arch=skylake 在舊 CPU 會 RE」。
- **fac_no_p**：不再需要 `mpow`：`prod[pk]` ≡ ±1 (mod pk)，所以它的 n/pk 次方只看奇偶，改成 `if (n / pk % 2) rt = rt * prod[pk] % pk;`。
- **NTT、Vizing**：空迴圈 `for (...);` 改成 `for (...) {}`（跟 Delaunay 一致）；Vizing 的換行迴圈併成一行。
- **Linear Equations**：`solve()` 開頭清 `fixed[]`、`sol[]`、`basis`；原本同一個物件解第二個系統時 basis 與自由變數的 sol 會殘留舊值（已用兩次 solve 的例子確認）。
- **Minimum Arborescence**：`/* TODO` 改 `/* needs:`；無解回傳處註明 n = 1 也回傳 `{}`。
- **QuadraticResidue**：`rand()` 改 `mt19937 rng(random_device{}())`（Windows `RAND_MAX` = 32767）。
- **Minimum Clique Cover**：不再呼叫 `fwt`（書上沒寫這個相依），改成自己寫兩行的子集和（OR 正變換）。對拍也拿掉自備的 `fwt`。
- **Fast Walsh Transform**：`subset_convolution` 原本整個在 `int` 上做、沒有取模，值一大就溢位（luogu P6097 要 mod 1e9+9）。`fwt` 改用 `add`/`mul`（跟 NTT 同一套），逆變換傳 `MOD - 1`；迴圈改用半長 `L`；函式行註明 OR only。對拍改成 mod 1e9+9、值到 MOD − 1，另加 L = 18 抽查。
- **Fast Walsh Transform**：`fwt(a, n, op)` 改成 KACTL 式：OR/AND/XOR 三行 butterfly 都寫出來、留一行，`inv` 直接當參數（XOR 逆變換另乘 1/n）；拿掉原本要自己翻譯的公式表頭。新增 **Tips for FWT**（`7_Polynomial/FWT_tips.tex`）：三種變換的封閉式與逆變換。
- **stress/run.sh**：測試旁邊若有 `<name>.pre.sh`，編譯前先跑它在 `build/` 產生模板的變體（FWT 用來產生三份各留一行的版本）。
- **SIMD、Static Top Tree**（becaido 的修改）：超過 61 欄的 10 行改寫或換行，只動排版不動邏輯；SIMD 缺結尾換行補上。SIMD 對拍跟上改名（`count_less` → `cnt_lt`）並定義速查表用的 `N`（原本編譯失敗）。
- **Black Magic**：`join` 後的註解跟著 push 改成 `h1 = {1, 2}`。
- **SIMD**：改回 becaido 原本的註解文字（先前為了 61 欄縮寫過），只把超寬的 5 行換行。
- **Vizing**：註解說明 becaido 新增的 `bip`：`bip=1: bipartite, Delta colors suffice`；對拍加二分圖情形。
- **Static Top Tree**：新對拍 `StaticTopTree_queries.cpp` 測 `query_subtree`、`query_reroot`（非交換 DP vs 暴力），並用 6 個植入錯誤確認測得出來。
- **SIMD**：對拍檢查速查表上每個 intrinsic 的語意，不只是能編譯；確認 prefix max 技巧只在值 ≥ 0 時對（左移補 0）。
- **Minimum Arborescence**：註解改成「有解 iff `size() == n - 1`」（n = 1 的 `{}` 也符合），取代原本含糊的寫法；新增本地對拍（含無解與 n = 1）。
- **fac_no_p**：複雜度註解跟著拿掉 `mpow` 改成 `O(p^k + log_p n)`。
- **Segment Tree Beats**：新增純文字筆記（節點要維護什麼、chmin 三種情況、合併、tag 順序、複雜度），程式碼不放進書。
- **Centroid Decomposition**：移出書（comment out）。

## 2026-11-25
- **PalTree 加 series link**：節點多 `diff`、`slink`，新增 `best(dp)` 回傳 $\min(dp[j] + 1, j)$（$s[j, n)$ 是回文），每次 `add()` 後都要呼叫，$O(\log n)$。最小回文分割（ICPC 2024 Seoul G）一行；CF 906E 在外面加偶數位置與免費步驟。新增 `stress/String/PalTree.cpp`（最小分割與 906E 都對暴力）。Library Checker Eertree 重跑 AC。`clear()`、`size()` 與註解壓短。
- **Theorems 拿掉 Palindromic suffixes 敘述**：程式已實作，改看 PalTree。
- **BoundedFlow 縮短**（80 → 67 行）：三參數 `add_edge` 改呼叫四參數版（下界 0 本來就一樣）、`dfs` 合併、`solve()` 清輔助點改成整個清空。對拍 30 000 組通過；20 000 點、20 萬邊計時與舊版相同。
- **`CCinter` 改名 `circleInter`**（和 `circleLine` 一致，也是 kactl 的名字）；CircleCover 註解寫明需要它、在哪個模板。
- 仍是 25 頁。
- **PalTree 改用隊友的寫法**：`best(dp)` 換成 `process(t)`（直接算最小回文分割），series link 在 `add()` 裡照定義算。修掉原稿三個問題：全形空白編譯失敗、`diff`/`slink` 未初始化（根 0 的 `diff` 會被比較）、用到契約外的 `INF`；dp 改成以前綴長度為索引，拿掉 `>= 0` 特判。節點改用預設成員初始化（56 行）。906E 等變形照 `process()` 的迴圈自己寫。Lib-Checker Eertree 重跑 AC。
- **BoundedFlow `dfs`**：拿掉 `return a, b;` 逗號回傳的寫法，改回 `dis[u] = -1; return 0;`；保留 `if (int df = dfs(...))`。

## 2026-11-24
- **新章 Theorems（第 10 章，放最後）**：所有純敘述的理論集中在一處，用粗體分組標籤（Graph、Combinatorics、Number Theory、Game Theory、String、Polynomial、Geometry and Calculus），不進目錄。檔案搬到 `10_Theorem/`：`6_Math/Theorem.tex` 拆成 `Graph`、`Combinatorics`、`NumberTheory`、`Geometry`（內容不變；Misère Nim 改放 Game）；`numbers.tex`、`Estimation.tex`、`Generating_function.tex`、`FWT_tips.tex`、`Newton.tex` 原樣搬過來。Euclidean Algorithms（floor sum 遞迴）、Flow Models、Simplex Construction、Mo's、Matroid、Segment Tree Beats 筆記留在程式碼旁。
- **新增敘述**（math-drafts 勾選的）：König、Tournaments、Colouring（$\chi \le k$ 的排容、弦圖 MCS）、Random walks；Game：Staircase、Moore's Nim$_k$、Wythoff、SJ 定理、Undirected vertex geography、Colon principle；String：borders、回文後綴的等差數列、DFA 接受字數是線性遞迴；Polynomial：power sums、Lagrange inversion、falling factorials、linear recurrences。
- **Newton's Method**：拿掉 inv/ln/exp/sqrt 表（Polynomial Operation 已實作），只留一般迭代式並加 $P = 1 + xP^2$ 例子。
- PDF 25 頁、最後一頁全滿；分組標籤原本 8 個會變 26 頁，Estimation 併入 Combinatorics 不另標。
- **hash.sh**：拿掉 `echo` 說明行；標題 Hash.sh 改成 Typo Check Hash。
- **typo_check.sh**：`1_Basic/hash.sh` 改名 `typo_check.sh`；刪掉沒用到的舊副本 `9_Else/hash.sh`。
- **Python**（`misc.py`）：加 `sys.stdin.readline`、`setrecursionlimit`、`set_int_max_str_digits(0)`（3.11+ 超過 4300 位的 int 轉字串會錯）、`pow(a, -1, m)`/`isqrt`/`comb`（3.8+）、Decimal 一般精度的 sqrt；註明 `MAX_PREC` 只能做精確的 `+ - *`，除法和 sqrt 會 `MemoryError`（已實測）。
- **Matroid Intersection**：演算法說明文字移出書（comment out，檔案保留），程式碼已涵蓋；之後換成擬陣定義與常見模型（待寫）。
- **Matroid**：`9_Else/Matroid.tex` 搬到 `10_Theorem/Matroid.tex`（Theorems 新增 Matroid 標籤），內容改寫成給沒學過的人看：三條公理、常見的擬陣（顏色上限、森林、線性獨立、刪邊後仍連通）、Matroid Intersection 的 `build`/`ind` 介面與兩個用法。
- **敘述修訂**（review-items.md 逐條確認）：Tournaments 拆三點（二分插入、Camion、king）；Landau 補「等號處即 SCC 切點」；Random walks 拿掉 commute time；Game 統一成「名稱（規則）⇒ 何時先手輸」；Periods、回文 series link 定義、DFA 的 DP 與 Aho–Corasick 避開 pattern 改寫；falling factorial 補定義；linear recurrences 拆兩點。
- **新增 Probability bounds**（Combinatorics）：Chernoff、reverse Markov、Paley–Zygmund。
- **Derivatives/Integrals**：拿掉 $\int \tan ax$、$\int x \sin ax$、erf、$\int x e^{ax}$。
- **Python**：`set_int_max_str_digits` 加 `hasattr` 保護（舊版直譯器沒有）、Decimal context 補 `Emin=MIN_EMIN`、加 Fraction 精確運算範例。
- **版面回到 25 頁**：CircleCover 改寫成函式（58 → 36 行，對拍 1 500 組通過）、Convexhull3D 縮短（66 → 59 行，介面不變，8 693 個凸包通過）、2D Convex hull 拿出書（10 行；Minkowski Sum、minMaxEnclosingRectangle 仍呼叫 `hull()`，比賽時要自己寫 Andrew）。
- **hull() 需求寫在呼叫端**：Minkowski Sum、minMaxEnclosingRectangle、TangentPointToHull 註明輸入／輸出是「凸包、逆時針、無三點共線」，2D Convex hull 不在書中也看得懂要寫什麼。
- **CircleCover**：註解補上複雜度 $O(n^2 \log n)$。
