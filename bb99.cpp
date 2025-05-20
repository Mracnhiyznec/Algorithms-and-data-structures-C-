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
    for (ll i =0; i < n; i++){
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
    for (ll mask = 0; mask < (1LL << n); mask++){
        for (ll now = 0; now < n; now++) {
            for (ll nxt = 0; nxt < n; nxt++) {
                if (!check_bit(mask, nxt) && check_bit(mask, now)) {
                    dp[nxt][(1LL << nxt) | mask] = min(dp[now][mask] + matrix[now][nxt], dp[nxt][(1LL << nxt) | mask]);
                }
            }
        }
    }
    ll to_return = INF;
    for (ll i = 1; i < n; i++){
        to_return = min(to_return, dp[i][(1LL << n) - 1] + matrix[i][0]);
    }
    return to_return;
}
void generate_good_graph(){
    ll min_coord = 0;
    ll max_coord = 1e6;
    uniform_int_distribution<> dist(min_coord, max_coord);
    vector<pair<ll, ll>> polls;
    ll x, y;
    for (ll i = 0; i < n; i++){
        x = dist(gen);
        y = dist(gen);
        polls.push_back({x,y});
    }
    for (ll i =0; i < polls.size(); i++){
        for (ll j = 0; j < polls.size(); j++){
            matrix[i][j] = abs(polls[i].first - polls[j].first) + abs(polls[i].second - polls[j].second);
        }
    }
    for (ll i =0; i < n; i++){
        for (ll j = i + 1; j < n; j++){
            edges.push_back({matrix[i][j], {i, j}});
        }
    }
}

const ll N = 105;                // <= 2 * максимум вершин

struct Blossom {
    const long long INF = 1e18;
    struct Edge { ll u, v, w; };

    ll           v_cnt = 0;
    ll           tot_cnt = 0;
    Edge         adj[N * 2][N * 2];

    ll           mate[N * 2];
    ll (&match)[N * 2] = mate;

    ll           slack[N * 2];
    ll           root[N * 2];
    ll           parent[N * 2];
    ll           from[N * 2][N * 2];

    ll           state[N * 2];
    ll           vis[N * 2];
    ll           label[N * 2];

    vector<ll>   blossom[N * 2];
    deque<ll>    bfs;

#define DIST(e) (label[(e).u] + label[(e).v] - adj[(e).u][(e).v].w * 2)

    Blossom() = default;
    explicit Blossom(ll n) { init(n); }

    void init(ll n) {
        v_cnt = tot_cnt = n;
        for (ll i = 0; i < 2 * N; ++i) {
            mate[i] = slack[i] = root[i] = parent[i] =
            state[i] = vis[i] = label[i] = 0;
            blossom[i].clear();
            for (ll j = 0; j < 2 * N; ++j) {
                adj[i][j] = {i, j, 0};
                from[i][j] = 0;
            }
        }
    }

