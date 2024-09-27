struct SCC { // need adj
	int n, dfcnt = 0, sccnt = 0;
	vector<int> dfn, low, scc, st, ist;
	vector<vector<int>> scc_adj;
	SCC() {}
	SCC(int n) : n(n) {
		dfn = low = scc = ist = vector<int>(n + 1, 0);
		scc_adj = vector<vector<int>>(n + 1, vector<int>());
	}
	void tarjan(int pos) {
		dfn[pos] = low[pos] = ++dfcnt;
		st.emplace_back(pos);
		ist[pos] = 1;
		for (int np : adj[pos]) {
			if (dfn[np] == 0) {
				tarjan(np);
				low[pos] = min(low[pos], low[np]);
			} else if (ist[np] == 1) {
				low[pos] = min(low[pos], dfn[np]);
			}
		}
		if (dfn[pos] == low[pos]) {
			sccnt++;
			while (1) {
				int x = st.back();
				ist[x] = 0;
				scc[x] = sccnt;
				st.pop_back();
				if (x == pos) {
					break;
				}
			}
		}
	}
	void work() {
		for (int i = 1; i <= n; i++) {
			if (dfn[i] == 0) {
				tarjan(i);
			}
		}
	}
	void build_adj() {
		for (int i = 1; i <= n; i++) {
			for (int j : adj[i]) {
				if (scc[i] != scc[j]) {
					scc_adj[scc[i]].emplace_back(scc[j]);
				}
			}
		}
	}
};
