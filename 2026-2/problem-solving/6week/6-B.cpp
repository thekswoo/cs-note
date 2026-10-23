#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        int n,m;
        cin >> n >> m;

        vector<vector<int>> adj(n+1);
        for(int i=0; i<m; i++) {
            int u,v;
            cin >> u >> v;
            adj[u].push_back(v);
        }

        vector<bool> blocked(n+1);
        int k;
        cin >> k;
        for(int i=0; i<k; i++) {
            int b; cin >> b;
            blocked[b] = 1;
        }

        queue<int> q;
        vector<bool> visited(n+1);
        q.push(1);
        visited[1] = 1;
        bool found = 0;
        while(!q.empty()) {
            int cur = q.front();
            q.pop();

            if(adj[cur].empty()) {
                cout << "YES\n";
                found = 1;
                break;
            }

            for(int nxt : adj[cur]) {
                if(blocked[nxt]) continue;
                if(visited[nxt]) continue;

                q.push(nxt);
                visited[nxt] = 1;
            }
        }
        if(!found) cout << "NO\n";
    }
}