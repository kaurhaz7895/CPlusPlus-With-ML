
# Linked List Practice in C++

Today's lesson builds upon the foundational understanding of **linked lists** by focusing on practical implementation exercises in **C++**. The goal is to strengthen implementation skills and expose common patterns that frequently appear in technical interviews.

---
# Table of Contents

- [Problem 1 — Reverse Linked List Traversal](#problem-1--reverse-linked-list-traversal)
- [Problem 2 — Calculate Linked List Length](#problem-2--calculate-linked-list-length)
- [Additional Interview Problems](#additional-interview-problems)
  - [Problem 3 — Reverse Linked List In‑Place](#problem-3--reverse-linked-list-inplace)
  - [Problem 4 — Find the Middle Node](#problem-4--find-the-middle-node)
  - [Problem 5 — Detect Cycle in Linked List](#problem-5--detect-cycle-in-linked-list)
- [Key Takeaways](#key-takeaways)

# Problem 1 — Reverse Linked List Traversal

Consider a sequence of events stored in a linked list. Sometimes we need to review those events in reverse chronological order. The challenge here is to **traverse a singly linked list in reverse order without modifying the original structure**.

This situation appears in many real systems — for example reviewing logs, replaying transactions, or navigating a playlist backwards.

---

## Problem 1: Real‑World Actualization

A simple analogy is a **browser back button**. When browsing, pages are visited in forward order but revisited in reverse order. The most recently visited page must be accessed first.

This mirrors the requirement of **reverse traversal** of a linked list.

---

## Problem 1: Naive Approach

One potential solution is:

1. Traverse the original linked list.
2. Build a **new linked list**.
3. Insert each visited element at the **head of the new list**.

Although functional, this approach introduces:

- Extra memory allocation
- Additional pointer manipulation
- Unnecessary complexity

We can achieve the same goal more efficiently.

---

## Problem 1: Efficient Approach

A more suitable structure for this task is a **stack**.

A stack follows the **Last‑In‑First‑Out (LIFO)** principle.

**Process:**

1. Traverse the linked list.
2. Push each node value onto the stack.
3. Pop values from the stack to retrieve them in reverse order.

### Visualization

Imagine handling a **deck of cards**:

- Pick cards one by one from the deck (linked list)
- Place them on a pile (stack)
- When removing them from the pile, they naturally appear in **reverse order**

---

## Problem 1: C++ Implementation

```cpp
#include <iostream>
#include <stack>

struct ListNode {
    int value;
    ListNode* next;

    ListNode(int val) : value(val), next(nullptr) {}
};

void ReversePrintLinkedList(ListNode* head) {

    // Stack to store node values
    std::stack<int> stack;

    // Traverse the list and push values
    ListNode* currentNode = head;

    while (currentNode != nullptr) {
        stack.push(currentNode->value);
        currentNode = currentNode->next;
    }

    // Pop elements to print in reverse order
    while (!stack.empty()) {
        std::cout << stack.top() << std::endl;
        stack.pop();
    }
}
```

### Explanation

1. A `std::stack<int>` is used to temporarily store node values.
2. The linked list is traversed once.
3. Each node value is pushed onto the stack.
4. Because stacks follow **LIFO**, popping elements produces the reversed order.

**Time Complexity:** O(n)  
**Space Complexity:** O(n)

---

# Problem 2 — Calculate Linked List Length

The next problem is determining the **length of a linked list**.

Imagine a **conga line at a party**. To determine how many people are in the line, you start from the front and count each person until you reach the end.

The same logic applies to linked lists.

---

## Problem 2: Real‑World Actualization

Determining the size of a list is necessary in several real scenarios:

- Buffering data streams
- Scheduling batch operations
- Allocating memory resources
- Validating dataset sizes

Therefore, computing the length efficiently is important.

---

## Problem 2: Naive Approach

One could convert the linked list into another data structure such as an array and then retrieve the size from that structure.

However this introduces:

- Extra memory overhead
- Unnecessary copying of data

A direct traversal is far more efficient.

---

## Problem 2: Efficient Approach

The optimal strategy is straightforward:

1. Start at the **head node**
2. Initialize a **counter**
3. Traverse each node
4. Increment the counter until the end of the list

When the pointer becomes `nullptr`, the traversal stops and the counter contains the list length.

---

## Problem 2: C++ Implementation

```cpp
int GetLength(ListNode* head) {

    int length = 0;

    ListNode* currentNode = head;

    while (currentNode != nullptr) {
        length++;
        currentNode = currentNode->next;
    }

    return length;
}
```

### Explanation

- A counter `length` starts at zero.
- The list is traversed node by node.
- Each visited node increments the counter.
- When the pointer becomes `nullptr`, traversal stops.

**Time Complexity:** O(n)  
**Space Complexity:** O(1)

---
# Additional Interview Problems

These problems are extremely common in **data structure interviews** and strengthen understanding of pointer manipulation.

---

# Problem 3 — Reverse Linked List In‑Place

## Problem Statement

Reverse the linked list **without using extra memory structures**.

### Idea

Modify pointer directions while traversing the list.

### Algorithm

1. Maintain three pointers  
   - `prev`  
   - `current`  
   - `next`
2. Reverse the `next` pointer of each node
3. Move all pointers forward

### C++ Implementation

```cpp
ListNode* ReverseList(ListNode* head) {

    ListNode* prev = nullptr;
    ListNode* current = head;

    while (current != nullptr) {

        ListNode* next = current->next;

        current->next = prev;

        prev = current;
        current = next;
    }

    return prev;
}
```

**Time Complexity:** O(n)  
**Space Complexity:** O(1)

---

# Problem 4 — Find the Middle Node

## Problem Statement

Return the **middle node** of a linked list.

### Approach — Fast and Slow Pointer

Use two pointers:

- **slow pointer** moves one step
- **fast pointer** moves two steps

When fast reaches the end, slow reaches the middle.

### C++ Implementation

```cpp
ListNode* FindMiddle(ListNode* head) {

    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {

        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
```

**Time Complexity:** O(n)  
**Space Complexity:** O(1)

---

# Problem 5 — Detect Cycle in Linked List

## Problem Statement

Determine whether a linked list contains a **cycle**.

### Approach — Floyd’s Cycle Detection Algorithm

Also known as the **Tortoise and Hare Algorithm**.

Two pointers move at different speeds.

If they meet, a cycle exists.

### C++ Implementation

```cpp
bool HasCycle(ListNode* head) {

    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {

        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return true;
    }

    return false;
}
```

**Time Complexity:** O(n)  
**Space Complexity:** O(1)

---

# Key Takeaways

- Linked lists rely on **pointer traversal**
- Stack structures help with **reverse operations**
- Efficient algorithms minimize **extra memory usage**
- Fast‑slow pointer techniques are common in **interview problems**

---

# Summary

This lesson introduced two fundamental linked list exercises:

1. **Reverse Traversal using a Stack**
   - Maintains list structure
   - Uses stack LIFO behavior to reverse output

2. **Computing Linked List Length**
   - Uses simple traversal
   - Requires constant additional memory

Both problems reinforce a critical skill: **efficient traversal of linked data structures**, which is frequently tested in technical interviews and real system implementations.
