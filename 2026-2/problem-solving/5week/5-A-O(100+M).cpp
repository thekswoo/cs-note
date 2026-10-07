#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--)
    {

        ll n, m;
        ll ans = 0;
        cin >> n >> m;
        vector<int> a(m), b(m);
        ll cnt[101] = {0}; // 0으로 초기화

        for (int i = 0; i < m; i++)
        {
            cin >> a[i];
            ans += a[i];
        }
        for (int i = 0; i < m; i++)
        {
            cin >> b[i];
            int need = 100 - a[i];
            cnt[b[i]] += need/b[i]; // 누적
            cnt[need % b[i]]++; 
        }

        for(int i=100; i >= 1 && n > 0; i--) {
            ll take = min(n, cnt[i]); // min 인자가 동일해야함
            ans += take * i;
            n -= take;
        }

        cout << ans << '\n';
    }
    return 0;
}