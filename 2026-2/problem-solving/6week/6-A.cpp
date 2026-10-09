#include <bits/stdc++.h>
using namespace std;

#define ll long long

void solution()
{
    ll a,b,n;
    cin >> a >> b >> n;
    cout << n/a + n/b - n/lcm(a,b) << '\n';
    
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
    {
        solution();
    }

    return 0;
}