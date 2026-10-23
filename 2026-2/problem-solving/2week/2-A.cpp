#include <bits/stdc++.h>
using namespace std;

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
        vector<queue<int>> q(51);
        int sum = 0;

        for (int i = 0; i < n; i++)
        {
            int cmd;
            cin >> cmd;
            if (cmd == 0)
            {
                int p, m;   
                cin >> p >> m;
                q[p].push(m);
            }
            else
            {
                int p;
                cin >> p;
                if(q[p].empty()) continue;
                sum += q[p].front();
                q[p].pop();
            }
        }
        cout << sum << '\n';
    }
}