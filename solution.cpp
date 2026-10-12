#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <climits>

using namespace std;

class MaxFlowSolver {
private:
    int n;
    vector<vector<int>> adj;
    vector<vector<int>> capacity;
    vector<vector<int>> flow;
    
public:
    MaxFlowSolver(int n) : n(n) {
        adj.resize(n + 1);
        capacity.assign(n + 1, vector<int>(n + 1, 0));
        flow.assign(n + 1, vector<int>(n + 1, 0));
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] = 1;
        capacity[v][u] = 1;
    }
    
    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = s;
        queue<pair<int, int>> q;
        q.push(make_pair(s, INT_MAX));
        
        while (!q.empty()) {
            int u = q.front().first;
            int flow_val = q.front().second;
            q.pop();
            
            for (int v : adj[u]) {
                if (parent[v] == -1 && capacity[u][v] - flow[u][v] > 0) {
                    parent[v] = u;
                    int new_flow = min(flow_val, capacity[u][v] - flow[u][v]);
                    if (v == t) {
                        return new_flow;
                    }
                    q.push(make_pair(v, new_flow));
                }
            }
        }
        return 0;
    }
    
    int maxFlow(int s, int t) {
        // Reset flow matrix for this pair
        for (int i = 1; i <= n; i++) {
            fill(flow[i].begin(), flow[i].end(), 0);
        }
        
        int flow_sum = 0;
        vector<int> parent(n + 1);
        int new_flow;
        
        while ((new_flow = bfs(s, t, parent)) > 0) {
            flow_sum += new_flow;
            
            // Update flow along the path
            int v = t;
            while (v != s) {
                int u = parent[v];
                flow[u][v] += new_flow;
                flow[v][u] -= new_flow;
                v = u;
            }
        }
        
        return flow_sum;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    MaxFlowSolver solver(n);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        solver.addEdge(u, v);
    }
    
    long long total_sum = 0;
    
    // Calculate max flow for all pairs (a, b) where a < b
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            total_sum += solver.maxFlow(a, b);
        }
    }
    
    cout << total_sum << endl;
    
    return 0;
}