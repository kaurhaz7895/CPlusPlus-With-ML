# Lab 2: Working with One-Dimensional Arrays in C++

## 🎯 Objectives

By the end of this lab, students should be able to:

- Declare and initialize **one-dimensional arrays** in C++.
- Access and modify array elements using **indexing**.
- Traverse arrays using **loops**.
- Perform simple computations such as **sum, average, and maximum value**.

---

# Background

A **one-dimensional array** is a collection of elements of the same data type stored in **contiguous memory locations**.

Key characteristics:

- All elements have the **same data type**
- Accessed using **index positions**
- Indexing starts at **0**

Example memory layout:

| Index | 0 | 1 | 2 | 3 | 4 |
|------|---|---|---|---|---|
| Value | 10 | 20 | 30 | 40 | 50 |

Access example:

```cpp
arr[0]  // first element
arr[3]  // fourth element
```

---

# Procedure

## Step 1 — Declare and Initialize an Array

Create a new **C++ Console Application** and write the following code.

```cpp
#include <iostream>
using namespace std;

int main() {

    int numbers[5] = {10, 20, 30, 40, 50};

    cout << "Array elements are:" << endl;

    for(int i = 0; i < 5; i++) {
        cout << numbers[i] << endl;
    }

    return 0;
}
```

Build and run the program.

---

## Step 2 — Modify Array Elements

Update the program to modify an element.

Example:

```cpp
numbers[2] = 100;
```

Print the array again and observe the change.

Questions to consider:

- Which element changed?
- Which index corresponds to the third position?

---

## Step 3 — Input Values into an Array

Modify the program so that values are entered by the user.

```cpp
#include <iostream>
using namespace std;

int main() {

    int arr[5];

    cout << "Enter 5 integers:" << endl;

    for(int i = 0; i < 5; i++) {
        cin >> arr[i];
    }

    cout << "You entered:" << endl;

    for(int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }

    return 0;
}
```

---

## Step 4 — Compute Sum and Average

Extend the program to calculate the **sum** and **average** of array elements.

```cpp
int sum = 0;

for(int i = 0; i < 5; i++) {
    sum += arr[i];
}

double average = sum / 5.0;

cout << "Sum = " << sum << endl;
cout << "Average = " << average << endl;
```

---

## Step 5 — Find the Maximum Value

Add logic to determine the **largest element** in the array.

```cpp
int max = arr[0];

for(int i = 1; i < 5; i++) {
    if(arr[i] > max) {
        max = arr[i];
    }
}

cout << "Maximum value = " << max << endl;
```

---

# Expected Observations

| Operation | Result |
|----------|--------|
| Array declaration | Memory reserved for multiple values |
| Index access | Retrieves specific element |
| Loop traversal | Processes all elements |
| Sum/Average | Basic aggregation of array data |

---

# Analysis Questions

1. Why does array indexing start at **0** in C++?
2. What happens if a program accesses `arr[10]` when the array size is 5?
3. Why are loops commonly used with arrays?
4. How would you modify the program to compute the **minimum value**?

---

# Conclusion

This lab demonstrated how to declare, initialize, and manipulate **one-dimensional arrays** in C++. Arrays allow efficient storage and processing of multiple values using a single variable name and indexed access.
