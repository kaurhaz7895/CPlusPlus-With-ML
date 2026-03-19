# Lab 7: Minimum Spanning Tree Algorithms (Kruskal & Prim) – C++

## 🎯 Objective
To understand and implement:
- Minimum Spanning Tree (MST)
- Kruskal’s Algorithm
- Prim’s Algorithm
- Role of Disjoint Set (Union-Find)
- Practical differences and usage in real systems

---

# 1. Minimum Spanning Tree (MST)

## Definition
A Minimum Spanning Tree is a subset of edges of a connected, weighted graph such that:
- All vertices are connected
- No cycles exist
- Total edge weight is minimum
- Number of edges = n - 1

---

# 2. Disjoint Set (Union-Find)

## Why Needed?
In Kruskal’s algorithm, we must detect cycles efficiently.

Instead of checking the entire graph repeatedly:
👉 We use Disjoint Set to track connected components.

---

## Key Operations

### find(x)
Returns the parent (representative) of the set to which x belongs.

### union(a, b)
Merges two different sets into one.

---

## Conceptual Insight

- Each node starts as its own component
- When an edge is added → components merge
- If two nodes already belong to same component → adding edge creates cycle

---

# 3. Kruskal’s Algorithm

## Definition
Builds MST by selecting smallest edges first while avoiding cycles.

---

## Algorithm Steps

1. Sort all edges by weight
2. Initialize Disjoint Set
3. For each edge (u, v):
   - If find(u) != find(v):
       - Include edge in MST
       - union(u, v)
4. Stop when n-1 edges are selected

---

## Full C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, w;
};

class DSU {
    vector<int> parent;

public:
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        parent[find(a)] = find(b);
    }
};

bool cmp(Edge a, Edge b) {
    return a.w < b.w;
}

int main() {
    int n = 6;

    vector<Edge> edges = {
        {0,1,2},{0,3,1},{1,3,3},{1,2,4},
        {3,2,3},{3,5,5},{3,4,7},{2,5,10}
    };

    sort(edges.begin(), edges.end(), cmp);

    DSU ds(n);
    int total = 0;

    cout << "Kruskal MST:\n";

    for (auto e : edges) {
        if (ds.find(e.u) != ds.find(e.v)) {
            cout << e.u << " - " << e.v << " : " << e.w << endl;
            total += e.w;
            ds.unite(e.u, e.v);
        }
    }

    cout << "Total Weight: " << total << endl;
}
```

---

## Use Cases
- Network design (LAN, electrical grids)
- Clustering (machine learning)
- Sparse graph optimization

---

# 4. Prim’s Algorithm

## Definition
Builds MST by expanding from a starting node using minimum edge selection.

---

## Algorithm Steps

1. Start from any node
2. Mark node as visited
3. Push all adjacent edges into min heap
4. Select smallest edge connecting new node
5. Repeat until all nodes are included

---

## Full C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int,int> pii;

int main() {
    int n = 6;

    vector<vector<pii>> adj(n);

    adj[0] = {{1,2},{3,1}};
    adj[1] = {{0,2},{3,3},{2,4}};
    adj[2] = {{1,4},{3,3},{5,10}};
    adj[3] = {{0,1},{1,3},{2,3},{5,5},{4,7}};
    adj[4] = {{3,7}};
    adj[5] = {{3,5},{2,10}};

    vector<bool> visited(n, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    pq.push({0,0});
    int total = 0;

    cout << "Prim MST:\n";

    while (!pq.empty()) {
        auto [w, node] = pq.top();
        pq.pop();

        if (visited[node]) continue;

        visited[node] = true;
        total += w;

        for (auto [next, weight] : adj[node]) {
            if (!visited[next]) {
                pq.push({weight, next});
            }
        }
    }

    cout << "Total Weight: " << total << endl;
}
```

---

## Use Cases
- Dense graphs
- Network routing
- Real-time path optimization

---

# 5. Kruskal vs Prim 

| Feature | Kruskal | Prim |
|--------|--------|------|
| Approach | Edge-based | Node-based |
| Data Structure | Sorting + Disjoint Set | Priority Queue |
| Cycle Detection | Required (Union-Find) | Not required |
| Graph Type | Sparse | Dense |
| Complexity | O(E log E) | O(E log V) |

---

## 🎯 Conclusion

- Kruskal = global edge selection + cycle detection  
- Prim = local expansion + priority queue  
- Disjoint Set is essential only for Kruskal  

