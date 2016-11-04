#include <stdio.h>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;
typedef long long ll;

ll xGCD(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

ll inv_mod(ll a, ll m) {
    ll x, y; xGCD(a, m, x, y);
    return x;
}
struct Interval { //[left, right)
    ll left, right;
};

Interval intervals[501];
vector<ll> x = { 0 };
ll s[1005][505];
ll dp[1005][505];
ll inv[1005];

const ll MOD = 1e9 + 7;

int main(void) {
    int N; scanf("%d", &N);

    for (int i = 1; i <= N+1; i++) {
        inv[i] = inv_mod(i, MOD);
        if (inv[i] < 0) inv[i] += MOD;
        inv[i] %= MOD;
    }
    for (int i = 1; i <= N; i++) {
        ll left, right;
        scanf("%lld %lld", &left, &right);
        right++;
        intervals[i].left = left;
        intervals[i].right = right;
        x.push_back(left);
        x.push_back(right);
    }
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());


    for (int i=1; i<x.size()-1; i++) {
        for (int j=1; j<=N; j++) {
            if (j==1) dp[i][j] = x[i+1] - x[i];
            else {
                dp[i][j] = dp[i][j-1] * (x[i+1] - x[i] + j-1) % MOD * inv[j] % MOD;
                dp[i][j] %= MOD;
            }
        }
    }

    s[0][0] = 1;
    for (int i = 1; i < x.size() - 1; i++) {
        s[i][0] = 1;
        for (int j = 1; j <= N; j++) {
            s[i][j] = s[i - 1][j];
            ll cnt = 1;
            if (intervals[j].left <= x[i] && x[i + 1] <= intervals[j].right) {
                for (int k = j - 1; k >= 0; k--) {
                    s[i][j] += s[i - 1][k] * dp[i][cnt] % MOD;
                    s[i][j] %= MOD;
                    if (intervals[k].left <= x[i] && x[i + 1] <= intervals[k].right) {
                        cnt++;
                    }
                }

            }
        }
    }

    ll sum = 0;
    for (int i = 1; i <= N; i++) {
        sum += s[x.size() - 2][i];
        sum %= MOD;
    }
    printf("%lld\n", sum);
    return 0;
}

