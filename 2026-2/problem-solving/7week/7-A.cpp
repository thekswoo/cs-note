#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t; cin >> t;
    while(t--){
        string s; cin >> s;
        int n; cin >> n;

        map<string,char> table;
        for(int i=0; i<n; i++) {
            char c;
            string bi;
            cin >> c >> bi;
            table[bi] = c;
        }

        string ans,buf;
        for(char bit : s) {
            buf += bit;
            auto it = table.find(buf);
            if(it != table.end()){
                ans += it->second;
                buf.clear();
            }
        }

        if(buf.empty()) {
            cout << buf << '\n';
        }else {
                cout << "No\n";
        }
    }
}