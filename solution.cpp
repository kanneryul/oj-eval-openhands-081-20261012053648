#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class OptimizedMaxFlow {
private:
    int n;
    vector<vector<int>> adj;
    vector<pair<int, int>> edges;
    
public:
    OptimizedMaxFlow(int n) : n(n) {
        adj.resize(n + 1);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    
    // Find edge-disjoint paths using BFS with edge marking
    int maxFlow(int s, int t) {
        if (s == t) return 0;
        
        vector<bool> edge_used(n * n + 1, false); // Mark used edges
        int flow = 0;
        
        // Maximum possible flow is at most 3 due to degree constraint
        for (int attempt = 0; attempt < 3; attempt++) {
            vector<int> parent(n + 1, -1);
            vector<int> edge_idx(n + 1, -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            while (!q.empty() && parent[t] == -1) {
                int u = q.front();
                q.pop();
                
                for (size_t i = 0; i < adj[u].size(); i++) {
                    int v = adj[u][i];
                    if (parent[v] == -1) {
                        // Check if this edge is already used
                        int edge_id = u * n + v; // Simple edge ID
                        if (!edge_used[edge_id]) {
                            parent[v] = u;
                            edge_idx[v] = edge_id;
                            q.push(v);
                            if (v == t) break;
                        }
                    }
                }
            }
            
            if (parent[t] == -1) break; // No more paths found
            
            // Mark edges along the found path as used
            int v = t;
            while (v != s) {
                int u = parent[v];
                int edge_id = u * n + v;
                edge_used[edge_id] = true;
                edge_used[v * n + u] = true; // Mark both directions
                v = u;
            }
            
            flow++;
        }
        
        return flow;
    }
    
    int getEdgeCount() const {
        return edges.size();
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    OptimizedMaxFlow solver(n);
    
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