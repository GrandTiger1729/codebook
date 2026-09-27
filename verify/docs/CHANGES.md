# Phase 1–3：macro 正規化 + 迴圈風格

改動範圍：`codebook/codebook/` 底下 **105 個 `.cpp` + `content.tex`**
（575 insertions / 535 deletions）。**所有 CRLF 檔案的行尾都原封不動**
（`git diff` 沒有任何整檔重寫）。

## 契約

`1_Basic/Default_code.cpp` 現在就是這六行（`content.tex` 那行**維持註解，不進 PDF**）：

```cpp
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define Waimai ios::sync_with_stdio(false), cin.tie(0)
#define FOR(x,a,b) for(int x = a, I = b; x <= I; x++)
#define pb emplace_back
#define F first
#define S second
```

## Phase 1a — 幾何章級 prelude

`8_Geometry/Default_code.cpp`（和沒收錄的 `Default_code_int.cpp`）最前面加六行：

```cpp
#define X first
#define Y second
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
```

幾何章 25 個檔案照你的指示**保留 `.X`/`.Y`**，SZ/ALL/pii/pll/pdd 也都由這個檔供應。

## Phase 1b — 沒 define 過的符號

門檻用字元數損益平衡點算。**檔首 define 的 12 個檔**：

| define | 檔案（使用次數） |
|---|---|
| `#define SZ(a) ((int)a.size())` | `Value_Poly`(10)、`Berlekamp-Massey`(8)、`BoundedFlow`(5)、`Vizing`(4)、`SchreierSims`(13, 未收錄) |
| `#define ALL(v) v.begin(), v.end()` | `Maximum_Weight_Matching`(10)、`discrete_trick`(5) |
| `typedef pair<int, int> pii;` | `1d1d`(6)、`IntervalContainer`(4)、`SchreierSims`(8, 未收錄) |
| `typedef pair<ll, ll> pll;` | `Centroid_Decomposition`(9)、`ax+by=gcd`(4)、`MinimumMeanCycle`(3)、`MincostMaxflow_dijkstra`(4, 未收錄) |
| `typedef unsigned long long ull;` | `tree_hash`(5) |

其餘 37 個檔就地展開。`vi` 其實從來沒被當成型別用過
（`MinimumSteinerTree` 那兩處是參數名 `vi`），所以沒有動。

## Phase 1c — 套用契約 macro

- **非幾何 `.X`/`.Y` → `.F`/`.S`**：54 處，集中在 `Centroid_Decomposition`(14)、
  `IntervalContainer`(8)、`Vizing`(5)、`AdaptiveSimpson`(3)、`ax+by=gcd`(3)、
  `Minimum_Arborescence_fast`(3)、`SchreierSims`(3)、`min_heap`(2)、`ax=b%n`(2)、
  `DynamicConvexTrick_bb`(1)、`Texas_holdem`(27, 未收錄)。
- **裸 `.first`/`.second` → `.F`/`.S`**：`Polynomial_Operation`(4)、`DynamicMST`(17)、
  `chineseRemainder`(1)、`AdaptiveSimpson`(1)、`KDTree_useful`(2)。
  幾何章的一處（`PolyUnion:28`）改成 `.Y` 以配合該章慣例。
- **`push_back` → `pb`**：13 處。
- **`REP`/`fi` 刪 define 改用 `FOR`**：`Maximum_Weight_Matching` 的 `REP`(18 處)、
  `Polynomial_Operation` 的 `fi`(16 處)。`DLX` 的 `TRAV`、`SW-mincut` 的無參數 `REP`
  保留（`FOR` 表達不了無參數迴圈頭，換掉反而變長）。
- **可換的迴圈 → `FOR`**：**236 處**。判定方式是逐一取出迴圈本體做括號配對，
  確認 (1) 上界的識別字沒有在本體裡被賦值 / `++` / `push_back` / `resize` / `erase`…
  (2) 本體沒有用到 `I` (3) 上界不是含 `&&`/`||`/`?:` 的複合條件 (4) 上界沒有真正的函式呼叫。
  沒換的只有 3 個：`De_Bruijn_sequence:8`（上界 `ptr` 在本體裡會變）、
  `DelaunayTriangulation:57`（`i` 在本體裡會變）、`DelaunayTriangulation:106`
  （上界是 `now->num_chd()`，真的函式呼叫）。`5_String/SAIS-C++20.cpp` 整檔跳過
  （它自己有 `auto I`，會跟 `FOR` 展開的 `I` 撞名）。

## Phase 1d — `F`/`S` 撞名改名

| 檔案 | 改名 |
|---|---|
| `5_String/KMP.cpp` | `int F[MAXN]` → `fail` |
| `8_Geometry/Convexhull3D.cpp` | `auto F = [&]` → `add` |
| `4_Flow_Matching/Maximum_Weight_Matching.cpp` | `vector<int> S` → `sl` |
| `8_Geometry/Intersection_of_polygon_and_circle.cpp` | `double S` → `ar` |
| `8_Geometry/HPIGeneralLine.cpp` | `LN(pll S, pll T)` → `LN(pll p, pll q)` |
| `2_Graph/NumberofMaximalClique.cpp` | `int S` → `cnt` |
| `9_Else/SubsetSum.cpp` | `template<size_t S>` → `MX`（未收錄） |

順帶：`2_Graph/Vizing.cpp` 有一個全域 `int X[N]`。在舊的 `#define X first` 之下它被
一路改寫成 `first[N]`，能編但很嚇人；現在 X/Y 只在幾何章 define，這個地雷就沒了。

## Phase 1 順手修掉的三個 macro 地雷

- `9_Else/ManhattanMST.cpp:9` `push_back({a, b, c})` → `pb(a, b, c)`
  （`pb` 是 `emplace_back`，braced-init-list 推導不出來）。
- `9_Else/DynamicConvexTrick_bb.cpp:6` `ll(1e9)` → `(ll)1e9`
  （`ll` 是 `#define` 不是 typedef，functional cast 會變成 `long long(1e9)`）。
- `5_String/Manacher.cpp:8` **不能**用 `pb` — `s` 是 `std::string`，
  只有 `push_back` 沒有 `emplace_back`。這是唯一一處 `push_back` 保留原樣。

## Phase 2 — `++i` → `i++`

for 迴圈第三段、且是內建整數型別的，**38 處**全部改成後綴。沒動的：

- `5_String/PalTree.cpp:41` — 迴圈變數是 `reverse_iterator`，後綴不等價
- `5_String/Z-value.cpp:5` `++z[i]`、`2_Graph/Vizing.cpp:12` `++X[u]` — 不是單純變數
- `5_String/KMP.cpp:5` `fail[++i] = ++j` — 前綴的值有被用到
- for 以外的所有 `++x`

`clang-format` 當時本機沒裝，所以這一階段先跳過；見下面的 Phase 6。

## Phase 3 — 重跑驗證

| 檢查 | 結果 |
|---|---|
| `g++ -fsyntax-only`（108 個收錄檔，只給那六行 + 幾何章 prelude） | **43 通過**（改之前 41），**零退步**；新通過的是 `SA_LCP_becaido`、`Fast_Walsh_Transform` |
| Library Checker 53 列 | **42 AC，判定與 baseline 逐列完全相同**（`RESULT.md` vs `RESULT.legacy.md`） |
| 執行時間 | 同樣 `-j 4` 之下，只有 `graph/biconnected_components` 0.617→0.719s（12%→14% TL），其餘皆在雜訊內 |
| 行尾 | 81 個 CRLF 檔全部保持 CRLF，沒有整檔重寫 |
| PDF | 重編過，25 → **26 頁**。收錄檔淨增 **14 行**（10 行檔首 define + 6 行幾何章 prelude − 2 行 `2SAT` 重寫 − 2 行 `Polynomial_Operation` 刪 `fi` define/`#undef`）。實驗過：把幾何 prelude 那 6 行拿掉還是 26 頁，所以不是卡在那個邊界。（重編出來的 `codebook.pdf` 已還原成 HEAD 版本，沒有留在 working tree） |

`content.tex` 的 `% test by` 註解已回寫 28 條 Library Checker 結果，
並把 `LiChaoST` 那句假的 "test by Library Checker Line Add Get Min" 改掉。

## Phase 5 — ModMin 修正（審核後追加）

`6_Math/ModMin.cpp:6`

```diff
-  if (ll y = mod_min(c, a, a - r % a, a - l % a))
+  if (ll y = mod_min(c, a, a - r % a, a - l % a); ~y)
```

失敗值 `-1` 在 C++ 是 truthy，所以「子問題無解」會被當成「答案是 −1」繼續算，
結果就是無解時回傳 `0` 或負數而不是 `-1`。對稱的另一半（合法的 `y == 0` 是 falsy）
量過之後在 m ≤ 60 的 171 萬組裡出現 **0 次**，所以只有這一個成因。

- 改後行寬 53，仍在 `ColumnLimit: 55` 之內。
- 暴力對拍：m ≤ 90、`0 ≤ a < m`、`0 ≤ l ≤ r < m` 共 **8 508 045 組，零不符**。
- `verify/drivers/min_of_mod_of_linear.cpp` 原本會重新驗證 `mod_min` 的回傳值來繞過
  這個 bug，現在拿掉了，直接相信 `-1`；`number_theory/min_of_mod_of_linear`
  仍然 **AC，17 筆，max 0.265s / 10s (3%)**。
