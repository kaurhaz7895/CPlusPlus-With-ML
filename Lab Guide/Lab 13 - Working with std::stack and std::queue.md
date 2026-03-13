
# Lab 13 — Working with STL Stack and Queue Containers in C++

## Objectives

After completing this lab, participants will be able to:

- Understand the concepts of **stack (LIFO)** and **queue (FIFO)**
- Use C++ STL containers `std::stack` and `std::queue`
- Perform common operations:
  - push
  - pop
  - access front/top
  - check size
  - check empty
- Implement practical examples using these containers
- Understand performance characteristics and real-world use cases

## Table of Contents
1. [Introduction](#introduction)
2. [Stack Overview](#stack-overview)
3. [Stack STL Methods](#stack-stl-methods)
4. [Stack Demo Program](#stack-demo-program)
5. [Stack Use Cases](#stack-use-cases)
6. [Queue Overview](#queue-overview)
7. [Queue STL Methods](#queue-stl-methods)
8. [Queue Demo Program](#queue-demo-program)
9. [Queue Use Cases](#queue-use-cases)
10. [Container Adaptors in STL](#container-adaptors-in-stl)
11. [Complexity Analysis](#complexity-analysis)
12. [Stack vs Queue Comparison](#stack-vs-queue-comparison)
13. [Mini Project Challenge](#mini-project-challenge)
14. [Project Solution](#project-solution)
15. [Key Takeaways](#key-takeaways)

---

# Introduction

Stacks and queues are fundamental data structures widely used in system design, algorithms, and real-world applications.

C++ provides ready-to-use implementations through the **Standard Template Library (STL)**.

These containers are known as **container adaptors** because they provide a restricted interface over other STL containers.

This lab explores:

- `std::stack`
- `std::queue`
- their operations
- real-world use cases
- implementation details

---

# Stack Overview

A **stack** follows the **Last-In-First-Out (LIFO)** principle.

Example push order:

```
10 → 20 → 30
```

Stack structure:

```
Top
 ↓
30
20
10
```

Removal order:

```
Pop → 30
Pop → 20
Pop → 10
```

---

# Stack STL Methods

Include library:

```cpp
#include <stack>
```

Declaration:

```cpp
std::stack<int> s;
```

| Method | Description |
|------|-------------|
| push() | Insert element |
| pop() | Remove top element |
| top() | Access top element |
| empty() | Check if stack empty |
| size() | Number of elements |
| emplace() | Construct element |
| swap() | Swap two stacks |

---

# Stack Demo Program

```cpp
#include <iostream>
#include <stack>

using namespace std;

int main()
{
    stack<int> s;

    // insert elements
    s.push(10);
    s.push(20);
    s.emplace(30);   // construct element directly

    cout << "Top element: " << s.top() << endl;
    cout << "Stack size: " << s.size() << endl;

    // remove element
    s.pop();

    cout << "Top after pop: " << s.top() << endl;

    cout << "Stack contents: ";

    while(!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }

    cout << endl;
}
```

Expected output:

```
Top element: 30
Stack size: 3
Top after pop: 20
Stack contents: 20 10
```

---

# Stack Use Cases

Stacks are widely used in:

### Function Call Stack
Programming languages manage function calls using stacks.

### Undo / Redo Systems
Text editors store history in stacks.

### Expression Evaluation
Compilers use stacks for parsing expressions.

### Depth First Search (DFS)
Graph traversal algorithms rely on stacks.

---

# Queue Overview

A **queue** follows the **First-In-First-Out (FIFO)** principle.

Example insertion order:

```
10 → 20 → 30
```

Queue structure:

```
Front → 10 20 30 ← Rear
```

Removal order:

```
Pop → 10
Pop → 20
Pop → 30
```

---

# Queue STL Methods

Include:

```cpp
#include <queue>
```

Declaration:

```cpp
std::queue<int> q;
```

| Method | Description |
|------|-------------|
| push() | Insert element |
| pop() | Remove front element |
| front() | Access first element |
| back() | Access last element |
| empty() | Check if empty |
| size() | Number of elements |
| emplace() | Construct element |
| swap() | Swap queues |

---

# Queue Demo Program

```cpp
#include <iostream>
#include <queue>

using namespace std;

int main()
{
    queue<int> q;

    q.push(10);
    q.push(20);
    q.emplace(30);

    cout << "Front element: " << q.front() << endl;
    cout << "Rear element: " << q.back() << endl;

    q.pop();

    cout << "Front after pop: " << q.front() << endl;

    cout << "Queue contents: ";

    while(!q.empty())
    {
        cout << q.front() << " ";
        q.pop();
    }

    cout << endl;
}
```

Expected output:

```
Front element: 10
Rear element: 30
Front after pop: 20
Queue contents: 20 30
```

---

# Queue Use Cases

Queues are commonly used in:

### CPU Scheduling
Operating systems schedule processes using queues.

### Print Queues
Printers process jobs in order.

### Breadth First Search (BFS)
Graph traversal uses queues.

### Task Scheduling
Job processing systems rely on queues.

---

# Container Adaptors in STL


In the C++ Standard Template Library (STL), **`std::stack`** and **`std::queue`** are not full containers.

They are known as **container adaptors**.

A **container adaptor** is a wrapper that uses another STL container internally but exposes a **restricted interface** to enforce specific data structure behavior.

---

# Definition

A **container adaptor** is:

> A wrapper around another STL container that restricts the available operations to enforce a particular data structure behavior.

For example:

- A **stack** must follow **LIFO (Last-In-First-Out)**
- A **queue** must follow **FIFO (First-In-First-Out)**

To ensure this behavior, the adaptor only exposes operations that maintain the structure rules.

---

# Default Underlying Containers

Container adaptors internally use other STL containers to store data.

| Adaptor | Default Container |
|-------|------------------|
| `std::stack` | `std::deque` |
| `std::queue` | `std::deque` |
| `std::priority_queue` | `std::vector` |

Example:

```cpp
std::stack<int> s;
```

Internally this is equivalent to:

```cpp
std::stack<int, std::deque<int>> s;
```

---

# Why Use Container Adaptors

Container adaptors ensure that a data structure behaves correctly.

For example, a stack should only allow operations related to the **top element**.

Allowed operations:

```
push()
pop()
top()
```

Disallowed operations:

```
random access
inserting in the middle
direct indexing
```

This prevents users from accidentally breaking the **LIFO behavior** of a stack.

---

# Visual Explanation

Internal container:

```
deque
--------------------
10 20 30 40
```

Stack adaptor view:

```
Top
 ↓
40
30
20
10
```

The internal structure exists, but the user only interacts through **stack operations**.

---

# Customizing the Underlying Container

You can specify a different container if needed.

Example using `vector` as the underlying container:

```cpp
std::stack<int, std::vector<int>> s;
```

Example using `list` for a queue:

```cpp
std::queue<int, std::list<int>> q;
```

This flexibility allows developers to optimize for memory usage or performance.

---

# Real-World Analogy

A **container adaptor** is similar to a vending machine interface.

Inside the machine there may be many items stored, but the user can only perform limited operations:

```
insert money
press button
receive item
```

The user cannot directly access the internal storage.

Similarly, container adaptors restrict access to ensure correct behavior.

---

---

# Complexity Analysis

| Operation | Stack | Queue |
|---------|------|------|
push | O(1) | O(1) |
pop | O(1) | O(1) |
top/front | O(1) | O(1) |
size | O(1) | O(1) |

These operations are constant time.

---

# Stack vs Queue Comparison

| Feature | Stack | Queue |
|------|------|------|
Principle | LIFO | FIFO |
Insertion | push | push |
Removal | pop(top) | pop(front) |
Access | top() | front() |
Real world example | Undo operations | Task scheduling |

---

# Mini Project Challenge

## Browser Back / Forward Navigation

Design a browser navigation system using stacks.

Requirements:

- `visit(url)`
- `back()`
- `forward()`

Hint:

```
Use two stacks
```

---

# Project Solution

```cpp
#include <iostream>
#include <stack>

using namespace std;

class BrowserHistory
{
    stack<string> backStack;
    stack<string> forwardStack;
    string current;

public:

    BrowserHistory(string homepage)
    {
        current = homepage;
    }

    void visit(string url)
    {
        backStack.push(current);
        current = url;

        while(!forwardStack.empty())
            forwardStack.pop();
    }

    void back()
    {
        if(backStack.empty())
            return;

        forwardStack.push(current);
        current = backStack.top();
        backStack.pop();
    }

    void forward()
    {
        if(forwardStack.empty())
            return;

        backStack.push(current);
        current = forwardStack.top();
        forwardStack.pop();
    }

    void show()
    {
        cout << "Current page: " << current << endl;
    }
};

int main()
{
    BrowserHistory browser("google.com");

    browser.visit("github.com");
    browser.visit("stackoverflow.com");

    browser.back();
    browser.show();

    browser.forward();
    browser.show();
}
```

---

# Key Takeaways

- `std::stack` implements **Last-In-First-Out (LIFO)**
- `std::queue` implements **First-In-First-Out (FIFO)**
- Both are **container adaptors**
- Operations run in **constant time O(1)**
- Widely used in algorithms, compilers, operating systems, and distributed systems
