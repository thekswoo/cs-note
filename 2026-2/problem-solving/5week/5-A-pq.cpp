#include <bits/stdc++.h>
#include <cassert>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> pll;
const ll INF{999'999'999'999'999'999};
const ll MOD{1'000'000'007};
const ll dr[]{-1, 0, 1, 0};
const ll dc[]{0, 1, 0, -1};

#define B first
#define A second

void solve()
{
    ll N, M;
    ll ans = 0;
    cin >> N >> M;
    vector<ll> a(M), b(M);
    priority_queue<ll> pq;
    for(int i=0; i<M; i++){
        cin >> a[i];
        ans += a[i];
    }
    for(int i=0; i<M; i++) {
        cin >> b[i];
        ll rest = 100 - a[i];
        for(int j=0; j<rest/b[i]; j++) {
            pq.push(b[i]);
        }
        pq.push(rest % b[i]);
    }

    while(N-- && !pq.empty()){
        ans += pq.top();
        pq.pop();
    }

    cout << ans << '\n';
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    ll t{1};
    cin >> t;
    while (t--)
    {
        solve();
    }
}