- `content.tex:83` 的括號註解移除，只剩 `% test by Lib-Checker Min of Mod of Linear`。


## Phase 6 — clang-format（審核後追加）

裝法：`uv tool install clang-format==22.1.8`（PyPI 的 static wheel，不動系統套件）。
先比過 13.0.1 / 18.1.8 / 20.1.8 / 22.1.8：20 和 22 輸出完全相同，13 明顯較差
（見下），而且 18 以後對這份 `.clang-format` 沒有任何 deprecated-key 警告。

**`.clang-format` 改了一行**：`ForEachMacros` 加入 `FOR`。
沒加的話 clang-format 不知道 `FOR (...)` 是迴圈，會 (a) 把 `FOR (` 的空白吃掉變成 `FOR(`，
(b) 不縮排迴圈本體 —— 巢狀 `FOR` 會全部被拉平到同一欄。加了之後兩個問題都沒有。
（clang-format 13 加了 `FOR` 之後又會把單行的 `FOR (...) stmt;` 硬拆成兩行，
18 以上才會遵守 `AllowShortLoopsOnASingleLine`，這是不用 13 的主因。）

**只 format 這次動過的行**，不是全書重排：

```
git-clang-format --extensions cpp HEAD -- codebook \
  ':(exclude)codebook/1_Basic/Default_code.cpp'
```

- 排除 `1_Basic/Default_code.cpp`：`#define FOR(x,a,b) ...` 加上空白之後是 56 欄，
  超過 `ColumnLimit: 55`，clang-format 會把它拆成反斜線續行的兩行。macro 契約那六行要逐字保留。
- 全書重排的話是 83/106 檔、字元 +2.5%、行數 +9.6%，而且會拆掉作者手調的排版
  （例如 `struct E { int s, t; ll w; };` 會變成四行）。這個沒有做。

結果：

| 檢查 | 結果 |
|---|---|
| 動到的檔案 | 49 個，淨 **+579 字元 / +44 行** |
| 單檔字元變化最大 | `1_Basic/Texas_holdem.cpp` +10.4%、`8_Geometry/Intersection_of_polygon_and_circle.cpp` +11.8%，其餘都在 8% 以內 |
| 超過 55 欄的行（收錄檔） | 205 → **170** |
| `g++ -fsyntax-only` | format 前後都是 **42 通過**，失敗清單逐檔完全相同 |
| Library Checker | **42/53 AC，逐列判定與 baseline 相同** |
| 行尾 | 49 個檔全部保持原本的 CRLF / LF，零例外 |
| `6_Math/Big_number.cpp`（非 UTF-8） | 10 個 non-ASCII byte 的序列逐 byte 相同 |
| `#include` | 沒有重排，只有 `#include<bits/stdc++.h>` → `#include <bits/stdc++.h>` 一處補空白 |

## Phase 7 — 人工審核後的改動

### 修正（A1/A2/A4/A5/A6）

| 檔案 | 改了什麼 | 驗證 |
|---|---|---|
| `6_Math/ModMin.cpp` | `; ~y`（見 Phase 5） | m ≤ 90 共 851 萬組對拍全對；LC 17 筆 AC |
| `7_Polynomial/Fast_Fourier_Transform.cpp` | 整份換成 kactl `FastFourierTransform.h`：自足的 `fft(vector<cplx>&)` + `conv(vd, vd)`，不再是「照抄 NTT」的宣告 | 500 組隨機卷積對拍，最大絕對誤差 1.86e-9 |
| `7_Polynomial/Fast_Fourier_Transform_Mod.cpp` **(新)** | kactl `convMod`，拆 √M 的三次卷積 | `convolution_mod_1000000007` **48/48 AC**（原本 40/48，`fft_killer` 全過），0.370s / 10s |
| `8_Geometry/Minimum_Enclosing_Circle.cpp` | `random_shuffle` → `shuffle(ALL(dots), mt19937(random_device{}()))`；不存在的 `excenter(i,j,k,r)` → `circenter(i,j,k)` + 自己算 `r` | `geo/minimum_enclosing_circle` **25 筆 AC**，0.032s / 5s |
| `8_Geometry/Heart.cpp` | `circenter` 補上 `pdd center;` 宣告（原本寫進一個沒宣告的全域）；`incenter` 的 `a * p1` → `p1 * a`（只有 `operator*(pdd, double)`） | 同上（driver 同時 include 兩個檔） |
| `6_Math/DiscreteLog.cpp` | `kStep = sqrt(m) + 1`（原本寫死 32000）；giant-step 迴圈變數改 `ll`（原本 `i < m + 10` 在 m 近 2^31 會溢位） | m ≤ 300 的 904 萬組對拍全對；LC 20 筆 AC，時間不變 |
| `2_Graph/SCC.cpp` | **只做 tab → 2 空格**，演算法沒動 | `graph/scc` 12 筆 AC |
| `2_Graph/2SAT.cpp` | 換成 kactl `2sat.h` 的移植：自持有圖（不再需要全域 `adj` 和 `SCC.cpp`），補 `set_value` / `at_most_one` | `other/two_sat` 18 筆 AC；2 萬組隨機實例對 2^n 暴力（判定 + 回傳解都驗）全對 |
| `4_Flow_Matching/MincostMaxflow.cpp` | 換成 kactl `MinCostMaxFlow.h` 的移植（含 pbds decrease-key） | 4000 組隨機（2000 組含負權走 `setpi`）對 SPFA 參考全對；`graph/assignment` 14 筆 AC，**1.617s / 5s (32%)** |

**A3 撤回**：`LiChaoST.cpp` 沒有錯，`content.tex` 那行已還原成原本的
`% test by Library Checker Line Add Get Min`。

### 新增模版

| 檔案 | 行數 | 出處 | 驗證 |
|---|---|---|---|
| `7_Polynomial/Fast_Fourier_Transform_Mod.cpp` | 32 | kactl `FastFourierTransformMod.h` | LC 48/48 |
| `8_Geometry/ClosestPair.cpp` | 19 | kactl `ClosestPair.h`（改寫成 `pll` + `X`/`Y`，不用 `Point<ll>`） | LC 29/29 |
| `9_Else/MatroidIntersection.cpp` | 67 | 照 `9_Else/Matroid.tex` 自己寫（std_abs 那份有死迴圈，不能抄） | 3000 組隨機圖擬陣 ∩ 分割擬陣，對 2^m 暴力全對 |

`content.tex` 新增三行，並回寫 7 條 `% test by` 註解。

### MinCostMaxflow：一開始的偏離，量過之後改回 kactl 原樣

我第一版把 kactl 的 `__gnu_pbds::priority_queue`（decrease-key）換成
`std::priority_queue` + lazy deletion，理由是「複雜度一樣，可以少一個 include」。
**這個判斷是錯的**，`graph/assignment` 的資料直接打臉：

`hand_minus_00` 的矩陣是 $a_{ij} = -(i(i{+}1) + j(j{+}1))$ ——**可分離**，
所以每個排列的總和都一樣，全部同分。`multiplication_table_00` 是
$a_{ij} = (1+i)(1+j)$，rank-1。這兩種矩陣讓 reduced cost 大量打平，
Dijkstra 每一輪幾乎每條邊都會鬆弛成功：

| case | Dijkstra 輪數 | pq push（lazy 版） | lazy | pbds decrease-key |
|---|---:|---:|---:|---:|
| `hand_minus_00` | 501 | **63 000 252** | 4.31s | **0.50s** |
| `multiplication_table_00` | 501 | 42 042 751 | 4.22s | 1.31s |
| `max_random_00` | 501 | 1 973 837 | 0.59s | 0.56s |
| `hand_plus_00` | 501 | 501 001 | 0.46s | 0.80s |

lazy deletion 的堆最多裝 $O(E)$ = 25 萬個項目，decrease-key 版最多裝 $O(V)$ = 1002 個。
隨機資料上兩者一樣（甚至 lazy 略快），但退化資料上差 8.7 倍。

所以最後**照 kactl 原樣用 pbds**，模版第一行是
`#include <ext/pb_ds/priority_queue.hpp>`（比 kactl 的 `<bits/extc++.h>` 小，編譯快）。

### 驗證總結

| 檢查 | 結果 |
|---|---|
| Library Checker | **46 AC / 55 列**（上一輪 42/53），0 WA，9 BLOCKED，**0 列超過 50% TL** |
| `g++ -fsyntax-only`（109 個收錄檔） | 47 通過。今天動過的檔案裡沒過的 4 個都是「相依前面的模版」：`SCC`(全域 `adj`)、`DiscreteLog`(`fpow`)、`FFT_Mod`(`fft`)、`Minimum_Enclosing_Circle`(`circenter`)，不是缺陷 |
| 行尾 | 對 HEAD 逐檔比對，**零漂移**；三個新檔跟所在章節一致（都 CRLF） |
| `ColumnLimit: 55` | 新增/改寫的檔案全部在 55 欄內 |
| PDF | 重編 25 → **27 頁**（上一輪 26）。`codebook.pdf` 已還原成 HEAD 版本 |


## Phase 8 — 效能回頭審查（你要求的）

