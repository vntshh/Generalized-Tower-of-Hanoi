#include <bits/stdc++.h>
using namespace std;
const long long int INF = 1000000000000ll;
long long int dp[70][70];
stack <int> tower[70];
vector<pair<int,int>> all_moves;
void go(int min_disk, int max_disk, int from, int to, int m) {
    if(min_disk == max_disk) {
        all_moves.push_back(make_pair(from, to));
        tower[from].pop();
        tower[to].push(min_disk);
        return;
    }
    int n = max_disk+1-min_disk;
    int split = -1;
    for (int i = 1; i < n; ++i) {
        if(dp[n][m] == 2*dp[i][m]+dp[n-i][m-1])
            split = i;
    }
    split = min_disk+split-1;
    int first_empty = 0;
    while(true) {
        first_empty++;
        if(first_empty == to) continue;
        if(tower[first_empty].empty() || tower[first_empty].top() > split) break;
    }
    go(min_disk,split,from,first_empty,m);
    go(split+1,max_disk,from,to,m-1);
    go(min_disk,split,first_empty,to,m);
}
vector<pair<int,int>> calc(int n, int m) {
    memset(dp, -1, sizeof dp);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if(j == 1) dp[i][j] = INF;
            else if(i == 1) dp[i][j] = 1;
            else {
                dp[i][j] = INF;
                for (int k = 1; k < i; k++) {
                    dp[i][j] = min(dp[i][j],2*dp[k][j] + dp[i-k][j-1]);
                }
            }
        }
    }
    for(int i = n; i >= 1; --i) tower[1].push(i);
    go(1,n,1,m,m);
    return all_moves;
}