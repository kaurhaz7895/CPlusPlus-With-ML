# Lab 04 - Working with Red-Back Trees in C++

## Table of Contents

-   [1. Introduction](#1-introduction)
-   [2. Definition of Red-Black Tree](#2-definition-of-red-black-tree)
-   [3. Properties of Red-Black Trees](#3-properties-of-red-black-trees)
-   [4. Why Red-Black Trees are
    Needed](#4-why-red-black-trees-are-needed)
-   [5. Height Guarantee](#5-height-guarantee)
-   [6. Insertion Fix Strategy](#6-insertion-fix-strategy)
-   [7. Rotation Concepts](#7-rotation-concepts)
-   [8. Insertion Case Decision Chart](#8-insertion-case-decision-chart)
-   [9. Red-Black Tree Implementation in
    C++](#9-red-black-tree-implementation-in-c)
-   [10. Example Execution](#10-example-execution)
-   [11. Visual Representation with
    Mermaid](#11-visual-representation-with-mermaid)
-   [12. AVL vs Red-Black Comparison](#12-avl-vs-red-black-comparison)
-   [13. Complexity Analysis](#13-complexity-analysis)

------------------------------------------------------------------------

# 1. Introduction

A **Red-Black Tree** is a self-balancing Binary Search Tree.

Each node contains an additional property:

    color = RED or BLACK

Balancing is maintained using:

-   recoloring
-   rotations

This guarantees that the tree height remains logarithmic.

------------------------------------------------------------------------

# 2. Definition of Red-Black Tree

A Red-Black Tree is a Binary Search Tree where each node follows
specific **color rules** that enforce balanced height.

The extra color information helps detect violations and repair the tree
after insertion or deletion.

------------------------------------------------------------------------

# 3. Properties of Red-Black Trees

A valid Red-Black Tree must satisfy the following rules.

1.  Every node is either **Red or Black**
2.  The **root is always Black**
3.  All **NULL leaves are Black**
4.  A **Red node cannot have a Red child**
5.  Every path from a node to descendant NULL nodes must contain the
    **same number of Black nodes**

These rules guarantee that the tree height remains balanced.

------------------------------------------------------------------------

# 4. Why Red-Black Trees are Needed

A normal BST can become skewed.

Example:

    Insert: 10 20 30 40

Skewed BST:

``` mermaid
graph TD
10 --> 20
20 --> 30
30 --> 40
```

Search complexity becomes:

    O(n)

Balanced Red-Black Tree:

``` mermaid
graph TD
20 --> 10
20 --> 30
30 --> 40
```

Search complexity remains:

    O(log n)

------------------------------------------------------------------------

# 5. Height Guarantee

For a Red-Black Tree with **n nodes**:

    height ≤ 2 log(n+1)

This ensures efficient operations.

------------------------------------------------------------------------

# 6. Insertion Fix Strategy

When inserting a node:

Step 1 --- Insert node like a **BST**\
Step 2 --- Color the new node **RED**\
Step 3 --- Fix violations using:

-   recoloring
-   rotations

------------------------------------------------------------------------

# 7. Rotation Concepts

Rotations restructure the tree without breaking BST ordering.

## Left Rotation

``` mermaid
graph TD
X --> Y
Y --> T3
Y --> T2
```

After rotation

``` mermaid
graph TD
Y --> X
Y --> T3
X --> T2
```

## Right Rotation

``` mermaid
graph TD
Y --> X
X --> T1
X --> T2
```

After rotation

``` mermaid
graph TD
X --> T1
X --> Y
Y --> T2
```

------------------------------------------------------------------------

# 8. Insertion Case Decision Chart

When inserting a node, examine:

-   Parent color
-   Uncle color
-   Tree shape

  Condition                             Case     Action
  ------------------------------------- -------- ------------------
  Parent is Black                       Valid    No action
  Parent Red + Uncle Red                Case 1   Recolor
  Parent Red + Uncle Black + Triangle   Case 2   Rotate once
  Parent Red + Uncle Black + Line       Case 3   Rotate + recolor

This chart helps students quickly determine which correction to apply.

------------------------------------------------------------------------

# 9. Red-Black Tree Implementation in C++

``` cpp
#include <iostream>
using namespace std;

enum Color {RED, BLACK};

class Node{
public:
    int data;
    Color color;
    Node *left, *right, *parent;
};

class RBTree{

private:

    Node *root;
    Node *TNULL;

    /*
    Initialize the sentinel NULL node.
    This node represents leaf nodes.
    */

    void initializeNULLNode(Node *node, Node *parent){

        node->data = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = BLACK;
    }

    /*
    Left Rotation

        x                 y
         \               / \
          y    →        x   T3
         / \             \
       T2  T3            T2
    */

    void leftRotate(Node *x){

        Node *y = x->right;

        x->right = y->left;

        if(y->left != TNULL)
            y->left->parent = x;

        y->parent = x->parent;

        if(x->parent == nullptr)
            root = y;

        else if(x == x->parent->left)
            x->parent->left = y;

        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    /*
    Right Rotation
    */

    void rightRotate(Node *x){

        Node *y = x->left;

        x->left = y->right;

        if(y->right != TNULL)
            y->right->parent = x;

        y->parent = x->parent;

        if(x->parent == nullptr)
            root = y;

        else if(x == x->parent->right)
            x->parent->right = y;

        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    /*
    Fix Red-Black violations after insertion
    */

    void fixInsert(Node *k){

        Node *u;

        while(k->parent->color == RED){

            if(k->parent == k->parent->parent->right){

                u = k->parent->parent->left;

                if(u->color == RED){

                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                }

                else{

                    if(k == k->parent->left){

                        k = k->parent;
                        rightRotate(k);
                    }

                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            }

            else{

                u = k->parent->parent->right;

                if(u->color == RED){

                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                }

                else{

                    if(k == k->parent->right){

                        k = k->parent;
                        leftRotate(k);
                    }

                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }

            if(k == root)
                break;
        }

        root->color = BLACK;
    }

public:

    RBTree(){

        TNULL = new Node;
        TNULL->color = BLACK;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    /*
    Insert operation
    */

    void insert(int key){

        Node *node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = RED;

        Node *y = nullptr;
        Node *x = root;

        while(x != TNULL){

            y = x;

            if(node->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;

        if(y == nullptr)
            root = node;

        else if(node->data < y->data)
            y->left = node;
        else
            y->right = node;

        if(node->parent == nullptr){

            node->color = BLACK;
            return;
        }

        if(node->parent->parent == nullptr)
            return;

        fixInsert(node);
    }

    void preorder(Node *node){

        if(node != TNULL){

            cout << node->data << " ";
            preorder(node->left);
            preorder(node->right);
        }
    }

    void printTree(){

        preorder(root);
    }
};

int main(){

    RBTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);

    cout << "Preorder traversal of Red-Black Tree:\n";

    tree.printTree();
}
```

------------------------------------------------------------------------

# 10. Example Execution

Inserted values:

    10 20 30 15 25

Program output:

    Preorder traversal of Red-Black Tree:
    20 10 15 30 25

------------------------------------------------------------------------

# 11. Visual Representation with Mermaid

``` mermaid
graph TD
20 --> 10
20 --> 30
10 --> 15
30 --> 25
```

------------------------------------------------------------------------

# 12. AVL vs Red-Black Comparison

  Feature         AVL Tree          Red-Black Tree
  --------------- ----------------- ------------------
  Balance         Strict            Approximate
  Rotations       More frequent     Less frequent
  Search speed    Slightly faster   Slightly slower
  Insert/Delete   Slower            Faster
  Used in         Databases         OS kernels, maps

------------------------------------------------------------------------

# 13. Complexity Analysis

  Operation   Time Complexity
  ----------- -----------------
  Search      O(log n)
  Insert      O(log n)
  Delete      O(log n)

Red-Black Trees maintain **logarithmic height**, ensuring efficient
operations.
