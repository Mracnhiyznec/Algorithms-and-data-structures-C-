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
complex<long double> product(vector<complex<long double>> &one,vector<complex<long double>> &two){
    complex<long double> res=0;
    for (int i =0; i < one.size(); i++){
        res=res+one[i]*conj(two[i]);
    }
    return res;
}
vector<complex<long double>> vect_product(vector<vector<complex<long double>>> &A, vector<complex<long double>> &x){
    ll n = x.size();
    vector<complex<long double>> res(n);
    for (int i =0; i < n; i++){
        for (int j = 0; j < n; j++){
            res[i] = res[i] + A[i][j] * x[j];
        }
    }
    return res;
}
void normalize(vector<complex<long double>> &x){
    ld nrm = 0;
    for (int i =0 ; i < x.size(); i++){
        nrm = nrm + abs(x[i]) * abs(x[i]);
    }
    nrm = sqrt(nrm);
    for (int i =0; i < x.size(); i++){
        x[i]/=nrm;
    }
}
ld eps = 1e-12;
void solve(){
    cout << "Введите размер матрицы\n";
    int n;
    cin >> n;
    vector<vector<complex<long double>>> A(n, vector<complex<long double>>(n));
    for (int i =0; i < n; i++){
        for (int j =0; j < n; j++){
            cin >> A[i][j];
        }
    }
    vector<complex<long double>> x(n, 1);
    vector<complex<long double>> xn(n);
    complex<long double> old_lambd = 0;
    complex<long double> now_lambd = INF;
    while (abs(old_lambd - now_lambd) >= eps){
        xn = vect_product(A, x);
        x = xn;
        normalize(x);
        old_lambd = now_lambd;
        now_lambd = product(xn, x) / product(x, x);
    }
//    for (int i = 0; i < 100; i++){
//        xn = vect_product(A, x);
//        x = xn;
//        normalize(x);
//    }
//    xn = vect_product(A, x);
    cout << "Наибольшее по модулю собственное значение: " << fixed << setprecision(10) << now_lambd;
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
