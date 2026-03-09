# Lab Guide: Array of Pointers in C++

## Learning Objectives

After completing this lab, students will be able to:

-   Understand what a **pointer** is
-   Explain the concept of an **array of pointers**
-   Distinguish between **array of pointers** and **pointer to array**
-   Implement programs using arrays of pointers
-   Understand memory representation of pointer arrays
-   Apply pointer arrays to real-world problems such as strings and
    dynamic memory

------------------------------------------------------------------------

# 1. Introduction to Pointers

A **pointer** is a variable that stores the **memory address** of
another variable.

Example:

``` cpp
int x = 10;
int* p = &x;
```

Explanation:

-   `x` stores the value `10`
-   `p` stores the **address of x**

Memory representation:

  Variable   Address   Value
  ---------- --------- -------
  x          1000      10
  p          2000      1000

*(Addresses are illustrative)*

------------------------------------------------------------------------

# 2. What is an Array of Pointers?

An **array of pointers** is an array where **each element is itself a
pointer**.

Instead of storing actual values, the array stores **addresses of
variables**.

Declaration:

``` cpp
int* ptr[3];
```

Meaning:

    ptr is an array of 3 elements
    each element is a pointer to an integer

Memory representation:

  Index      Stored Value
  ---------- --------------
  ptr\[0\]   address
  ptr\[1\]   address
  ptr\[2\]   address

------------------------------------------------------------------------

# 3. Basic Example

``` cpp
#include <iostream>
using namespace std;

int main()
{
    int a = 10;
    int b = 20;
    int c = 30;

    int* ptr[3];

    ptr[0] = &a;
    ptr[1] = &b;
    ptr[2] = &c;

    for(int i = 0; i < 3; i++)
    {
        cout << *ptr[i] << endl;
    }
}
```

### Output

    10
    20
    30

------------------------------------------------------------------------

# 4. Step-by-Step Explanation

### Step 1: Variables

    a = 10
    b = 20
    c = 30

### Step 2: Array of pointers

    int* ptr[3]

Creates three pointer variables:

    ptr[0]
    ptr[1]
    ptr[2]

### Step 3: Assign addresses

    ptr[0] = &a
    ptr[1] = &b
    ptr[2] = &c

### Step 4: Dereference pointers

    *ptr[0] = 10
    *ptr[1] = 20
    *ptr[2] = 30

------------------------------------------------------------------------

# 5. Visual Representation

    ptr array

    +--------+--------+--------+
    | ptr[0] | ptr[1] | ptr[2] |
    +--------+--------+--------+
        |        |        |
        v        v        v
       [a]      [b]      [c]
       10       20       30

Each element in the array points to a variable.

------------------------------------------------------------------------

# 6. Accessing Values

Two common operations:

### Access pointer

    ptr[i]

### Access actual value

    *ptr[i]

------------------------------------------------------------------------

# 7. Array of Pointers vs Pointer to Array

These two declarations are very different.

### Array of pointers

``` cpp
int* ptr[5];
```

Meaning:

    ptr is an array
    each element is a pointer

### Pointer to array

``` cpp
int (*ptr)[5];
```

Meaning:

    ptr points to an array of 5 integers

------------------------------------------------------------------------

# 8. Using Array of Pointers with Strings

Example:

``` cpp
#include <iostream>
using namespace std;

int main()
{
    const char* fruits[3] = {"Apple", "Mango", "Orange"};

    for(int i=0;i<3;i++)
        cout << fruits[i] << endl;
}
```

Output:

    Apple
    Mango
    Orange

Each element stores the address of a string.

------------------------------------------------------------------------

# 9. Dynamic Memory Example

``` cpp
#include <iostream>
using namespace std;

int main()
{
    int* arr[3];

    for(int i=0;i<3;i++)
        arr[i] = new int;

    *arr[0] = 10;
    *arr[1] = 20;
    *arr[2] = 30;

    for(int i=0;i<3;i++)
        cout << *arr[i] << endl;
}
```

Output:

    10
    20
    30

Each pointer stores a dynamically allocated memory address.

------------------------------------------------------------------------

# 10. Jagged Arrays Using Pointer Arrays

Pointer arrays allow rows of **different sizes**.

Example:

``` cpp
int* rows[3];

rows[0] = new int[2];
rows[1] = new int[5];
rows[2] = new int[3];
```

Memory structure:

    rows
    +-----+-----+-----+
    |  p  |  p  |  p  |
    +-----+-----+-----+
       |      |      |
       v      v      v
    [ x x ] [ x x x x x ] [ x x x ]

Each row is independent.


------------------------------------------------------------------------

# 11. Applications

Array of pointers are used in:

-   String storage
-   Dynamic 2‑D arrays
-   Jagged arrays
-   Function pointer tables
-   Operating system kernels
-   Compilers and interpreters

Pointer arrays are widely used for **lookup tables**.

Example:

``` cpp
const char* keywords[] =
{
    "if",
    "else",
    "while",
    "return"
};
```

Memory layout:

    keywords
    +----+----+----+----+
    | p  | p  | p  | p  |
    +----+----+----+----+
      |    |    |    |
      v    v    v    v
    "if" "else" "while" "return"

Common in:

-   interpreters
-   compilers
-   command dispatch tables
------------------------------------------------------------------------

# 12. Performance Considerations

### Advantages

Pointer arrays provide:

-   flexible allocation
-   jagged data structures
-   memory indirection
-   ability to reference existing objects

### Drawbacks

They introduce:

-   pointer indirection
-   reduced cache locality
-   manual memory management

Compared with contiguous arrays:

    std::vector<int>

pointer arrays may result in **more cache misses**.

------------------------------------------------------------------------
Modern C++ Alternatives

In modern C++ pointer arrays are often replaced by safer containers.

### Vector of pointers

``` cpp
std::vector<int*> ptrs;
```

### Vector of vectors

``` cpp
std::vector<std::vector<int>> matrix;
```

### Smart pointers

``` cpp
std::unique_ptr<int>
```

These avoid many **memory safety issues**.
------------------------------------------------------------------------

# 14. Lab Exercises

### Exercise 1

Modify the program to store **five variables** instead of three.

------------------------------------------------------------------------

### Exercise 2

Create an array of pointers pointing to:

    10, 20, 30, 40

Print them using dereferencing.

------------------------------------------------------------------------

### Exercise 3

Create a program that stores **names of students using pointer arrays**.

------------------------------------------------------------------------

# 15. FAQs

### What is an array of pointers?

An array where each element stores the **address of a variable**.

### Why are arrays of pointers useful?

They allow flexible memory usage and dynamic data structures.

### Are pointer arrays faster?

They can reduce memory usage and allow flexible allocation.

### Where are pointer arrays used in real systems?

They are widely used in:

-   compilers
-   operating systems
-   memory management systems
-   interpreters

------------------------------------------------------------------------

# Summary

Key takeaways:

-   A pointer stores a **memory address**
-   An array of pointers stores **multiple addresses**
-   Accessing values requires **dereferencing**
-   Pointer arrays enable **dynamic and flexible memory structures**
