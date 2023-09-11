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
string mines = "-";
string plus = "+";
void mystrreverse(string s){
    string rs;
    while(rs.size() < s.size()){
        rs.push_back('0');
    }
    for (int i = 0; i < rs.size(); i++){
        rs[i] = s[s.size() - i - 1];
    }
    s = rs;
}
class BigInteger{
public:
    BigInteger(){
        number.assign(1, 0);
    }
    BigInteger(string s){
        for (auto digit: s){
            if (digit == '-'){
                this->sgn = false;
                continue;
            }
            number.push_back(digit - '0');
        }
        reversever(number);
    }
    BigInteger(vector<int> number){
        this->number = number;
    }
    BigInteger(int number){
        if (number < 0){
            this->sgn = false;
        }
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
        if (!(this->sgn)){
            cout << "-";
        }
        for (int i = number.size() - 1; i >= 0; i--){
            cout << number[i];
        }
    }
    string to_String(){
        string res;
        for (int i = number.size() - 1; i >= 0; i--){
            if (number[i] == '-'){
                res.push_back('-');
                continue;
            }
            res.push_back(number[i] + '0');
        }
        return res;
    }
    friend BigInteger operator + (BigInteger &a, BigInteger &b);
    friend BigInteger operator - (BigInteger &a, BigInteger &b);
    friend BigInteger operator * (BigInteger &a, BigInteger &b);
    friend BigInteger operator / (BigInteger &a, BigInteger &b);
    friend bool operator >= (const BigInteger &a, const BigInteger &b);
    friend bool operator > (const BigInteger &a, const BigInteger &b);
    friend bool operator < (const BigInteger &a, const BigInteger &B);
    friend bool operator <= (const BigInteger &a, const BigInteger &b);
    friend istream& operator >> (istream& in, BigInteger &a);
    friend ostream& operator << (ostream& os, BigInteger &a);
    friend bool operator == (const BigInteger &a, const BigInteger &b);
    friend bool operator != (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator % (BigInteger &a, BigInteger &b);
    friend bool abscomp(BigInteger &a, BigInteger &b);
    friend void swapB(BigInteger &a, BigInteger &b);
    friend BigInteger& operator++ (BigInteger &a);
    friend BigInteger operator++(BigInteger &a, int);
    friend BigInteger& operator--(BigInteger &a);
    friend BigInteger operator--(BigInteger &a, int);
    friend BigInteger operator -(BigInteger &a);
    operator bool() const{
        if (this->number.size() > 1 || (this->number[0] != 0)){
            return true;
        }
        else{
            return false;
        }
    }

private:
    int base = 10;
    bool sgn = true;
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
BigInteger operator -(BigInteger &a){
    a.sgn = !a.sgn;
    return a;
}
BigInteger operator--(BigInteger &a, int) {
    string mines_one = "1";
    BigInteger b(mines_one);
    if (a.sgn && b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c = BigInteger(result);
        a = c;
        if (waswmp){
            a.sgn = false;
        }
        return a;
    }
    if (a.sgn && !b.sgn){
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
        a = BigInteger(result);
        return a;
    }
    if (!a.sgn && b.sgn){
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
        BigInteger c = BigInteger(result);
        a = c;
        a.sgn = false;
        return a;
    }
    if (!a.sgn && !b.sgn){
        bool waswmp = false;
        swapB(a, b);
        if (!abscomp(a, b)){
            waswmp = true;
            swapB(a, b);
        }
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
        BigInteger c = BigInteger(result);
        a = c;
        if (waswmp){
            a.sgn = false;
        }
        return a;
    }
}
BigInteger& operator--(BigInteger &a){
    string mines_one = "1";
    BigInteger b(mines_one);
    if (a.sgn && b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c = BigInteger(result);
        a = c;
        if (waswmp){
            a.sgn = false;
        }
        return a;
    }
    if (a.sgn && !b.sgn){
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
        a = BigInteger(result);
        return a;
    }
    if (!a.sgn && b.sgn){
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
        BigInteger c = BigInteger(result);
        a = c;
        a.sgn = false;
        return a;
    }
    if (!a.sgn && !b.sgn){
        bool waswmp = false;
        swapB(a, b);
        if (!abscomp(a, b)){
            waswmp = true;
            swapB(a, b);
        }
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
        BigInteger c = BigInteger(result);
        a = c;
        if (waswmp){
            a.sgn = false;
        }
        return a;
    }
}
BigInteger operator++(BigInteger &a, int) {
    bool sign = a.sgn;
    string one = "1";
    BigInteger b(one);
    if (a.sgn && b.sgn){
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
        a = BigInteger(result);
        return a;
    }
    if (a.sgn && !b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c;
        c = BigInteger(result);
        a = c;
        if (waswmp){
            a.sgn = false;
        }
        return a;
    }
    if (!a.sgn && b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c;
        c = BigInteger(result);
        a = c;
        if (!waswmp){
            a.sgn = false;
        }
        return a;
    }
    if (!a.sgn && !b.sgn){
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
        BigInteger c = BigInteger(result);
        a = c;
        a.sgn = false;
        return a;
    }
}
BigInteger& operator ++(BigInteger &a){
    bool sign = a.sgn;
    string one = "1";
    BigInteger b(one);
    if (a.sgn && b.sgn){
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
        a = BigInteger(result);
        return a;
    }
    if (a.sgn && !b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c;
        c = BigInteger(result);
        a = c;
        if (waswmp){
            a.sgn = false;
        }
        return a;
    }
    if (!a.sgn && b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c;
        c = BigInteger(result);
        a = c;
        if (!waswmp){
            a.sgn = false;
        }
        return a;
    }
    if (!a.sgn && !b.sgn){
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
        BigInteger c = BigInteger(result);
        a = c;
        a.sgn = false;
        return a;
    }
}

BigInteger operator % (BigInteger &a, BigInteger &b){
    BigInteger division = (a / b);
    BigInteger mult = division * b;
    BigInteger c = a - mult;
    return c;
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
bool abscomp(BigInteger &a, BigInteger &b){
    if (a.number.size() > b.number.size()){
        return true;
    }
    else if(a.number.size() < b.number.size()){
        return false;
    }
    for (int i = a.number.size() - 1; i >= 0; i--){
        if (a.number[i] > b.number[i]){
            return true;
        }
        if (a.number[i] < b.number[i]){
            return false;
        }
    }
    return true;
}
void swapB(BigInteger &a, BigInteger &b){
    BigInteger c = a;
    a = b;
    b = c;
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
                return true;
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
BigInteger operator + (BigInteger &a, BigInteger &b){
    if (a.sgn && b.sgn){
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
    if (a.sgn && !b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c;
        c = BigInteger(result);
        if (waswmp){
            c.sgn = false;
        }
        return c;
    }
    if (!a.sgn && b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c;
        c = BigInteger(result);
        if (!waswmp){
            c.sgn = false;
        }
        return c;
    }
    if (!a.sgn && !b.sgn){
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
        BigInteger c = BigInteger(result);
        c.sgn = false;
        return c;
    }
}
BigInteger operator - (BigInteger &a, BigInteger &b){
    if (a.sgn && b.sgn){
        bool waswmp = false;
        if (!abscomp(a, b)){
            swapB(a, b);
            waswmp = true;
        }
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
        BigInteger c = BigInteger(result);
        if (waswmp){
            c.sgn = false;
        }
        return c;
    }
    if (a.sgn && !b.sgn){
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
    if (!a.sgn && b.sgn){
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
        BigInteger c = BigInteger(result);
        c.sgn = false;
        return c;
    }
    if (!a.sgn && !b.sgn){
        bool waswmp = false;
        swapB(a, b);
        if (!abscomp(a, b)){
            waswmp = true;
            swapB(a, b);
        }
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
        BigInteger c = BigInteger(result);
        if (waswmp){
            c.sgn = false;
        }
        return c;
    }
}
BigInteger operator * (BigInteger &a, BigInteger &b){
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
    BigInteger c = BigInteger(result);
    if (a.sgn && b.sgn){
        c.sgn = true;
    }
    if (a.sgn && !b.sgn){
        c.sgn = false;
    }
    if (!a.sgn && b.sgn){
        c.sgn = false;
    }
    if (!a.sgn && !b.sgn){
        c.sgn = true;
    }
    return c;
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
BigInteger operator / (BigInteger &a, BigInteger &b){
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
        if (!a.sgn){
            os << mines;
        }
        for (int i = a.number.size() - 1; i >= 0; i--){
           os << a.number[i];
        }
        return os;
    }
istream& operator >> (istream &in, BigInteger &a) {
        string s;
        in >> s;
        bool mines = false;
        if (s[0] == '-'){
            mines = true;
            for (int i = 1; i < s.size(); i++){
                s[i - 1] = s[i];
            }
            s.pop_back();
        }
        mystrreverse(s);
        a = BigInteger(s);
        if (mines){
            a.sgn = false;
        }
        return in;
}
void solve(){
    BigInteger a, b, c;
    cin >> a >> b;
    a = a * b;
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
