#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int func(int n, int r, int c)
{
    if (n == 0)
        return 1;
    int half = 1 << (n - 1);
    if (r < half && c < half)
        return func(n - 1, r, c);
    if (r < half && c >= half)
        return half * half + func(n - 1, r, c - half);
    if (r >= half && c < half)
        return half * half * 2 + func(n - 1, r - half, c);
    else
        return half * half * 3 + func(n - 1, r - half, c - half);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        int n, r, c;
        cin >> n >> r >> c;
        cout << func(n, r - 1, c - 1) << '\n';
    }
}