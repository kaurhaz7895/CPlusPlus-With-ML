# AVL Tree Rotation Walkthrough (LL, RR, LR, RL)

This document demonstrates **all four AVL rotation cases** starting from
the **same initial tree provided**.

Height definition used:

    height = number of edges on longest path to a leaf
    height(NULL) = -1
    leaf height = 0

------------------------------------------------------------------------

# Table of Contents

-   [1. Initial Tree](#1-initial-tree)
-   [2. Insert 3 → LL Rotation](#2-insert-3--ll-rotation)
-   [3. Insert 14 → No Rotation](#3-insert-14--no-rotation)
-   [4. Insert 45 → No Rotation](#4-insert-45--no-rotation)
-   [5. Insert 2 → Another LL
    Rotation](#5-insert-2--another-ll-rotation)
-   [6. Insert 60 → RR Rotation](#6-insert-60--rr-rotation)
-   [7. Insert 7 → LR Rotation](#7-insert-7--lr-rotation)
-   [8. Insert 17 → RL Rotation](#8-insert-17--rl-rotation)
-   [9. Rotation Summary](#9-rotation-summary)

------------------------------------------------------------------------

# 1. Initial Tree

Given AVL tree:

```
            13
           /  \
         10    15
        /  \     \
       5   11     16
      / \
     4   6
```

Tree is balanced.

------------------------------------------------------------------------

# 2. Insert 3 → LL Rotation

BST insertion path:

    13 → 10 → 5 → 4 → left

After insertion:
```
            13
           /  \
         10    15
        /  \     \
       5   11     16
      / \
     4   6
    /
    3
```
Node **10 becomes unbalanced**.

    BF(10) = +2

Pattern:

    Left → Left

Apply **Right Rotation at 10**.

Before rotation:
```
            10
           /  \
          5   11
         / \
        4   6
       /
      3
```
After rotation:
```
            5
           / \
          4   10
         /   /  \
        3   6   11
```
Reattach to root:
```
               13
             /    \
            5      15
           / \        \
          4  10        16
         /   / \
        3   6  11
```
------------------------------------------------------------------------

# 3. Insert 14 → No Rotation

BST path:

    13 → 15 → left

Insert 14:
```
               13
             /    \
            5      15
           / \     / \
          4  10   14 16
         /   / \
        3   6  11
```
All balance factors remain valid.

    No rotation required

------------------------------------------------------------------------

# 4. Insert 45 → No Rotation

BST path:

    13 → 15 → 16 → right

Insert 45:
```
               13
             /    \
            5      15
           / \     / \
          4  10   14 16
         /   / \        \
        3   6  11        45
```
Tree remains balanced.

------------------------------------------------------------------------

# 5. Insert 2 → Another LL Rotation

BST path:

    13 → 5 → 4 → 3 → left

Insert 2:
```
               13
             /    \
            5      15
           / \     / \
          4  10   14 16
         /   / \        \
        3   6  11        45
       /
      2
```
Node **5 becomes unbalanced**.

Pattern:

    Left → Left

Right rotate at **5**.

After rotation:
```
          4
         / \
        3   5
       /     \
      2      10
```
Reattach to root:
```
               13
             /    \
            4      15
           / \     / \
          3   5   14 16
         /     \        \
        2      10        45
              /  \
             6   11
```
------------------------------------------------------------------------

# 6. Insert 60 → RR Rotation

BST path:

    13 → 15 → 16 → 45 → right

Before rotation:
```
          16
            \
            45
               \
               60
```
Pattern:

    Right → Right

Apply **Left Rotation at 16**.

After rotation:
```
            45
           /  \
          16   60
```
------------------------------------------------------------------------

# 7. Insert 7 → LR Rotation

BST path:

    13 → 4 → 5 → 10 → 6 → right

Before rotation:
```
          10
         /
        6
         \
          7
```
Pattern:

    Left → Right

Step 1:

    LeftRotate(6)

Result:
```
          10
         /
        7
       /
      6
```
Step 2:

    RightRotate(10)

Final:
```
           7
          / \
         6  10
```
------------------------------------------------------------------------

# 8. Insert 17 → RL Rotation

BST path:

    13 → 15 → 16 → right → left

Before rotation:
```
          15
            \
            16
           /
          17
```
Pattern:

    Right → Left

Step 1:

    RightRotate(16)

Result:
```
          15
            \
            17
              \
              16
```
Step 2:

    LeftRotate(15)

Final:
```
            17
           /  \
          15  16
```
------------------------------------------------------------------------

# 9. Rotation Summary

  Case   Pattern          Rotation
  ------ ---------------- ----------------------------
  LL     Left of Left     Right Rotate
  RR     Right of Right   Left Rotate
  LR     Right of Left    Left Rotate + Right Rotate
  RL     Left of Right    Right Rotate + Left Rotate

------------------------------------------------------------------------

# Key Insight

During AVL rotations:

    The middle node of the insertion path
    becomes the new root of the rotated subtree.

This preserves the **Binary Search Tree ordering invariant** while
restoring AVL balance.
