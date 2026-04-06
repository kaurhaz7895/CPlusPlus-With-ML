# Lab 2: Tensor Fundamentals in C++

## Objective
Create tensors, inspect shapes, and understand data types for representing structured data

## Expected Outcome
Participants can represent structured data (sensor arrays, telemetry, images) using tensors

---

## Duration: 45-60 minutes

## Prerequisites
- Lab 1 completed (TensorFlow setup verified)
- Understanding of arrays and data types
- Basic C++ knowledge

---

## Part 1: Understanding Tensor Shapes (15 minutes)

### 1.1 Concept: What is a Tensor Shape?

A tensor shape describes how data is organized:

```
0-D Tensor (Scalar):
  Shape: ()
  Example: 42

1-D Tensor (Vector):
  Shape: (5)
  Example: [10, 20, 30, 40, 50]

2-D Tensor (Matrix):
  Shape: (3, 4)
  Example: [[1,  2,  3,  4],
            [5,  6,  7,  8],
            [9, 10, 11, 12]]

3-D Tensor (Cube):
  Shape: (2, 3, 4)
  Example: Stack of 2 matrices, each 3x4

4-D Tensor (Batch of images):
  Shape: (32, 224, 224, 3)
  Example: 32 images, 224×224 pixels, 3 color channels
```

### 1.2 Tensor Shape in C API

```cpp
// 1-D tensor: 5 elements
int64_t dims_1d[1] = {5};
int num_dims = 1;

// 2-D tensor: 3 rows × 4 columns
int64_t dims_2d[2] = {3, 4};
int num_dims = 2;

// 3-D tensor: 2 × 3 × 4
int64_t dims_3d[3] = {2, 3, 4};
int num_dims = 3;

// Total elements = product of all dimensions
// 1D: 5 elements
// 2D: 3 × 4 = 12 elements
// 3D: 2 × 3 × 4 = 24 elements
```

### 1.3 Real-World Examples

```
Sensor Array (1-D):
  Shape: (10)
  Meaning: 10 temperature sensors

Image (3-D):
  Shape: (480, 640, 3)
  Meaning: 480 height × 640 width × 3 RGB channels

Batch of Images (4-D):
  Shape: (32, 224, 224, 3)
  Meaning: 32 images, each 224×224 with 3 colors

Time Series (2-D):
  Shape: (100, 5)
  Meaning: 100 timesteps, 5 features per step
```

---

## Part 2: Create Tensors of Different Shapes (20 minutes)

### 2.1 Complete Lab Program

Create file: `main.cpp`