把所有改動重新查一次有沒有變慢。方法：從 HEAD `git archive` 出一份原始 codebook，
配上一份「還原舊 macro」的 prelude，把**同一個 driver** 各編一次，
序列（`-j 1`）、每題跑完全部測資、best-of-2 對比。

### 結論一：macro 正規化 + `FOR` 轉換沒有任何退步

33 個 driver 兩邊都編得起來，比值全部落在 **0.91x – 1.03x**：

- 最快的是 `discrete_logarithm_mod` **0.91x**（`kStep` 改自適應之後小 m 省掉 32000 次 baby step）
- 之前 `-j 4` 的表上 `general_matching` 2.00x、`min_plus_convolution` 1.42x、`eertree` 1.29x
  全是**平行競爭造成的量測雜訊**，序列重測分別是 0.96x / 1.00x / 0.99x

順帶做的正確性複查：

- 所有 `size_t` / 非 int 的迴圈都**沒有**被轉成 `FOR`（`FOR` 會強制 `int`），
  只是 `++i` → `i++`。`6_Math/Big_number.cpp` 那個在迴圈裡 `pb(0)` 的
  `for (size_t i = 0; i < size(); i++)` 也正確地保持成一般迴圈。
- 沒有任何 `++it` → `it++`（iterator 後綴會多一次複製）。
- 沒有任何 `FOR` 把「本體會改動的容器大小」快取起來。
  （`9_Else/DynamicMST.cpp:57` 掃出來過，但 `x.clear()` 在迴圈**之後**，是誤判。）

### 結論二：兩個我自己弄出來的退步，已修

**(1) `MincostMaxflow` 改用 vector —— 改回全域 `N` 陣列。**

你說得對，全域 `N` 除了是範圍提醒，也是速度考量。單一大圖上沒差（LC assignment 1.01x），
但多測資時 `init()` 能留住 capacity，vector 版每次重新配置：

| T × n × m | 全域 `N[]` | vector | |
|---|---:|---:|---|
| 200000 × 8 × 12 | 0.084s | 0.140s | **1.67x 慢** |
| 50000 × 50 × 150 | 0.858s | 1.070s | 1.25x 慢 |
| 5000 × 300 × 1500 | 2.475s | 2.801s | 1.13x 慢 |
| 200 × 1000 × 20000 | 4.893s | 5.134s | 1.05x 慢 |

已改回 `Edge *par[N]; vector<Edge> g[N]; int n, vis[N]; ll dis[N], pot[N];` + `init(n)`，
跟 `Dinic` / `isap` / `MinCostCirculation` 同一個形狀。
4000 組隨機圖（2000 組含負權走 `setpi`）重驗全對，LC 14 筆 AC、1.4s / 5s。

**(2) `Minimum_Enclosing_Circle` 每次呼叫都建一個 `mt19937`。**

我原本寫 `shuffle(ALL(dots), mt19937(random_device{}()))`，
`random_device` 每次要讀 `/dev/urandom`、`mt19937` 每次要初始化 624 個 word，
**約 30 µs／次**。多測資時直接爆炸：

| T × n | 每次建 rng | `static` rng |
|---|---:|---:|
| 200000 × 4 | 6.250s | 0.036s（**175x**） |
| 50000 × 20 | 1.580s | 0.061s（26x） |
| 2000 × 500 | 0.077s | 0.041s（1.9x） |
| 50 × 20000 | 0.033s | 0.031s（1.05x） |

已改成兩行：

```cpp
  static mt19937 rng(random_device{}());
  shuffle(ALL(dots), rng);
```

### 結論三：換掉的模版都比原本快

| | HEAD | 現在 |
|---|---:|---:|
| $2^{20}$ 點的正向 FFT ×20 | 2.484s（`FFT<MAXN>::trans`，貼 NTT 本體） | **0.871s**（kactl `fft`）—— 2.85x |
| `convolution_mod_1000000007` | 0.718s（40/48） | **0.369s（48/48）** |
| `assignment`（MCMF） | 無（HEAD 沒測過） | 1.4s / 5s |

`2_Graph/2SAT.cpp` 沒有可比的 baseline（HEAD 那份呼叫的 `scc.add_edge/solve/bln`
根本不存在，編不過）。**已依指示改成全域 `N` 陣列**，見 Phase 9。

### 現況

| 檢查 | 結果 |
|---|---|
| Library Checker（`-j 1` 序列） | **46 AC / 55 列**，0 WA，最慢一列 31% TL |
| `g++ -fsyntax-only` | 46 / 109；`MincostMaxflow` 改回需要全域 `N`（和 `Dinic`/`isap` 一樣） |
| 行尾 | 對 HEAD 零漂移 |
| PDF | 27 頁；`codebook.pdf` 已還原成 HEAD |


## Phase 9 — 全量比對「哪些改動不是純機械的」＋ 2SAT 改全域 N

### 全量比對的做法

寫了一個正規化器（`scratchpad/norm.py`），把這次做過的每一種機械轉換都還原掉：

- `FOR(x,a,b)` / `REP` / `SZ()` / `ALL()` 展開回原形（括號用平衡掃描，不是 regex）
- 每個 `for` 標頭拆成三段重寫成正規形：`i < X` → `i <= X-1`、`++i` → `i++`、
  丟掉 `FOR` 展開留下的 `, I = bound`
- `.X` `.Y` `.F` `.S` `.first` `.second` → `.@1` / `.@2`（`->` 同理）
- `pii` `pll` `pdd` `ull` `i128` `vi` → 展開；`pb` / `push_back` / `emplace_back` → `PUSH`
- 刪掉註解、`#define` 行、`typedef` 行、全部空白

然後把 HEAD（`git archive`）和現在的每個檔案都正規化後逐字元比對。

**結果：107 個 .cpp 裡 76 個正規化後完全相同**，也就是純機械改寫。
剩下 31 個逐一看過，全部落在下面兩類：

**(a) 我的正規化器沒做常數摺疊造成的假警報**（11 個檔案）——
`n+2-1` vs `n+1`、`26-1` vs `25`、`33-1` vs `32`、`m-1-1` vs `m-2`、`3-1` vs `2`、
`10-1` vs `9`、`2-1` vs `1`、`100-1` vs `99`。都是同一個值。

**(b) 我已經報告過的改動**：`SCC` 的 `tarjan(i,i)`→`tarjan(i)`、
`Heavy_light_Decomposition` 的懸空逗號 + `return res;`、`smawk` 的 `lli`→`ll`、
`ModMin` 的 `; ~y`、`DiscreteLog`、`Heart`、`Minimum_Enclosing_Circle`、
`ManhattanMST` 的 `PUSH({..})`→`PUSH(..)`、`DynamicConvexTrick_bb` 的 `ll(1e9)`→`(ll)1e9`、
`Polynomial_Operation` 的 `fi`→`FOR`、`SubsetSum` 的 `S`→`MX`、
以及六個 `F`/`S` 撞名改名（`NumberofMaximalClique` S→cnt、`Maximum_Weight_Matching` S→sl、
`KMP` F→fail、`Convexhull3D` F→add、`HPIGeneralLine` S,T→p,q、
`Intersection_of_polygon_and_circle` S→ar），加上三個換掉的模版
（`2SAT` / `MincostMaxflow` / `Fast_Fourier_Transform`）。

**沒有漏報的改動。**

順帶確認 `Polynomial_Operation` 的 `fi`→`FOR` 安全：`fi(s,n)` 原本是
`for (int i = s; i < n; ++i)`，`n()` 是 `(int)size()`，而 6 個用到 `n()` / `ret.n()`
當上界的迴圈本體都只寫入既有元素，沒有 `resize`/`isz`。所以把上界提到迴圈外
是**純粹的加速**（LC 的 polynomial 五列量到 0.80x–0.97x）。

### 2SAT 改成全域 `N`

跟 `MinCostCirculation` / `Kuhn_Munkres` / `Bipartite_Matching` / `Dominator_Tree`
等約 20 個模版同一個慣例：

```cpp
// 0-base, need global N >= 2 * #vars; ~x means NOT x
struct TwoSat {
  int n, dft;
  vector<int> g[N], st;
  int val[N], comp[N], ans[N];
  void init(int _n) { ... }
  int add_var() { g[n * 2].clear(), g[n * 2 + 1].clear(); return n++; }
```

（`add_var()` 要自己清那兩格，否則物件重複使用時會讀到上一輪的邊。）

比 kactl 的 `vector<vector<int>>` 版快，而且答案逐位元相同：

| T × n × m | 全域 `N[]` | vector | |
|---|---:|---:|---|
| 200000 × 8 × 12 | 0.086s | 0.163s | **1.89x** |
| 20000 × 200 × 400 | 0.237s | 0.567s | **2.39x** |
| 200 × 20000 × 40000 | 0.435s | 0.825s | 1.90x |
| 20 × 100000 × 200000 | 0.608s | 0.844s | 1.39x |

連「單一大實例」都快 1.39x，因為 `solve()` 每次 `val.assign` / `comp = val`
都要重新配置，改成 `fill_n` 打靜態陣列就沒有這個成本。

重驗：LC `other/two_sat` 18 筆 AC（0.372s / 5s）；2 萬組隨機實例對 $2^n$ 暴力
（同一個全域物件重複使用、`at_most_one` 每輪都 `add_var`）判定與解都全對，
數字和 vector 版逐項相同（sat 16068 / unsat 3932）。

### 現況

