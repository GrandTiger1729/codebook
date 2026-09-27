# codebook vs kactl vs std_abs — 人工審核清單

**A 節全部已修**（A1/A2/A4/A5/A6/A7，A3 是我誤判已撤回），另外換掉了
`MincostMaxflow`、加了三個新模版。

> **2026-09-17 更新**：這份清單寫於 09-10，之後又處理掉 B 節 2 條、C 節 4 條。
> 已解決的就地標成 ~~刪除線~~ + ✅ 並註明怎麼解的；**沒有標記的仍然是待辦**。
> 這段期間新發現的問題另外列在最後一節。
每一項都標了證據來源：
**[LC]** = `RESULT.md` 的 Library Checker 實測，**[BF]** = 本地暴力對拍，
**[讀]** = 三方原始碼比對。

比較對象：`codebook-playground/kactl/content/`、`codebook-playground/std_abs/`。

---

## A. 找到的錯誤 —— **全部已修**（A3 是我誤判，已撤回）

| # | 模版 | 問題 | 證據 |
|---|---|---|---|
| A1 | `6_Math/ModMin.cpp` | 有解時完全正確，**無解時回傳 `0` 或負數而不是 `-1`**（m ≤ 60 的 171 萬組裡有 28 679 組），因為遞迴用 `if (ll y = mod_min(...))` 判斷而 `-1` 是 truthy。**已修**：`; ~y`。改後 m ≤ 90 的 851 萬組全對。 | [BF] |
| A2 | `7_Polynomial/Fast_Fourier_Transform.cpp` | 原本只有 `bitrev`/`trans` 的宣告（刻意的，照抄 NTT），補完之後 mod $10^9+7$ 在 $n \approx 10^6$ 仍 WA 8 筆 `fft_killer` —— `complex<double>` 53 bits 尾數撐不住。**已修**：整份換成 kactl `FastFourierTransform.h`（自足的 `fft` + `conv`），另外新增 `Fast_Fourier_Transform_Mod.cpp`（kactl `convMod`，拆 $\sqrt M$ 的三次卷積）。`convolution_mod_1000000007` 現在 **48/48 AC，0.370s / 10s**。 | [LC] |
| ~~A3~~ | `3_Data_Structure/LiChaoST.cpp` | **撤回，這一項我判斷錯了。** 這份 Li Chao 是寫在整數定義域 $x \in [0, n)$ 上的，在那個定義域上完全正確（隨機 200 組全對）。它只是不能直接吃離散化後的座標 —— 那需要把 `at(l)` 改成 `at(xs[l])`，因為 `m * idx + k` 不是原函數。不是 bug，是定義域限制。`content.tex` 那行已還原成你原本寫的 `% test by Library Checker Line Add Get Min`。 | [BF] |
| A4 | `8_Geometry/Minimum_Enclosing_Circle.cpp` | ① `random_shuffle` C++17 已移除 ② 呼叫的 `excenter()` **整本書都不存在**（`Heart.cpp` 只有 `circenter/incenter/masscenter/orthcenter`）③ 連帶 `Heart.cpp` 自己也編不過：`circenter` 寫進一個沒宣告的全域 `center`，`incenter` 寫 `a * p1`（`double * pdd`，只有 `operator*(pdd, double)`）。**全部已修**，`geo/minimum_enclosing_circle` **25 筆 AC，0.032s / 5s**。 | [LC][讀] |
| A5 | `6_Math/DiscreteLog.cpp` | `kStep` 寫死 `32000`，m 超過 $10^9$ 就退化成 $O(m/32000)$；而且 `i < m + 10` 在 m 接近 $2^{31}$ 時 int 溢位。**已修**：`kStep = sqrt(m) + 1`、迴圈變數改 `ll`。$m \le 300$ 的 904 萬組對拍全對，LC 20 筆 AC、時間不變。 | [LC][BF][讀] |
| A6 | `2_Graph/SCC.cpp` + `2_Graph/2SAT.cpp` | SCC **只做了 tab → 2 空格**（依你的指示，演算法和 `scc_adj` 都沒動）。`2SAT.cpp` 換成 kactl `2sat.h` 的移植，補上 `set_value` / `at_most_one`，不再需要 `SCC.cpp`；**並依指示改成全域 `N` 陣列**（跟 `MinCostCirculation`/`Kuhn_Munkres`/`Bipartite_Matching` 一致），比 kactl 的 vector 版快 1.4x–2.4x。`other/two_sat` 18 筆 AC，另外 2 萬組隨機實例對 $2^n$ 暴力驗過。 | [LC][BF] |

