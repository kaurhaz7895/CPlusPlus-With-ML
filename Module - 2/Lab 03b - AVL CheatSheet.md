# AVL Tree Cheat Sheet 

## 1. AVL Tree Definition

An **AVL Tree** is a self‑balancing Binary Search Tree where the height
difference between the left and right subtrees of any node is at most
**1**.

Balance Factor:

    BF(node) = height(left) − height(right)

Valid balance factors:

    -1, 0, +1

If \|BF\| \> 1 → **rotation required**.

------------------------------------------------------------------------

## 2. Height Convention

    height(NULL) = -1
    leaf node = 0
    height(node) = 1 + max(height(left), height(right))

Example:

        20
       /  \
     10   30

    height(10)=0
    height(30)=0
    height(20)=1

------------------------------------------------------------------------

## 3. AVL Rotation Decision Table

  Balance Factor   Insertion Path   Case   Rotation
  ---------------- ---------------- ------ ----------------------------
  +2               Left → Left      LL     Right Rotate
  -2               Right → Right    RR     Left Rotate
  +2               Left → Right     LR     Left Rotate → Right Rotate
  -2               Right → Left     RL     Right Rotate → Left Rotate

Shortcut:

    LL → Right rotation
    RR → Left rotation
    LR → Left then Right
    RL → Right then Left

------------------------------------------------------------------------

## 4. Rotation Rules

### Right Rotation (LL Case)

Before

            y
           / \
          x   T3
         / \
        T1  T2

After

            x
           / \
          T1  y
             / \
            T2  T3

Key rule:

    T2 becomes left child of y

------------------------------------------------------------------------

### Left Rotation (RR Case)

Before

            x
           / \
          T1  y
             / \
            T2  T3

After

            y
           / \
          x   T3
         / \
        T1  T2

Key rule:

    T2 becomes right child of x

------------------------------------------------------------------------

### LR Rotation

Before

            z
           / \
          x   T4
         / \
        T1  y
           / \
          T2 T3

Step 1 → Left Rotate(x)\
Step 2 → Right Rotate(z)

After

            y
           / \
          x   z
         / \ / \
        T1 T2 T3 T4

------------------------------------------------------------------------

### RL Rotation

Before

            z
           / \
          T1  y
             / \
            x   T4
           / \
          T2 T3

Step 1 → Right Rotate(y)\
Step 2 → Left Rotate(z)

After

            x
           / \
          z   y
         / \ / \
        T1 T2 T3 T4

------------------------------------------------------------------------

## 5. AVL Insertion Algorithm (Simplified)

1.  Insert node using **BST rules**
2.  Update **height**
3.  Compute **balance factor**
4.  If imbalance detected → apply rotation

Pseudo logic:

    if BF > 1 and key < left.key → LL
    if BF < -1 and key > right.key → RR
    if BF > 1 and key > left.key → LR
    if BF < -1 and key < right.key → RL

------------------------------------------------------------------------

## 6. Time Complexity

  Operation   Time Complexity
  ----------- -----------------
  Search      O(log n)
  Insert      O(log n)
  Delete      O(log n)

AVL trees maintain **strict balance**, guaranteeing logarithmic height.

------------------------------------------------------------------------

## 7. Memory Trick

Look at the **insertion path**:

    Left → Left  → LL
    Right → Right → RR
    Left → Right → LR
    Right → Left → RL

The **middle node becomes the new root of the rotated subtree**.

------------------------------------------------------------------------

## 8. Example Balanced AVL Tree

            30
           /  \
         20    40
        /  \     \
       10  25     50

Heights (edges):

    10 = 0
    25 = 0
    50 = 0
    20 = 1
    40 = 1
    30 = 2

AVL property satisfied because:

    |BF| ≤ 1 for all nodes
