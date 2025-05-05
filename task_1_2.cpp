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
void solve(){
    ld x0, y0, x1, y1, x2, y2;
    cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
    if (x1 == x2 && y1 == y2){
        if (x0 == x1 && y0 == y1){
            cout << "YES\n";
            return;
        }
        cout << "NO\n";
        return;
    }
    if (x1 != x2){
        ld tt = (x0 - x2) / (x1 - x2);
        if (tt >= 0 && tt <= 1 && y1 * tt + (1 - tt) * y2 == y0){
            cout << "YES\n";
            return;
        }
        cout << "NO\n";
        return;
    }
    ld tt = (y0 - y2) / (y1 - y2);
    if (tt >= 0 && tt <= 1 && x1 * tt + (1 - tt) * x2 == x0){
        cout << "YES\n";
        return;
    }
    cout << "NO\n";
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
