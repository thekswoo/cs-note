#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solution()
{
    int X, N;
    cin >> X >> N;

    vector<vector<int>> dp(N + 1, vector<int>(X + 1, 0));

    for (int i = 1; i <= N; i++)
    {
        int a, c;
        cin >> a >> c;

        for (int w = 0; w <= X; w++)
        {
            dp[i][w] = dp[i - 1][w];
            if (w >= c)
            {
                dp[i][w] = max(dp[i][w], dp[i - 1][w - c] + a);
            }
        }
    }
    cout << dp[N][X] << '\n';
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