```cpp
#include <iostream>
#include <tensorflow/c/c_api.h>

using namespace std;

// Helper function to print tensor info
void PrintTensorInfo(const string& name, TF_Tensor* tensor) {
    cout << "\n[Tensor: " << name << "]" << endl;
    cout << "Data Type: ";
    
    TF_DataType type = TF_TensorType(tensor);
    switch(type) {
        case TF_FLOAT: cout << "TF_FLOAT (32-bit)"; break;
        case TF_DOUBLE: cout << "TF_DOUBLE (64-bit)"; break;
        case TF_INT32: cout << "TF_INT32 (32-bit integer)"; break;
        case TF_INT64: cout << "TF_INT64 (64-bit integer)"; break;
        case TF_UINT8: cout << "TF_UINT8 (8-bit unsigned, 0-255)"; break;
        default: cout << "UNKNOWN"; break;
    }
    cout << endl;
    
    // Number of dimensions
    int num_dims = TF_NumDims(tensor);
    cout << "Number of Dimensions: " << num_dims << endl;
    
    // Shape
    cout << "Shape: (";
    int64_t num_elements = 1;
    for (int i = 0; i < num_dims; i++) {
        cout << TF_Dim(tensor, i);
        if (i < num_dims - 1) cout << ", ";
        num_elements *= TF_Dim(tensor, i);
    }
    cout << ")" << endl;
    
    // Total elements
    cout << "Total Elements: " << num_elements << endl;
    
    // Byte size
    cout << "Byte Size: " << TF_TensorByteSize(tensor) << " bytes" << endl;
}

int main() {
    cout << "=====================================" << endl;
    cout << "Lab 2: Tensor Fundamentals in C++" << endl;
    cout << "=====================================" << endl;

    // =============================================
    // Exercise 1: Create 1-D Tensor (Sensor Array)
    // =============================================
    cout << "\n--- EXERCISE 1: 1-D Tensor (Sensor Array) ---" << endl;
    cout << "Scenario: Array of 5 temperature sensors reading values" << endl;
    
    float sensor_data[5] = {25.5f, 26.2f, 24.8f, 27.1f, 25.9f};
    int64_t dims_1d[1] = {5};
    
    TF_Tensor* tensor_1d = TF_NewTensor(
        TF_FLOAT, dims_1d, 1, 
        sensor_data, sizeof(sensor_data),
        [](void* data, size_t, void*) {}, nullptr
    );
    
    PrintTensorInfo("Sensor Readings", tensor_1d);
    
    cout << "Values: [";
    float* data_1d = static_cast<float*>(TF_TensorData(tensor_1d));
    for (int i = 0; i < 5; i++) {
        cout << data_1d[i];
        if (i < 4) cout << ", ";
    }
    cout << "]" << endl;
    TF_DeleteTensor(tensor_1d);

    // =============================================
    // Exercise 2: Create 2-D Tensor (Matrix)
    // =============================================
    cout << "\n--- EXERCISE 2: 2-D Tensor (Matrix) ---" << endl;
    cout << "Scenario: Student grades table (3 students × 4 subjects)" << endl;
    
    float grades[12] = {
        95.0f, 88.0f, 92.0f, 85.0f,  // Student 1
        88.0f, 92.0f, 90.0f, 88.0f,  // Student 2
        90.0f, 85.0f, 88.0f, 92.0f   // Student 3
    };
    int64_t dims_2d[2] = {3, 4};  // 3 rows, 4 columns
    
    TF_Tensor* tensor_2d = TF_NewTensor(
        TF_FLOAT, dims_2d, 2,
        grades, sizeof(grades),
        [](void* data, size_t, void*) {}, nullptr
    );
    
    PrintTensorInfo("Student Grades", tensor_2d);
    
    cout << "Matrix (3×4):" << endl;
    float* data_2d = static_cast<float*>(TF_TensorData(tensor_2d));
    for (int row = 0; row < 3; row++) {
        cout << "  Student " << (row+1) << ": [";
        for (int col = 0; col < 4; col++) {
            cout << data_2d[row * 4 + col];
            if (col < 3) cout << ", ";
        }
        cout << "]" << endl;
    }
    TF_DeleteTensor(tensor_2d);

    // =============================================
    // Exercise 3: Create 3-D Tensor (Stack of Matrices)
    // =============================================
    cout << "\n--- EXERCISE 3: 3-D Tensor (Time Series) ---" << endl;
    cout << "Scenario: Weekly temperature readings (7 days × 3 sensors × 24 hours)" << endl;
    
    int total_3d = 7 * 3 * 24;  // 504 elements
    float* temp_data = new float[total_3d];
    
    // Fill with sample data (just incrementing values for demo)
    for (int i = 0; i < total_3d; i++) {
        temp_data[i] = 20.0f + (i % 10);  // Values between 20-30
    }
    
    int64_t dims_3d[3] = {7, 3, 24};  // days, sensors, hours
    
    TF_Tensor* tensor_3d = TF_NewTensor(
        TF_FLOAT, dims_3d, 3,
        temp_data, total_3d * sizeof(float),
        [](void* data, size_t, void*) { delete[](float*)data; },  // Custom deallocator
        nullptr
    );
    
    PrintTensorInfo("Weekly Temperature", tensor_3d);
    
    cout << "Shape breakdown:" << endl;
    cout << "  Dimension 0 (Days): " << TF_Dim(tensor_3d, 0) << endl;
    cout << "  Dimension 1 (Sensors): " << TF_Dim(tensor_3d, 1) << endl;
    cout << "  Dimension 2 (Hours): " << TF_Dim(tensor_3d, 2) << endl;
    
    TF_DeleteTensor(tensor_3d);

    // =============================================
    // Exercise 4: Image Tensor (4-D)
    // =============================================
    cout << "\n--- EXERCISE 4: 4-D Tensor (Image) ---" << endl;
    cout << "Scenario: Single RGB image (Height × Width × Channels)" << endl;
    
    // Small 4×4 RGB image
    int height = 4, width = 4, channels = 3;
    int total_pixels = height * width * channels;
    uint8_t* image_data = new uint8_t[total_pixels];
    
    // Fill with pattern (just for demo)
    for (int i = 0; i < total_pixels; i++) {
        image_data[i] = (i % 256);  // Values 0-255
    }
    
    int64_t dims_4d[3] = {height, width, channels};  // Height, Width, Channels
    
    TF_Tensor* tensor_4d = TF_NewTensor(
        TF_UINT8, dims_4d, 3,
        image_data, total_pixels * sizeof(uint8_t),
        [](void* data, size_t, void*) { delete[](uint8_t*)data; },
        nullptr
    );
    
    PrintTensorInfo("4×4 RGB Image", tensor_4d);
    
    cout << "Image dimensions:" << endl;
    cout << "  Height: " << TF_Dim(tensor_4d, 0) << " pixels" << endl;
    cout << "  Width: " << TF_Dim(tensor_4d, 1) << " pixels" << endl;
    cout << "  Channels: " << TF_Dim(tensor_4d, 2) << " (RGB)" << endl;
    
    TF_DeleteTensor(tensor_4d);

    // =============================================
    // Exercise 5: Different Data Types
    // =============================================
    cout << "\n--- EXERCISE 5: Different Data Types ---" << endl;
    
    // INT32 (integer data)
    cout << "\nCreating INT32 tensor (age data):" << endl;
    int32_t ages[5] = {25, 30, 35, 40, 45};
    int64_t dims_int[1] = {5};
    
    TF_Tensor* tensor_int = TF_NewTensor(
        TF_INT32, dims_int, 1,
        ages, sizeof(ages),
        [](void* data, size_t, void*) {}, nullptr
    );
    PrintTensorInfo("Age Data", tensor_int);
    TF_DeleteTensor(tensor_int);
    
    // UINT8 (image pixel data)
    cout << "\nCreating UINT8 tensor (pixel intensities):" << endl;
    uint8_t pixels[5] = {0, 64, 128, 192, 255};
    int64_t dims_uint[1] = {5};
    
    TF_Tensor* tensor_uint = TF_NewTensor(
        TF_UINT8, dims_uint, 1,
        pixels, sizeof(pixels),
        [](void* data, size_t, void*) {}, nullptr
    );
    PrintTensorInfo("Pixel Intensities", tensor_uint);
    TF_DeleteTensor(tensor_uint);
    
    // DOUBLE (high precision)
    cout << "\nCreating DOUBLE tensor (precise measurements):" << endl;
    double measurements[3] = {3.14159265, 2.71828183, 1.41421356};
    int64_t dims_double[1] = {3};
    
    TF_Tensor* tensor_double = TF_NewTensor(
        TF_DOUBLE, dims_double, 1,
        measurements, sizeof(measurements),
        [](void* data, size_t, void*) {}, nullptr
    );
    PrintTensorInfo("Precise Measurements", tensor_double);
    TF_DeleteTensor(tensor_double);

    // =============================================
    // Summary
    // =============================================
    cout << "\n=====================================" << endl;
    cout << "LAB 2 COMPLETE" << endl;
    cout << "=====================================" << endl;
    cout << "\nYou learned:" << endl;
    cout << "✓ How to create 1-D, 2-D, 3-D, and 4-D tensors" << endl;
    cout << "✓ How to inspect tensor properties (shape, type, size)" << endl;
    cout << "✓ How to represent real-world data as tensors:" << endl;
    cout << "  - Sensor readings (1-D)" << endl;
    cout << "  - Matrices/tables (2-D)" << endl;
    cout << "  - Time series (3-D)" << endl;
    cout << "  - Images (3-D or 4-D)" << endl;
    cout << "✓ How to use different data types" << endl;
    cout << "✓ How to properly manage memory with deallocators" << endl;

    return 0;
}
```