    void add_edge(ll u, ll v, long long w) {
        adj[u][v].w = max(adj[u][v].w, w);
        adj[v][u].w = max(adj[v][u].w, w);
    }
    void print_graph(ll n) const {
        for (ll i = 1; i <= n; ++i) {
            for (ll j = 1; j <= n; ++j) cout << adj[i][j].w << ' ';
            cout << '\n';
        }
    }

private:
    inline void update_slack(ll u, ll x) {
        if (!slack[x] || DIST(adj[u][x]) < DIST(adj[slack[x]][x]))
            slack[x] = u;
    }
    inline void set_slack(ll x) {
        slack[x] = 0;
        for (ll u = 1; u <= v_cnt; ++u)
            if (adj[u][x].w && root[u] != x && state[root[u]] == 0)
                update_slack(u, x);
    }
    inline void push(ll x) {
        if (x <= v_cnt) { bfs.push_back(x); return; }
        for (ll v : blossom[x]) push(v);
    }
    inline void set_root(ll x, ll r) {
        root[x] = r;
        if (x <= v_cnt) return;
        for (ll v : blossom[x]) set_root(v, r);
    }
    inline ll pos_in_blossom(ll b, ll xr) {
        ll p = find(blossom[b].begin(), blossom[b].end(), xr)
               - blossom[b].begin();
        if (p & 1) {
            reverse(blossom[b].begin() + 1, blossom[b].end());
            return (ll)blossom[b].size() - p;
        }
        return p;
    }
    inline void link(ll u, ll v) {
        mate[u] = adj[u][v].v;
        if (u <= v_cnt) return;
        Edge e = adj[u][v];
        ll xr = from[u][e.u];
        ll p  = pos_in_blossom(u, xr);
        for (ll i = 0; i < p; ++i)
            link(blossom[u][i], blossom[u][i ^ 1]);
        link(xr, v);
        rotate(blossom[u].begin(), blossom[u].begin() + p, blossom[u].end());
    }
    inline void augment(ll u, ll v) {
        ll nv = root[mate[u]];
        link(u, v);
        if (!nv) return;
        link(nv, root[parent[nv]]);
        augment(root[parent[nv]], nv);
    }
    inline ll lca(ll u, ll v) {
        static ll tick = 0;
        for (++tick; u || v; swap(u, v)) {
            if (!u) continue;
            if (vis[u] == tick) return u;
            vis[u] = tick;
            u = root[mate[u]];
            if (u) u = root[parent[u]];
        }
        return 0;
    }
    inline void add_blossom(ll u, ll w, ll v) {
        ll b = v_cnt + 1;
        while (b <= tot_cnt && root[b]) ++b;
        if (b > tot_cnt) ++tot_cnt;

        label[b] = 0;  state[b] = 0;  mate[b] = mate[w];
        blossom[b].clear();  blossom[b].push_back(w);

        for (ll x = u, y; x != w; x = root[parent[y]]) {
            blossom[b].push_back(x);
            blossom[b].push_back(y = root[mate[x]]);
            push(y);
        }
        reverse(blossom[b].begin() + 1, blossom[b].end());
        for (ll x = v, y; x != w; x = root[parent[y]]) {
            blossom[b].push_back(x);
            blossom[b].push_back(y = root[mate[x]]);
            push(y);
        }
        set_root(b, b);

        for (ll x = 1; x <= tot_cnt; ++x) adj[b][x].w = adj[x][b].w = 0;
        for (ll x = 1; x <= v_cnt; ++x)  from[b][x] = 0;

        for (ll xs : blossom[b]) {
            for (ll x = 1; x <= tot_cnt; ++x)
                if (!adj[b][x].w || DIST(adj[xs][x]) < DIST(adj[b][x]))
                    adj[b][x] = adj[xs][x], adj[x][b] = adj[x][xs];
            for (ll x = 1; x <= v_cnt; ++x)
                if (from[xs][x]) from[b][x] = xs;
        }
        set_slack(b);
    }
    inline void expand(ll b) {          // state[b] == 1
        for (ll v : blossom[b]) set_root(v, v);

        ll xr = from[b][adj[b][parent[b]].u];
        ll p  = pos_in_blossom(b, xr);

        for (ll i = 0; i < p; i += 2) {
            ll xs  = blossom[b][i];
            ll xns = blossom[b][i + 1];
            parent[xs] = adj[xns][xs].u;
            state[xs]  = 1;  state[xns] = 0;
            slack[xs]  = 0;  set_slack(xns);
            push(xns);
        }
        state[xr] = 1;  parent[xr] = parent[b];
        for (ll i = p + 1; i < (ll)blossom[b].size(); ++i) {
            ll xs = blossom[b][i];
            state[xs] = -1;  set_slack(xs);
        }
        root[b] = 0;
    }
    inline bool on_edge(const Edge &e) {
        ll u = root[e.u], v = root[e.v];
        if (state[v] == -1) {
            parent[v] = e.u;  state[v] = 1;
            ll nu = root[mate[v]];
            slack[v] = slack[nu] = 0;
            state[nu] = 0;  push(nu);
        } else if (state[v] == 0) {
            ll w = lca(u, v);
            if (!w) { augment(u, v); augment(v, u); return true; }
            add_blossom(u, w, v);
        }
        return false;
    }
    inline bool bfs_phase() {
        fill(state, state + tot_cnt + 1, -1);
        fill(slack, slack + tot_cnt + 1, 0);
        bfs.clear();

        for (ll x = 1; x <= tot_cnt; ++x)
            if (root[x] == x && !mate[x]) { parent[x] = 0; state[x] = 0; push(x); }

        if (bfs.empty()) return false;

        for (;;) {
            while (!bfs.empty()) {
                ll u = bfs.front(); bfs.pop_front();
                if (state[root[u]] == 1) continue;
                for (ll v = 1; v <= v_cnt; ++v)
                    if (adj[u][v].w && root[u] != root[v]) {
                        if (DIST(adj[u][v]) == 0) {
                            if (on_edge(adj[u][v])) return true;
                        } else update_slack(u, root[v]);
                    }
            }

            long long d = INF;
            for (ll b = v_cnt + 1; b <= tot_cnt; ++b)
                if (root[b] == b && state[b] == 1) d = min(d, label[b] / 2);

            for (ll x = 1; x <= tot_cnt; ++x)
                if (root[x] == x && slack[x]) {
                    if (state[x] == -1) d = min(d, DIST(adj[slack[x]][x]));
                    else if (state[x] == 0) d = min(d, DIST(adj[slack[x]][x]) / 2);
                }

            for (ll u = 1; u <= v_cnt; ++u) {
                if (state[root[u]] == 0) {
                    if (label[u] <= d) return false;
                    label[u] -= d;
                } else if (state[root[u]] == 1) label[u] += d;
            }
            for (ll b = v_cnt + 1; b <= tot_cnt; ++b)
                if (root[b] == b)
                    label[b] += (state[root[b]] == 0 ? 2 * d : -2 * d);

            bfs.clear();
            for (ll x = 1; x <= tot_cnt; ++x)
                if (root[x] == x && slack[x] && root[slack[x]] != x &&
                    DIST(adj[slack[x]][x]) == 0)
                    if (on_edge(adj[slack[x]][x])) return true;

            for (ll b = v_cnt + 1; b <= tot_cnt; ++b)
                if (root[b] == b && state[b] == 1 && label[b] == 0)
                    expand(b);
        }
    }

public:
    /***** главный интерфейс *****/
    pair<long long, ll> solve() {
        fill(mate, mate + v_cnt + 1, 0);
        tot_cnt = v_cnt;

        for (ll u = 0; u <= v_cnt; ++u) { root[u] = u; blossom[u].clear(); }

        long long w_max = 0;
        for (ll u = 1; u <= v_cnt; ++u)
            for (ll v = 1; v <= v_cnt; ++v) {
                from[u][v] = (u == v ? u : 0);
                w_max = max(w_max, adj[u][v].w);
            }
        for (ll u = 1; u <= v_cnt; ++u) label[u] = w_max;

        ll pairs = 0;  long long weight = 0;
        while (bfs_phase()) ++pairs;

        for (ll u = 1; u <= v_cnt; ++u)
            if (mate[u] && mate[u] < u) weight += adj[u][mate[u]].w;

        return {weight, pairs};
    }
};


