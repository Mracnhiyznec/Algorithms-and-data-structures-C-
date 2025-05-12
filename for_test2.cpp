#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <queue>
#include <ctime>
#include <cassert>
#include <complex>
#include <string>
#include <cstring>
#include <chrono>
#include <random>
#include <bitset>
#include <fstream>
#include <array>
#include <functional>
#include <memory>
using namespace std;
using ll = long long;
using ld = long double;
#define debug(x) std::cerr << __FUNCTION__ << ":" << __LINE__ << " " << #x << " = " << x << '\n';
const ll INF = 4e18;
//#pragma GCC optimize("Ofast")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.1,sse4.2,popcnt,abm,mmx,avx,avx2,tune=native")
//#pragma GCC optimize("unroll-loops")
vector<ll> parent;
vector<ll> size_set;
vector<vector<ll>> matrix;
vector<pair<ll, pair<ll, ll>>> edges;
vector<multiset<pair<ll,ll>>> span_tree;
vector<ll> isk_path;
vector<ll> already_added;
vector<ll> euler;
ll ans =0;
ll n;
random_device rd;
mt19937 gen(rd());
void init(ll n){
    size_set.resize(n,1);
    parent.resize(n,0);
    for (int i =0; i < n; i++){
        parent[i]=i;
    }
}
ll get_main(ll a){
    if (parent[a] != a){
        parent[a] = get_main(parent[a]);
    }
    return parent[a];
}
void union_two_sets(ll a, ll b){
    a = get_main(a);
    b = get_main(b);
    if (size_set[a] < size_set[b]){
        swap(a,b);
    }
    parent[b]=a;
}
void euler_path(ll ver){
    while (span_tree[ver].size()){
        pair<ll,ll> nxt = *span_tree[ver].begin();
        span_tree[ver].erase(span_tree[ver].find(nxt));
        span_tree[nxt.first].erase(span_tree[nxt.first].find({ver, nxt.second}));
        euler_path(nxt.first);
        euler.push_back(nxt.first);
    }
}
bool check_bit(ll mask, ll bit){
    return ((1LL << bit) & mask);
}
ll etalon_solution(){
    vector<vector<ll>> dp(n, vector<ll>(1LL << n,INF));
    dp[0][1]=0;
    for (int mask = 0; mask < (1LL << n); mask++){
        for (int now = 0; now < n; now++) {
            for (int nxt = 0; nxt < n; nxt++) {
                if (!check_bit(mask, nxt) && check_bit(mask, now)) {
                    dp[nxt][(1LL << nxt) | mask] = min(dp[now][mask] + matrix[nxt][now], dp[nxt][(1LL << nxt) | mask]);
                }
            }
        }
    }
    ll to_return = INF;
    for (int i = 1; i < n; i++){
        to_return = min(to_return, dp[i][(1LL << n) - 1] + matrix[i][0]);
    }
    return to_return;
}
void generate_good_graph(){
    ll min_coord = 0;
    ll max_coord = 100000;
    uniform_int_distribution<> dist(min_coord, max_coord);
    vector<pair<ll, ll>> points;
    ll x, y;
    for (int i = 0; i < n; i++){
        x = dist(gen);
        y = dist(gen);
        points.push_back({x,y});
    }
    for (int i =0; i < points.size(); i++){
        for (int j = 0; j < points.size(); j++){
            matrix[i][j] = abs(points[i].first - points[j].first) + abs(points[i].second - points[j].second);
        }
    }
    for (int i =0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            edges.push_back({matrix[i][j], {i, j}});
        }
    }
}