---

## Part 3: Data Type Reference (10 minutes)

### 3.1 TensorFlow Data Types

| Type | C++ | Size | Use Case |
|------|-----|------|----------|
| `TF_FLOAT` | `float` | 4 bytes | Neural network weights, activations |
| `TF_DOUBLE` | `double` | 8 bytes | High precision calculations |
| `TF_INT32` | `int32_t` | 4 bytes | Labels, counts, IDs |
| `TF_INT64` | `int64_t` | 8 bytes | Large indices, large counts |
| `TF_UINT8` | `uint8_t` | 1 byte | Image pixels (0-255) |
| `TF_INT8` | `int8_t` | 1 byte | Quantized models |
| `TF_BOOL` | `bool` | 1 byte | Masks, flags |
| `TF_HALF` | Half | 2 bytes | Memory efficient models |

### 3.2 Memory Calculation

```cpp
// Total bytes = Product of dimensions × Size per element

// 1-D: 5 float values
// Bytes = 5 × 4 = 20 bytes

// 2-D: 3×4 float matrix
// Bytes = 3 × 4 × 4 = 48 bytes

// 3-D: 7×3×24 float array
// Bytes = 7 × 3 × 24 × 4 = 2016 bytes

// 4-D: 32×224×224×3 image batch (UINT8)
// Bytes = 32 × 224 × 224 × 3 × 1 = 4,793,344 bytes ≈ 4.7 MB
```