| 檢查 | 結果 |
|---|---|
| Library Checker（`-j 1`） | **46 AC / 55 列**，0 WA，最慢一列 31% TL |
| `g++ -fsyntax-only` | 45 / 109（`2SAT` 和 `MincostMaxflow` 現在需要全域 `N`，和另外約 20 個模版一樣） |
| 行尾 | 對 HEAD 零漂移 |
| PDF | 27 頁 |


## Phase 10 — 全部改成 LF

之前每一階段的驗證項目都有「行尾對 HEAD 零漂移」，因為原本 CRLF / LF 是混的
（84 個 tracked 檔是 CRLF、89 個是 LF）。現在**全部統一成 LF**。

- 轉換方式是 byte-level 的 `\r\n` → `\n`，並且每個檔案都 assert 過
  「移除的 byte 數 == CRLF 的個數」且「加回 CR 之後和原檔逐 byte 相同」，
  所以 `6_Math/Big_number.cpp` 的 10 個 Big5 byte 原封不動（已逐 byte 比對）。
- 沒有任何檔案是混合行尾的，也沒有落單的 CR，所以不需要人工判斷。
- 範圍：84 個 tracked 檔（81 `.cpp` + `content.tex` + `9_Else/hash.sh` + `note.txt`）
  加上這次新增的 3 個檔，共 **87 個**。
- `.clang-format` 的 `DeriveLineEnding` 由 `true` 改成 `false`。原本是「跟著檔案現有的
  行尾走」，改成 `false` 之後就吃 `UseCRLF: false`，也就是**一律輸出 LF**，
  避免以後又混回來。

驗證：Library Checker **46 AC / 55 列**（不變）、`g++ -fsyntax-only` **46 / 109**（不變）、
PDF 重編 **27 頁**（不變）、`latexmk` exit 0。

### 一個副作用要注意

在這次改動 commit 之前，`git-clang-format HEAD` 會把每個轉過行尾的檔案**整份**
當成「改動過的行」，因此會想重排全書 —— 包含 `1_Basic/Shell_script.cpp`，
那其實是個副檔名叫 `.cpp` 的 **shell script**，clang-format 會把它打成
`g++ - O2 - std = c++ 17 - Dbbq ...`。所以這一輪**沒有**跑 git-clang-format。
commit 之後行為就恢復正常（只看真正改動的行）。

### `.gitattributes`

```
# Every text file is stored and checked out with LF, on every platform.
# Matches .clang-format (UseCRLF: false, DeriveLineEnding: false).
* text=auto eol=lf

# The built book is binary; never touch its bytes.
*.pdf binary
```

`text=auto` 讓 git 自己判斷文字/二進位（判準是有沒有 NUL byte），`eol=lf` 同時管
「commit 進 index」和「checkout 到工作目錄」兩個方向。`codebook.pdf` 另外明確標成
`binary`，避免任何情況下被正規化。

`6_Math/Big_number.cpp` 的 Big5 內容是安全的：正規化只動 CR/LF，而 `0x0D` 不可能是
Big5 的 trail byte（trail byte 是 `0x40–0x7E` 和 `0xA1–0xFE`）。

驗證方式是把整個 repo 複製到暫存目錄、在**複製品**上 commit，再從它 clone 一份：

| 檢查 | 結果 |
|---|---|
| commit 後 `git ls-files --eol` | 177 個 `i/lf w/lf` + 1 個 `-text`（PDF） |
| 全新 clone 出來的 178 個檔 | **含 CR 的：0 個** |
| `Big_number.cpp` 的非 ASCII byte | `[165,166,170,173,183,184,188,206,236]`，不變 |
| `codebook.pdf` | 與原檔 byte-for-byte 相同 |

（驗證用的複製品和 clone 都已刪除，你的 repo 沒有被 commit 過。）

---

# Phase 11：把 std_abs 的 polynomial 章移植進契約

`7_Polynomial/` 底下原本躺著 6 個沒進 `content.tex` 的 `*_stdabs` 檔。
這一輪把它們**真正用得到的部分**搬進我們的 macro 契約，重複的就刪掉。

## 逐檔處置

| std_abs 檔 | 處置 | 理由 |
|---|---|---|
| `FFT_stdabs.cpp` | 刪 | 本體是 `// see NTT` 的殼；`Fast_Fourier_Transform.cpp`（kactl 版）已自足且過 LC |
| `NTT_stdabs.cpp` | 刪 | 與 `Number_Theory_Transform.cpp` 同一套蝶形，但依賴外部 `mul/add/sub/Pow/mod/G/N` |
| `Fwt_stdabs.cpp` | 刪 | `fwt()` body 是 `// do something`，`subs_conv()` 叫的 `or_fwt` 不存在；`Fast_Walsh_Transform.cpp` 是完整版 |
| `FastLinearRecursion_stdabs.cpp` | 刪 | 與 `Poly::LinearRecursion` 同一個 Kitamasa，連「每輪重算一次 `Inverse`」的慢法都一樣 |
| `Operation_stdabs.cpp` | 取三個新東西後刪 | Mul/Inverse/Divide/Derivative/Integral/Ln/Exp/PolyPow/Evaluate/Interpolate 我們都有 |
| `NTTprime_stdabs.tex` | 改寫成 `NTTprime.tex` 並收進書 | 22 組 NTT 質數，原本沒進 `content.tex` |

## `Polynomial_Operation.cpp`：+4 個成員

- `_fac(m)` — 就地算 $i!$ 與 $1/i!$，所以下面兩個不依賴全域 `fac[]/facp[]`。
- `Shift(c)` — Taylor shift，$f(x) \to f(x+c)$。
- `SampleShift(a, c, m)` — 給 $f(0..k-1)$ 求 $f(c..c+m-1)$。
- `PowerProj(w, m)` — $\sum_j w_j [x^j] f^i$，$i = 0..m$。**要求 `(*this)[0] == 0`**。

移植時踩到的一個坑：原版 `power_proj` 裡 `n2`/`n4` 是「一開始的 n」算出來的定值，
但 gather 那兩行的 stride `2 * n` 用的是**每輪減半的 n**。照 `n2` 寫會全錯。

## `Sqrt()` 補上前導零與「無解」

原本的 `Sqrt()` 前提是 `(*this)[0]` 是二次剩餘，否則靜靜回傳一堆 0。
照 `Operation_stdabs.cpp` 的作法拆成兩層：

- `_sqrt()`：原本那四行 Newton 疊代，前提不變。
- `Sqrt()`：剝掉 $x^z$、`z` 是奇數或 $a_z$ 非二次剩餘就回傳 `{}`，其餘丟給 `_sqrt()` 再左補 $z/2$ 個零。

## 順帶抓到的 harness bug（重要）

`run.py` 呼叫官方 checker 的參數順序是
`checker <in> <jury answer> <our answer>`，但 testlib 是
`checker <in> <our answer> <jury answer>`。

對 wcmp 那種「兩邊 token 比對」的 checker 沒差（對稱）；
但對**只驗證選手輸出**的 special judge（`sqrt_of_formal_power_series`、
`sort_points_by_argument`、`bipartitematching`、`sqrt_mod`、`find_linear_recurrence`…），
這等於每次都拿標準答案去驗標準答案，**必然 AC**。

修好之後 58 列全部重跑：只有 `sqrt_of_formal_power_series` 真的翻成 WA
（35 筆裡 20 筆錯，連 `example_00` 都錯），其餘維持原判。上面的 `Sqrt()` 就是為了修它。

## 驗證

| 檢查 | 結果 |
|---|---|
| Library Checker（`-j 1`，修正後的 checker 順序） | **48 AC / 58 列**，0 WA |
| 新增的 3 列 | `polynomial_taylor_shift` AC 38 筆 7% TL、`shift_of_sampling_points_of_polynomial` AC 32 筆 17% TL、`kth_term_of_linearly_recurrent_sequence` **TLE** 123% TL（答案正確） |
| `PowerProj` 本地對拍 | 400 組隨機 vs $O(n^2m)$ 樸素，$n \le 60$、$m \le 80$、係數取遍 mod；另跑 $n = 2^{16}/2^{17}$ 確認不炸 |
| PDF | 重編 **27 頁**，`Overfull \hbox` 0 個 |

## 還沒修的

`Poly::LinearRecursion` 在 $d = 10^5$、$k = 10^{18}$ 要 12.3s（TL 10s）。
答案是對的，慢在 `DivMod` 每一輪都重算一次 `Inv(C)`——但 `C` 是固定的，
約 120 輪重算 120 次。要嘛把 `Inv(C)` 提到迴圈外，要嘛整個換成 Bostan–Mori。
std_abs 的 `FastLinearRecursion` 也是同一個寫法，所以不是移植造成的退步。

---

# Phase 12：polynomial 的係數型別 ll -> int

## 起因

「std_abs 那份跑起來比較快」是真的。量給定的五題（`-j 1`，總時間）：

| | 我們（`ll`） | std_abs（`int`） |
|---|---|---|
| Inv | 6.45s | 4.52s（快 1.43x）|
| Exp | 26.73s | 17.41s（快 1.54x）|
| Log | 9.84s | 7.13s（快 1.38x）|
| Sqrt | 12.13s | 11.21s（快 1.08x）|
| Pow | 31.22s | **4 筆無窮迴圈**（30s 上限砍掉）|

