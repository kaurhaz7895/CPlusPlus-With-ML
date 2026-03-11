# C++ Linked List Guide — Concepts, Implementation, STL, and Use Cases

A **Linked List** is a dynamic data structure where elements are stored in **nodes connected using pointers**.  
Unlike arrays, nodes are **not stored in contiguous memory**.

Linked lists allow efficient insertion and deletion operations without shifting elements.

---

# Table of Contents

- [Overview](#overview)
- [Node Structure in C++](#node-structure-in-c)
- [Memory Representation](#memory-representation)
- [Types of Linked Lists](#types-of-linked-lists)
- [Why Linked Lists Are Used](#why-linked-lists-are-used)
- [Applications and Use Cases](#applications-and-use-cases)
- [Linked List Implementation Using User Defined Functions](#linked-list-implementation-using-user-defined-functions)
- [Operations on Linked Lists](#operations-on-linked-lists)
- [Linked List Using STL](#linked-list-using-stl)
- [Vector vs List Comparison](#vector-vs-list-comparison)
- [Complexity Analysis](#complexity-analysis)
- [Advantages](#advantages)
- [Limitations](#limitations)
- [Best Practices](#best-practices)
- [Practice Exercises](#practice-exercises)
- [Summary](#summary)
- [Frequently Asked Questions](#frequently-asked-questions)

---

# [Overview]

A linked list consists of nodes where each node contains:

1. Data
2. Pointer to the next node

Conceptually:

```
[Data | Next] → [Data | Next] → [Data | Next] → NULL
```

The last node points to `NULL`, indicating the end of the list.

---

# Node Structure in C++

```cpp
struct Node
{
    int data;
    Node* next;
};
```

Explanation:

- `data` → stores value
- `next` → stores address of next node
- last node → `next = NULL`

---

# Memory Representation

Example list:

```
10 → 20 → 30 → NULL
```

Visualization:

```
Head
 ↓
+------+     +------+     +------+
| 10   | --> | 20   | --> | 30   |
+------+     +------+     +------+
```

Nodes may exist anywhere in memory, but pointers connect them logically.

---

# Types of Linked Lists

## 1. Singly Linked List

Each node points to the next node.

```
Node → Node → Node → NULL
```

---

## 2. Doubly Linked List

Each node stores pointers to both previous and next nodes.

```
Prev ← Node → Next
```

---

## 3. Circular Linked List

The last node points back to the first node.

```
Node → Node → Node
  ↑              ↓
  ← ← ← ← ← ← ← ←
```

---

# Why Linked Lists Are Used

Arrays have limitations:

- fixed size
- expensive insertions
- memory resizing

Linked lists solve these problems because they:

- grow dynamically
- allow easy insertions
- require no element shifting

Example insertion:

```
Before:
10 → 20 → 30

Insert 25

After:
10 → 20 → 25 → 30
```

---

# Applications and Use Cases

## Dynamic Data Storage

Used when the number of elements is unknown.

Examples:

- real-time data streams
- dynamic memory systems

---

## Implementing Data Structures

Linked lists are used to implement:

- stacks
- queues
- hash tables
- adjacency lists in graphs

---

## Undo / Redo Systems

Editors store command history using linked structures.

---

## Operating Systems

Used for:

- process scheduling
- memory management
- resource tracking

---

# Linked List Implementation Using User Defined Functions

## Complete Example

```cpp
#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node* next;
};

Node* createNode(int value)
{
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void insertEnd(Node*& head, int value)
{
    Node* newNode = createNode(value);

    if(head == NULL)
    {
        head = newNode;
        return;
    }

    Node* temp = head;

    while(temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
}

void display(Node* head)
{
    Node* temp = head;

    while(temp != NULL)
    {
        cout << temp->data << " -> ";
        temp = temp->next;
    }

    cout << "NULL\n";
}

int main()
{
    Node* head = NULL;

    insertEnd(head,10);
    insertEnd(head,20);
    insertEnd(head,30);

    display(head);
}
```

Output:

```
10 -> 20 -> 30 -> NULL
```

---

# Operations on Linked Lists

## Insert at Beginning

```cpp
void insertBeginning(Node*& head, int value)
{
    Node* newNode = createNode(value);
    newNode->next = head;
    head = newNode;
}
```

---

## Search Element

```cpp
bool search(Node* head, int key)
{
    Node* temp = head;

    while(temp != NULL)
    {
        if(temp->data == key)
            return true;

        temp = temp->next;
    }

    return false;
}
```

---

## Delete First Node

```cpp
void deleteFirst(Node*& head)
{
    if(head == NULL)
        return;

    Node* temp = head;
    head = head->next;
    delete temp;
}
```

---

# Linked List Using STL

C++ provides a built-in linked list container.

```
std::list
```

Header:

```cpp
#include <list>
```

Example:

```cpp
#include <iostream>
#include <list>
using namespace std;

int main()
{
    list<int> numbers;

    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    for(int n : numbers)
        cout << n << " ";
}
```

Output:

```
10 20 30
```

---

# Vector vs List Comparison

| Feature | vector | list |
|------|------|------|
| Memory | contiguous | non-contiguous |
| Random access | O(1) | O(n) |
| Insert middle | O(n) | O(1) |
| Cache performance | high | lower |

---

# Complexity Analysis

| Operation | Time Complexity |
|-----------|----------------|
| Access element | O(n) |
| Insert at beginning | O(1) |
| Insert at end | O(n) |
| Delete node | O(n) |
| Search | O(n) |

If a pointer to the node is known, insertion and deletion become **O(1)**.

---

# Advantages

- Dynamic size
- Efficient insertion and deletion
- Flexible memory allocation
- Good for frequently changing data

---

# Limitations

## No Random Access

Arrays allow:

```
arr[i] → O(1)
```

Linked lists require traversal:

```
O(n)
```

---

## Extra Memory Usage

Each node stores additional pointer(s).

---

## Poor Cache Locality

Nodes are scattered in memory.

---

# Best Practices

- Prefer STL containers in production code.
- Avoid manual memory management where possible.
- Free dynamically allocated memory.
- Consider `vector` for most applications unless frequent insertions are required.

---

# Practice Exercises

1. Implement a function to **reverse a linked list**.
2. Count the number of nodes in the list.
3. Implement **delete by value**.
4. Detect a loop in a linked list.
5. Implement a **doubly linked list**.
6. Convert a linked list into an array.

---

# Summary

A linked list is a dynamic data structure consisting of nodes connected by pointers. Each node stores data and a reference to the next node. Unlike arrays, elements are not stored contiguously in memory, allowing flexible insertion and deletion operations. Linked lists are widely used in system programming, graph representations, memory management, and dynamic data processing. In C++, they can be implemented manually using pointers or by using the STL container `std::list`, which provides a robust and optimized implementation.

---

# Frequently Asked Questions

## Why use linked list instead of array?

When the data size changes frequently and many insertions or deletions occur.

---

## Are linked lists faster than arrays?

It depends on the operation.  
Arrays are faster for indexing.  
Linked lists are better for insertions and deletions.

---

## Why do linked lists use pointers?

Pointers connect nodes that exist in different memory locations.

---

## What is the difference between singly and doubly linked lists?

Singly linked list:

```
Node → Next
```

Doubly linked list:

```
Prev ← Node → Next
```

---

## Does STL `list` use singly or doubly linked lists?

`std::list` is implemented as a **doubly linked list**.

---

## When should `std::list` be avoided?

When frequent **random access** is needed. Containers like `vector` or `deque` are usually better.

---
