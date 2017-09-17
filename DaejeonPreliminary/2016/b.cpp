#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
typedef pair<int, int> pii;
struct Point {
	int dist, x, y;
	bool operator< (const Point& other) const {
		return dist < other.dist;
	}
};
	
int x[5000], y[5000]; double q_dist[5001];
int get_dist(int x1, int y1, int x2, int y2) {
	return (x1- x2) * (x1-x2) + (y1-y2)* (y1-y2);
}
	
int main(void) {
	int N; scanf("%d", &N);
	vector<Point> v;
	for (int i=0; i<N; i++) {
		scanf("%d %d", x+i, y+i);
	}

	int max_dist = 0;
	int p=-1, q=-1;
	for (int i=0; i<N; i++) {
		for (int j=0; j<N; j++) {
			int now = get_dist(x[i], y[i], x[j], y[j]);
			if (max_dist < now) {
				max_dist = now; p = i; q= j;
			}

		}
	}
	double ans = sqrt(max_dist);
	for (int i=0; i<N; i++) {
		v.push_back({get_dist(x[p], y[p], x[i], y[i]), x[i], y[i]});
	}
	sort(v.begin(), v.end());

	// [0, i) will be attached to p
	for (int i=N-1; i>=0; i--) {
		q_dist[i] = q_dist[i+1];
		for (int j=i+1; j<N; j++) {
			double now = sqrt(get_dist(v[i].x, v[i].y, v[j].x, v[j].y));
			if (q_dist[i] < now) q_dist[i] = now;
		}
	}	
	
	double p_dist = 0;
	for (int i=0; i<N-1; i++) {
		for (int j=0; j<i; j++) {
			double now = sqrt(get_dist(v[i].x, v[i].y, v[j].x, v[j].y));
			if (p_dist < now) p_dist = now;
		}
        if (ans > p_dist + q_dist[i+1]) {
            ans = p_dist + q_dist[i+1];
        }
	}

	printf("%.4lf\n", ans);
	return 0;
}
