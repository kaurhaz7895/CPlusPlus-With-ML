
# Linked List Practice in C++

Today's lesson builds upon the foundational understanding of **linked lists** by focusing on practical implementation exercises in **C++**. The goal is to strengthen implementation skills and expose common patterns that frequently appear in technical interviews.

---

# Problem 1: Reverse Linked List Traversal

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

# Problem 2: Calculate Linked List Length

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

# Summary

This lesson introduced two fundamental linked list exercises:

1. **Reverse Traversal using a Stack**
   - Maintains list structure
   - Uses stack LIFO behavior to reverse output

2. **Computing Linked List Length**
   - Uses simple traversal
   - Requires constant additional memory

Both problems reinforce a critical skill: **efficient traversal of linked data structures**, which is frequently tested in technical interviews and real system implementations.
