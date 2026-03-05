# Lab 2B: Working with One-Dimensional Arrays in C++

## 🎯 Objectives

After completing this lab, participants should be able to:

- Understand how one-dimensional arrays are stored in memory.
- Declare and initialize arrays in C++.
- Process arrays using loops.
- Perform basic data analysis tasks (min, max, average).
- Understand the relationship between arrays and pointers.
- Recognize common pitfalls such as out-of-bounds access.

---

# Scenario

A monitoring system collects 10 temperature readings from sensors every minute.  
These readings must be stored and analyzed to determine system health.

You will implement a program that:

- Stores readings in an array
- Computes statistics
- Detects abnormal values

---

# Background

A one-dimensional array stores elements of the same type in contiguous memory locations.

Example:

```cpp
int values[5] = {10,20,30,40,50};
```

Memory representation:

| Index | 0 | 1 | 2 | 3 | 4 |
|------|---|---|---|---|---|
| Value | 10 | 20 | 30 | 40 | 50 |

Important characteristics:

- Indexing starts at **0**
- Access time is **O(1)** (constant time)
- Memory is **contiguous**, improving cache locality

---

# Procedure

## Step 1 — Declare an Array

```cpp
#include <iostream>
using namespace std;

int main()
{
    const int SENSOR_COUNT = 10;
    double readings[SENSOR_COUNT];

    cout << "Enter 10 sensor readings:" << endl;

    for(int i = 0; i < SENSOR_COUNT; i++)
        cin >> readings[i];

    cout << "Data recorded successfully." << endl;

    return 0;
}
```

---

## Step 2 — Display the Data

```cpp
cout << "Sensor Readings:" << endl;

for(int i = 0; i < SENSOR_COUNT; i++)
{
    cout << readings[i] << " ";
}
```

---

## Step 3 — Compute Average Reading

```cpp
double sum = 0;

for(int i = 0; i < SENSOR_COUNT; i++)
{
    sum += readings[i];
}

double average = sum / SENSOR_COUNT;

cout << "Average reading: " << average << endl;
```

---

## Step 4 — Detect Maximum and Minimum Values

```cpp
double maxValue = readings[0];
double minValue = readings[0];

for(int i = 1; i < SENSOR_COUNT; i++)
{
    if(readings[i] > maxValue)
        maxValue = readings[i];

    if(readings[i] < minValue)
        minValue = readings[i];
}

cout << "Maximum reading: " << maxValue << endl;
cout << "Minimum reading: " << minValue << endl;
```

---

## Step 5 — Detect Abnormal Readings

Assume the safe range is **0–100**.

```cpp
for(int i = 0; i < SENSOR_COUNT; i++)
{
    if(readings[i] < 0 || readings[i] > 100)
    {
        cout << "Warning: abnormal reading at index "
             << i << endl;
    }
}
```

---

# Memory Insight

Arrays occupy **contiguous memory locations**.

Example:

```
readings[0] -> address 0x1000
readings[1] -> address 0x1008
readings[2] -> address 0x1010
```

The difference between addresses corresponds to the **size of the data type**.

You can inspect addresses using:

```cpp
for(int i = 0; i < SENSOR_COUNT; i++)
{
    cout << &readings[i] << endl;
}
```

---

# Analysis Questions

1. Why are arrays faster than linked structures for sequential access?  
2. What advantages does contiguous memory provide?  
3. What risks arise from out-of-bounds access?  
4. How does pointer arithmetic relate to array indexing?

---

# Sample Answers

### 1. Why are arrays faster than linked structures for sequential access?

Arrays store elements in contiguous memory, allowing the CPU to access elements directly using index arithmetic. Linked structures require pointer traversal, which introduces additional memory accesses and reduces cache efficiency.

---

### 2. What advantages does contiguous memory provide?

Contiguous memory improves **cache locality**. When the CPU loads one element into cache, nearby elements are often loaded as well, making sequential processing faster.

---

### 3. What risks arise from out-of-bounds access?

Accessing an element outside the array boundary results in **undefined behavior**. It may overwrite other memory, corrupt data, or cause program crashes.

Example:

```cpp
int arr[5];
arr[10] = 3;   // invalid access
```

---

### 4. How does pointer arithmetic relate to array indexing?

Array indexing is implemented using pointer arithmetic.

Example:

```cpp
arr[i]  ==  *(arr + i)
```

The array name represents the **base address**, and the index determines the offset.

---

# Extension Challenge

Modify the program to:

1. Sort the readings in ascending order.
2. Identify the top three highest readings.
3. Allocate the array dynamically using heap memory.

Example:

```cpp
double* readings = new double[SENSOR_COUNT];
```

Release memory after use:

```cpp
delete[] readings;
```

---

# Conclusion

Arrays provide a simple and efficient mechanism for storing fixed-size datasets. Because elements are stored in contiguous memory, arrays enable fast indexed access and efficient traversal, which are important characteristics in performance-sensitive applications.