| A7 | `6_Math/Miller_Rabin.cpp` + `6_Math/Pollard_Rho.cpp` | 兩份都呼叫一個整本書都沒有的 `mul(a, b, n)`（128-bit modmul）。**已修**：直接就地展開成 `(__int128)a * a % n`，不新增 define、不新增相依。`Miller_Rabin.cpp` 現在可以獨立編譯；`primality_test` 12 筆 AC、`factorize` 31 筆 AC，兩個 driver 都不用再自己補 `mul`。（`Pollard_Rho` 還缺 `prime()`，見 B。） | [LC] |
## B. 「書上沒有、但模版需要」的缺件

這些符號在整個 codebook 都找不到定義。已經排除掉你說「背得起來、不用帶」的
（`fpow` / `mpow`）和「本來就該 problem dependent」的（`INF`），剩下的是真的缺：

| 模版 | 缺的東西 | 怎麼補 |
|---|---|---|
| `6_Math/Pollard_Rho.cpp` | `prime(n)` —— `Miller_Rabin.cpp` 匯出的是 `Miller_Rabin(a, n)`，那七個 base 只寫在註解裡，**沒有人把它們串成 `prime()`**，所以 Pollard Rho 直接編不過 | 五行的 wrapper（見下），或把 Pollard Rho 改成自己跑 base 迴圈 |
| ~~NTT 的 `mpow`~~ ✅ | ~~宣告了但從未定義~~ | 主線現在是 `NTT_stdabs.cpp`，檔首的 header 就帶了 `mod`/`G`/`N`/`SZ`/`ALL`/`add`/`sub`/`mul`/`Pow`/`fac`/`facp`，整個檔自足，三個 driver 各自那六行的 out-of-class 定義全部刪掉 |
| `8_Geometry/Convex_hull.cpp`, `8_Geometry/Polar_Angle_Sort.cpp` | **整數版** `ori`/`sign`/`cross`/`abs2` —— 兩者都吃 `pll`，但收錄的 `8_Geometry/Default_code.cpp` 只有 `pdd` 版 | 書裡就有 `8_Geometry/Default_code_int.cpp`，只是**沒收進 content.tex**。新加的 `ClosestPair.cpp` 也吃 `pll` |
| `5_String/MainLorentz.cpp` | 回傳 vector 的 `Zalgo(s)`；`5_String/Z-value.cpp` 是填全域陣列，介面對不上 | 讓 `Z-value.cpp` 回傳 vector，或在 MainLorentz 裡包一層 |
| `2_Graph/Minimum_Arborescence_fast.cpp` | `DSU` | `3_Data_Structure/DSU.cpp` 在 repo 裡但沒收錄 —— 你說背得起來，所以不補。**`min_heap` 已經收進 content.tex 了**（第 35 行） |
| `4_Flow_Matching/Gomory_Hu_tree.cpp` | `MaxFlow` | `4_Flow_Matching/Dinic.cpp` 有，但 content.tex 第 49 行**被註解掉了** |
| ~~`6_Math/Linear_Equations.cpp`~~ ✅ | ~~需要 `fraction`~~ | 改成 `template <typename T>`（只要 `+ - * /` 和 `== T()`），不再綁任何特定型別。`system_of_linear_equations` AC 27 筆；`Fraction.cpp` 補了 `operator==`，有理數那條路仍然可走 |
| `7_Polynomial/Value_Poly.cpp` | `mint` | 整個 repo 沒有 modint |
| `6_Math/Gaussian_gcd.cpp` | `cpx`（高斯整數型別） | 整個 repo 沒有 |
| `9_Else/Mos_Algorithm_On_Tree.cpp` | `LCA` | 書裡沒有獨立的 LCA 模版（`Heavy_light_Decomposition` 能走路徑但沒露 LCA） |

