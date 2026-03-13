# Lab 5: Dynamic Two-Dimensional Arrays for Device Telemetry

## 🎯 Objectives

After completing this lab, participants should be able to:

- Allocate **two-dimensional arrays dynamically on the heap**
- Store **device telemetry data collected over time**
- Process dynamically allocated matrices
- Release heap memory correctly
- Understand differences between **static and dynamic 2-D arrays**

---

# Scenario

Smart garage door openers transmit **daily activity telemetry** such as the number of door open/close events.

However, the number of devices being monitored may change depending on deployment size.

Your program must:

1. Ask the user for the number of devices and days
2. Allocate memory dynamically
3. Store telemetry data
4. Analyze device activity
5. Release memory properly

---

# Background

Previously we used **fixed multidimensional arrays**:

```cpp
int activity[3][5];
```

Limitations:

- Size must be known at **compile time**
- Not flexible for varying deployments

Dynamic allocation allows arrays to be created **during program execution**.

Example:

```cpp
int** activity = new int*[devices];
```

Each row is allocated separately.

---

# Procedure

## Step 1 — Allocate the Dynamic Matrix

```cpp
#include <iostream>
using namespace std;

int main()
{
    int devices;
    int days;

    cout << "Enter number of devices: ";
    cin >> devices;

    cout << "Enter number of days: ";
    cin >> days;

    int** activity = new int*[devices];

    for(int i = 0; i < devices; i++)
        activity[i] = new int[days];

    cout << "Memory allocated successfully." << endl;

    return 0;
}
```

---

## Step 2 — Input Telemetry Data

```cpp
for(int device = 0; device < devices; device++)
{
    for(int day = 0; day < days; day++)
    {
        cout << "Enter activations for device "
             << device << " day "
             << day+1 << ": ";

        cin >> activity[device][day];
    }
}
```

---

## Step 3 — Display the Telemetry Table

```cpp
cout << "\nDevice Telemetry Data\n";

for(int device = 0; device < devices; device++)
{
    for(int day = 0; day < days; day++)
    {
        cout << activity[device][day] << " ";
    }

    cout << endl;
}
```

---

## Step 4 — Compute Total Activations Per Device

```cpp
for(int device = 0; device < devices; device++)
{
    int deviceTotal = 0;

    for(int day = 0; day < days; day++)
    {
        deviceTotal += activity[device][day];
    }

    cout << "Device " << device
         << " total activations: "
         << deviceTotal << endl;
}
```

---

## Step 5 — Identify the Most Active Device

```cpp
int maxDevice = 0;
int maxActivity = 0;

for(int device = 0; device < devices; device++)
{
    int deviceTotal = 0;

    for(int day = 0; day < days; day++)
        deviceTotal += activity[device][day];

    if(deviceTotal > maxActivity)
    {
        maxActivity = deviceTotal;
        maxDevice = device;
    }
}

cout << "Most active device: " << maxDevice << endl;
```

---

## Step 6 — Release Heap Memory

All dynamically allocated memory must be freed.

```cpp
for(int i = 0; i < devices; i++)
    delete[] activity[i];

delete[] activity;
```

Failure to do this may result in **memory leaks**.

---

# Memory Layout Insight

In this allocation model:

```
activity
  ↓
+-----+     +-----+-----+-----+
| ptr | →   | d0  | d1  | d2  |
+-----+     +-----+-----+-----+

| ptr | →   | d0  | d1  | d2  |
+-----+     +-----+-----+-----+
```

Important characteristics:

- Rows are allocated **independently**
- Memory may **not be contiguous**
- Allows flexible row sizes

---

# Analysis Questions

1. Why must each row of the matrix be allocated separately?
2. What happens if `delete[] activity[i]` is not called?
3. How does this allocation differ from a fixed 2-D array?
4. Why might dynamic allocation be necessary in real telemetry systems?

---

# Sample Answers

### 1. Why must each row be allocated separately?

Because `activity` stores pointers to rows. Each pointer must reference a separate block of memory allocated for that row.

---

### 2. What happens if `delete[] activity[i]` is not called?

The program will leak memory because the allocated row blocks remain reserved in the heap.

---

### 3. How does this differ from a fixed 2-D array?

Fixed arrays allocate a **single contiguous block**, while this method allocates **multiple blocks**.

---

### 4. Why is dynamic allocation useful in telemetry systems?

The number of devices or monitoring days may vary across deployments, requiring flexible memory allocation during runtime.

---

# Extension Challenge

Modify the program to:

1. Detect days with **unusually high activity**.
2. Compute **average activations per device**.
3. Print the **memory addresses** of each row pointer to observe allocation patterns.

Example:

```cpp
cout << activity[i] << endl;
```

---

# Resizing Dynamic Arrays

Dynamic arrays created with `new` have a **fixed size after allocation**.

Example:

```cpp
int* arr = new int[5];
```

The array cannot be resized directly. To increase its size, the program must:

1. Allocate a new larger array
2. Copy existing elements
3. Delete the old array
4. Update the pointer

Example:

```cpp
int oldSize = 5;
int newSize = 10;

int* oldArray = new int[oldSize];

int* newArray = new int[newSize];

for(int i = 0; i < oldSize; i++)
{
    newArray[i] = oldArray[i];
}

delete[] oldArray;

oldArray = newArray;
```

This process is sometimes referred to as **manual reallocation**.

---

# Why Modern C++ Uses `std::vector`

Because manual resizing is error-prone and inefficient, modern C++ programs typically use **dynamic containers**.

Example:

```cpp
#include <vector>

std::vector<int> readings;

readings.push_back(10);
readings.push_back(20);
readings.push_back(30);
```

Advantages:

- Automatic resizing
- Memory management handled internally
- Safer than raw pointers

Internally, `std::vector` performs operations similar to manual reallocation when capacity is exceeded.

---

# Note on `realloc` from C

In the C programming language, memory can be resized using:

```c
arr = realloc(arr, new_size);
```

However, this function is **not compatible with memory allocated using `new` in C++**, because C++ objects may require constructors and destructors.

For this reason, C++ encourages the use of **containers like `std::vector` instead of raw dynamic arrays**.

# Conclusion

Dynamic two-dimensional arrays allow programs to manage datasets whose size is unknown until runtime. Proper memory allocation and deallocation are essential for building scalable systems that process telemetry and operational data efficiently.
