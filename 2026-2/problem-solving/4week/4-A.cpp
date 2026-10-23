#include <bits/stdc++.h>
using namespace std;

int main(){
    int t; cin >> t;
    while(t--){
        int n; cin >> n;
        vector<vector<int>> child(n);
        int root;
        for(int i=0; i<n; i++) {
            int p; cin >> p;
            if(p == -1) root = i;
            else {
                child[p].push_back(i);
            } 
        }

        int x; cin >> x;
        if(x == root) {
            cout << "0\n";
            continue;
        }
        stack<int> stk;
        stk.push(root);
        int leaf = 0;
        while(!stk.empty()){
            int cur = stk.top();
            stk.pop();

            int cnt = 0;
            for(auto c : child[cur]) {
                if(c == x) continue;
                cnt++;
                stk.push(c);
            }
            if(cnt == 0) leaf++;
        }

        cout << leaf << '\n';
    }
}