#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        string s;
        cin >> s;
        deque<char> left, right;

        for (char c : s) right.push_back(c);

        int m;
        cin >> m;
        while (m--)
        {
            char opt;
            cin >> opt;
            if (opt == 'L')
            {
                if(left.empty()) continue;
                right.push_front(left.back());
                left.pop_back();
            }
            else if (opt == 'D')
            {
                if(right.empty()) continue;
                left.push_back(right.front());
                right.pop_front();
            }
            else if (opt == 'B')
            {
                if(left.empty()) continue;
                left.pop_back();
            }
            else if (opt == 'C')
            {
                if(left.empty() || right.empty()) continue;
                swap(left.back(), right.front());
            }
            else if (opt == 'P')
            {
                char c; cin >> c;
                left.push_back(c);
            }
        }

        if(left.empty() && right.empty()) {
            cout << "EMPTY\n";
            continue;
        }
        for(auto a : left) cout << a;
        for(auto a : right) cout << a;
        cout << '\n';
    }
}