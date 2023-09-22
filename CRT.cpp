#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int gcd(int a, int b) {
    if(b == 0) return a;
    return gcd(b, a % b);
}

vector<int> coprimeDivisors(int modulus) {
    vector<int> divisors;
    for(int i = 2; i < modulus; i++) {
        if(gcd(i, modulus) == 1) divisors.push_back(i);
    }

    return divisors;
}

vector<pair<int, int>> splitCongruence(int residue, int modulus) {
    vector<pair<int, int>> congruences;
    vector<int> divisors = coprimeDivisors(modulus);
    for(const auto& divisor : divisors) {
        congruences.push_back({residue % (modulus / divisor), modulus / divisor});
    }
    return congruences;
}

int modularInverse(int a, int m) {
    a %= m;
    if (a < 0) a += m;

    int m0 = m;
    int x0 = 0;
    int x1 = 1;

    while (a > 1) {
        int q = a / m;
        int t = m;

        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;
    return x1;
}

pair<long long, long long> ChineseRemainderTheorem(vector<pair<int, int>> congruences) {
    long long M = 1;
    long long x = 0;

    for(const auto& congruence : congruences) {
        M  *= congruence.second;
    }

    for(const auto& congruence : congruences) {
        int a_i = congruence.first;
        int m_i = congruence.second;
        int M_i = M / m_i;
        int M_i_inv = modularInverse(M_i, m_i);
        x += a_i * M_i * M_i_inv;
    }

    while(x < 0) {
        x += M;
    }

    return make_pair(x % M, M);
}

int main() {
    int n;
    cin >> n;

    vector<pair<int, int>> congruences;
    vector<pair<int, int>> coprime_congruences;
    for(int i = 0; i < n; i++) {
        int r, m;
        cin >> r >> m;
        congruences.push_back({r, m});
    }

    for(const auto& congruence : congruences) {
        int residue = congruence.first;
        int modulus = congruence.second;

        bool isCoprime = true;

        for(const auto& coprime_congruence : coprime_congruences) {
            if(gcd(modulus, coprime_congruence.second) != 1) {
                isCoprime = false;
                break;
            }
        }

        if(isCoprime) {
            coprime_congruences.push_back(congruence);
        } else {
            vector<pair<int, int>> split_congruences = splitCongruence(residue, modulus);
            coprime_congruences.insert(coprime_congruences.end(), split_congruences.begin(), split_congruences.end());
        }
    }

    pair<long long, long long> result = ChineseRemainderTheorem(coprime_congruences);
    cout << "Solution: x is congruent with " << result.first << " (mod " << result.second <<")\n";
}