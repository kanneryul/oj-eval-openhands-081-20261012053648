#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class FastMaxFlow {
private:
    int n, m;
    vector<vector<int>> adj;
    vector<pair<int, int>> edges;
    
public:
    FastMaxFlow(int n, int m) : n(n), m(m) {
        adj.resize(n + 1);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    
    // Optimized BFS for unit capacity graphs
    int maxFlow(int s, int t) {
        if (s == t) return 0;
        
        // Use edge indices for capacity tracking
        vector<int> edge_cap(m * 2, 1); // Forward and backward edges
        vector<vector<int>> edge_list(n + 1);
        vector<vector<int>> rev_edge(n + 1);
        
        // Build edge list with indices
        for (int i = 0; i < m; i++) {
            int u = edges[i].first;
            int v = edges[i].second;
            edge_list[u].push_back(i * 2);
            edge_list[v].push_back(i * 2 + 1);
            rev_edge[u].push_back(i * 2 + 1);
            rev_edge[v].push_back(i * 2);
        }
        
        int flow = 0;
        
        // Since max degree is 3, max flow is at most 3
        for (int attempt = 0; attempt < 3; attempt++) {
            vector<int> parent(n + 1, -1);
            vector<int> parent_edge(n + 1, -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            while (!q.empty() && parent[t] == -1) {
                int u = q.front();
                q.pop();
                
                for (int edge_idx : edge_list[u]) {
                    if (edge_cap[edge_idx] > 0) {
                        int v = (edge_idx % 2 == 0) ? edges[edge_idx / 2].second : edges[edge_idx / 2].first;
                        if (parent[v] == -1) {
                            parent[v] = u;
                            parent_edge[v] = edge_idx;
                            q.push(v);
                            if (v == t) break;
                        }
                    }
                }
            }
            
            if (parent[t] == -1) break;
            
            // Update capacities along the path
            int v = t;
            while (v != s) {
                int u = parent[v];
                int edge_idx = parent_edge[v];
                edge_cap[edge_idx]--; // Use forward edge
                edge_cap[edge_idx ^ 1]++; // Increase reverse edge capacity
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
    
    FastMaxFlow solver(n, m);
    
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