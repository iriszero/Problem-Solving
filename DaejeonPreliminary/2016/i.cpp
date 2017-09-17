#include <stdio.h>

int arr[1005];
int N;
int solve() {
    for (int ans = 0; ans <= N; ans ++) {
        int cnt[3] = {0, 0, 0};
        for (int i=0; i<N; i++) {
            if (arr[i] < ans) {
                cnt[0] ++;
            }
            else if (arr[i] == ans) {
                cnt[1] ++;
            }
            else if (arr[i] > ans) {
                cnt[2]++;
            }

        }
        if (cnt[0] + cnt[1] >= N- ans && cnt[1] + cnt[2] >= ans) {
            return ans;
        }
    }
}
int main(void) {
    scanf("%d", &N);
    for (int i=0; i<N; i++) scanf("%d", arr+i);
    printf("%d\n", solve());
}
