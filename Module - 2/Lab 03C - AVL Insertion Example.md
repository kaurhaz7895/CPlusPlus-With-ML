# AVL Tree Rotation Walkthrough (LL, RR, LR, RL)

This walkthrough starts from the **given AVL tree** and demonstrates
insertions step‑by‑step.\
All rotations follow the AVL rule:

> Rotate at the **first ancestor whose balance factor becomes ±2**.

Height convention used:

    height(NULL) = -1
    leaf height = 0
    height = number of edges to deepest leaf

------------------------------------------------------------------------

# Table of Contents

-   [1. Initial Tree](#1-initial-tree)
-   [2. Insert 3 → LL Rotation](#2-insert-3--ll-rotation)
-   [3. Insert 14 → No Rotation](#3-insert-14--no-rotation)
-   [4. Insert 45 → No Rotation](#4-insert-45--no-rotation)
-   [5. Insert 2 → LL Rotation](#5-insert-2--ll-rotation)
-   [6. Insert 60 → RR Rotation](#6-insert-60--rr-rotation)
-   [7. Insert 7 → LR Rotation](#7-insert-7--lr-rotation)
-   [8. Insert 17 → RL Rotation](#8-insert-17--rl-rotation)
-   [9. Rotation Summary](#9-rotation-summary)

------------------------------------------------------------------------

# 1. Initial Tree

                13
               /  \
             10    15
            /  \     \
           5   11     16
          / \
         4   6

Tree is balanced.

------------------------------------------------------------------------

# 2. Insert 3 → LL Rotation

BST insertion path

    13 → 10 → 5 → 4 → left

                13
               /  \
             10    15
            /  \     \
           5   11     16
          / \
         4   6
        /
       3

Node **10 becomes unbalanced**.

    BF(10) = +2
    Pattern = Left → Left

RightRotate(10)

Before

            10
           /  \
          5   11
         / \
        4   6
       /
      3

After

            5
           / \
          4   10
         /   /  \
        3   6   11

Reattached

                   13
                 /    \
                5      15
               / \        \
              4  10        16
             /   / \
            3   6  11

------------------------------------------------------------------------

# 3. Insert 14 → No Rotation

BST path

    13 → 15 → left

                   13
                 /    \
                5      15
               / \     / \
              4  10   14 16
             /   / \
            3   6  11

Balanced → **No rotation required**

------------------------------------------------------------------------

# 4. Insert 45 → No Rotation

BST path

    13 → 15 → 16 → right

                   13
                 /    \
                5      15
               / \     / \
              4  10   14 16
             /   / \        \
            3   6  11        45

Balanced → **No rotation required**

------------------------------------------------------------------------

# 5. Insert 2 → LL Rotation

BST path

    13 → 5 → 4 → 3 → left

                   13
                 /    \
                5      15
               / \     / \
              4  10   14 16
             /   / \        \
            3   6  11        45
           /
          2

First unbalanced node

    BF(4) = +2
    Pattern = Left → Left

RightRotate(4)

Before

        4
       /
      3
     /
    2

After

        3
       / \
      2   4

Reattach

                   13
                 /    \
                5      15
               / \     / \
              3  10   14 16
             / \ / \        \
            2  4 6  11       45

------------------------------------------------------------------------

# 6. Insert 60 → RR Rotation

Subtree

          16
            \
            45
               \
               60

Pattern

    Right → Right

LeftRotate(16)

            45
           /  \
          16   60

------------------------------------------------------------------------

# 7. Insert 7 → LR Rotation

Subtree

          10
         /
        6
         \
          7

Pattern

    Left → Right

LeftRotate(6)

          10
         /
        7
       /
      6

RightRotate(10)

           7
          / \
         6  10

------------------------------------------------------------------------

# 8. Insert 17 → RL Rotation

Subtree

          15
            \
            16
           /
          17

Pattern

    Right → Left

RightRotate(16)

          15
            \
            17
              \
              16

LeftRotate(15)

            17
           /  \
          15  16

------------------------------------------------------------------------

# 9. Rotation Summary

  Case   Pattern         Rotation
  ------ --------------- ----------------------------
  LL     Left → Left     Right Rotate
  RR     Right → Right   Left Rotate
  LR     Left → Right    Left Rotate + Right Rotate
  RL     Right → Left    Right Rotate + Left Rotate

------------------------------------------------------------------------

# Key Insight

    Rotate at the first ancestor whose |BF| > 1

This guarantees AVL height remains **O(log n)**.
