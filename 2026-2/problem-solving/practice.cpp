#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        int n,m; cin >> n >> m;

        vector<vector<pair<int,int>>> adj(n+1);
        for(int i=0; i<m; i++)
        {
            int u,v,w;
            cin >> u >> v >> w;
            adj[u].push_back({v,w});
            adj[v].push_back({u,w});
        }

        vector<ll> dist(n+1,1e18);
        vector<bool> visited(n+1,0);
        priority_queue<pair<int,int>> pq;
        
        pq.push({0,1});
        dist[1] = 0;

        while(!pq.empty())
        {
            int cur = pq.top().second;
            pq.pop();

            if(visited[cur]) continue;
            visited[cur] = 1;

            if(cur == n) break;

            for(auto nxt : adj[cur])
            {
                int v = nxt.first;
                int w = nxt.second;

                if(visited[v]) continue;

                if(dist[cur] + w < dist[v])
                {
                    dist[v] = dist[cur] + w;
                    pq.push({-dist[v],v});
                }
            }
        }
        cout << dist[n] << '\n';
    }
}