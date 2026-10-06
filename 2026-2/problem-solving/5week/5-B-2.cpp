#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define X first
#define Y second
ll dx[4] = {1, 0, -1, 0};
ll dy[4] = {0, 1, 0, -1};
string board[1001];
ll dist1[1001][1001];
ll dist2[1001][1001];
const ll INF = 1e18;

void solution()
{
    int n,m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n+1);
    
    for(int i=0; i<m; i++) {
        int u,v,w;
        cin >> u >> v >> w;
        
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }
    
    vector<ll> dist(n+1,INF);
    dist[1] = 0;

    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<>> pq;
    pq.push({dist[1],1}); // 1번 정점까지의 거리, 1번 정점

    while(!pq.empty()) {
        auto cur = pq.top();
        pq.pop();
        auto d = cur.X; // cur 정점까지 거리
        auto u = cur.Y; // cur 정점 번호
        
        if(d > dist[u]) continue;
        if(u == n) break;

        for(auto &e : adj[u]){
            int v = e.X;
            int w = e.Y;


            if(dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v],v});
            }
        }
    }
    cout << dist[n] << '\n';
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