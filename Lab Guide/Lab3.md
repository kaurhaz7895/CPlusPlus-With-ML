# Lab 3: Dynamic Arrays and Heap Memory in C++

## 🎯 Objectives

After completing this lab, participants should be able to:

- Allocate arrays dynamically using `new`
- Understand the difference between **stack arrays and heap arrays**
- Process dynamically allocated arrays
- Properly release memory using `delete[]`
- Recognize risks such as **memory leaks**

---

# Scenario

A monitoring system receives **a variable number of sensor readings**.  
Unlike the previous lab, the number of readings is **not known at compile time**.

Your program must:

- Ask the user how many readings will be stored
- Allocate memory dynamically
- Process the readings
- Release allocated memory

---

# Background

Previously we declared arrays like this:

```cpp
int arr[10];
```

This allocates memory on the **stack**, and the size must be known at compile time.

Dynamic arrays allow memory allocation during program execution.

Example:

```cpp
int* arr = new int[10];
```

This memory is allocated on the **heap**.

Memory must be released manually:

```cpp
delete[] arr;
```

---

# Procedure

## Step 1 — Allocate a Dynamic Array

Create a new C++ console program.

```cpp
#include <iostream>
using namespace std;

int main()
{
    int size;

    cout << "Enter number of sensor readings: ";
    cin >> size;

    double* readings = new double[size];

    cout << "Memory allocated successfully." << endl;

    delete[] readings;

    return 0;
}
```

---

## Step 2 — Input Values

Extend the program to store sensor readings.

```cpp
for(int i = 0; i < size; i++)
{
    cout << "Enter reading " << i+1 << ": ";
    cin >> readings[i];
}
```

---

## Step 3 — Display the Readings

```cpp
cout << "Sensor Readings:" << endl;

for(int i = 0; i < size; i++)
{
    cout << readings[i] << " ";
}
```

---

## Step 4 — Compute Average

```cpp
double sum = 0;

for(int i = 0; i < size; i++)
{
    sum += readings[i];
}

double average = sum / size;

cout << "Average reading: " << average << endl;
```

---

## Step 5 — Free Heap Memory

Always release dynamically allocated memory.

```cpp
delete[] readings;
```

Failing to do so may cause **memory leaks**.

---

# Stack vs Heap Comparison

| Feature | Stack Array | Heap Array |
|-------|-------------|-----------|
| Allocation time | compile time | runtime |
| Size flexibility | fixed | dynamic |
| Memory management | automatic | manual |
| Allocation syntax | `int arr[10]` | `new int[10]` |

---

# Analysis Questions

1. Why must dynamically allocated memory be freed manually?
2. What happens if `delete[]` is not used?
3. Why do dynamic arrays require pointers?
4. What are advantages of runtime memory allocation?

---

# Sample Answers

### 1. Why must dynamically allocated memory be freed manually?

Heap memory is not automatically reclaimed when a function ends. The programmer must explicitly release it using `delete[]`.

---

### 2. What happens if `delete[]` is not used?

The program may produce a **memory leak**, where allocated memory remains reserved but unused.

---

### 3. Why do dynamic arrays require pointers?

The `new` operator returns the **starting address of allocated memory**, which must be stored in a pointer.

Example:

```
int* arr = new int[10];
```

---

### 4. What are advantages of runtime memory allocation?

Dynamic memory allows programs to handle **variable-sized data**, improving flexibility when the data size is unknown before execution.

---

# Extension Challenge

Modify the program to:

1. Reallocate the array if more readings are required.
2. Detect abnormal readings outside the range `0–100`.
3. Print the **memory addresses** of the array elements.

Example:

```cpp
cout << &readings[i] << endl;
```

Observe how addresses increase sequentially.

---

# Conclusion

Dynamic arrays allow programs to allocate memory during runtime, making them useful when data size is unknown in advance. However, heap memory must be carefully managed to avoid leaks and memory errors.
