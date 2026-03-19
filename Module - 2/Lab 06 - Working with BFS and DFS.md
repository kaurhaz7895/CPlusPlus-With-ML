# Lab 6 - Graph Traversal (using C++)

## 🎯 Objective
To understand and implement:
- Breadth First Search (BFS)
- Depth First Search (DFS)

---

# 📘 1. Breadth First Search (BFS)

## Definition
BFS explores nodes **level by level** using a **queue**.

---

## Algorithm
1. Start from a source node
2. Mark it visited
3. Push into queue
4. While queue not empty:
   - Pop node
   - Visit it
   - Push unvisited neighbors

---

## ✅ Complete C++ Program (BFS)

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(int start, vector<vector<int>>& adj, int n) {
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        cout << node << " ";

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    int n = 6;
    vector<vector<int>> adj(n);

    adj[0] = {1, 3, 4};
    adj[1] = {0, 2};
    adj[2] = {1};
    adj[3] = {0, 5};
    adj[4] = {0, 5};
    adj[5] = {3, 4};

    cout << "BFS Traversal:\n";
    bfs(0, adj, n);

    return 0;
}
```

---

## Use Cases
- Shortest path (unweighted graph)
- Level order traversal
- Network broadcasting

---

# 📘 2. Depth First Search (DFS)

## Definition
DFS explores nodes **deep first**, then backtracks.

---

## Algorithm (Recursive)
1. Visit node
2. Mark visited
3. Recursively visit neighbors

---

## ✅ Complete C++ Program (DFS - Recursive)

```cpp
#include <iostream>
#include <vector>
using namespace std;

void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited) {
    visited[node] = true;
    cout << node << " ";

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, adj, visited);
        }
    }
}

int main() {
    int n = 6;
    vector<vector<int>> adj(n);

    adj[0] = {1, 3, 4};
    adj[1] = {0, 2};
    adj[2] = {1};
    adj[3] = {0, 5};
    adj[4] = {0, 5};
    adj[5] = {3, 4};

    vector<bool> visited(n, false);

    cout << "DFS Traversal (Recursive):\n";
    dfs(0, adj, visited);

    return 0;
}
```

---

## ✅ Complete C++ Program (DFS - Iterative)

```cpp
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

void dfs_iterative(int start, vector<vector<int>>& adj, int n) {
    vector<bool> visited(n, false);
    stack<int> st;

    st.push(start);

    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";

            for (int i = adj[node].size() - 1; i >= 0; i--) {
                int neighbor = adj[node][i];
                if (!visited[neighbor]) {
                    st.push(neighbor);
                }
            }
        }
    }
}

int main() {
    int n = 6;
    vector<vector<int>> adj(n);

    adj[0] = {1, 3, 4};
    adj[1] = {0, 2};
    adj[2] = {1};
    adj[3] = {0, 5};
    adj[4] = {0, 5};
    adj[5] = {3, 4};

    cout << "DFS Traversal (Iterative):\n";
    dfs_iterative(0, adj, n);

    return 0;
}
```

---

## Use Cases
- Cycle detection
- Topological sorting
- Backtracking problems

---

# ⚖️ 3. Difference Between BFS and DFS

| Feature | BFS | DFS |
|--------|-----|-----|
| Strategy | Level-wise | Depth-wise |
| Data Structure | Queue | Stack/Recursion |
| Shortest Path | Yes | No |
| Memory Usage | Higher | Lower |
| Use Case | Shortest path | Deep traversal |

---

## 🎯 Conclusion

- Use **BFS** when shortest path is required  
- Use **DFS** for deep exploration and recursion-based problems  

