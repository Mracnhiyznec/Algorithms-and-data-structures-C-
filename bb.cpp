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
    int min_coord = -1e6;
    int max_coord = 1e6;
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
        for (int i =0; i < n; i++){
            for (int j =0; j < n; j++){
                cout << matrix[i][j] << " ";
            }
            cout << '\n';
        }
        cout << '\n';
    }
    else {
        cout << "Введите матрицу смежности графа\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> matrix[i][j];
                if (j > i) {
                    edges.push_back({matrix[i][j], {i, j}});
                }
            }
        }
    }
    sort(edges.begin(), edges.end());
    span_tree.resize(n);
    for (int i =0; i < edges.size(); i++){
        if (get_main(edges[i].second.first) != get_main(edges[i].second.second)){
            span_tree[edges[i].second.first].insert({edges[i].second.second, edges[i].first});
            span_tree[edges[i].second.second].insert({edges[i].second.first, edges[i].first});
            union_two_sets(edges[i].second.first, edges[i].second.second);
        }
    }
    for (int i =0; i < n; i++){
        cout << i << ": ";
        for (auto c: span_tree[i]){
            cout << c.first << " ";
        }
        cout << '\n';
    }
    cout << '\n';
    for (int i =0; i < n; i++){
        multiset<pair<ll,ll>> tt = span_tree[i];
        for (auto c: tt){
            span_tree[i].insert(c);
        }
    }
    for (int i =0; i < n; i++){
        cout << i << ": ";
        for (auto c: span_tree[i]){
            cout << c.first << " ";
        }
        cout << '\n';
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

