# Lab 01 -- Working with Binary Trees in C++

## Table of Contents

1.  [Objective](#objective)
2.  [What is a Binary Tree?](#what-is-a-binary-tree)
3.  [Visual Representation of a Binary Tree](#visual-representation-of-a-binary-tree)
4.  [Terminology](#terminology)
5.  [Types of Binary Trees](#types-of-binary-trees)
6.  [Code Example 1 – Creating a Basic Binary Tree](#code-example-1-creating-a-basic-binary-tree)
7.  [Code Example 2 – Binary Tree with Operations](#code-example-2-binary-tree-with-operations)
8.  [Traversal Explanation](#traversal-explanation)
9.  [Sample Output](#sample-output)
10. [Key Takeaways](#key-takeaways)

------------------------------------------------------------------------

# Objective

This lab introduces working professionals to the **foundations of Binary
Trees in C++**.

You will learn:

-   What a binary tree is
-   How nodes are structured in memory
-   Different types of binary trees
-   How to implement a binary tree in C++
-   How traversal algorithms work

------------------------------------------------------------------------

# What is a Binary Tree?

A **Binary Tree** is a hierarchical data structure in which each node
can have **at most two children**.

These children are called:

-   **Left Child**
-   **Right Child**

Each node contains:

-   Data
-   Pointer to the left child
-   Pointer to the right child

Binary trees are used in:

-   Searching algorithms
-   Expression trees
-   File systems
-   Database indexing
-   Compiler design

------------------------------------------------------------------------

# Visual Representation of a Binary Tree

Example:

            10
           /  \
         20    30
        /  \
      40   50

Explanation:

-   **10** → Root node\
-   **20, 30** → Children of root\
-   **40, 50** → Children of node 20\
-   **40, 50, 30** → Leaf nodes

------------------------------------------------------------------------

# Terminology

| Term | Meaning |
|------|---------|
| Root | Topmost node |
| Parent | Node that has children |
| Child | Node below another node |
| Leaf | Node with no children |
| Subtree | Tree formed from a node and its descendants |

------------------------------------------------------------------------

# Types of Binary Trees

## Full Binary Tree

Every node has **0 or 2 children**.

          1
         / \
        2   3

------------------------------------------------------------------------

## Complete Binary Tree

All levels are filled except possibly the last.

            1
           / \
          2   3
         / \
        4   5

------------------------------------------------------------------------

## Perfect Binary Tree

All internal nodes have two children and all leaves are at the same
level.

Formula:

    Nodes = 2^h − 1

Where **h** is the height.

------------------------------------------------------------------------

# Code Example 1: Creating a Basic Binary Tree

This example manually builds a tree.

## Code

``` cpp
#include <iostream>
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

int main()
{
    // Insert nodes into the binary tree
    Node* root = new Node(10);
    root->left = new Node(20);
    root->right = new Node(30);
    root->left->left = new Node(40);
    root->left->right = new Node(50);

    cout << "Binary tree created successfully!" << endl;

    return 0;
}
```

Tree created:

            10
           /  \
         20    30
        /  \
      40   50

Output:

    Binary tree created successfully!

Explanation:

-   `Node` structure represents a tree node
-   Each node stores data and two pointers
-   Nodes are connected using pointers

Example:

    root->left = new Node(20);

------------------------------------------------------------------------

# Code Example 2: Binary Tree with Operations

This implementation includes:

-   Insert operation
-   Inorder traversal
-   Preorder traversal
-   Postorder traversal

## Code

``` cpp

#include <iostream>
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

//Implement binary tree class with basic operations like insert, search, and traversal (in-order, pre-order, post-order).
class BinaryTree
{
private:
    Node* root;
    
    // Helper functions 
    Node* insertNode(Node* node, int value) {
        // If the tree is empty, create a new node and return it
        if (node == nullptr) {
            return new Node(value);
        }

        /* Otherwise, recur down the tree
           If the value to be inserted is smaller than the node's data,
           insert it into the left subtree
           else, insert it into the right subtree
        */

        if (value < node->data) {
            node->left = insertNode(node->left, value);
        }
        else {
            node->right = insertNode(node->right, value);
        }
        return node;
    }
	//Traversal functions for the binary tree. These functions will visit each node in a specific order and print its value.
    // Inorder Traversal (Left → Root → Right)
    void inorder(Node* node) {
		if (node == nullptr) return; // Base case: if the node is null, return
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    // Preorder Traversal (Root → Left → Right)
    void preorder(Node* node) {
        if (node == nullptr) return;
        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }

    // Postorder Traversal (Left → Right → Root)
    void postorder(Node* node) {
        if (node == nullptr) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
    }

public:
    // Constructor
    BinaryTree() {
        root = nullptr;
    }

    // This function inserts a new node with the given value into the binary tree.
    void insert(int value) {
        root = insertNode(root, value);
    }

    void inorderTraversal() {
        inorder(root);
        cout << endl;
    }

    void preorderTraversal() {
        preorder(root);
        cout << endl;
    }

    void postorderTraversal() {
        postorder(root);
        cout << endl;
    }


};

int main()
{
    BinaryTree tree;

    // Insert nodes using the insert method
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);

    cout << "Inorder Traversal: ";
    tree.inorderTraversal();

    cout << "Preorder Traversal: ";
    tree.preorderTraversal();

    cout << "Postorder Traversal: ";
    tree.postorderTraversal();

    return 0;
}

```

------------------------------------------------------------------------

# Traversal Explanation

Traversal means **visiting every node exactly once**.

### Inorder

    Left → Root → Right

### Preorder

    Root → Left → Right

### Postorder

    Left → Right → Root

------------------------------------------------------------------------

# Sample Output

    Inorder Traversal: 10 20 30 40 50
    Preorder Traversal: 10 20 30 40 50
    Postorder Traversal: 50 40 30 20 10

------------------------------------------------------------------------

# Key Takeaways

After completing this lab you understand:

-   Binary tree fundamentals
-   Node structure in C++
-   Pointer-based tree construction
-   Recursive insertion
-   Tree traversal techniques

Binary trees are the foundation for:

-   Binary Search Trees
-   AVL Trees
-   Red-Black Trees
-   Heaps
-   Graph algorithms
