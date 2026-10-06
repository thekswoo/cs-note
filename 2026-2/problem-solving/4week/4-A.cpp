#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solution()
{
    int N;
    cin >> N;

    vector<vector<int>> child(N);
    int root;
    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        if (a == -1)
            root = i;                   //i를 root로 지정
        else
            child[a].push_back(i);      //a의 자식으로 i를 삽입해야함!!!
    }

    int X; cin >> X;
    if(X == root) {
        cout << "0\n";
        return;
    }

    int leaf = 0;
    stack<int> st; st.push(root);
    while (!st.empty()){
        int v = st.top();               //현재 노드
        st.pop();
        int cnt = 0;
        for(int c : child[v]) {         //v의 자식들 순회
            if(c == X) continue;        //자식이 X인 경우는 넘어감 스택에 넣지도 않음
            cnt++;                      //현재 노드의 자식 수
            st.push(c);                 //현재 노드의 자식을 스택애 push
        }
        if(cnt == 0) leaf++;            //leaf노드이면 총 leaf++
    }

    cout << leaf << '\n';
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