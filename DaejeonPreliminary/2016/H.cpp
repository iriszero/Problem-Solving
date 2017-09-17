#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int q[100], qq[100];
int inv_plan[20000];
struct Vacation {
    int minDay;
    int from, to;
};

const long long INF = 10005;
typedef long long flow_t;
struct Edge {
    int dest;
    int inv;
    flow_t res;
};

struct Dinic {
    vector<vector<Edge>> adj;
    vector<int> level, start;
    Dinic(int n) {
        adj.resize(n);
        level.resize(n);
        start.resize(n);
    }

    void addEdge(int here, int there, flow_t cap, flow_t caprev = 0) {
        Edge forward = { there, adj[there].size(), cap };
        Edge backward = { here, adj[here].size(), caprev };
        adj[here].push_back(forward);
        adj[there].push_back(backward);
    }
    void addLREdge(int here, int there, flow_t L, flow_t R, int old_src, int old_sink, int new_src, int new_sink) {
        printf("LR Edge %d->%d [%d, %d]\n", here,there, L, R);
        addEdge(new_src, there, L);
        addEdge(here, new_sink, L);
        addEdge(here, there, R-L);
    }

    bool assignLevel(int source, int sink) {
        fill(level.begin(), level.end(), 0);
        queue<int> q;
        q.push(source);
        level[source] = 1;
        while (!q.empty() && level[sink] == 0) {
            int here = q.front();
            q.pop();
            for (Edge &edge : adj[here]) {
                int next = edge.dest;
                if (level[next] == 0 && edge.res > 0) {
                    level[next] = level[here] + 1;
                    q.push(next);
                }
            }
        }
        return level[sink] != 0;
    }

    flow_t blockFlow(int here, int sink, flow_t flow) {
        if (here == sink) return flow;
        for (int &i = start[here]; i < adj[here].size(); ++i) {
            Edge &edge = adj[here][i];
            if (level[edge.dest] != level[here] + 1 || edge.res == 0) continue;
            flow_t res = blockFlow(edge.dest, sink, min(flow, edge.res));
            if (res > 0) {
                edge.res -= res;
                adj[edge.dest][edge.inv].res += res;
                return res;
            }
        }
        return 0;
    }

    flow_t solve(int source, int sink) {
        flow_t ret = 0;
        while (assignLevel(source, sink)) {
            fill(start.begin(), start.end(), 0);
            while (flow_t flow = blockFlow(source, sink, numeric_limits<flow_t>::max()))
                ret += flow;
        }
        return ret;
    }
};

vector<vector<Vacation>> v;
int main(void) {
    int M, N, P, PP; scanf("%d %d %d %d", &M, &N, &P, &PP);
    for (int i=0; i<N; i++) {
        scanf("%d %d", q+i, qq+i);
    }
    v.resize(M);
    int cnt_plans=0;
    for (int i=0; i<M;i ++) {
        int k; scanf("%d", &k); cnt_plans+= k;
        while(k--) {
            int d, r, rr; scanf("%d %d %d", &d, &r, &rr);
            r--, rr--;
            v[i].push_back({d, r, rr});
        }
    }
    Dinic dinic(M+cnt_plans+N+4); //4 for new/old source/sink
    const int OLD_SOURCE	= M+cnt_plans+N;
    const int OLD_SINK 	= M+cnt_plans+N+1;
    const int NEW_SOURCE	= M+cnt_plans+N+2;
    const int NEW_SINK	= M+cnt_plans+N+3;
    int lower_sum = 0;
    dinic.addEdge(OLD_SINK, OLD_SOURCE, INF);
    for (int i=0; i<M; i++) {
        dinic.addLREdge(OLD_SOURCE, i, N-PP, N-P,
                        OLD_SOURCE, OLD_SINK, NEW_SOURCE, NEW_SINK);
        lower_sum += N-PP;
    }

    int idx = 0;
    for (int i=0; i<M; i++) {

        for (auto &plan : v[i]) {
            dinic.addLREdge(i, M+idx, plan.minDay, plan.to - plan.from + 1,
                            OLD_SOURCE, OLD_SINK, NEW_SOURCE, NEW_SINK);
            lower_sum += plan.minDay;
            for (int j=plan.from; j<=plan.to; j++) {
                dinic.addLREdge(M+idx, M+cnt_plans+j, 0, 1,
                                OLD_SOURCE, OLD_SINK, NEW_SOURCE, NEW_SINK);

            }
            inv_plan[idx++] = i;
        }
    }
    for (int i=0; i<N; i++) {
        dinic.addLREdge(M+cnt_plans+i, OLD_SINK, M-qq[i], M-q[i],
                        OLD_SOURCE, OLD_SINK, NEW_SOURCE, NEW_SINK);
        lower_sum += M-qq[i];
    }
    int res = dinic.solve(NEW_SOURCE, NEW_SINK);
    if (res != lower_sum) {
        printf("%d\n", -1);
    }
    else {
        printf("%d\n", 1);
        int idx = M;
        for (int i=0; i<M; i++) {
            vector<int> res;
            for (auto &p : v[i]) {
                for (auto e: dinic.adj[idx]) {
                    if (M+cnt_plans <= e.dest && e.dest < M+cnt_plans + N && e.res > 0) {
                        res.push_back(e.dest - M- cnt_plans);
                    }
                }
                idx++;
            }
            sort(res.begin(), res.end());
            printf("%d ", res.size());
            for (auto e : res) {
                printf("%d ", e);
            }
            printf("\n");
        }


    }

}


