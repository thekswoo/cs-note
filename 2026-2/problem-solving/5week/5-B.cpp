#include <bits/stdc++.h>
using namespace std;
const long long INF = 1e18;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        int n, m;
        cin >> n >> m;

        vector<vector<pair<int, int>>> adj(n + 1);
        for (int i = 0; i < m; i++)
        {
            int u, v, w; // u-v 가 w 시간 걸림
            cin >> u >> v >> w;
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }

        vector<long long> dist(n + 1, INF);      // 1번부터 i번까지 거리
        vector<bool> visited(n + 1, 0);          // i번째 방문 여부
        priority_queue<pair<long long, int>> pq; // 현재 정점

        dist[1] = 0;
        pq.push({0, 1});

        while (!pq.empty())
        {
            int cur = pq.top().second; // 탐색 정점
            pq.pop();

            if (visited[cur])
                continue;     // 이미 방문되었으면 skip
            visited[cur] = 1; // 방문 여부 표시

            if (cur == n)
                break; // 끝 정점이면 중단

            for (auto e : adj[cur])
            {
                int v = e.first;  // cur주변 정점 번호
                int w = e.second; // cur부터 주변 정점 거리

                if (visited[v])
                    continue; // 이미 방문되었으면 skip

                if (dist[cur] + w < dist[v])
                { // 원래 거리보다 cur까지의 거리 + 주변 정점거리까지 거리가 짧다면 갱신
                    dist[v] = dist[cur] + w;
                    pq.push({-dist[v], v}); // 거리 음수로 넣음
                }
            }
        }

        cout << dist[n] << '\n';
    }
}