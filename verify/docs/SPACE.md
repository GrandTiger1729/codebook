# 版面不夠時的備案

PDF 上限 25 頁。下面是試過、量過但**還沒採用**的省行方法，下次超頁時從這裡挑。
行數是模板本身的行數；實際省幾頁要重編量（最後幾欄的表格、公式區塊不能拆，常常會整塊跳欄）。

## 改寫（行為不變，已有測試可驗）

- **Bipartite Matching `cover()`**（−7 行）：不另外 BFS，直接用 `matching()` 最後一次失敗的 `bfs()` 留下的 `dis`。
  König 覆蓋 = `!~dis[i]` 的左點 + `~dis[mq[e]]` 的右點（未匹配右點 `mq[e] = l`，`dis[l] = -1` 自動排除）。
  2026-11-26 做過、對拍與 Lib-Checker 都過，但**撤回**：`cover()` 會依賴 `bfs()` 走完全部可達點、且不提早停；
  以後若改 `bfs()` 會悄悄壞掉。要用的話建議寫成 `mq[e] != l && ~dis[mq[e]]`，別靠哨兵。
- **Mo's Algorithm On Tree 開頭註解**（−6 行）：8 行的 `/* ... */` 改成兩行
  `// needs LCA, dfs with in[u] = dft++, out[u] = dft++,` / `// ord[in[u]] = ord[out[u]] = u, bitset<MAXN> inset`。

## 拿出書（comment out `content.tex`，檔案保留）

| 模板 | 行數 | 備註 |
|---|---|---|
| Maximum Weight Matching | 170 | 比賽沒用過，帶著以防萬一 |
| HPI Alternative Form | 26 | |
| Determinant Any Mod | 15 | |

## 注意

- 第 7 章（Polynomial）目前從第 15 頁頂開始，第 14 頁最後一欄以不能拆的 Euclidean 公式結尾：第 15 頁之前省幾行通常會被吃掉，要省頁就從第 15 頁之後下手。

## 不要用的省法

- `return a = b, 0;` 這類逗號回傳（使用者明確拒絕）。`if (int x = f()) { ...; return x; }` 可以。