但差距不在演算法，在**係數型別**：我們的 `NTT` 是 `ll w[MAXN]`、`Poly : vector<ll>`，
std_abs 是 `int`。$P < 2^{31}$，係數放 `int` 綽綽有餘，而 NTT 是記憶體頻寬綁住的，
8 bytes 改 4 bytes 直接砍半。

驗證：把我們的 `Inv` 原封不動搬去 `int` 係數，總時間 6.34s → **3.66s**，
比 std_abs 的 4.46s 還快 1.22x。所以要換的是型別，不是整份程式碼。

## 改了什麼

- `NTT<int MAXN, int P, int RT>`，`int w[MAXN]`，轉換 `int *a`。
- `Poly : vector<int>`。
- 每個乘法逐一加 `(ll)` 前綴：`(ll)a * b % P`。
- 兩處三個剩餘相加會爆 `int` 的（`PowerProj` 的 `b[n2 + i]`）也補上 `(ll)`。
- `mpow(int a, ll n)` — 第二個參數保持 `ll`，**這正是 std_abs 踩到的地雷**：
  它的 `PolyPow` 把 $k \approx 10^{18}$ 丟進 `Pow(int a, int b)`，截斷後的負 `b`
  在 `b >>= 1` 下永遠到不了 0，5e5 的四筆測資全部卡死。

## 結果

| LC 題 | 之前 | 之後 | 加速 |
|---|---|---|---|
| `inv_of_formal_power_series` | 0.617s | **0.467s** | 1.32x |
| `exp_of_formal_power_series` | 2.221s | **1.520s** | 1.46x |
| `log_of_formal_power_series` | 0.869s | **0.667s** | 1.30x |
| `pow_of_formal_power_series` | 3.174s | **2.377s** | 1.34x |
| `sqrt_of_formal_power_series` | 1.570s | **1.118s** | 1.40x |
| `polynomial_taylor_shift` | 0.368s | **0.267s** | 1.38x |
| `shift_of_sampling_points` | 0.868s | **0.667s** | 1.30x |
| `convolution_mod` | 0.371s | **0.326s** | 1.14x |

對照 std_abs 的 max 時間：exp 1.618s vs 我們 1.520s、sqrt 1.618s vs 我們 1.118s，
全面追平或超越，而且 `Pow` 會正常結束。

**48 AC / 58 列**不變，0 WA。

## 介面變動

`Eval` / `Interpolate` / `SampleShift` / `PowerProj` / `LinearRecursion`
現在收發 `vector<int>`，`LinearRecursion` 回傳 `int`。`Pow(ll k)` 的 `k` 維持 `ll`。

## 還沒修的

`kth_term_of_linearly_recurrent_sequence` 從 142% 進步到 **115% TL**，還是差一點。
剩下的是演算法問題不是型別問題：`DivMod` 每輪重算 `Inv(C)`，而 `C` 固定，約 120 輪重算 120 次。

---

# Phase 13：LinearRecursion 加速 + BM 介面 1-based

## LinearRecursion：11.485s -> 5.5s（TL 10s），TLE 變 AC

`DivMod` 每次被呼叫都會對 `reverse(rhs)` 求一次逆，而 `LinearRecursion` 拿同一個固定的
`C` 呼叫它約 $2\log_2 n \approx 120$ 次。把 `Inv` 提到迴圈外算一次，取模那步 inline 進來。

**中途踩到的坑**：第一版把預算的逆算到長度 `max(1, k - 1)`——那是「M 已經 mod C 之後」的商長度，
但 `M` 的初值是 `{0, 1}`，所以 $k = 1$ 時第一次 `M.Mul(M)` 就需要長度 2，
`Poly(Ci, m)` 只會補零而不是真的求逆。結果每個 $k = 1$ 的 case 從 $a_2$ 起全錯。

**Library Checker 的測資完全沒有 $d = 1$**，所以那題照樣 20 筆 AC。
是本地對拍（BM 推係數再重播整段前綴）抓到的：2000 組裡 260 組錯，正好是 $k = 1$ 的比例。
改成 `max(2, k)` 之後 0 錯。

## BM 改成 1-based

原本 `BerlekampMassey` 回傳 $c_1..c_k$ 放在索引 $0..k-1$，而 `Poly::LinearRecursion`
要 $c_j$ 放在索引 $j$、外加一個沒用的 `coef[0]`，所以每次串接都得平移一格。
現在兩邊都是 1-based，階數變成 `SZ(c) - 1`。

空的情況也更規律了：完全沒有遞迴的數列回傳 `{0}`，`SZ(c) - 1 == 0` 照樣讀作階數。

膠水現在只剩型別轉換：

```cpp
auto c = BerlekampMassey(s);
vector<int> a(c.size() - 1), coef(c.size());
FOR (i, 0, (int)c.size() - 2) a[i] = s[i].v;
FOR (j, 1, (int)c.size() - 1) coef[j] = c[j].v;
int ans = Poly_t::LinearRecursion(a, coef, n);
```

**型別刻意不統一**：BM 是 `template <typename T>`（只要求 `+= -= * /` 和一元 `-`），
可以吃自訂 modint 甚至有理數；`Poly` 則被 NTT 的 $P$ 綁死，本來就不可能泛型。
要統一型別就得讓 codebook 收一份 modint 模版，那是另一個決策
（順帶會一起解掉 `Value_Poly.cpp` 的「需要外部 `mint`」）。

## 驗證

| 檢查 | 結果 |
|---|---|
| Library Checker | **49 AC / 58 列**，0 WA、**0 TLE**——第一次沒有任何非 BLOCKED 的失敗列 |
| `other/find_linear_recurrence` | AC 18 筆 · 0.164s/5s (3%) |
| `other/kth_term_of_linearly_recurrent_sequence` | AC 20 筆 · 6.625s/10s (66%) |
| BM -> LinearRecursion 對拍 | 600 組 1..40 階隨機遞迴，重播整段前綴，0 錯；另外拿產生用的係數直接餵也 0 錯 |

## PDF 變成 28 頁

原本 27 頁。這一輪 polynomial 章加的東西（`Shift` / `SampleShift` / `PowerProj` /
`_fac` 約 60 行、NTT 質數表、`LinearRecursion` 的 8 行）把它推過了邊界，
第 28 頁只有 9 行（`BitsetLCS` 的尾巴 + Python 那節）。

我已經把註解壓掉 3 行，還是回不去。要回 27 頁最省事的是砍 `PowerProj`（25 行，
這批新東西裡最冷門的一個，而且要另外的理論才用得上）——但那是取捨，沒有自己決定。

---

# Phase 14：把「介面不足無法驅動」的那幾個補起來

九個 BLOCKED 裡挑了四個處理。判準：先修真 bug，再補介面；語意本來就跟 LC 題不同的不動。

## A. `MinimumSteinerTree.cpp`：兩個真 bug

### A1. int 溢位

`int dst[N][N]`、`int dp[1 << T][N]`，但 LC 的參數是 $N \le 100$、$W \le 10^9$，
答案上界 $99 \times 10^9 \approx 9.9 \times 10^{10}$，遠超 `int`。全部改 `ll`。
對拍時實際看到的最大答案是 **8 710 560 576**，確認不是理論上的擔心。

### A2. `vcst`（頂點成本）整個是壞的

`dp[0][i] = vcst[i]`、合併那步 `dp[sub][i] + dp[msk^sub][i] - vcst[i]` 都假設
`dp[msk][i]` 含 `vcst[i]`，但單終端的初始化 `vcst[ter[who]] + dst[ter[who]][i]`
和延伸那步 `dp[msk][j] + dst[j][i]` 都不含 —— `dst` 是純邊權最短路，
路徑上的中間點和終點的 `vcst` 一律沒算進去。

對拍：`vcst` 全 0 時 4000 組全對；一開 `vcst` 就 **2921/4000 錯**。

修法只動 `shortest_path()`：Floyd 改成 `dst[i][k] + vcst[k] + dst[k][j]`
（把中間點的成本併進最短路），最後再對 $i \ne j$ 補上 `dst[i][j] += vcst[j]`。
這樣 `dst[i][j]` 的語意變成「$i \to j$ 這條路上除了 $i$ 以外每個點的 `vcst` 都算到」，
原本 dp 那三條轉移就全部自洽了，而且 `vcst` 全 0 時新舊 `dst` 完全相同，向下相容。

### 為什麼不改成能上 LC

LC 要輸出**選了哪些邊**，回溯得同時記 Floyd 的中繼點、dp 的來源、以及邊的索引，
模版會胖 15–20 行。改用本地對拍：隨機小圖 vs「枚舉 Steiner 點集 + Prim」，
$4 \times 4000$ 組（大小權重 × 有無 `vcst`）全對，模版一行沒動。

## B. 三個補介面後上 Library Checker

| 模版 | 改了什麼 | 結果 |
|---|---|---|
| `9_Else/tree_hash.cpp` | `return h[u] = sum;` + 一個 `h[N]`。逐點 hash 本來就是這模版該給的 | `rooted_tree_isomorphism_classification` **AC** 17 筆 · 3% TL |
| `3_Data_Structure/link_cut_tree.cpp` | 聚合從寫死的 xor 改成 sum（`val`/`sum` 一起換 `ll`，2e5 個點 × 1e9 會爆 int）；註解寫明換回 `^` 就是 xor 版 | `dynamic_tree_vertex_add_path_sum` **AC** 15 筆 · 10% TL |
| `9_Else/All_LCS.cpp` | `void all_lcs(s, t)` 改成 `struct AllLCS`，建構吃 `t`、`push(c)` 一次推進一個字元、`h` 公開 | `prefix_substring_lcs` **AC** 11 筆 · 2% TL |