`prime()` 的 wrapper（如果要補的話）：

```cpp
bool prime(ll n) { // n < 2^64
  if (n < 2) return 0;
  for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    if (!Miller_Rabin(a, n)) return 0;
  return 1;
}
```

**關於 `INF`**（`Kuhn_Munkres` / `Maximum_Weight_Matching` / `MinimumSteinerTree` /
`LiChaoST` / `min_plus_convolution`）：照你說的，這是 problem dependent，不該寫死。
挑法有兩條規則：① 要大於任何合法答案 ② 要能承受模版對它做的運算而不溢位 ——
會做 `INF + w` 的（最短路鬆弛）留一倍餘裕，會做 `INF + INF` 的留兩倍。
`ll` 的安全預設是 `4e18`（只加一次）或 `numeric_limits<ll>::max() / 4`（加兩次都安全）。
`MincostMaxflow.cpp` 裡那個 `INF` 是**不同的東西** —— 它是 Dijkstra 的「不可達」哨兵，
跟題目無關，所以直接寫死 `numeric_limits<ll>::max() / 4` 是對的。

**不列為問題**（你說背得起來）：`fpow`（`DiscreteLog` 用）、`mpow`（`fac_no_p` 用）。

## C. 「介面露太少、無法測」

| 模版 | 缺什麼 |
|---|---|
| ~~`9_Else/All_LCS.cpp`~~ ✅ | ~~`all_lcs()` 是 `void`，`h` 是區域變數~~ → 改成 `struct AllLCS`，建構吃 `t`、`push(c)` 推進一個字元、`h` 公開。`prefix_substring_lcs` AC 11 筆 |
| ~~`9_Else/tree_hash.cpp`~~ ✅ | ~~拿不到每個點的 hash~~ → `return h[u] = sum;`，一行。`rooted_tree_isomorphism_classification` AC 17 筆 |
| `3_Data_Structure/Centroid_Decomposition.cpp` | `info`/`upinfo` 寫死「到所有標記點的距離和」，不是通用的 merge |
| `3_Data_Structure/Treap.cpp` | `node` 沒有 lazy/aggregate，只能當 ordered set |
| `3_Data_Structure/link_cut_tree.cpp` | ~~聚合寫死 xor~~ ✅ 已改成 sum（`val`/`sum` 一起換 `ll`），`dynamic_tree_vertex_add_path_sum` AC 15 筆——這是 LCT 第一次有回歸測試。**`nil.size` 是 1 不是 0 仍然沒動**（所以 `cut()` 判的還是 `y->size != 5`），那是可讀性問題不是正確性問題 |
| ~~`2_Graph/MinimumSteinerTree.cpp`~~ ✅ | ~~只回傳 cost、int 會溢位~~ → 全部改 `ll`（對拍實際看到 8.7e9），加了 `mid`/`eid`/`fr` 三個回溯表，`solve()` 順便填 `ans`。`minimum_steiner_tree` AC 24 筆。**另外對拍抓到 `vcst` 整個是壞的**（一開就 2921/4000 錯），見最後一節 |
| `4_Flow_Matching/MinCostCirculation.cpp` | 沒有對偶變數 `p_v`、沒有不可行偵測 |

## D. 建議取代 / 升級

