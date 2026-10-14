#include <bits/stdc++.h>
using namespace std;

void solution()
{
    int n, m;
    cin >> n >> m;
    queue<int> q;
    vector<vector<int>> adj(n + 1);
    vector<int> gongsa(n + 1, 0);
    vector<int> visited(n + 1, 0);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    int k;
    cin >> k;
    for (int i = 0; i < k; i++)
    {
        int gs;
        cin >> gs;
        gongsa[gs] = 1;
    }

    q.push(1);
    visited[1] = 1;

    while (!q.empty())
    {
        int cur = q.front();
        q.pop();

        if (adj[cur].empty()) // cur의 인접 정점이 없다면 외곽 건물 -> YES 출력
        {
            cout << "YES\n";
            return;
        }

        for(int nxt : adj[cur])
        {
            if (!gongsa[nxt] && !visited[nxt]) // nxt가 공사중이 아니고 방문하지도 않았다면 q에 push
            {
                q.push(nxt);
                visited[nxt] = 1;
            }
        }
    }

    cout << "NO\n";
    return;
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