LCT 是全書最容易寫錯的模版，這是它第一次有回歸測試。

## C. 沒動的五個

- `Centroid_Decomposition`、`fac_no_p`、`LiChaoST`：語意本來就跟 LC 題不同，為了測而改是扭曲模版設計
- `Linear_Equations`（要 modint）、`MinCostCirculation`（要對偶變數、答案超 $2^{64}$）：牽涉更大的決策，不是補幾行

## 結果

| | 之前 | 之後 |
|---|---|---|
| Library Checker | 49 AC / 58 列 | **52 AC / 58 列**，0 WA、0 TLE |
| 無法驅動的 | 9 | **6** |
| 有本地對拍的模版 | 11 | **12** |

---

# Phase 15：Linear_Equations 泛型化

`matrix` 綁死在 `fraction` 上，而且直接摸表示法來判斷零（`!M[i][piv].n`）。
`Fraction.cpp` 在 `content.tex` 裡是註解掉的，所以書上根本沒有能餵給它的型別；
而 LC 那題是 mod 998244353，500×500 的有理數消去也會爆。

改成 `template <typename T>`，T 只需要 `+ - * /` 和 `== T()`。
模版原本就已經回傳 `rank`（解空間維度）、`sol`（一組解）、`basis`（核空間基底），
**跟 LC 要的輸出一字不差**，所以 driver 只要自備一個 modint 就通了。

`Fraction` 順手補上 `operator==`（兩個欄位永遠是約分過且 `d > 0`，比 pair 就夠）。

| 檢查 | 結果 |
|---|---|
| `linear_algebra/system_of_linear_equations` | **AC** 27 筆 · 0.515s/5s (10%) |
| fraction 路徑 | 3000 組隨機系統，驗 `A·sol = b` 且每個 `basis[k]` 在核空間，0 錯 |
| 全套 | **53 AC / 58 列** |

## 跟 kactl / std_abs 比

| | 有效行數 | 給什麼 | 型別 |
|---|---|---|---|
| codebook（現行） | 54 | rank + 一組解 + **核空間基底** | 泛型 `T` |
| kactl `SolveLinear.h` | 35 | rank + 一組解 | 只有 `double` |
| kactl `SolveLinear2.h` | 8（接在上面） | 標出哪些變數唯一確定——**仍然不是基底** | `double` |
| std_abs `LinearEquationSolver.cpp` | 51 | 增量加方程式 + 一組解 | 寫死 mod，只吃方陣 |

**只有 codebook 這份給核空間基底**，所以 kactl 和 std_abs 兩份都沒辦法直接打 LC 那題。

## 可以更短：36 行的候選版（未套用）

放在 `scratchpad/short/Linear_Equations.cpp`，功能完全相同，**54 → 36 行（省 33%）**，
LC 27 筆全過、fraction 3000 組全對，而且**還快一點**（`max_00` 0.469s vs 0.515s）。

縮的三處：

1. `while (piv < m && M[i][piv] == T()) piv++;` 在三個地方重複找 pivot。
   第一輪存進 `pv[i]` 就好——之後的消去動不到它（第 i 列在自己的 pivot 那一行，
   對其他列而言已經是 0，所以再怎麼消都不會改變）。省兩次掃描。
2. 拿掉 `with_basis` 開關（6 行的 if/else 變 3 行）。它的用途是「不要基底時少做整列正規化」，
   但實測拿掉反而比較快——省下的重找比多做的除法貴。
3. `return` 之後那個 `else if` 是多餘的。

---

# Phase 16：SteinerTree 加構造解

`solve()` 只回傳花費，所以打不了 LC（那題要輸出選了哪些邊）。補上回溯。

要記三樣東西：

- `mid[i][j]`：Floyd 鬆弛用的中繼點，用來把一條最短路展開成實際的邊
- `eid[i][j]`：`add_edge` 時記下這對點目前用的是哪條邊（多重邊只留最短的那條）
- `fr[msk][i]`：dp 的來源。編碼成一個 int —— `0` 是 base，`s > 0` 表示拆成 `s` 和 `msk ^ s`，`-j - 1` 表示從 `(msk, j)` 沿最短路延伸過來

回溯 `walk(full, argmin)` 就把邊收齊，`sort + unique` 去重。
（去重理論上是多餘的：若回溯的邊有重複，去掉重複後仍連通所有終端且權重更小，
跟 dp 的最小性矛盾。留著是便宜的保險。）

## 順帶清掉的兩處

- **seed 那步**從 `dp[msk][i] = vcst[ter[w]] + dst[ter[w]][i]` 改成只設 `dp[msk][ter[w]] = vcst[ter[w]]`，
  剩下交給後面的延伸步驟做——結果完全一樣，但回溯時它就單純是個 base case。
- **延伸那步**原本要 `tdst[]`/`tfr[]` 兩個暫存陣列再 `copy_n` 回去，現在直接就地更新。
  安全的理由：`dst` 滿足三角不等式，所以用到已更新的 `dp[msk][j]` 不會給出更小的值
  （`dp'[j] + dst[j][i] = min_k dp[k] + dst[k][j] + dst[j][i] >= min_k dp[k] + dst[k][i] = dp'[i]`），
  而未更新的那些又保證至少取到原本的最小值。省掉兩個陣列和兩行 `copy_n`。

## 結果

| 檢查 | 結果 |
|---|---|
| `graph/minimum_steiner_tree` | **AC** 24 筆 · 0.114s/5s (2%) |
| 本地對拍 | 4 × 4000 組維持 0 錯 |
| 行數 | 46 → **64**（先寫出來是 74，壓掉 10 行） |
| 全套 | **54 AC / 58 列**，無法驅動的剩 **4** |

PDF 因此回到 28 頁。

---

# Phase 17：測試改用平行跑（但時間數字要分開看）

之前每次全套都加 `-j 1`，理由是 Phase 3 踩過「平行跑讓時間膨脹、誤判成效能退步」的坑。
實測之後這個做法是矯枉過正。

## 實測（12 核，`-j 11` vs `-j 1`，同一份程式碼）

| | `-j 1` | `-j 11` |
|---|---|---|
| 全套牆鐘時間 | 6 分 26 秒 | **1 分 25 秒**（快 4.5x）|
| verdict | 54 AC / 58 列 | 54 AC / 58 列，**0 筆不一致** |

逐列比對 49 個有意義的時間（>0.05s）：

| | 膨脹倍率 |
|---|---|
| 中位數 | **1.80x** |
| 最大 | **4.02x**（`string/suffixarray#becaido` 0.166s → 0.667s，3% TL → 13% TL）|
| 最小 | 1.02x |

膨脹最兇的是記憶體頻寬吃很重的那些（suffix array、HLD + BIT、二分圖匹配）；
polynomial 那幾題只有 1.07–1.09x，因為它們跑得久，後段幾乎是獨佔機器。

## 結論與做法

- **判對錯用平行**：verdict 跟 `-j` 完全無關，而且快 4.5 倍。日常回歸測試應該這樣跑。
- **量時間必須 serial**：`% of TL` 在 `-j 11` 下中位數虛高 1.8 倍，
  拿它判斷「會不會 TLE」或「這個改動有沒有變慢」都會出錯——
  Phase 3 報過的「general_matching 慢了 2.00x」和
  「graph/assignment 吃到 97% TL」兩個都是這樣來的假警報。

`run.py` 現在把 `-j` 寫進 `RESULT.md` 的表頭，而且 `jobs > 1` 時會在時間表前面
直接印出警語，免得以後有人（包括我）又拿平行跑的數字做比較。

## 關於「正式 judge 會不會平行」

會，但形狀不一樣。judge 平行的是**不同的提交**，而且通常各自綁定 CPU／放在
獨立的 cgroup 或機器裡，因為 TL 判定必須可重現；Library Checker 官方
（judge.yosupo.jp）也是一個 worker 跑一題。我們這裡是 11 個 process 搶同一組
記憶體通道，膨脹是必然的，跟 judge 的情境不同。

所以：**平行拿來過篩，serial 拿來定案。**

---

# Phase 18：subset_convolution 的三個問題

## 1. `f`/`g`/`h` 第一維少一格

`ct[i]` 是 popcount，對 $i < 2^L$ 最大等於 $L$，而三個迴圈也都掃到 `L`。
所以第一維必須是 `L + 1`，但宣告成 `int f[N][1 << N]`——只要 `L == N` 就越界。
ASan 直接抓到 `f` 讀出界 4 bytes。改成 `f[N + 1][1 << N]`。

## 2. 只能呼叫一次

`f`/`g`/`h` 是全域，函式開頭只寫 `f[ct[i]][i] = a[i]`，
**其他 `k != ct[i]` 的格子留著上一次 `fwt` 之後的殘值**；
而 `h[i][x] += ...` 又是累加。所以第二次呼叫整個錯掉：
L=3 的測試裡 call 1 全對、call 2 **8/8 全錯**。

