struct cmpl{
    double a, b;
    cmpl(double a = 0, double b = 0): a(a), b(b) {}
    cmpl operator+(const cmpl &other) const{
        return cmpl(a + other.a, b + other.b);
    }
    cmpl operator-(const cmpl &other) const{
        return cmpl(a - other.a, b - other.b);
    }
    cmpl operator*(const cmpl &other) const{
        return cmpl(a * other.a - b * other.b, a * other.b + b * other.a);
    }
    cmpl operator/(const double c) const{
        return cmpl(a/c, b/c);
    }
    double &real(){
        return a;
    }
    double &imag(){
        return b;
    }
};
const double PI = atan2(0,-1);
vector<vector<cmpl>> w;
void precompute_w(int max_level){
    w.resize(max_level + 1);
    w[0].resize(1,1);
    for (int l = 1; l <= max_level; l++){
        int n = (1 << l);
        w[l].resize(n / 2);
        cmpl firstw(cos(2 * PI / n), sin(2 * PI / n));
        for (int j =0; j < n / 2; j++){
            if (j % 2 == 0){
                w[l][j] = w[l - 1][j / 2];
            }
            else{
                w[l][j] = w[l - 1][j / 2] * firstw ;
            }
        }
    }
}
void fft(vector<cmpl> &P, bool inv = false){
    int n = P.size();
    int logn = 0;
    while ((1 << logn) != n){
        logn++;
    }
    vector<int> bit_rev(n);
    bit_rev[0] = 0;
    for (int j = 1; j < n; j++){
        bit_rev[j] = bit_rev[j / 2] / 2 + ((j & 1) << (logn - 1));
    }
    for (int j = 0; j < n; j++){
        if (j < bit_rev[j]){
            swap(P[bit_rev[j]], P[j]);
        }
    }
    for (int lvl = 1; lvl <= logn; lvl++){
        int len = 1 << lvl;
        int half = len >> 1;
        for (int st = 0; st < n; st += len){
            for (int j = 0; j < half; j++){
                cmpl tw = w[lvl][j];
                if (inv) tw.imag() = -tw.imag();
 
                cmpl u = P[st + j];
                cmpl v = tw * P[st + j + half];
 
                P[st + j]        = u + v;
                P[st + j + half] = u - v;
            }
        }
    }
    if (inv){
        for (int i = 0; i < n; i++) P[i] = P[i] / n;
    }
}
vector<ll> multiply(vector<ll> P, vector<ll> Q){
    int n = 1, logn = 0;
    while (n < P.size() + Q.size() - 1){
        n*=2;
        logn++;
    }
    precompute_w(logn);
    vector<cmpl> A(n);
    for (int j = 0; j < n; j++){
        A[j] = cmpl(j < P.size() ? P[j] : 0, j < Q.size() ? Q[j] : 0);
    }
    fft(A);
    for (int j = 0; j < n; j++){
        A[j] = A[j] * A[j];
    }
    fft(A, true);
    vector<ll> R(n);
    for (int j = 0;j < n; j++){
        R[j] = round(A[j].imag() / 2);
    }
    return R;
}
