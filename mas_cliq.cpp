#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <iostream>
#include <random>
#include <queue>
#include <ctime>
#include <cassert>
#include <numeric>
#include <complex>
#include <string>
#include <cstring>
#include <chrono>
#include <random>
#include <bitset>
#include <stack>
#include <fstream>
#include <array>
#include <iomanip>
#include <functional>
#include <memory>
#include <ctime>
using namespace std;
using ll = long long;
using ld = long double;
#define debug(x) std::cerr << __FUNCTION__ << ":" << __LINE__ << " " << #x << " = " << x << '\n';
const ll INF = 4e18;
ll col_bits(ll a){
    ll res = 0;
    while (a){
        if(a%2){
            res++;
        }
        a/=2;
    }
    return res;
}
vector<ll> graph;
void solve(){
    cout << "Введите количество вершин в графе\n";
    ll n, m;
    cin >> n;
    cout << "Введите количество ребер\n";
    cin >> m;
    ll x, y;
    graph.resize(n);
    for (int i =0; i < m; i++) {
        cin >> x >> y;
        x--;
        y--;
        graph[x] |= (1LL << y);
        graph[y] |= (1LL << x);
    }
    vector<ll> first_group;
    vector<ll> second_group;
    for (int i =0; i < (n + 1) / 2; i++){
        first_group.push_back(i);
    }
    for (int i = (n + 1) / 2; i < n; i++){
        second_group.push_back(i);
    }
    vector<ll> dp1((1LL << first_group.size()));
    vector<ll> dp2((1LL << second_group.size()));
    for (int i = 0; i < (1LL << first_group.size()); i++){
        ll mask = (1LL << n) - 1;
        for (int j =0 ; j < first_group.size(); j++){
            if ((1LL << j)&i){
                dp1[i]=max(dp1[i],dp1[i^(1LL<<j)]);
                mask = (mask & ((graph[first_group[j]]) | (1LL << first_group[j])));
            }
        }
        if ((mask&i)==i){
            dp1[i]= col_bits(i);
        }

    }

    for (int i = 0; i < (1LL << second_group.size()); i++){
        ll mask = (1LL << n) - 1;
        for (int j =0 ; j < second_group.size(); j++){
            if ((1LL << j)&i){
                dp2[i]=max(dp2[i],dp2[i^(1LL<<j)]);
                mask = (mask & ((graph[second_group[j]]) | (1LL << second_group[j])));
            }
        }
        mask = mask / (1LL << first_group.size());
        if ((mask&i)==i){
            dp2[i]= col_bits(i);
        }
    }
    ll ans = max(*max_element(dp1.begin(), dp1.end()), *max_element(dp2.begin(), dp2.end()));
    for (int i =0; i < (1LL << first_group.size()); i++) {
        ll mask = (1LL << n) - 1;
        for (int j =0 ; j < first_group.size(); j++){
            if ((1LL << j)&i){
                mask&=(graph[first_group[j]]);
            }
        }
        mask/=(1LL<<first_group.size());
        ans = max(ans, dp1[i]+dp2[mask]);
    }
    cout << "Размер максимальной клики: " << ans;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    t=1;
    //cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