void solve(){
    cout << "Введите количество вершин в графе\n";
    cin >> n;
    init(n);
    matrix.resize(n, vector<ll>(n,0));
    cout << "Введите 1 если хотите сгнереировать случайных граф, 2 если хотите сделать свой граф\n";
    ll type;
    cin >> type;
    if (type==1){
        generate_good_graph();
        cout << "Сгенерированный граф\n";
        for (ll i =0; i < n; i++){
            for (ll j =0; j < n; j++){
                cout << matrix[i][j] << " ";
            }
            cout << '\n';
        }
        cout << '\n';
    }
    else {
        cout << "Введите матрицу смежности графа\n";
        for (ll i = 0; i < n; i++) {
            for (ll j = 0; j < n; j++) {
                cin >> matrix[i][j];
                if (j > i) {
                    edges.push_back({matrix[i][j], {i, j}});
                }
            }
        }
    }
    sort(edges.begin(), edges.end());
    span_tree.resize(n);
    for (ll i =0; i < edges.size(); i++){
        if (get_main(edges[i].second.first) != get_main(edges[i].second.second)){
            span_tree[edges[i].second.first].insert({edges[i].second.second, edges[i].first});
            span_tree[edges[i].second.second].insert({edges[i].second.first, edges[i].first});
            union_two_sets(edges[i].second.first, edges[i].second.second);
        }
    }
    vector<ll> nch;
    for (ll i =0; i < n; i++){
        ll cnt =span_tree[i].size();
        if (cnt % 2){
            nch.push_back(i);
        }
    }
    vector<ll> pere(nch.size());
    Blossom y(pere.size());
    for (ll i =0; i < nch.size(); i++){
        pere[i]=nch[i];
    }
    ll K=-INF;
    for (int i = 0; i < nch.size(); i++){
        for (int j =0 ; j < nch.size(); j++){
            K = max(matrix[pere[i]][pere[j]], K);
        }
    }
    K = K * 10;
    for (ll i =1; i <= nch.size(); i++){
        for (ll j = i + 1; j <= nch.size(); j++){
            y.add_edge(i, j, K - matrix[pere[i - 1]][pere[j - 1]]);
        }
    }
    y.print_graph(nch.size());
    ll tt=0;
    if (nch.size()) {
        tt = y.solve().first;
    }
    tt = K * nch.size() / 2 - tt;
    for (ll i =0; i < nch.size(); i++){
        ll v1 = i + 1;
        ll v2 = y.match[v1];
        v1--;
        v2--;
        span_tree[pere[v1]].insert({pere[v2], matrix[pere[v1]][pere[v2]]});
    }
    already_added.resize(n);
    for (int i =0; i < n; i++){
        cout << i << ": ";
        for (auto c: span_tree[i]){
            if (span_tree[c.first].find({i, c.second}) == span_tree[c.first].end()){
                cout << "a";
                exit(0);
            }
            cout << c.first << " ";
        }
        cout << '\n';
    }
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
    for (auto c: euler){
        cout << c << " ";
    }
    cout << '\n';
    cout << "Полученная длина пути: " << ans << endl;
    for (auto c: isk_path){
        cout << c + 1 << " ";
    }
    cout << endl;
    ll et = etalon_solution();
    cout << "Опмтимальный ответ " << et << endl;
    cout << fixed << setprecision(10) << "OUR/OPT = " << (long double)(ans)/et << endl;
}
int main() {
    solve();
    return 0;
}
