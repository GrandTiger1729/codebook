vector<int> suffix_array(string s) {
    int n = s.size();
    if (n == 1) return {0};
    s += "\0";
    vector<int> sa(n + 1), rnk(n + 1), cnt(n + 1);
    array<int, 256> alp{};
    for (int i = 0; i <= n; i++) alp[s[i]]++;
    for (int i = 1; i < 256; i++) alp[i] += alp[i - 1];
    for (int i = 0; i <= n; i++) sa[--alp[s[i]]] = i;
    for (int i = 1, cur = 0; i <= n; i++) {
        if (s[sa[i]] != s[sa[i - 1]]) cur++;
        rnk[sa[i]] = cur;
    }
    for (int len = 1; len <= n; len <<= 1) {
        vector<int> lp(n + 1), nrnk(n + 1);
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i <= n; i++) {
            lp[i] = sa[i] - len;
            if (lp[i] < 0) lp[i] += n + 1;
            cnt[rnk[i]]++;
        }
        for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
        for (int i = n; i >= 0; i--) sa[--cnt[rnk[lp[i]]]] = lp[i];
        nrnk[sa[0]] = 0;
        for (int i = 1, cur = 0; i <= n; i++) {
            int np1 = sa[i] + len, np2 = sa[i - 1] + len;
            if (np1 > n + 1) np1 -= n + 1;
            if (np2 > n + 1) np2 -= n + 1;
            if (rnk[sa[i]] != rnk[sa[i - 1]] || rnk[np1] != rnk[np2]) cur++;
            nrnk[sa[i]] = cur;
        }
        rnk = nrnk;
    }
    sa.erase(sa.begin());
    return sa;
}
vector<int> lcp_array(string s, vector<int> sa = {}) {
    int n = s.size(), len = 0;
    if (sa.size() == 0) sa = suffix_array(s);
    vector<int> rnk(n), lcp(n - 1);
    for (int i = 0; i < n; i++) rnk[sa[i]] = i;
    for (int i = 0; i < n; i++) {
        if (rnk[i] == n - 1) {
            len = 0;
            continue;
        }
        int j = sa[rnk[i] + 1];
        while (i + len < n && j + len < n && s[i + len] == s[j + len]) len++;
        lcp[rnk[i]] = len;
        len = max(0, len - 1);
    }
    return lcp;
}
