#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--)
    {

        ll n, m;
        cin >> n >> m;
        vector<int> a(m), b(m);
        
        ll ans = 0;
        for (int i = 0; i < m; i++)
        {
            cin >> a[i];
            ans += a[i];
        }
        
        ll cnt[101] = {0}; // 0으로 초기화
        for (int i = 0; i < m; i++)
        {
            cin >> b[i];
            int need = 100 - a[i];  // need는 i번째 기기 잔량
            cnt[b[i]] += need/b[i]; // cnt[i번째 기기 충전량] = 잔량/충전량으로 해당 퍼센티지로 1분에 충전할 수 있는 횟수
            cnt[need % b[i]]++;     // i번째 남은 찌꺼지 충전량 
        }

        for(int i=100; i >= 1 && n > 0; i--) { //n은 남은 시간(분)
            ll take = min(n, cnt[i]); // take는 몇분 사용할건지 남은 잔량, cnt[i]에 있는 i번째 기기의 충전량의 충전횟수 비교 후 작은 수
            ans += take * i;        // 시간 * 충전량 누적
            n -= take;              // 남은 시간 사용
        }

        cout << ans << '\n';
    }
    return 0;
}