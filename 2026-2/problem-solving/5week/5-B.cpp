#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define X first
#define Y second
ll dx[4] = {1, 0, -1, 0};
ll dy[4] = {0, 1, 0, -1};

int status[100001];
const ll INF = 1e18;

void solution()
{
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w}); // 양방향이므로 양쪽에 추가
        adj[v].push_back({u, w});  
    }

    // dist[i] = 1번에서 i번까지 현재까지 알려진 최소 시간
    vector<ll> dist(n + 1, INF); // INF으로 초기화

    // (거리, 정점) 최소 힙 : 거리가 가장 작은 것부터 꺼냄
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    // priority_queue<pair<ll,int>> pq; 이렇게 선언하고 음수트릭 해도 됨.

    dist[1] = 0;
    pq.push({0, 1});
    while (!pq.empty())
    {
        ll d = pq.top().first; // pq에서 거리 가장 짧은 정점의 거리
        int u = pq.top().second; // pq에서 가장 짧은 정점
        pq.pop();

        if (d > dist[u])
            continue; // 이미 더 짧은 경로로 처리된 낡은 정보면 버림
        if (u == n)
            break; // N번이 확정되면 바로 종료 가능

        for (auto &e : adj[u]) // u와 인접한 정점들 순회
        {
            int v = e.first;
            int w = e.second;
            if (dist[u] + w < dist[v])
            { // 더 빠른 경로 발견 → 갱신(relaxation)
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    cout << dist[n] << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
    {
        solution();
    }

    return 0;
}