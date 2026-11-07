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
        ll n, m;
        cin >> n >> m; // n 세로, m 가로
        ll p, P;
        cin >> p >> P; // p 단일 설치 비용, P 단일 생산 비용
        ll c, C;
        cin >> c >> C; // c 대형 설치 비용, C 대형 생산 비용

        if (2 * (P - p) > C - c)
        {
            C = 2 * P;
            c = 2 * p;
        }

        if (P - p < 0)
        {
            P = p = 0;
        }

        if (C - c < 0)
        {
            C = c = 0;
        }

        ll doub_cnt = n * m / 2;
        ll sing_cnt = n * m % 2;

        ll ans = doub_cnt * (C - c) + sing_cnt * (P - p);

        cout << ans << '\n';
    }
}