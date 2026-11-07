#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t; cin >> t;
    while(t--)
    {
        int n;
        cin >> n;
        priority_queue<pair<ll,ll>> pq;

        for(ll i=0; i<n; i++)
        {
            ll s,e;
            cin >> s >> e;
            pq.push({-e,-s});
        }

        ll cur_time = 0;
        ll ans = 0;

        while(!pq.empty())
        {
            auto cur = pq.top();
            pq.pop();
            
            auto en = -cur.first;
            auto st = -cur.second;

            if(st >= cur_time) {
                ans++;
                cur_time = en;
            }else{
                continue;
            }
        }

        cout << ans << '\n';

        // while(!pq.empty())
        // {
        //     auto cur = pq.top();
        //     pq.pop();
        //     auto en = -cur.first;
        //     auto st = -cur.second;

        //     cout << en << ' ' << st << '\n';
        // }
    }
}