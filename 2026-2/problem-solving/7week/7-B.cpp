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
        int n;
        cin >> n;

        vector<long long> a(n + 1), d(n + 1), u(n + 1);
        for (int i = 1; i <= n; i++)
        {
            cin >> a[i] >> d[i] >> u[i];
        }
        
        priority_queue<tuple<long long, long long, int>> pq;
        long long t = 0;
        int idx = 1;

        for (int i = 0; i < n; i++)
        {
            while(1){
                if(a[idx] <= t) {
                    pq.emplace(u[idx] - t - a[idx], -a[idx], -idx);
                    idx++;
                }else {
                    break;
                }
            }
            
            int num = -get<2>(pq.top());
            cout << t << ' ' << num << '\n';
            pq.pop();
            
            t += d[num];
        }
    }
}