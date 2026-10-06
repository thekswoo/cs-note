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
    int N, M;
    cin >> N >> M;

    vector<int> a, b;
    vector<pair<int, int>> v;
    int ans = 0;

    for (int i = 0; i < M; i++)
    {
        int n;
        cin >> n;
        a.push_back(n);
        ans += n;
    }
    for (int i = 0; i < M; i++)
    {
        int n;
        cin >> n;
        b.push_back(n);
        v.push_back({b[i], a[i]});
    }

    sort(v.begin(), v.end()); // 내림차순으로 정렬

    priority_queue<int> pq;

    for (int i = M - 1; i >= 0; i--)
    {
        int tmp = (100 - v[i].A) / v[i].B;
        int r = (100 - v[i].A) % v[i].B;

        // cout << "i : " << i << '\n';
        // cout << "before" << '\n';
        // cout << "tmp : " << tmp << '\n';
        // cout << "N : " << N << '\n';
        // cout << "ans : " <<  ans << '\n';
        // cout << '\n';

        if (r != 0)
            pq.push(r);

        // 자투리 넣을지 말지
        while(!pq.empty() && N > 0){
            if(pq.top() > v[i].B){
                // cout << "add pq.top() : " << pq.top() << '\n';
                N--;
                ans += pq.top();
                pq.pop();
            }else{
                break;
            }
        }

        if (N - tmp < 0)
        {
            tmp = N;
            N = 0;
        }
        else
        {
            N -= tmp;
        }

        ans += v[i].B * tmp;
    }
    // 남은 자투리 사용
    while (!pq.empty() && N > 0)
    {
        ans += pq.top();
        pq.pop();
        N--;

        // cout << "after" << '\n';
        // cout << "tmp : " << tmp << '\n';
        // cout << "N : " << N << '\n';
        // cout << "add " << v[i].B*tmp << '\n';
        // cout << "ans : " <<  ans << '\n';
        // cout << '\n';
    }

    cout << ans << '\n';
    // cout << '\n';
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
