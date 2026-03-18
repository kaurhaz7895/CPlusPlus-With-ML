# Graphs Data Structure in C++

## 📌 Objective
- Understand what graphs are
- Learn real-world use cases
- Study graph representations
- Implement graphs in C++ using:
  - Adjacency Matrix
  - Adjacency List

---

## 1. Introduction to Graphs

A **Graph** is a non-linear data structure consisting of:
- **Vertices (Nodes)** → represent entities
- **Edges** → represent relationships between nodes

A graph is defined as:
G = (V, E)

Where:
- V = set of vertices
- E = set of edges

---

## 2. Types of Graphs

- Undirected Graph → edges have no direction  
- Directed Graph (Digraph) → edges have direction  
- Weighted Graph → edges have weights  
- Unweighted Graph → edges have no weights  

---

## 3. Real-World Use Cases

- Social Networks (friends connection)
- Maps & Navigation (Google Maps)
- Computer Networks (routing)
- Recommendation Systems
- Web Crawlers (page linking)

---

## 4. Graph Representations

There are two primary ways to represent graphs:

---

## 4.1 Adjacency Matrix

### Concept
A 2D matrix where:
- matrix[i][j] = 1 → edge exists
- matrix[i][j] = 0 → no edge

### Example

   0 1 2
0 [0 1 1]
1 [1 0 0]
2 [1 0 0]

---

### C++ Implementation

```cpp
#include <iostream>
using namespace std;

#define MAX 100

class Graph {
    int adjMatrix[MAX][MAX];
    int vertices;

public:
    Graph(int v) {
        vertices = v;
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                adjMatrix[i][j] = 0;
            }
        }
    }

    void addEdge(int u, int v) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
    }

    void display() {
        cout << "Adjacency Matrix:\n";
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    Graph g(3);

    g.addEdge(0, 1);
    g.addEdge(0, 2);

    g.display();

    return 0;
}
```

---

## 4.2 Adjacency List

### Concept
Each vertex stores a list of its neighbors.

### Example

0 → 1 → 2
1 → 0
2 → 0

---

### C++ Implementation

```cpp
#include <iostream>
#include <vector>
using namespace std;

class Graph {
    int vertices;
    vector<vector<int>> adjList;

public:
    Graph(int v) {
        vertices = v;
        adjList.resize(v);
    }

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void display() {
        cout << "Adjacency List:\n";
        for (int i = 0; i < vertices; i++) {
            cout << i << " -> ";
            for (int neighbor : adjList[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    Graph g(3);

    g.addEdge(0, 1);
    g.addEdge(0, 2);

    g.display();

    return 0;
}
```

---

## 5. Comparison

| Feature              | Adjacency Matrix | Adjacency List |
|---------------------|-----------------|---------------|
| Space Complexity     | O(V²)           | O(V + E)      |
| Edge Lookup          | Fast (O(1))     | Slower        |
| Best For             | Dense Graphs    | Sparse Graphs |

---

## 6. Summary

- Graphs represent relationships between entities
- Matrix → simple but memory-heavy
- List → efficient and commonly used

---

## 7. Practice Tasks

1. Modify code for directed graph
2. Add weighted edges
3. Implement DFS
4. Implement BFS

---

## ✅ End of Lab
