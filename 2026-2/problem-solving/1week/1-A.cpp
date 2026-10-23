#include <bits/stdc++.h>
using namespace std;

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
        int mx = 0;
        int cnt = 0;
        for (int i = 0; i < n; i++)
        {
            int a;
            cin >> a;
            if (a > mx)
                cnt++;
            mx = max(a, mx);
        }

        cout << cnt << '\n';
    }
}