開頭補一行 `fill_n`（三個陣列各 `L + 1` 列）。

## 3. `N = 21` 這個預設值會 MLE

修好第一維之後是 $3 \times 22 \times 2^{21} \times 4\text{B} = 554$ MB，照抄直接爆。
改成 `N = 18`（60 MB），註解寫出公式 `L <= N, 3 (N + 1) 2^N ints` 讓人自己換算。
（記憶體是這個演算法的本質 $(L+1)2^L$，不是實作問題，所以只能把預設值調到合理範圍。）

## 驗證

隨機 $L \in [1, 5]$、2000 組、全部共用同一組全域陣列、跟樸素 $O(4^L)$ 比對，
整程在 ASan + UBSan 下跑：**0 錯、0 報告**。修之前同樣的測試在第二次呼叫就全錯。

28 → 31 行。這個模版沒有 LC 對應題，所以全套不受影響。

---

# Phase 19：NTT 改成 std_abs 形狀 + Poly 改自由函式

（這兩個 commit 當時沒寫進來，補記。後來 Phase 21 又把它們換掉了，
所以這節的內容現在只適用於**支線**。）

## `f52d168` NTT 用 std_abs 的寫法，而且自足

`add`/`sub`/`mul`/`pw` helper 取代手寫的 `%` 和條件修正、吃一個 vector 而不是
指標加長度、bit reversal inline 掉。43 → 48 行。

順帶解掉 `mpow` **宣告了但從未定義**的老問題（REVIEW.md B 節那條）——
以前每個 driver 都得補一段 out-of-class 定義才能連結，三個 driver 各刪掉六行。

效能：三次獨立測量 old/new = 1.048 / 1.035 / 0.977，**沒有可測得的差異**。

## `1e314ed` Poly 改成自由函式

`typedef vector<int> Poly` + 自由函式，`cut(a, m)` / `rev(a)` 取代 `isz` / `irev`。
248 → 233 行，而且不用寫 `template<> decltype(Poly_t::ntt) Poly_t::ntt = {};`。

效能：`Inverse`+`Ln` @1e5，三次 old/new = 1.021 / 0.948 / 0.992，一樣沒有差異。

**但這個判斷後來被 Phase 23 推翻了一半**：codebook 原本的 struct 版有上游
（PCkomachi），我改成自由函式等於脫離上游。形式本身沒有效能代價（Phase 23 量到
同樣算術下 struct 與自由函式是 1.07x），但「不好抄」是唯一的理由，而那是主觀的。

---

# Phase 20：註解上的測速數字改成實測

`Polynomial_Operation.cpp` 裡那些 `1e5/55ms` 之類的數字，**在這之前沒有一個是量出來的**：

- 原始那組（`1e5/95ms`、`235ms`、`170ms`、`360ms`、`1s`、`1.4s`）是 codebook 原作者留下的，
  來源不明，大概是別台機器。
- Phase 12 把係數從 `ll` 換成 `int` 之後，我把它們統統乘上 ~0.6
  （從 LC 量到的 1.3–1.46x 整體加速反推），變成 `55ms / 145ms / 100ms / 215ms / 0.6s / 0.9s`。
  `PowerProj` 的 `2^17/3.2s` 也是這樣從實測過的 `5.2s` 推出來的。

看起來像實測，其實是推算。現在實際量了。

## 電池模式下量得準嗎——準

一開始擔心 `powersave` + 沒插電會讓數字沒意義，但交叉驗證說沒問題：

| | |
|---|---|
| `convolution_mod` 最慢那筆（現在，電池，`-j 1`） | **0.278s** |
| 同一筆（插電時記錄在 STATUS.md 的值） | 0.326s |

**現在還比較快**。原因是 `powersave` 在持續負載下照樣升頻，只是很短的 process 來不及——
之前 bench NTT 時每個 process 只跑兩輪，所以絕對值才會在 0.17–0.35s 之間亂跳。
連續跑很多次取最小值（`run.py` 和這次的 timing 都是）就會落在高頻。

另一個佐證：量到的 anchor（一次 $2^{20}$ `Mul`）是 0.126s，
加上讀寫 $2 \times 10^6$ 個數的 I/O 差不多就是 `convolution_mod` 那筆的 0.278s。

## 實測 vs 推算

| | 註解原本（推算） | 實測 | 差 |
|---|---|---|---|
| `Inverse` 1e5 | 55ms | **51ms** | −7% |
| `sqrtQR` 1e5 | 145ms | **150ms** | +3% |
| `Ln` 1e5 | 100ms | **96ms** | −4% |
| `Exp` 1e5 | 215ms | **239ms** | +11% |
| `Evaluate` 1e5 | 0.6s | **0.79s** | **+32%** |
| `Interpolate` 1e5 | 0.9s | **1.19s** | **+32%** |
| `PowerProj` 2^17 | 3.2s | **2.02s** | **−37%** |

前四個推得還行，後三個明顯偏掉——因為它們的常數結構跟 `Inverse`/`Exp` 不一樣
（`Evaluate`/`Interpolate` 是 $O(n\log^2 n)$ 的乘積樹，`PowerProj` 是另一套遞迴），
所以「整體加速比」那個縮放根本不適用。

註解改成實測值，另外補了 `PolyPow` 的 `1e5/340ms`（之前沒有）。
數字彼此自洽：`Ln` ≈ `Inverse` + 一次 1e5 的 `Mul`，`Exp` ≈ 2.5 × `Ln`（每層呼叫一次 `Ln`）。

量法：每個函式連續跑 3–7 次取最小值，同一個 process 裡先量 anchor。

---

# Phase 21：主線換成 std_abs 原檔，我們自己的降為支線

## `6fe8e32` 用 std_abs 的 NTT 和 Operation，逐字不改

```
rm  7_Polynomial/Number_Theory_Transform.cpp
rm  7_Polynomial/Polynomial_Operation.cpp
add 7_Polynomial/NTT_stdabs.cpp
add 7_Polynomial/Operation_stdabs.cpp
add 7_Polynomial/FastLinearRecursion_stdabs.cpp
```

`Operation_stdabs.cpp` 和 `FastLinearRecursion_stdabs.cpp` 是 `3eca4e1` 刪掉的那兩份，
一個 byte 沒動。`NTT_stdabs.cpp` 的 struct 本體也是，但前面補了一段 header——
原檔開頭寫著 `// mul, add, sub, Pow`，實際上還假設 `mod`/`G`/`N`/`sz`/`all` 都存在，
六行契約一個都沒有。`fac`/`facp`/`build()` 一起補，因為 `TaylorShift`/`SamplingShift` 要用。

**header 裡有一個刻意的差異**：`Pow(int a, ll b)` 的第二參數是 `ll`。
std_abs 自己的是 `Pow(int, int)`，而 `PolyPow` 直接把 $k \approx 10^{18}$ 丟進去——
截斷成負數後 `b >>= 1` 卡在 `-1`，`pow_of_formal_power_series` 的四筆 5e5 測資**全部無窮迴圈**。
參數寫成 `ll` 零成本，而且 `Pow` 不在要保留原樣的那兩個檔案裡。有了它那題 AC 37 筆。

代價：`kth_term_of_linearly_recurrent_sequence` 從 AC 66% 變成 **TLE 182%**——
`FastLinearRecursion` 算 $x^k \bmod C$ 是呼叫 `Divide`，而 `Divide` 每次都從頭
反轉再求 `Inverse`，同一個固定的 `C` 算約 120 次。正是 `5143263` 從我們版本裡提出來的東西。

兩個介面落差：`Sqrt` 無解回傳 `{-1}` 不是 `{}`；`FastLinearRecursion` 吃 **0-based** 的 `c`，
而 `BerlekampMassey` 從 `b5e1566` 起回傳 1-based，串接要平移一格。

## `599a34f` 我們自己的那兩份降為支線

從 `2d38ec3` 撈回來，一個 byte 沒改，**不進 `content.tex`**——
跟 `NTT.2.cpp` 一樣的地位，要用的時候在，不佔頁數。

支線沒人跑就會爛，所以 `MAP.tsv` 加了六列 `#ours`
（`convolution_mod` / `inv` / `exp` / `pow` / `sqrt` / `kth_term`），全部 AC。
最後一列就是留著它的理由：同一題 std_abs 那份 TLE，支線 AC。

---

# Phase 22：支線模版的兩處「我加了但沒根據」的東西

兩個都是隊友讀 code 時直接看出來的，兩個都源自同一個毛病：
**我基於沒驗證過的假設加了複雜度，然後因為量不出差別就留著。**

## 1. `cut(Poly a, int m)` 複製了它要丟掉的部分

```cpp
Poly cut(Poly a, int m) { return a.resize(m), a; }
```

by value，所以傳 lvalue 會整個複製進去 —— **然後才 resize**。
`cut(a, (n + 1) / 2)` 等於「先複製全部 $n$ 個係數，再丟掉一半」，
兩倍的 memcpy，而且 `Inverse` / `sqrtQR` / `Exp` 每層遞迴都來一次。

來源：struct 版的對應物 `isz()` 是就地的（`resize(m), *this`），
改成自由函式時我需要一個還能寫在表達式中間的東西，就寫成 by-value。
by-value 對臨時物件是 move，而大多數呼叫點確實是臨時物件，所以「看起來沒事」。

