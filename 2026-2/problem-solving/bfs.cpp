#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define X first
#define Y second

int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};
string board[1001];
int dist[1001][1001];


void solution()
{
    int R,C; cin >> R >> C;

    fill(&dist[0][0], &dist[1000][1000], 0); // 2차원 배열 초기화
    queue<pair<int, int>> Q;
    for(int i=0; i<R; i++) {
        cin >> board[i];
    }
    // 시작점 찾기
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            if(board[i][j] == '1') {
                dist[i][j] = 1;
                Q.push({i,j});
            }
            else if(board[i][j] == '#') dist[i][j] = -1;
        }
    }
    
    while(!Q.empty()){
        auto cur = Q.front();
        Q.pop();
        for(int dir = 0; dir < 4; dir++) {
            int nx = cur.X + dx[dir];
            int ny = cur.Y + dy[dir];
            if(nx < 0 || nx >=R || ny < 0 || ny >= C ) continue; // board판 넘어가면 스킵
            if(dist[nx][ny] == -1 || dist [nx][ny] > 0) continue; // 벽 or 이미 방문 스킵
            dist[nx][ny] = dist[cur.X][cur.Y] + 1;
            Q.push({nx,ny});
        }
    }
    
    int mx = 0;
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            if(dist[i][j] == 0) {
                cout << -1 << '\n';
                return;
            }
            if(dist[i][j] > mx) mx = dist[i][j];
        }
    }
    cout << max(mx-1,0) << '\n';
    return;

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