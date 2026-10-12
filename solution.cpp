#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

using namespace std;

class OptimizedMaxFlow {
private:
    int n, m;
    vector<vector<pair<int, int>>> adj; // node -> (neighbor, edge_id)
    vector<pair<int, int>> edges;
    
public:
    OptimizedMaxFlow(int n, int m) : n(n), m(m) {
        adj.resize(n + 1);
    }
    
    void addEdge(int u, int v, int edge_id) {
        adj[u].emplace_back(v, edge_id);
        adj[v].emplace_back(u, edge_id);
        edges.emplace_back(u, v);
    }
    
    // Fast BFS with edge marking for unit capacity graphs
    int maxFlow(int s, int t) {
        if (s == t) return 0;
        
        int flow = 0;
        vector<bool> edge_used(m, false);
        
        // Since max degree is 3, max flow is at most 3
        for (int attempt = 0; attempt < 3; attempt++) {
            vector<int> parent(n + 1, -1);
            vector<int> edge_idx(n + 1, -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            bool found = false;
            while (!q.empty() && !found) {
                int u = q.front();
                q.pop();
                
                for (auto& edge_info : adj[u]) {
                    int v = edge_info.first;
                    int eid = edge_info.second;
                    if (parent[v] == -1 && !edge_used[eid]) {
                        parent[v] = u;
                        edge_idx[v] = eid;
                        if (v == t) {
                            found = true;
                            break;
                        }
                        q.push(v);
                    }
                }
            }
            
            if (!found) break;
            
            // Mark edges along the found path as used
            int v = t;
            while (v != s) {
                int u = parent[v];
                int edge_id = edge_idx[v];
                edge_used[edge_id] = true;
                v = u;
            }
            
            flow++;
        }
        
        return flow;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    OptimizedMaxFlow solver(n, m);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        solver.addEdge(u, v, i);
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