| 現況 | 建議 | 理由 |
|---|---|---|
| ~~`4_Flow_Matching/MincostMaxflow.cpp`~~ | **已換成 kactl `MinCostMaxFlow.h` 的移植** | `add_edge` / `maxflow(s,t) -> {flow, cost}` / `setpi(s)`。兩次量測推翻了我的兩個判斷：① 把 kactl 的 pbds decrease-key 換成 lazy deletion，退化資料 `hand_minus_00` 慢 8.7 倍（4.31s vs 0.50s）——改回 pbds；② 把全域 `N` 陣列換成 vector，多測資時慢到 1.67x——改回全域 `N`，形狀跟 `Dinic`/`isap` 一致。4000 組隨機圖（2000 組含負權走 `setpi`）對 SPFA 參考全對；LC 14 筆 AC，1.4s / 5s |
| `3_Data_Structure/BIT_kth.cpp` | 補上 `add`/`prefix` | 現在只有 8 行的 descent，`bit[]` 要自己開、加值查值都要自己寫（driver 就是這樣補的） |
| `8_Geometry/Polar_Angle_Sort.cpp` | **不建議換成 kactl `Angle.h`** | 會弄壞三個收錄中的模版。codebook 的 `cmp(a, b, same)` 是 9 行、吃 `pll`、回傳 int，其中 `same=false` 時用 `-1` 當「同角」哨兵；`Half_plane_intersection.cpp`(3 處)、`HPIGeneralLine.cpp`(1 處) 都靠 `!= -1` 判平行半平面，`rotatingSweepLine.cpp` 則直接把它當 sort comparator。kactl 的 `Angle` 是個帶繞圈數 `t` 的 struct + `operator<`（23 行；連 `segmentAngles`/`operator+`/`angleDiff` 是 34 行），**沒有同角哨兵**（要寫成 `!(a<b) && !(b<a)`，兩次比較），**座標是 `int` 不是 `ll`**，而且預設不比長度（codebook 的 `same=true` 會）。唯一的好處是 360° 以上的旋轉掃描，三個呼叫點都不需要。真的要的話建議**另外新增**一個 `Angle` 模版，而不是取代 `cmp` |
| ~~`2_Graph/2SAT.cpp`~~ | **已換成 kactl `2sat.h` 的移植**，見 A6 | `set_value` / `at_most_one` 都補上了，而且不再相依 `SCC.cpp` |
| `2_Graph/SCC.cpp` | 後來改了：43 → 31 行 | 拿掉 `ist[]` 和 `low[]`（`scc[v] == 0` 就涵蓋「沒走過或還在堆疊上」，low-link 寫回 `dfn[]`）。仍然比 kactl `SCC.h` 的 15 行長，但多給 `scc_adj`。**2SAT 後來改成重新委派給 SCC**（你指出兩者內容重複），所以 SCC 又有相依者了 |
| ~~`3_Data_Structure/LiChaoST.cpp`~~ | 撤回，見 A3 | |

## E. 建議新增

五個已經加了（見章節末）。剩下**還沒加**的：

| 主題 | 參考 | 為什麼值得 |
|---|---|---|
| **XOR linear basis** | std_abs `Math/LinearBasis.cpp` | 20 行，很常考，而且插入 / 查最大值 / 查第 k 小的細節不好臨場推 |
| **mod 2 bitset 高斯消去** | kactl `SolveLinearBinary.h` | `Linear_Equations` 現在是泛型的，理論上餵 `bool` 也行，但 bitset 版是 64 倍的常數差距，值不值得帶自己判斷 |
| **獨立的 LCA** | — | `9_Else/Mos_Algorithm_On_Tree.cpp` 直接需要（見 B）；現在書裡沒有 |

**你說可以背、所以不列入**：字串 hashing、DSU（含 rollback）、persistent segment tree、
01-trie。不過兩個相關的點還是留著：

- `9_Else/DynamicMST.cpp` 需要**可撤銷**的 DSU（按 size 併 + 操作堆疊回滾），
  跟普通 DSU 不是同一件事，要不要帶你自己判斷。
- `2_Graph/Minimum_Arborescence_fast.cpp` 需要的 `min_heap`（可合併堆 + 懶標）
  不在「背得起來」那一類，檔案在 repo 裡但沒收錄（見 B）。

### 已經加進去的

