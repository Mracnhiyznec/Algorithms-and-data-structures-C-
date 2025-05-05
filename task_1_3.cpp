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
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.1,sse4.2,popcnt,abm,mmx,vx,avx2,tune=native")
//#pragma GCC optimize("unroll-loops")
ld len_vec(pair<ld, ld> &a){
    return sqrt(a.first * a.first + a.second * a.second);
}
void solve(){
    ld xa, ya, xb, yb, xc, yc;
    cin >> xa >> ya >> xb >> yb >> xc >> yc;
    pair<ld, ld> vec_a = {xa - xb, ya - yb};
    pair<ld, ld> vec_b = {xc - xb, yc - yb};
    ld len_a = len_vec(vec_a);
    ld len_b = len_vec(vec_b);
    ld ans = (vec_a.first * vec_b.first + vec_b.first * vec_b.second) / (len_a * len_b);
    if (ans > 0){
        cout << "острый\n";
        return;
    }
    if (ans == 0){
        cout << "прямой\n";
        return;
    }
    cout << "тупой\n";
}
int main(){
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    int t;
   // cin >> t;
    t=1;
    while(t--){
        solve();
    }
    return 0;
}
