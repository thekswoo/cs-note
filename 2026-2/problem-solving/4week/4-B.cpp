#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t; cin >> t;
    while(t--){
        int x,n; cin >> x >> n;

        vector<vector<int>> dp(n+1,vector<int>(x+1,0));
        for(int i=1; i<=n; i++) {
            int a,c;
            cin >> a >> c;

            for(int w=0; w<=x; w++){
                dp[i][w] = dp[i-1][w];
                if(c <= w){
                    dp[i][w] = max(dp[i-1][w], dp[i-1][w-c] + a);
                }
            }
        }
        cout << dp[n][x] << '\n';
    }
}