| 主題 | 檔案 | 驗證 |
|---|---|---|
| 任意模數 FFT | `7_Polynomial/Fast_Fourier_Transform_Mod.cpp`（32 行，kactl `convMod`） | `convolution_mod_1000000007` **48/48 AC** |
| Closest pair | `8_Geometry/ClosestPair.cpp`（19 行，只依賴 `pll` + `X`/`Y`） | `geo/closest_pair` **29 筆 AC** |
| Matroid intersection | `9_Else/MatroidIntersection.cpp`（67 行，泛型 `matroid_isect` + `GraphMat` / `ColorMat` 兩個 oracle） | 3000 組隨機（圖擬陣 ∩ 分割擬陣）對 $2^m$ 暴力全對 |
| Minimum vertex cover | `4_Flow_Matching/Bipartite_Matching.cpp` 加了 `cover()`（13 行，König） | 3 萬組隨機二分圖，四項全對：是合法覆蓋、大小 == 最大匹配、等於 $2^{L+R}$ 暴力的最小值、無重複點 |
| 可合併堆 | `3_Data_Structure/min_heap.cpp` 收進 content.tex（21 行，原本 30 行） | `graph/directedmst` 12 筆 AC，driver 不再自己補 |

**不建議新增**（原以為缺、實際上有）：

- **Linear recurrence 第 k 項** — `7_Polynomial/Polynomial_Operation.cpp` 的
  `LinearRecursion()` 已經做掉了，而且是 $O(n \log n \log k)$，比 kactl
  `LinearRecurrence.h` 的 $O(n^2 \log k)$ 好。搭 `Berlekamp-Massey.cpp` 就完整了。
- **Subset transform / FWT** — `7_Polynomial/Fast_Walsh_Transform.cpp` 有。
- **Mo's algorithm、Hilbert order** — `9_Else/` 兩個 Mo + `HilbertCurve.cpp` 都有。
- **整數幾何 primitives** — `8_Geometry/Default_code_int.cpp` 有，只是沒收進 content.tex（見 B）。

## F. 沒有 LC 對應、目前完全沒有自動測試的模版（48 個）

幾何章 25 個裡有 21 個沒有 LC 對應，另外還有 `Gomory_Hu_tree`、`Virtual_Tree`、
`DLX`、`BitsetLCS`、`Half_plane_intersection`、`CircleCover`、`Maximum_Clique_Dyn`、
`Minimum_Clique_Cover`、`NumberofMaximalClique`、`Vizing`、`MinimumMeanCycle` 等。

可選做法（不在這次範圍內）：移植 kactl `stress-tests/` 的做法 —— 純 `main` + `assert`，
`utilities/` 有現成的亂數、圖/樹/多邊形產生器，幾何章最適合這樣測。


---

## 附註：std_abs 的 `Graph/MatroidIntersection.cpp` 不能直接抄

我本來打算照它移植，讀完之後放棄了，改成照 `Matroid.tex` 自己寫。它至少有三處壞掉：

- `GraphMatroid::build_exchange_graph` 的內層迴圈寫 `for (int j = 0; j < m; ++i)`
  —— 增加的是 `i` 不是 `j`，死迴圈。
- 同一個函式在迴圈裡 `adj.assign(n, ...)`（n 是點數），然後往 `adj[i]` 塞東西，
  但 `i` 是**邊的編號**（0..m-1）—— 交換圖跟 DSU 的鄰接表被混在同一個變數上。
- `SPFA()` 用了 `mp(...)`，整個 repo 沒有這個定義。

`ColorMatroid` 那半邊是對的，可以當參考。

---

## G. 09-10 之後新發現的（全部已修）

這些不在原始清單上，是後來真的把模版跑起來才浮出來的。

