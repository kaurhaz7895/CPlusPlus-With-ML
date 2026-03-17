
# Binary Search Tree (BST) Deletion Lab – C++

## Objective
Understand and implement deletion in a Binary Search Tree (BST) covering all cases.

## BST Deletion Cases

1. Deleting a Leaf Node
2. Deleting a Node with One Child
3. Deleting a Node with Two Children

## Sample Tree
```
        50
       /  \
     30    70
    / \   / \
  20  40 60  80

```
## Case 1: Delete Leaf Node (20)
* Node 20 has no children.
* Simply remove it.
```
        50
       /  \
     30    70
      \   / \
      40 60  80

```
## Case 2: Delete Node with One Child (30)
* Node 30 has only one child (40) after 20 was deleted.
* Replace 30 with its child (40).
```
        50
       /  \
     40    70
           / \
         60  80

```

## Case 3: Delete Node with Two Children (50)

Replace with Inorder Successor (60)

* Node 50 has two children (30/40 on left, 70 on right).
* Standard BST deletion rule: replace with inorder successor (minimum in right subtree → 60).
* After replacement:
  * Root becomes 60.
  * Left subtree remains rooted at 40.
  * Right subtree is 70 with child 80.
```
        60
       /  \
     40    70
             \
             80

```

## C++ Implementation

```cpp
#include <iostream>
using namespace std;

/*
========================================
Node Structure
========================================
Each node contains:
- data → value stored
- left → pointer to left subtree
- right → pointer to right subtree
*/
struct Node {
    int data;
    Node* left;
    Node* right;

    // Constructor to create a new node
    Node(int val) {
        data = val;
        left = right = NULL;
    }
};

/*
========================================
INSERT FUNCTION (Recursive)
========================================
- Inserts a node following BST rules:
    left < root < right
*/
Node* insert(Node* root, int key) {

    // If tree/subtree is empty → create node
    if (root == NULL)
        return new Node(key);

    // If key is smaller → go to left subtree
    if (key < root->data)
        root->left = insert(root->left, key);

    // If key is greater → go to right subtree
    else if (key > root->data)
        root->right = insert(root->right, key);

    // If key == root->data → ignore (no duplicates)
    return root;
}

/*
========================================
FIND MINIMUM NODE
========================================
- Used in deletion (Case 3)
- Finds the smallest value in a subtree
- Always the leftmost node
*/
Node* findMin(Node* root) {
    if (root == NULL) return NULL; // safety check

    while (root->left != NULL)
        root = root->left;   // keep moving left

    return root;
}

/*
========================================
DELETE NODE FUNCTION (CORE LOGIC)
========================================
Steps:
1. Search the node
2. Apply one of the 3 deletion cases
*/
Node* deleteNode(Node* root, int key) {

    // Base case: if tree is empty
    if (root == NULL)
        return root;

    /*
    ========================================
    STEP 1: SEARCH THE NODE
    ========================================
    Traverse like BST
    */
    if (key < root->data)
        root->left = deleteNode(root->left, key);

    else if (key > root->data)
        root->right = deleteNode(root->right, key);

    /*
    ========================================
    STEP 2: NODE FOUND → APPLY CASES
    ========================================
    */
    else {

        /*
        ----------------------------------------
        CASE 1: LEAF NODE (No children)
        ----------------------------------------
        Simply delete and return NULL
        */
        if (root->left == NULL && root->right == NULL) {
			      delete root; // free memory dynamically allocated for the node
            return NULL;
        }

        /*
        ----------------------------------------
        CASE 2: ONE CHILD
        ----------------------------------------
        Replace node with its child
        */

        // Only right child exists
        else if (root->left == NULL) {
            Node* temp = root->right;  // store child
            delete root;               // delete current node
            return temp;               // return child
        }

        // Only left child exists
        else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        /*
        ----------------------------------------
        CASE 3: TWO CHILDREN
        ----------------------------------------
        Steps:
        1. Find inorder successor (smallest in right subtree)
        2. Replace current node data with successor
        3. Delete successor from right subtree
        */
        else {
            Node* successor = findMin(root->right);

            // Copy successor's value to current node
            root->data = successor->data;

            // Delete the successor node
            root->right = deleteNode(root->right, successor->data);
        }
    }

    // Return updated root
    return root;
}

/*
========================================
INORDER TRAVERSAL
========================================
- Prints BST in sorted order
*/
void inorder(Node* root) {
    if (root == NULL) return;

    inorder(root->left);          // visit left subtree
    cout << root->data << " ";    // visit root
    inorder(root->right);         // visit right subtree
}

/*
========================================
MAIN FUNCTION (DRIVER CODE)
========================================
*/
int main() {
    Node* root = NULL;

    /*
    Build BST:
            50
           /  \\
         30    70
        / \\   / \\
      20  40 60  80
    */
    int arr[] = { 50, 30, 70, 20, 40, 60, 80 };

    for (int x : arr)
        root = insert(root, x);

    cout << "Initial Tree (Inorder): ";
    inorder(root);

    /*
    Perform deletions:
    */
    root = deleteNode(root, 20); // Case 1: Leaf
    root = deleteNode(root, 30); // Case 2: One child
    root = deleteNode(root, 50); // Case 3: Two children

    cout << "\nAfter Deletions (Inorder): ";
    inorder(root);

    return 0;
}
```

---

## Expected Output

Initial Tree: 20 30 40 50 60 70 80  
After Deletions: 40 60 70 80

---

## Key Takeaways

- Deletion depends on number of children
- Inorder successor preserves BST property
- Time Complexity: O(h)

---

## Practice Tasks

- Delete nodes in different order
- Try skewed trees
- Observe traversal after each deletion

---

