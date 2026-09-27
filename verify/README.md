# verify

codebook 的測試。現況（每份模版測了什麼、結果）在 `STATUS.md`。

## 本地對拍：`stress/`

kactl 式：每個檔案是一個 `main` + `assert`，當場隨機生成（固定種子 1729）並和暴力解比對。
不需要任何測資檔，CI 每個 PR 都會跑。

```sh
verify/stress/run.sh                        # 全部，ASan + UBSan + _GLIBCXX_DEBUG
verify/stress/run.sh Geometry/Heart.cpp     # 單一個
SAN=0 verify/stress/run.sh                  # 不開 sanitizer
```

新增測試：放在 `stress/<章>/<模版名>.cpp`，依序 include `prelude.h`、（幾何章）`geo_prelude.h`、
`stress.h`、模版需要的外部符號、`"<章>/<檔>.cpp"`，最後印一行摘要。

## Library Checker：`run.py`

`library-checker-problems/` 是官方 repo 的 submodule；測資由官方 `generate.py` 在本機生成
（約 5 GB，不進 git）。`MAP.tsv` 是「LC 題目 ↔ driver ↔ 模版」的對照，driver 在 `drivers/`。

```sh
git submodule update --init verify/library-checker-problems
cd verify
uv venv && uv pip install colorlog          # generate.py 需要
.venv/bin/python run.py --all               # 全部，結果寫到 RESULT.md
.venv/bin/python run.py -p scc -p two_sat   # 用題名或模版路徑篩選
```

`-j` 預設平行跑：判定結果不受影響，但時間會膨脹（中位數約 1.8 倍），
要看「佔 TL 幾 %」請用 `-j 1` 定案。

## 排版：`check_layout.py`

listings 一行放得下 58 欄，超過就折行。`python3 verify/check_layout.py` 列出所有超過的行；
CI 也會檢查這個，另外檢查 PDF 不超過 25 頁。

## 其他

- `prelude.h`：六行 macro 契約。`geo_prelude.h`：幾何章 prelude（`8_Geometry/Default_code.cpp`）。
  `mod_helpers.h`：NTT 需要、書上不帶的 `add`/`sub`/`mul`/`Pow`。
- `docs/`：過去幾輪審查的紀錄（`REVIEW.md` 待辦與結論、`CHANGES.md` 逐項改動）。