| 模版 | 問題 | 怎麼發現的 |
|---|---|---|
| `7_Polynomial/Polynomial_Operation.cpp` | `Sqrt()` 只處理 `a[0]` 是二次剩餘的情形，其餘靜靜回傳一堆 0 | 修好 harness 的 checker 參數順序之後，`sqrt_of_formal_power_series` 35 筆裡 20 筆 WA，連 `example_00` 都錯 |
| `7_Polynomial/Fast_Walsh_Transform.cpp` | `f`/`g`/`h` 第一維少一格（`ct[i]` 可以等於 `L`），`L == N` 就越界 | ASan |
| 同上 | 三個全域陣列沒清空，**第二次呼叫整個錯**（L=3 時 8/8 全錯） | 本地對拍連續呼叫 |
| 同上 | 修好第一維之後 `N = 21` 要 554MB | 算出來的 |
| `2_Graph/MinimumSteinerTree.cpp` | `vcst`（頂點成本）整條路徑是壞的：`dp[0]` 和合併步驟假設 `dp[msk][i]` 含 `vcst[i]`，但 seed 和延伸步驟用的是純邊權最短路 | 本地對拍，`vcst` 全 0 時 4000 組全對，一開就 2921/4000 錯 |
| `7_Polynomial/Polynomial_Operation.cpp` | `LinearRecursion` 在 $d = 10^5$ 超時（`DivMod` 對固定的 `C` 重算約 120 次 `Inv`） | LC TLE 142% |
| 同上（我自己改出來的） | 提出 `Inv` 之後長度取 `max(1, k - 1)`，但 `M` 初值是 `{0, 1}`，$k = 1$ 時第一次 `M.Mul(M)` 就需要長度 2 | **LC 的測資完全沒有 $d = 1$**，是本地對拍抓到的（2000 組錯 260 組，正好是 $k=1$ 的比例） |

### harness 自己的兩個 bug

| 問題 | 影響 |
|---|---|
| `run.py` 用 `checker <in> <jury> <ours>` 呼叫官方 checker，但 testlib 是 `<in> <ours> <jury>` | 對「只驗證選手輸出」的 special judge 等於每次都在驗標準答案，**必然 AC**。修正後 58 列全部重跑，只有 `sqrt_of_formal_power_series` 真的翻成 WA |
| 一直用 `-j 1` 跑全套 | 不是錯，是浪費：verdict 跟 `-j` 完全無關，平行快 4.5 倍。但時間確實會膨脹（中位數 1.8x、最差 4x），所以現在的規則是**平行過篩、serial 定案**，`RESULT.md` 表頭會印出用了幾個 worker |

### 效能

| 改動 | 結果 |
|---|---|
| polynomial 章係數 `ll` → `int` | Inv/Exp/Log/Pow/Sqrt/Shift/SampleShift/convolution 全部快 1.14–1.46x。差距的來源是記憶體頻寬（NTT 是頻寬綁住的），不是演算法 |
| `Linear_Equations` 54 → 36 行 | 順便快一點（省掉兩次重找 pivot 比多做的整列除法便宜） |
| NTT 改成 std_abs 形狀 | 沒有可測得的差異（三次獨立測量 old/new = 1.048 / 1.035 / 0.977） |

### std_abs 的 polynomial 不能整份照抄

原本的打算是「std_abs 那份比較快就換過去」，實測之後改成只搬型別：

- `PolyPow` 把 $k \approx 10^{18}$ 丟進 `Pow(int a, int b)`，截斷成負數後 `b >>= 1` 永遠到不了 0 —— $5 \times 10^5$ 的四筆測資**全部無窮迴圈**。
- 它快的原因只是係數用 `int`。把我們的換成 `int` 之後反而比它快（Inv 總時間 3.66s vs 4.46s）。

---

## H. 待辦：NTT 的優化（2026-09-17 暫緩）

主線的 `7_Polynomial/NTT_stdabs.cpp` 維持 std_abs 原樣不動，之後再回來看。
回來的時候先讀 `CHANGES.md` 的 Phase 23，那裡有已經量過的東西：

- 蝶形的算術用**單向** `add`/`sub` 而不是一個雙向的 —— NTT 層 2.1x，是最大的一項
- `inv` 用線性表取代 `po(x, mod-2)` —— `Sx()` 每個 $i$ 呼叫一次，`Ln()` 每次都跑 `Sx()`
- 已經排除的假設：蝶形寫兩次、`int&` 介面、local 變數暫存，全部無影響

支線 `Number_Theory_Transform.cpp` / `Polynomial_Operation.cpp` 不在 `content.tex`，
由 `verify/MAP.tsv` 的六列 `#ours` 保護，不會腐爛。
