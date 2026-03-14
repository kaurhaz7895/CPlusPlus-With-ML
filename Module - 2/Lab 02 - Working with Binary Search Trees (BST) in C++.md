# Lab 02 -- Working with Binary Search Trees (BST) in C++

## Table of Contents

1. [Objective](#1-objective)
2. [Binary Search Tree Overview](#2-binary-search-tree-overview)
3. [BST Properties](#3-bst-properties)
4. [Traversal Concepts](#4-traversal-concepts)
5. [Traversal Types](#5-traversal-types)
6. [Industry Use Cases](#6-industry-use-cases)
7. [BST Demonstration Program](#7-bst-demonstration-program)
8. [Expected Output](#8-expected-output)
9. [Visual Understanding of Traversals](#9-visual-understanding-of-traversals)
10. [Exercises](#10-exercises)
11. [Challenge Problems](#11-challenge-problems)
12. [Key Takeaways](#12-key-takeaways)


------------------------------------------------------------------------

# 1. Objective

In this lab, you will explore the **Binary Search Tree (BST)** data
structure and learn how different traversal techniques are used in real
systems.

By completing this lab, you will:

-   Understand the BST ordering rule
-   Implement insertion, search, and deletion
-   Implement four traversal techniques
-   Understand practical use cases for each traversal
-   Observe traversal behavior through program output

------------------------------------------------------------------------

# 2. Binary Search Tree Overview

A **Binary Search Tree (BST)** is a hierarchical data structure where
each node follows the rule:

Left Subtree \< Root \< Right Subtree

Example BST:

        50
       /  \
     30    70
    / \   / \

20 40 60 80

Advantages:

-   Faster search operations
-   Sorted data retrieval using inorder traversal
-   Efficient insert and delete operations

------------------------------------------------------------------------

# 3. BST Properties

  Property              Description
  --------------------- ---------------------------------------
  Ordering Rule         Left subtree values \< root
  Recursive Structure   Each subtree is itself a BST
  Sorted Traversal      Inorder traversal gives sorted values
  Search Efficiency     Average complexity O(log n)

Worst-case scenario occurs when the tree becomes skewed:

O(n)

------------------------------------------------------------------------

# 4. Traversal Concepts

Traversal means **visiting every node in a tree exactly once**.

Four common traversal methods exist:

  Traversal     Order
  ------------- --------------------------
  Preorder      Root → Left → Right
  Inorder       Left → Root → Right
  Postorder     Left → Right → Root
  Level Order   Level-by-level traversal

The first three belong to **Depth-First Search (DFS)** and the last is
**Breadth-First Search (BFS)**.

------------------------------------------------------------------------

# 5. Traversal Types

## Inorder Traversal

Left → Root → Right

Important property:

When applied to a BST, it produces **sorted output**.

Example:

Tree:

        4
       / \
      2   6
     / \ / \
    1  3 5  7

Output:

1 2 3 4 5 6 7

------------------------------------------------------------------------

## Preorder Traversal

Root → Left → Right

Used when parent nodes must be processed before children.

Applications:

-   Tree serialization
-   Copying tree structures

------------------------------------------------------------------------

## Postorder Traversal

Left → Right → Root

Used when children must be processed before the parent.

Applications:

-   Deleting tree structures
-   Expression tree evaluation

------------------------------------------------------------------------

## Level Order Traversal

Level-by-level traversal of the tree.

Also called **Breadth First Search (BFS)**.

Uses a **queue**.

Example traversal:

1 2 3 4 5 6

------------------------------------------------------------------------

# 6. Industry Use Cases

Traversal methods appear in many systems.

Inorder:

-   Database indexing
-   Sorted data reports
-   Ordered containers

Preorder:

-   Configuration tree serialization
-   Exporting hierarchical data

Postorder:

-   Safe memory cleanup
-   Dependency resolution

Level-order:

-   Network routing algorithms
-   Shortest path problems
-   Job scheduling systems

------------------------------------------------------------------------

# 7. BST Demonstration Program

``` cpp
#include <iostream>
#include <queue>
using namespace std;

struct Node
{
    int data;
    Node* left;
    Node* right;

    Node(int value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

class BST
{
private:

    Node* root;   // Pointer to the root node of the BST

    // Recursive helper function to insert a new value in the BST
    Node* insertNode(Node* node, int value)
    {
        // If we reach a null position, create a new node
        if(node == nullptr)
            return new Node(value);

        // Follow BST property: smaller values go to the left subtree
        if(value < node->data)
            node->left = insertNode(node->left, value);

        // Larger values go to the right subtree
        else
            node->right = insertNode(node->right, value);

        // Return the unchanged node pointer
        return node;
    }

    // Recursive helper function to search for a key
    bool searchNode(Node* node, int key)
    {
        // If node is null, value does not exist in the tree
        if(node == nullptr)
            return false;

        // If value matches the current node, search successful
        if(node->data == key)
            return true;

        // Continue searching in the appropriate subtree
        if(key < node->data)
            return searchNode(node->left, key);
        else
            return searchNode(node->right, key);
    }

    // Inorder traversal: Left → Root → Right
    // Produces sorted output in a BST
    void inorder(Node* node)
    {
        if(node == nullptr) return;

        inorder(node->left);        // Visit left subtree
        cout << node->data << " ";  // Process current node
        inorder(node->right);       // Visit right subtree
    }

    // Preorder traversal: Root → Left → Right
    // Useful for copying or serializing tree structure
    void preorder(Node* node)
    {
        if(node == nullptr) return;

        cout << node->data << " ";  // Process current node
        preorder(node->left);       // Traverse left subtree
        preorder(node->right);      // Traverse right subtree
    }

    // Postorder traversal: Left → Right → Root
    // Useful for deleting tree structures safely
    void postorder(Node* node)
    {
        if(node == nullptr) return;

        postorder(node->left);      // Traverse left subtree
        postorder(node->right);     // Traverse right subtree
        cout << node->data << " ";  // Process current node
    }

public:

    // Constructor initializes an empty tree
    BST()
    {
        root = nullptr;
    }

    // Public insert function (calls recursive helper)
    void insert(int value)
    {
        root = insertNode(root, value);
    }

    // Public search function
    bool search(int key)
    {
        return searchNode(root, key);
    }

    // Public traversal wrappers
    void inorderTraversal()
    {
        inorder(root);
        cout << endl;
    }

    void preorderTraversal()
    {
        preorder(root);
        cout << endl;
    }

    void postorderTraversal()
    {
        postorder(root);
        cout << endl;
    }

    // Level-order traversal (Breadth-First Search)
    // Uses a queue to visit nodes level by level
    void levelOrderTraversal()
    {
        if(root == nullptr) return;

        queue<Node*> q;   // Queue stores nodes to visit
        q.push(root);     // Start with root node

        while(!q.empty())
        {
            Node* temp = q.front(); // Get next node
            q.pop();

            cout << temp->data << " "; // Process node

            // Add children to queue for future processing
            if(temp->left)
                q.push(temp->left);

            if(temp->right)
                q.push(temp->right);
        }

        cout << endl;
    }
};

int main()
{
    BST tree;

    // Insert elements into BST
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    // Demonstrate different traversals
    cout << "Inorder Traversal: ";
    tree.inorderTraversal();

    cout << "Preorder Traversal: ";
    tree.preorderTraversal();

    cout << "Postorder Traversal: ";
    tree.postorderTraversal();

    cout << "Level Order Traversal: ";
    tree.levelOrderTraversal();

    // Search for a value in the BST
    cout << "\nSearching 40: ";
    if(tree.search(40))
        cout << "Found\n";
    else
        cout << "Not Found\n";

    return 0;
}
```

------------------------------------------------------------------------

# 8. Expected Output

Inorder Traversal: 20 30 40 50 60 70 80\
Preorder Traversal: 50 30 20 40 70 60 80\
Postorder Traversal: 20 40 30 60 80 70 50\
Level Order Traversal: 50 30 70 20 40 60 80

Searching 40: Found

------------------------------------------------------------------------

# 9. Visual Understanding of Traversals

Example Tree:

        1
       / \
      2   3
     / \   \
    4   5   6

Traversal Results:

Inorder:\
4 2 5 1 3 6

Preorder:\
1 2 4 5 3 6

Postorder:\
4 5 2 6 3 1

Level Order:\
1 2 3 4 5 6

------------------------------------------------------------------------

# 10. Exercises

1.  Modify the program to **delete a node from the BST**.
2.  Insert additional nodes and observe traversal outputs.
3.  Print the **height of the tree**.
4.  Implement **iterative inorder traversal using a stack**.

------------------------------------------------------------------------

# 11. Challenge Problems

## Problem with Solution
Given a binary search tree and a "target" value, search the tree to see if it contains the target. The basic pattern of the **lookup()** code occurs in many recursive tree algorithms: deal with the base case where the tree is empty, deal with the current node, and then use recursion to deal with the subtrees. If the tree is a binary search tree, there is often some sort of less-than test on the node to decide if the recursion should go left or right.

``` cpp
#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Recursive lookup function
bool lookup(Node* root, int target) {
    if (root == nullptr) {
        return false;  // Base case: empty tree
    }
    if (root->data == target) {
        return true;   // Found the target
    }
    else if (target < root->data) {
        return lookup(root->left, target);  // Search left subtree
    }
    else {
        return lookup(root->right, target); // Search right subtree
    }
}

// Helper to insert nodes into BST
Node* insert(Node* root, int val) {
    if (root == nullptr) return new Node(val);
    if (val < root->data) root->left = insert(root->left, val);
    else root->right = insert(root->right, val);
    return root;
}

int main() {
    Node* root = nullptr;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 70);
    insert(root, 20);
    insert(root, 40);
    insert(root, 60);
    insert(root, 80);

    cout << (lookup(root, 40) ? "Found" : "Not Found") << endl;
    cout << (lookup(root, 25) ? "Found" : "Not Found") << endl;

    return 0;
}

```
## Problems without Solution
1.  Write a function to find the **kth smallest element** in a BST.
2.  Write a function to **validate if a binary tree is a BST**.
3.  Implement **level-by-level printing of the tree**.

------------------------------------------------------------------------

# 12. Key Takeaways

After completing this lab, you should understand:

-   BST structure and ordering property
-   Differences between DFS and BFS traversals
-   Practical applications of traversal algorithms
-   Implementation of recursive tree algorithms

Binary Search Trees are foundational structures used in:

-   Database indexing
-   Search engines
-   Memory management systems
-   Graph algorithms

------------------------------------------------------------------------

End of Lab 02
