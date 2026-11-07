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
        ll k, n;
        cin >> k >> n;

        ll a = n / k;
        ll b = n % k;

        ll index = 0;

        for (ll i = 1; i <= 31; i++)
        {
            if ((1LL << i) - 1 <= a && a < (1LL << (i + 1)) - 1)
            {
                index = i;
                break;
            }
        }
        b += k * (a - ((1LL << index) - 1)); // k * (a - (2^index - 1))

        ll ans = index * k + b / (1LL << index); // index * k + b / 2^index

        cout << ans << '\n';
    }
}