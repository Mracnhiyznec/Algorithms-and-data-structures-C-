#include<iostream>
#include<vector>
#include<string>
using namespace std;
using ll = long long;
using ld = long double;
#define debug(x) std::cerr << __FUNCTION__ << ":" << __LINE__ << " " << #x << " = " << x << '\n';
const ll INF = 4e18;
//#pragma GCC optimize("Ofast")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.1,sse4.2,popcnt,abm,mmx,avx,avx2,tune=native")
//#pragma GCC optimize("unroll-loops")
// template<class T> istream& operator >> (istream& in, vector<T>& A)
// {
//     for (int i = 0; i < A.size(); ++i)
//         in >> A[i];
//     return in;
// }
class BigInteger{
public:
    BigInteger(){
        number.assign(1, 0);
    }
    BigInteger(string s){
        for (auto digit: s){
            number.push_back(digit - '0');
        }
        reversever(number);
    }
    BigInteger(vector<int> number){
        this->number = number;
    }
    BigInteger(int number){
        while(number){
            this->number.push_back(number % base);
            number/=base;
        }
    }
    void printBigReversed () const{
        for (auto digit: number){
            cout << digit;
        }
    }
    void printBig () const{
        for (int i = number.size() - 1; i >= 0; i--){
            cout << number[i];
        }
    }
    string to_String(){
        string res;
        for (int i = number.size() - 1; i >= 0; i--){
            res.push_back(number[i] + '0');
        }
        return res;
    }
    friend BigInteger operator + (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator - (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator * (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator / (const BigInteger &a, const BigInteger &b);
    friend bool operator >= (const BigInteger &a, const BigInteger &b);
    friend bool operator > (const BigInteger &a, const BigInteger &b);
    friend bool operator < (const BigInteger &a, const BigInteger &B);;
    friend bool operator <= (const BigInteger &a, const BigInteger &b);
    friend istream& operator >> (istream& in, BigInteger &a);
    friend ostream& operator << (ostream& os, BigInteger &a);
    friend bool operator == (const BigInteger &a, const BigInteger &b);
    friend bool operator != (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator % (const BigInteger &a, const BigInteger &b);
private:
    int base = 10;
    vector<int> number;
    void reversever(vector<int> &number){
        vector<int> tempNumber;
        tempNumber.assign(number.size(), 0);
        tempNumber = number;
        for (int i = 0; i < number.size(); i++){
            number[i] = tempNumber[number.size() - i - 1];
        }
    }
};
BigInteger operator % (const BigInteger &a, const BigInteger &b){
    BigInteger
}
bool operator < (const BigInteger &a, const BigInteger &b){
    if (a.number.size() < b.number.size()){
        return true;
    }
    else if (a.number.size() > b.number.size()){
        return false;
    }
    else{
        for (int i = a.number.size() - 1; i >= 0; i--){
            if (a.number[i] > b.number[i]){
                return false;
            }
            if (a.number[i] < b.number[i]){
                return true;
            }
        }
        return false;
    }
}
bool operator > (const BigInteger &a, const BigInteger &b){
    if (a.number.size() > b.number.size()){
        return true;
    }
    else if (a.number.size() < b.number.size()){
        return false;
    }
    else{
        for (int i = a.number.size() - 1; i >= 0; i--){
            if (a.number[i] > b.number[i]){
                return true;;
            }
            if (a.number[i] < b.number[i]){
                return false;
            }
        }
        return false;
    }
}
bool operator >= (const BigInteger &a, const BigInteger &b){
    if (a.number.size() > b.number.size()){
        return true;
    }
    else if (a.number.size() < b.number.size()){
        return false;
    }
    else{
        for (int i = a.number.size() - 1; i >= 0; i--){
            if (a.number[i] > b.number[i]){
                return true;;
            }
            if (a.number[i] < b.number[i]){
                return false;
            }
        }
        return true;
    }
}
bool operator != (const BigInteger &a, const BigInteger &b){
    if (a.number.size() != b.number.size()){
        return true;
    }
    for (int i = 0; i < a.number.size(); i++){
        if (a.number[i] != b.number[i]){
            return true;
        }
    }
    return false;
}
bool operator == (const BigInteger &a, const BigInteger &b){
    if (a.number.size() != b.number.size()){
        return false;
    }
    for (int i = 0; i < a.number.size(); i++){
        if (a.number[i] != b.number[i]){
            return false;
        }
    }
    return true;
}
BigInteger operator + (const BigInteger &a, const BigInteger &b){
    vector<int> result;
    int carry = 0;
    for (int i = 0; i < min(a.number.size(), b.number.size()); i++){
        int curr_sum = carry + a.number[i] + b.number[i];
        result.push_back(curr_sum % 10);
        carry = curr_sum / 10;
    }
    for (int i = min(a.number.size(), b.number.size()); i < max(a.number.size(), b.number.size()); i++){
        if (a.number.size() > b.number.size()){
            int cur_sum = carry + a.number[i];
            result.push_back(cur_sum % 10);
            carry = cur_sum / 10;
        }
        else{
            int cur_sum = carry + b.number[i];
            result.push_back(cur_sum % 10);
            carry = cur_sum / 10;

        }
    }
    if (carry != 0){
        result.push_back(carry);
    }
    return BigInteger(result);
}
BigInteger operator - (const BigInteger &a, const BigInteger &b){
    vector<int> result;
    int carry = 0;
    for (int i = 0; i < b.number.size( ); i++){
        int curr_diff = a.number[i] - b.number[i] - carry;
        if (curr_diff < 0){
            curr_diff+=10;
            carry = 1;
        }
        else{
            carry = 0;
        }
        result.push_back(curr_diff);
    }
    for (int i = b.number.size(); i < a.number.size(); i++){
        if (a.number[i] - carry < 0){
            result.push_back(a.number[i] + 10 - carry);
            carry = 1;
        }
        else{
            result.push_back(a.number[i] - carry);
            carry = 0;
        }
    }
    if (carry != 0){
        result.push_back(carry);
    }
    while(result.size() > 1 && result.back() == 0){
        result.pop_back();
    }
    return BigInteger(result);
}
BigInteger operator * (const BigInteger &a, const BigInteger &b){
    vector<int> result(a.number.size() * b.number.size() + 10);
    for (int i = 0; i < a.number.size(); i++){
        int carry = 0;
        for (int j = 0; j < b.number.size(); j++){
            int cur = result[i + j] + a.number[i] * b.number[j] + carry;
            carry = cur / a.base;
            result[i + j] = cur % a.base;
        }
        int cur = b.number.size();
        while(carry){
            result[i + cur] = carry % a.base;
            cur++;
            carry/=a.base;
        }
    }
    while(result.size() > 1 && result.back() == 0){
        result.pop_back();
    }
    return BigInteger(result);
}
bool operator <= (const BigInteger &a, const BigInteger &b){
    if (a.number.size() < b.number.size()){
        return true;
    }
    else if (a.number.size() > b.number.size()){
        return false;
    }
    else{
        for (int i = a.number.size() - 1; i >= 0; i--){
            if (a.number[i] > b.number[i]){
                return false;
            }
            if (a.number[i] < b.number[i]){
                return true;
            }
        }
        return true;
    }
}
BigInteger operator / (const BigInteger &a, const BigInteger &b){
    vector<int> result(a.number.size() - b.number.size() + 5, 0);
    BigInteger c(result);
    for (int i = result.size() - 1; i >= 0; i--){
        while(b * c <= a){
            c.number[i]++;
        }
        c.number[i]--;
    }
    while (c.number.size() > 1 && !c.number.back()){
        c.number.pop_back();
    }
    return c;
}
ostream& operator << (ostream &os, BigInteger &a) {
        for (int i = a.number.size() - 1; i >= 0; i--){
           os << a.number[i];
        }
        return os;
    }
istream& operator >> (istream &in, BigInteger &a) {
        for (int i = a.number.size() - 1; i >= 0; i--){
            in >> a.number[i];
        }
        return in;
}
void solve(){
    BigInteger a("3123231231");
    BigInteger b("312");
    string k = a.to_String();
    cout << k << "\n";
    a = a + b;
    cout << a;
}
int main(){
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    int t;
    //cin >> t;
    t=1;
    while(t--){
        solve();
    }
    return 0;
}
