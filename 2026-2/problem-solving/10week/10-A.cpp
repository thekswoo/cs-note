#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        ll n;
        cin >> n;

        map<ll, ll> m;
        for (ll i = 0; i < n; i++)
        {
            string date, time;
            ll id;
            cin >> date >> time >> id;
            m.insert({id,0});
            ll cnt;
            cin >> cnt;
            for (ll j = 0; j < cnt; j++)
            {
                string com_name, com_state;
                cin >> com_name >> com_state;
                if (com_state == "OK")
                {
                    
                    m[id]++;
                }
            }
        }

        for (auto a : m)
        {
            cout << a.first << ' ';
            cout << a.second << '\n';
        }
    }
}