---

## Part 4: Build and Run (5 minutes)

### 4.1 Configure Project (Same as Lab 1)

- Include directories: Point to TensorFlow include
- Library directories: Point to TensorFlow lib
- Additional Dependencies: Add `tensorflow.lib`
- Copy DLL files or set PATH

### 4.2 Build and Run

```
Ctrl+Shift+B  → Build
Ctrl+F5       → Run
```

### 4.3 Expected Output

```
=====================================
Lab 2: Tensor Fundamentals in C++
=====================================

--- EXERCISE 1: 1-D Tensor (Sensor Array) ---
Scenario: Array of 5 temperature sensors reading values

[Tensor: Sensor Readings]
Data Type: TF_FLOAT (32-bit)
Number of Dimensions: 1
Shape: (5)
Total Elements: 5
Byte Size: 20 bytes
Values: [25.5, 26.2, 24.8, 27.1, 25.9]

--- EXERCISE 2: 2-D Tensor (Matrix) ---
[Tensor: Student Grades]
Data Type: TF_FLOAT (32-bit)
Number of Dimensions: 2
Shape: (3, 4)
Total Elements: 12
Byte Size: 48 bytes
Matrix (3×4):
  Student 1: [95, 88, 92, 85]
  Student 2: [88, 92, 90, 88]
  Student 3: [90, 85, 88, 92]

... (more output for exercises 3, 4, 5)
```

---

## Exercises

### Exercise 1: Modify Tensor Data

**Task:** After creating a tensor, modify its values:

```cpp
float* data = static_cast<float*>(TF_TensorData(tensor));
data[0] = 999.0f;  // Change first value
```

### Exercise 2: Calculate Statistics

**Task:** Write a function to calculate mean/max/min:

```cpp
void CalculateStats(TF_Tensor* tensor) {
    float* data = static_cast<float*>(TF_TensorData(tensor));
    int total = TF_TensorByteSize(tensor) / sizeof(float);
    
    float sum = 0, max_val = data[0], min_val = data[0];
    for (int i = 0; i < total; i++) {
        sum += data[i];
        if (data[i] > max_val) max_val = data[i];
        if (data[i] < min_val) min_val = data[i];
    }
    
    cout << "Mean: " << (sum / total) << endl;
    cout << "Max: " << max_val << endl;
    cout << "Min: " << min_val << endl;
}
```

### Exercise 3: Real Image Data

**Task:** Load an actual image and represent it as a tensor

---

## Verification Checklist

- [ ] Project compiles without errors
- [ ] All 5 exercises run successfully
- [ ] Can create 1-D tensors
- [ ] Can create 2-D tensors
- [ ] Can create 3-D tensors
- [ ] Can create 4-D tensors
- [ ] Can access tensor data
- [ ] Understand tensor shapes
- [ ] Understand different data types
- [ ] Memory management correct

---

## Learning Outcomes

By completing this lab, you should:

✅ Understand tensor shapes (0-D, 1-D, 2-D, 3-D, 4-D)
✅ Know how to calculate total elements and bytes
✅ Understand when to use different data types
✅ Know how to access tensor data
✅ Understand real-world tensor representations
✅ Be comfortable with TensorFlow C API tensors

---

## Next Lab

Lab 3: Basic Tensor Operations (add, multiply, reshape, normalize)

---

## Time Allocation

```
Part 1: Understanding Shapes      15 minutes
Part 2: Create Tensors            20 minutes
Part 3: Data Type Reference       10 minutes
Part 4: Build and Run              5 minutes
Exercises (optional)               10+ minutes
─────────────────────────────────
Total:                            45-60 minutes
```

Excellent work! You now understand how to structure data as tensors! 🎉
