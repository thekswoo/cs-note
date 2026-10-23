#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;

        vector<vector<ll>> mx(n, vector<ll>(n, LLONG_MIN));
        vector<vector<ll>> mn(n, vector<ll>(n, LLONG_MAX));

        for (int i = 0; i < n; i++)
        {
            int a;
            cin >> a;
            mx[i][i] = mn[i][i] = a;
        }

        for (int len = 2; len <= n; len++)
        {
            for (int i = 0; i <= n - len; i++)
            {
                int j = i - 1 + len;
                for (int k = i; k < j; k++)
                {
                    ll a_mx = mx[i][k];
                    ll a_mn = mn[i][k];
                    ll b_mx = mx[k + 1][j];
                    ll b_mn = mn[k + 1][j];

                    // +
                    mx[i][j] = max(mx[i][j], a_mx + b_mx);
                    mn[i][j] = min(mn[i][j], a_mn + b_mn);

                    // -
                    mx[i][j] = max(mx[i][j], a_mx - b_mn);
                    mn[i][j] = min(mn[i][j], a_mn - b_mx);

                    // *
                    for (auto x : {a_mx, a_mn})
                    {
                        for (auto y : {b_mx, b_mn})
                        {
                            mx[i][j] = max(mx[i][j], x * y);
                            mn[i][j] = min(mn[i][j], x * y);
                        }
                    }
                }
            }
        }
        cout << mx[0][n-1] << '\n';
    }
}