修法：`cut` 改吃 `const &`，只複製要留的那段；另外九個「自己擁有那份資料」的
呼叫點改成就地 `reverse`/`resize`。

**量到 2.8%**：`Inverse+Ln+Exp+Sqrt` @1e5 是 1.027x，`Evaluate+Interpolate` @3e4 是 1.028x，
各取 12 次交錯的最小值。幅度小 —— 這正是它當初躲過我的原因（$O(n)$ memcpy 埋在
$O(n\log n)$ 的變換底下），但兩個互不相干的工作負載給出同一個數字，所以是真的。

## 2. `int n = v.size(), *a = v.data();` 完全沒有作用

我加它是因為懷疑 `vector::operator[]` 每次都要走 `_M_start`，
而寫入 `a[j + dl]` 之後編譯器得重載那個指標。

那個懷疑是在被同-process cache 污染誤導的那一輪（Phase 19 那個假的「舊版快 1.6x」）加上去的。
實測五組：

| | 比值（vec / ptr） |
|---|---|
| 第一輪 $2^{16}$ / $2^{20}$ | 0.931 / 0.953 |
| 重跑 $2^{16}$ / $2^{20}$ | 1.000 / 1.002 |
| 再重跑 $2^{20}$ | 0.981 |

第一輪那個 7% 重跑就沒了，五組合起來沒有一致方向 —— **沒有差異**。
拿掉之後少一個變數、少一行，而且跟主線（std_abs 版直接對 `vector<int>&` 操作）一致。

## 教訓

「A/B 測不出差別」不等於「這個改動沒有代價」。
第 1 點的代價是真的（2.8%），只是被更大的項蓋住；
第 2 點是我先假設了一個瓶頸、加了程式碼、再用「反正測不出來」合理化它。
兩種都該在**加進去之前**先量。

驗證：支線六列全 AC，四個對拍（BM→LinearRecursion 600、PowerProj 400、
Evaluate/Interpolate 400、Divide 的 $bq + r = a$ 600）全 0 錯。

---

# Phase 23：三方多項式比較（NTT 暫不改，留待日後優化）

新的 `std_abs/` 和 `PCkomachi/` 進來之後做的比較。**結論是主線維持 std_abs 那三份不動**，
這一節只記錄發現，供之後回來優化 NTT 時參考。

## 1. PCkomachi 是 codebook 那份 struct 版的上游

`PCkomachi/Polynomial/Polynomial.cpp`（295 行）和 codebook 原本的
`Polynomial_Operation.cpp`（148 行，`18eb9c2` 版本）逐一對應：
`irev`/`isz`/`iadd`/`imul`/`_tmul`/`_eval`/`_tree1`/`Mul`/`Inv`/`Ln`/`Exp`/`Pow`/
`DivMod`/`Eval`/`Interpolate` 全部一樣，連 `// M := P(P - 1). If k >= M, k := k % M + M.`
這行註解都一樣。codebook 那份唯一多出來的函式是 `LinearRecursion`。

**所以 Phase 12–20 我做的事有一半是重新發明**：`Sqrt` 的前導零處理、`TaylorShift`、
`SamplingShift`、`PowerProj`、`int` 係數，PCkomachi 全部本來就有，而且
`power_projection` 比我從 std_abs 移植的更一般（不要求 `f[0] == 0`）。

PCkomachi 還有三方都沒有的：`mul_xk`、`TMul`、`FPSinv`（複合逆，`2^17, 4s`）、
`FPScomp`（複合 $f(g(x))$，`2^17, 5s`）—— 後兩個正是 LC 的
`compositional_inverse_of_formal_power_series` 和 `composition_of_formal_power_series`。

新 `std_abs/codebook/Polynomial/` 的 `NTT.cpp` 和 `Operation.cpp` 跟主線現有的**完全相同**，
不用更新。

## 2. 我在這個比較裡犯的錯（記下來免得再犯）

PCkomachi 和 std_abs 都不附 `add`/`mul`/`po`/`inv`（註解寫「自備」），所以我自己猜了一組。

**第一次量出「我們比 PCkomachi 快 2.24x」，那是假的。** 逐項排查：

| 我當時的結論 | 實際 |
|---|---|
| 「2.24x」 | 我猜的 `add` 造成的 |
| 「換值回傳 add 就變 1.04x，所以是我的錯」 | 也不對——那個變體同時改了「值回傳」和「單向」兩件事 |
| 「Poly 層我們快 1.5–1.9x」 | 在錯的 helper 前提下測的 |

拿到真正的實作之後才定案（見下）。教訓跟 Phase 22 同一個：
**用自己猜的前提去量別人的東西，量出來的是自己的猜測**。

## 3. 真正的發現：雙向 `add` 在 NTT 內層很貴

PCkomachi 實際用的是

```cpp
int add(int &a, int b) { a += b; a -= mod * (a >= mod); a += mod * (a < 0); }
int inv(int x) { return po(x, mod - 2); }
```

無分支，但**一個函式要同時吃 $[0,2mod)$ 和 $(-mod,mod)$，所以每次呼叫做兩次正規化**，
而且是兩次 `imul`（3–4 cycle，cmov 只要 1）。蝶形每輪呼叫兩次。

| $2^{20}$ 一次正向 + 一次逆向 | | |
|---|---|---|
| 我們的 NTT | 0.05770s | 1.00x |
| PCkomachi 原樣 | 0.13130s | **2.28x** |
| 同一個蝶形，換單向 `addp`/`subp` | 0.06177s | **1.07x** |

排除掉的其他假設（都實測過，全部無影響）：蝶形寫 `a[j+dl]` 兩次、`int&` 介面、
改成 local 變數再寫回（2.30x，一點沒動）。

**這也順帶說明「struct vs 自由函式」本身不花錢**——同樣的算術換個包裝是 1.07x。

## 4. PCkomachi 可以馬上加的加速（已回報，未套用）

| 改動 | Inv+Ln+Exp+Sqrt @1e5 | Eval+Interpolate @3e4 |
|---|---|---|
| 原樣 | 0.9838s | 0.7450s |
| + NTT 蝶形改單向 `addp`/`subp` | 0.7832s (1.26x) | 0.6044s (1.23x) |
| + `Poly(p,m)` 不預先清零整個 m | 0.7447s (1.32x) | 0.6645s (1.12x) ← 這項在乘積樹反而略負 |
| + `inv` 用線性表 | 0.6725s (**1.46x**) | 0.5407s (**1.38x**) |

`inv` 那項：`Sx()` 對每個 $i$ 呼叫 `inv(i+1)`，而 `Ln()` 每次都跑一遍 `Sx()`。
PCkomachi 自己的 `Math/FastInv_Log.cpp` 就有 O(1) 版，只是 `Polynomial.cpp` 沒用上。

## 5. 還沒歸因完的

同樣公平條件下，Poly 層我們仍然是 2.2x，但 NTT 層只有 1.07x ——
差距在 `Mul` 的中間配置、`_tmul` 的傳參、`Poly(p,m)` 的其餘成本裡，還沒拆開。
**在拆完之前不要把那個 2.2x 當成結論。**

---

# Phase 24：std_abs 那三份套進 macro 契約

`8f2d58e`。只動風格，演算法一字沒改。順帶帶上 `NTT.2.cpp` 的刪除（你刪的）。

| 原樣 | 改成 |
|---|---|
| `sz(a)` / `all(v)` | `SZ(a)` / `ALL(v)`，兩個 define 放 `NTT_stdabs.cpp` 檔首（`Operation` 本來就需要 NTT） |
| 遞增 +1 迴圈 | `FOR`，41 個 |
| for 第三段的 `++i` | `i++` |
| `vector <T>` / `pair <A, B>` | `vector<T>` / `pair<A, B>` |
| `reverse(1 + ALL(a))` | `reverse(a.begin() + 1, a.end())` |

六個迴圈保留普通 for，因為 `FOR` 表達不了：`m <<= 1`（三個）、`i += L`、兩個多變數初始化。

## 我第一遍寫錯的兩處（commit 前抓到）

1. regex 把 `for (int j = 1, x = 0; j < n - 1; ++j)` 轉成 `FOR (j, 1, x = 0, n - 1 - 1)`
   ——多變數初始化正是 `FOR` 不能吃的那種，macro 直接變四個參數。
2. 原本上界已經是 `X - 1` 的，被加成 `X - 1 - 1`。

## 驗證

機械改寫最怕靜默改到語意，所以兩層都測。

Library Checker 那九列 **15/16 AC**（唯一的 TLE 是 `kth_term`，改之前就是）。
本地對拍補 LC 沒覆蓋的，全部 0 錯：

| 檢查 | 結果 |
|---|---|
| `Divide`：$bq + r = a$ 逐項相等 | 0/500 |
| `Evaluate` 對 Horner 求值 | 0/300 |
| `Interpolate` 來回 | 0/300 |
| `TaylorShift` 對直接代入 $x + c$ | 0/300 |
| `SamplingShift` 對 Lagrange | 0/200 |
| `power_proj` 對 $O(n^2m)$ 樸素 | 0/250 |
| `BerlekampMassey` → `FastLinearRecursion` | 0/300 |

最後一項順帶確認了那個索引落差：BM 回傳 1-based、`FastLinearRecursion` 吃 0-based，
串接要平移一格，`content.tex` 的註解有寫明。
