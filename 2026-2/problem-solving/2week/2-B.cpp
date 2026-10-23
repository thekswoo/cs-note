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

        int cur = 0;
        int ans = 0;
        priority_queue<int> pq;
        for(int i=0; i<n; i++)
        {
            int a;
            cin >> a;

            cur += a;
            ans++;

            if(a < 0) pq.push(-a);

            if(cur < 0)
            {
                cur += pq.top();
                pq.pop();
                ans--;
            }
        }
        cout << ans << '\n';
    }
}