const int N = 105;
struct Blossom {
    const long long inf = 1e18;
#define dist(e) (lab[e.u] + lab[e.v] - g[e.u][e.v].w * 2)
    struct edge {
        int u, v;
        long long w;
    } g[N * 2][N * 2];
    int n, n_x, match[N * 2], slack[N * 2], st[N * 2], pa[N * 2], flower_from[N * 2][N * 2], S[N * 2], vis[N * 2];
    long long lab[N * 2];
    vector<int> flower[N * 2];
    deque<int> q;
    Blossom() {}
    Blossom(int _n) {
        n = _n;
        q = deque<int>();
        for (int u = 1; u <= n * 2; ++u) {
            match[u] = slack[u] = st[u] = pa[u] = S[u] = vis[u] = lab[u] = 0;
            for (int v = 1; v <= n * 2; ++v) {
                g[u][v] = edge{u, v, 0};
                flower_from[u][v] = 0;
            }
            flower[u].clear();
        }
    }
    void add_edge(int u, int v, long long w) {
        g[u][v].w = max(g[u][v].w, w);
        g[v][u].w = max(g[v][u].w, w);
    }
    inline void update_slack(int u, int x) {
        if (!slack[x] || dist(g[u][x]) < dist(g[slack[x]][x])) slack[x] = u;
    }
    inline void set_slack(int x) {
        slack[x] = 0;
        for (int u = 1; u <= n; ++u) {
            if (g[u][x].w > 0 && st[u] != x && S[st[u]] == 0) update_slack(u, x);
        }
    }
    inline void q_push(int x) {
        if (x <= n) return q.push_back(x);
        for (int i = 0; i < (int)flower[x].size(); i++) q_push(flower[x][i]);
    }
    inline void set_st(int x, int b) {
        st[x] = b;
        if (x <= n) return;
        for (int i = 0; i < (int)flower[x].size(); ++i) set_st(flower[x][i], b);
    }
    inline int get_pr(int b, int xr) {
        int pr = find(flower[b].begin(), flower[b].end(), xr) - flower[b].begin();
        if (pr % 2 == 1) {
            reverse(flower[b].begin() + 1, flower[b].end());
            return (int)flower[b].size() - pr;
        } else return pr;
    }
    inline void set_match(int u, int v) {
        match[u] = g[u][v].v;
        if (u <= n) return;
        edge e = g[u][v];
        int xr = flower_from[u][e.u], pr = get_pr(u, xr);
        for (int i = 0; i < pr; ++i) set_match(flower[u][i], flower[u][i ^ 1]);
        set_match(xr, v);
        rotate(flower[u].begin(), flower[u].begin() + pr, flower[u].end());
    }
    inline void augment(int u, int v) {
        int xnv = st[match[u]];
        set_match(u, v);
        if (!xnv) return;
        set_match(xnv, st[pa[xnv]]);
        augment(st[pa[xnv]], xnv);
    }
    inline int get_lca(int u, int v) {
        static int t = 0;
        for (++t; u || v; swap(u, v)) {
            if (u == 0) continue;
            if (vis[u] == t) return u;
            vis[u] = t;
            u = st[match[u]];
            if (u) u = st[pa[u]];
        }
        return 0;
    }
    inline void add_blossom(int u, int lca, int v) {
        int b = n + 1;
        while(b <= n_x && st[b]) ++b;
        if (b > n_x) ++n_x;
        lab[b] = 0, S[b] = 0;
        match[b] = match[lca];
        flower[b].clear();
        flower[b].push_back(lca);
        for (int x = u, y; x != lca; x = st[pa[y]]) {
            flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), q_push(y);
        }
        reverse(flower[b].begin() + 1, flower[b].end());
        for (int x = v, y; x != lca; x = st[pa[y]]) {
            flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), q_push(y);
        }
        set_st(b, b);
        for (int x = 1; x <= n_x; ++x) g[b][x].w = g[x][b].w = 0;
        for (int x = 1; x <= n; ++x) flower_from[b][x] = 0;
        for (int i = 0; i < (int)flower[b].size(); ++i) {
            int xs = flower[b][i];
            for (int x = 1; x <= n_x; ++x) {
                if (g[b][x].w == 0 || dist(g[xs][x]) < dist(g[b][x]))
                    g[b][x] = g[xs][x], g[x][b] = g[x][xs];
            }
            for (int x = 1; x <= n; ++x) {
                if (flower_from[xs][x]) flower_from[b][x] = xs;
            }
        }
        set_slack(b);
    }
    inline void expand_blossom(int b) { // S[b] == 1
        for (int i = 0; i < (int)flower[b].size(); ++i) set_st(flower[b][i], flower[b][i]);
        int xr = flower_from[b][g[b][pa[b]].u], pr = get_pr(b, xr);
        for (int i = 0; i < pr; i += 2) {
            int xs = flower[b][i], xns = flower[b][i + 1];
            pa[xs] = g[xns][xs].u;
            S[xs] = 1, S[xns] = 0;
            slack[xs] = 0, set_slack(xns);
            q_push(xns);
        }
        S[xr] = 1, pa[xr] = pa[b];
        for (int i = pr + 1; i < (int)flower[b].size(); ++i) {
            int xs = flower[b][i];
            S[xs] = -1, set_slack(xs);
        }
        st[b] = 0;
    }
    inline bool on_found_edge(const edge &e) {
        int u = st[e.u], v = st[e.v];
        if (S[v] == -1) {
            pa[v] = e.u, S[v] = 1;
            int nu = st[match[v]];
            slack[v] = slack[nu] = 0;
            S[nu] = 0, q_push(nu);
        } else if (S[v] == 0) {
            int lca = get_lca(u, v);
            if (!lca) return augment(u, v), augment(v, u), 1;
            else add_blossom(u, lca, v);
        }
        return 0;
    }
    inline bool matching() {
        fill(S, S + n_x + 1, -1), fill(slack, slack + n_x + 1, 0);
        q.clear();
        for (int x = 1; x <= n_x; ++x) {
            if (st[x] == x && !match[x]) pa[x] = 0, S[x] = 0, q_push(x);
        }
        if (q.empty()) return 0;
        for (;;) {
            while((int)q.size()) {
                int u = q.front();
                q.pop_front();
                if (S[st[u]] == 1)continue;
                for (int v = 1; v <= n; ++v) {
                    if (g[u][v].w > 0 && st[u] != st[v]) {
                        if (dist(g[u][v]) == 0) {
                            if (on_found_edge(g[u][v])) return 1;
                        } else update_slack(u, st[v]);
                    }
                }
            }
            long long d = inf;
            for (int b = n + 1; b <= n_x; ++b) {
                if (st[b] == b && S[b] == 1) d = min(d, lab[b] / 2);
            }
            for (int x = 1; x <= n_x; ++x) {
                if (st[x] == x && slack[x]) {
                    if (S[x] == -1) d = min(d, dist(g[slack[x]][x]));
                    else if (S[x] == 0) d = min(d, dist(g[slack[x]][x]) / 2);
                }
            }
            for (int u = 1; u <= n; ++u) {
                if (S[st[u]] == 0) {
                    if (lab[u] <= d)return 0;
                    lab[u] -= d;
                } else if (S[st[u]] == 1) lab[u] += d;
            }
            for (int b = n + 1; b <= n_x; ++b) {
                if (st[b] == b) {
                    if (S[st[b]] == 0) lab[b] += d * 2;
                    else if (S[st[b]] == 1) lab[b] -= d * 2;
                }
            }
            q.clear();
            for (int x = 1; x <= n_x; ++x) {
                if (st[x] == x && slack[x] && st[slack[x]] != x && dist(g[slack[x]][x]) == 0)
                    if (on_found_edge(g[slack[x]][x])) return 1;
            }
            for (int b = n + 1; b <= n_x; ++b) {
                if (st[b] == b && S[b] == 1 && lab[b] == 0) expand_blossom(b);
            }
        }
        return 0;
    }
    pair<long long, int> solve() {
        fill(match, match + n + 1, 0);
        n_x = n;
        int cnt = 0;
        long long ans = 0;
        for (int u = 0; u <= n; ++u) st[u] = u, flower[u].clear();
        long long w_max = 0;
        for (int u = 1; u <= n; ++u) {
            for (int v = 1; v <= n; ++v) {
                flower_from[u][v] = (u == v ? u : 0);
                w_max = max(w_max, g[u][v].w);
            }
        }
        for (int u = 1; u <= n; ++u) lab[u] = w_max;
        while (matching()) ++cnt;
        for (int u = 1; u <= n; ++u) {
            if (match[u] && match[u] < u) ans += g[u][match[u]].w;
        }
        return make_pair(ans, cnt);
    }
};
void solve(){
    ld maxi = 0;

    for (int q = 0; q < 50; q++) {
        parent.clear();
        size_set.clear();
        matrix.clear();
        edges.clear();
        span_tree.clear();
        isk_path.clear();
        already_added.clear();
        euler.clear();
        ans=0;
        n = 15;
        init(n);
        Blossom y(n);
        matrix.resize(n, vector<ll>(n,0));
        generate_good_graph();
        sort(edges.begin(), edges.end());
        span_tree.resize(n);
        for (int i =0; i < edges.size(); i++){
            if (get_main(edges[i].second.first) != get_main(edges[i].second.second)){
                span_tree[edges[i].second.first].insert({edges[i].second.second, edges[i].first});
                span_tree[edges[i].second.second].insert({edges[i].second.first, edges[i].first});
                union_two_sets(edges[i].second.first, edges[i].second.second);
            }
        }
        vector<ll> nch;
        for (int i =0; i < n; i++){
            ll cnt =span_tree[i].size();
            if (cnt % 2){
                nch.push_back(cnt);
            }
        }
        vector<ll> pere(nch.size());
        for (int i =0; i < nch.size(); i++){
            pere[i]=nch.size();
        }
        for (int i =0; i <nch.size(); i++){
            for (int j = i + 1; j < nch.size(); j++){
                y.add_edge(i, j, matrix[i][j]);
            }
        }
        ll tt=0;
        if (nch.size()) {
            tt = y.solve().first;
        }
        //  debug(nch.size())
        for (int i =0; i < nch.size(); i++){
            ll v1 = i;
            ll v2 = y.match[i];
            span_tree[pere[v1]].insert({pere[v2], matrix[pere[v1]][pere[v2]]});
            span_tree[pere[v2]].insert({pere[v1], matrix[pere[v2]][pere[v1]]});

        }
        for (int i =0; i < n; i++){
            multiset<pair<ll,ll>> tt = span_tree[i];
            for (auto c: tt){
                span_tree[i].insert(c);
            }
        }
        already_added.resize(n);
        euler_path(0);
        euler.push_back(0);
        already_added[0]=1;
        isk_path.push_back(0);
        for (auto c: euler){
            if (already_added[c] == 0){
                ans = ans + matrix[isk_path.back()][c];
                isk_path.push_back(c);
                already_added[c]=1;
            }
        }
        ans = ans + matrix[isk_path.back()][0];
        isk_path.push_back(0);
        ll et = etalon_solution();
        maxi = max((long double) (ans) / et, maxi);
        if ((long double) ans/et > 1.5){
            for (int i =0; i < n; i++){
                for (int j =0; j < n; j++){
                    cout << matrix[i][j] << " ";
                }
                cout << '\n';
            }
            exit(0);
        }
        cout << "Test № " << (q + 1) << " " << (long double) ans << " " << et << " " << (long double) ans/et << endl;
    }
    cout << maxi << endl;
}
int main() {
    solve();
    return 0;
}
