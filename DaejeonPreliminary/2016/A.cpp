#include <stdio.h>

typedef long long ll;

ll edge[2234567], dp[2234567];

ll abs(ll a) { return a>0? a: -a; }
ll min(ll a, ll b) { return a>b? b: a; }
ll max(ll a, ll b) { return a>b? a: b; }
int main(void) {
    ll ans = 0;
    int N; scanf("%d", &N);
    for (int i=2; i<=(1<<(N+1))-1; i++) {
        scanf("%lld", &edge[i]);
        ans += edge[i];
    }


    for (int i=(1 << N) -1 ; i>=1; i--) {
        ll left = edge[i*2] + dp[i*2];
        ll right = edge[i*2+1] + dp[i*2+1];

        ans += abs(left - right);
        dp[i] = max(left, right);
    }
    printf("%lld\n", ans);
}
