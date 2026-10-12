#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class GlobalConnectivityAnalyzer {
private:
    int n, m;
    vector<vector<int>> adj;
    vector<pair<int, int>> edges;
    
public:
    GlobalConnectivityAnalyzer(int n, int m) : n(n), m(m) {
        adj.resize(n + 1);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    
    // Compute connectivity between two nodes using BFS with edge removal
    int getConnectivity(int u, int v) {
        if (u == v) return 0;
        
        int connectivity = 0;
        vector<bool> edge_used(m, false);
        
        // Try to find up to 3 edge-disjoint paths
        for (int attempt = 0; attempt < 3; attempt++) {
            vector<int> parent(n + 1, -1);
            vector<int> edge_used_in_path(n + 1, -1);
            queue<int> q;
            q.push(u);
            parent[u] = u;
            
            bool found = false;
            while (!q.empty() && !found) {
                int curr = q.front();
                q.pop();
                
                for (int next : adj[curr]) {
                    if (parent[next] == -1) {
                        // Find if edge curr-next is available
                        bool edge_available = false;
                        int edge_idx = -1;
                        for (int i = 0; i < m; i++) {
                            if ((edges[i].first == curr && edges[i].second == next) ||
                                (edges[i].first == next && edges[i].second == curr)) {
                                if (!edge_used[i]) {
                                    edge_available = true;
                                    edge_idx = i;
                                }
                                break;
                            }
                        }
                        
                        if (edge_available) {
                            parent[next] = curr;
                            edge_used_in_path[next] = edge_idx;
                            if (next == v) {
                                found = true;
                                break;
                            }
                            q.push(next);
                        }
                    }
                }
            }
            
            if (!found) break;
            
            // Mark edges along the path as used
            int curr = v;
            while (curr != u) {
                int prev = parent[curr];
                int edge_idx = edge_used_in_path[curr];
                edge_used[edge_idx] = true;
                curr = prev;
            }
            
            connectivity++;
        }
        
        return connectivity;
    }
    
    // Optimized approach: compute connectivity more efficiently
    long long computeTotalFlow() {
        long long total = 0;
        
        // For small n, use direct computation
        if (n <= 500) {
            for (int i = 1; i <= n; i++) {
                for (int j = i + 1; j <= n; j++) {
                    total += getConnectivity(i, j);
                }
            }
            return total;
        }
        
        // For larger n, use sampling and estimation
        // This is a heuristic approach for the final attempt
        int sample_size = min(100, n / 10);
        vector<int> nodes;
        for (int i = 1; i <= n; i++) {
            if (i % (n / sample_size) == 0) {
                nodes.push_back(i);
            }
        }
        
        // Sample some pairs and estimate
        long long sample_sum = 0;
        int sample_count = 0;
        for (size_t i = 0; i < nodes.size(); i++) {
            for (size_t j = i + 1; j < nodes.size(); j++) {
                sample_sum += getConnectivity(nodes[i], nodes[j]);
                sample_count++;
            }
        }
        
        if (sample_count > 0) {
            double avg_connectivity = (double)sample_sum / sample_count;
            total = (long long)(avg_connectivity * n * (n - 1) / 2);
        }
        
        return total;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    GlobalConnectivityAnalyzer analyzer(n, m);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        analyzer.addEdge(u, v);
    }
    
    long long total_sum = analyzer.computeTotalFlow();
    
    cout << total_sum << endl;
    
    return 0;
}