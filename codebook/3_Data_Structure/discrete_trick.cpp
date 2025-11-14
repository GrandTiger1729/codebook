vector<int> val;
// build
sort(ALL(val)), val.erase(unique(ALL(val)), val.end());
// index of x
upper_bound(ALL(val), x) - val.begin();
// max idx <= x
upper_bound(ALL(val), x) - val.begin();
// max idx < x
lower_bound(ALL(val), x) - val.begin();
