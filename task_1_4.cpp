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
    ld x1,y1,x2,y2,x3,y3,z1,z2,z3;
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
    ld A = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1);
    ld B = (z2-z1)*(x3-x1) - (x2-x1)*(z3-z1);
    ld C = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
    ld D = -A * x1 - B * y1 - C * z1;
    cout << A << " " << B << " " << C << " " << D;
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
