// 3주차 B번 창고순회

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int func(int n, int r, int c)
{
    if (n == 0)
        return 1;

    int half = 1 << (n - 1); // 2^n-1

    if (r < half && c < half)
        return func(n - 1, r, c);
    if (r < half && c >= half)
        return half * half + func(n - 1, r, c - half);
    if (r >= half && c < half)
        return 2 * half * half + func(n - 1, r - half, c);
    if (r >= half && c >= half)
        return 3 * half * half + func(n - 1, r - half, c - half);
}

void solution()
{
    int n, r, c;
    cin >> n >> r >> c;
    cout << func(n, r - 1, c - 1) << '\n';
    /*
    0번째 index부터 시작해서 입력 받을 때 r-1, c-1로 받음
    */
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // 여기에 풀이 작성

    int T;
    cin >> T;

    while (T--)
    {
        solution();
    }

    return 0;
}

/*
- 시간복잡도 O(N)
- 공간복잡도 O(N)
*/