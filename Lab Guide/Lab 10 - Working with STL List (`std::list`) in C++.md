
# Lab 10 — STL List (`std::list`) in C++

## Table of Contents
- [1. Introduction](#1-introduction)
- [2. Learning Objectives](#2-learning-objectives)
- [3. Definition of std::list](#3-definition-of-stdlist)
- [4. Internal Structure](#4-internal-structure)
- [5. Manual Linked List Implementation](#5-manual-linked-list-implementation)
- [6. STL List Implementation](#6-stl-list-implementation)
- [7. Comparison: STL List vs Manual Linked List](#7-comparison-stl-list-vs-manual-linked-list)
- [8. Advantages](#8-advantages)
- [9. Drawbacks](#9-drawbacks)
- [10. Real World Use Cases](#10-real-world-use-cases)
- [11. Demonstration Program](#11-demonstration-program)
- [12. Program Output](#12-program-output)
- [13. Step-by-Step Explanation](#13-step-by-step-explanation)
- [14. Complexity Analysis](#14-complexity-analysis)
- [15. Conclusion](#15-conclusion)

---

# 1. Introduction

In earlier labs we implemented linked lists manually using **classes and pointers**.  
While this helps understand how memory and nodes work internally, modern C++ programs typically rely on the **Standard Template Library (STL)**.

STL provides containers such as:

- vector
- list
- deque
- set
- map

In this lab we focus on **`std::list`**, which internally implements a **doubly linked list**.

---

# 2. Learning Objectives

After completing this lab you should be able to:

- Understand how STL simplifies linked list usage
- Use the major operations supported by `std::list`
- Compare STL containers with manual implementations
- Identify practical scenarios where linked lists are useful

---

# 3. Definition of std::list

`std::list` is a **sequence container in C++ STL** that stores elements using a **doubly linked list structure**.

Each node contains:

- data
- pointer to previous node
- pointer to next node

Example declaration:

```cpp
#include <list>

std::list<int> numbers;
```

---

# 4. Internal Structure

Conceptually a doubly linked list looks like:

```
NULL <- [10] <-> [20] <-> [30] <-> [40] -> NULL
```

Each node:

```
struct Node
{
    int data;
    Node* prev;
    Node* next;
};
```

Because two pointers exist, traversal is possible **in both directions**.

---

# 5. Manual Linked List Implementation

```cpp
class Node
{
public:
    int data;
    Node* next;

    Node(int val)
    {
        data = val;
        next = nullptr;
    }
};

class LinkedList
{
public:
    Node* head;

    LinkedList()
    {
        head = nullptr;
    }

    void insertFront(int value)
    {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }
};
```

Problems with manual implementation:

- manual memory management
- pointer manipulation errors
- longer code

---

# 6. STL List Implementation

With STL the same concept becomes:

```cpp
#include <list>

std::list<int> numbers;
```

The library internally manages nodes and pointers.

---

# 7. Comparison: STL List vs Manual Linked List

| Feature | Manual Linked List | STL List |
|-------|------------------|---------|
| Code Length | Large | Very Small |
| Memory Management | Manual | Automatic |
| Reliability | Error Prone | Safe |
| Development Time | Long | Short |

---

# 8. Advantages

- Efficient insertion and deletion
- Dynamic size
- Bidirectional traversal
- Stable iterators

---

# 9. Drawbacks

- Higher memory usage (extra pointers)
- No random access
- Less cache friendly than arrays

---

# 10. Real World Use Cases

### Browser Navigation

```
Page1 <-> Page2 <-> Page3
```

### Undo/Redo in Editors

Used in:

- VS Code
- MS Word
- Photoshop

### Music Playlist Navigation

```
Song1 <-> Song2 <-> Song3
```

### LRU Cache

Combination of:

```
Hash Map + Doubly Linked List
```

---

# 11. Demonstration Program

This example demonstrates **most commonly used `std::list` functions**.

```cpp
#include <iostream>
#include <list>

using namespace std;

int main()
{

    // ------------------------------------------------
    // Create list with initial values
    // ------------------------------------------------
    list<int> numbers = {10,20,30};

    // Insert element at beginning
    numbers.push_front(5);

    // Insert element at end
    numbers.push_back(40);

    // Display list
    cout << "Initial list: ";
    for(int x : numbers)
        cout << x << " ";
    cout << endl;

    // Access first element
    cout << "Front element: " << numbers.front() << endl;

    // Access last element
    cout << "Back element: " << numbers.back() << endl;

    // ------------------------------------------------
    // Insert element using iterator
    // ------------------------------------------------
    auto it = numbers.begin();

    // Insert value before first element
    numbers.insert(it, 1);

    // ------------------------------------------------
    // Remove elements
    // ------------------------------------------------

    numbers.pop_front();   // remove first node

    numbers.pop_back();    // remove last node

    // ------------------------------------------------
    // Resize list
    // ------------------------------------------------
    numbers.resize(6,100); // expand list and fill new nodes with value 100

    // ------------------------------------------------
    // Sorting
    // ------------------------------------------------
    numbers.sort();

    // ------------------------------------------------
    // Reverse list order
    // ------------------------------------------------
    numbers.reverse();

    // ------------------------------------------------
    // Remove consecutive duplicates
    // ------------------------------------------------
    numbers.unique();

    // ------------------------------------------------
    // Display final list
    // ------------------------------------------------
    cout << "Final list: ";
    for(int x : numbers)
        cout << x << " ";

    cout << endl;

    return 0;
}
```

---

# 12. Program Output

```
Initial list: 5 10 20 30 40
Front element: 5
Back element: 40
Final list: 100 100 30 20 10
```

---

# 13. Step-by-Step Explanation

Initial list:

```
10 → 20 → 30
```

After push operations:

```
5 → 10 → 20 → 30 → 40
```

After insert:

```
1 → 5 → 10 → 20 → 30 → 40
```

After deletions:

```
5 → 10 → 20 → 30
```

After resize:

```
5 → 10 → 20 → 30 → 100 → 100
```

---

# 14. Complexity Analysis

| Operation | Complexity |
|----------|-----------|
| Insert | O(1) |
| Delete | O(1) |
| Traversal | O(n) |
| Random Access | Not Supported |

---

# 15. Conclusion

`std::list` is a powerful STL container implementing a **doubly linked list**.

It is most useful when:

- frequent insertions and deletions occur
- bidirectional traversal is required

For workloads requiring **fast indexing**, containers like `vector` are usually preferred.
