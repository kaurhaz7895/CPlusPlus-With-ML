# Lab 4: Multidimensional Arrays Using Device Activity Data

## 🎯 Objectives

After completing this lab, participants should be able to:

- Declare and use **two-dimensional arrays**
- Store and process **device activity data**
- Traverse arrays using **nested loops**
- Understand **row-major memory layout**
- Perform basic analytics such as **totals and averages**

---

# Scenario

Smart garage door openers collect operational data such as the **number of door activations per day**.

Each device records activity over several days.

Data model:

- **Rows → Devices**
- **Columns → Days**

Your program will store and analyze device activity using a **two-dimensional array**.

---

# Background

A two-dimensional array represents **tabular data**.

Example:

```cpp
int activity[3][5];
```

Meaning:

- 3 devices
- 5 days of activity data

Total elements:

```
3 × 5 = 15 values
```

Example table representation:

| Device | Day1 | Day2 | Day3 | Day4 | Day5 |
|------|------|------|------|------|------|
| D0 | | | | | |
| D1 | | | | | |
| D2 | | | | | |

---

# Procedure

## Step 1 — Declare the Activity Matrix

```cpp
#include <iostream>
using namespace std;

int main()
{
    const int DEVICES = 3;
    const int DAYS = 5;

    int activity[DEVICES][DAYS];

    return 0;
}
```

---

## Step 2 — Input Device Activity Data

```cpp
for(int device = 0; device < DEVICES; device++)
{
    for(int day = 0; day < DAYS; day++)
    {
        cout << "Enter activations for device "
             << device << " day "
             << day+1 << ": ";

        cin >> activity[device][day];
    }
}
```

---

## Step 3 — Display Activity Table

```cpp
cout << "\nDevice Activity Table\n";

for(int device = 0; device < DEVICES; device++)
{
    for(int day = 0; day < DAYS; day++)
    {
        cout << activity[device][day] << " ";
    }

    cout << endl;
}
```

Example output:

```
12 15 10 18 14
20 22 19 25 23
8  10 9  12 11
```

---

## Step 4 — Calculate Total Activations

```cpp
int total = 0;

for(int device = 0; device < DEVICES; device++)
{
    for(int day = 0; day < DAYS; day++)
    {
        total += activity[device][day];
    }
}

cout << "Total activations across all devices: "
     << total << endl;
```

---

## Step 5 — Find Most Active Device

```cpp
for(int device = 0; device < DEVICES; device++)
{
    int deviceTotal = 0;

    for(int day = 0; day < DAYS; day++)
    {
        deviceTotal += activity[device][day];
    }

    cout << "Device " << device
         << " total activations: "
         << deviceTotal << endl;
}
```

---

# Memory Insight

Multidimensional arrays are stored in **row-major order**.

Example layout:

```
activity[0][0]
activity[0][1]
activity[0][2]
activity[0][3]
activity[0][4]
activity[1][0]
...
```

You can observe addresses using:

```cpp
cout << &activity[device][day] << endl;
```

This demonstrates how rows are stored **sequentially in memory**.

---

# Analysis Questions

1. Why are nested loops required to process device activity data?
2. What advantage does row-major memory layout provide?
3. If the array is declared as `int activity[4][7]`, how many values can it store?
4. How might this data structure be used in a real IoT monitoring system?

---

# Sample Answers

### 1. Why are nested loops required?

Each device has multiple days of data. One loop iterates through devices and the inner loop processes daily activity.

---

### 2. What advantage does row-major layout provide?

It keeps each device's data contiguous in memory, improving cache performance when processing activity sequentially.

---

### 3. How many values does `int activity[4][7]` store?

```
4 × 7 = 28 values
```

---

### 4. How might this structure be used in an IoT system?

It can store device telemetry such as door activations, battery status logs, or error counts collected over time.

---

# Extension Challenge

Modify the program to:

1. Identify the **day with the highest total activations**.
2. Detect **abnormally high activity** for any device.
3. Convert the array to a **dynamically allocated 2-D array**.

---

# Conclusion

Multidimensional arrays are useful for representing structured operational data such as device telemetry. In systems handling multiple devices and time-based measurements, they provide an efficient way to store and process tabular datasets.
