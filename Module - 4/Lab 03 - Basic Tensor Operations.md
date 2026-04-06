# Lab 3: Basic Tensor Operations

## Objective
Perform add, multiply, reshape, normalization operations on tensors

## Expected Outcome
Participants understand preprocessing before ML inference

---

## Duration: 60-75 minutes

## Prerequisites
- Lab 1-2 completed
- Understanding of tensor creation and shapes
- Basic linear algebra concepts (optional)

---

## Part 1: Introduction to Tensor Operations (10 minutes)

### 1.1 Why Tensor Operations Matter

Before feeding data to a machine learning model, you typically need to:

1. **Normalize**: Scale values to consistent range (usually 0-1 or -1 to 1)
2. **Reshape**: Change tensor shape without changing data
3. **Add/Multiply**: Perform element-wise operations
4. **Concatenate**: Combine multiple tensors
5. **Transpose**: Reorder dimensions

### 1.2 Common Preprocessing Steps

```
Raw Data → Reshape → Add/Multiply → Normalize → ML Model
           (change shape)  (scale values)  (0-1 range)
```

### 1.3 Real-World Example

**Image preprocessing:**
```
Image loaded: 1920×1080 pixels
↓
Resize: 224×224 pixels (reshape)
↓
Extract channels: 224×224×3 (RGB)
↓
Normalize: Divide by 255 (0-1 range)
↓
Ready for model input
```

---

## Part 2: Implement Tensor Operations (40 minutes)

Create file: `main.cpp`

```cpp
#include <iostream>
#include <tensorflow/c/c_api.h>
#include <cmath>
#include <iomanip>

using namespace std;

// Helper function to print tensor
void PrintTensor(const string& name, float* data, int size, int cols = -1) {
    cout << "\n[" << name << "]" << endl;
    if (cols == -1) {
        // 1-D print
        cout << "[";
        for (int i = 0; i < size; i++) {
            cout << fixed << setprecision(2) << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]" << endl;
    } else {
        // 2-D print
        for (int i = 0; i < size / cols; i++) {
            cout << "[";
            for (int j = 0; j < cols; j++) {
                cout << fixed << setprecision(2) << data[i * cols + j];
                if (j < cols - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
    }
}

// Operation 1: Element-wise Addition
void ElementwiseAdd(float* a, float* b, float* result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = a[i] + b[i];
    }
}

// Operation 2: Element-wise Multiplication (Hadamard Product)
void ElementwiseMultiply(float* a, float* b, float* result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = a[i] * b[i];
    }
}

// Operation 3: Scalar Multiplication
void ScalarMultiply(float* data, float scalar, float* result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = data[i] * scalar;
    }
}

// Operation 4: Normalization (Min-Max)
void NormalizeMinMax(float* data, float* result, int size) {
    // Find min and max
    float min_val = data[0], max_val = data[0];
    for (int i = 0; i < size; i++) {
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
    }
    
    float range = max_val - min_val;
    if (range == 0) range = 1;  // Avoid division by zero
    
    // Normalize: (x - min) / (max - min)
    for (int i = 0; i < size; i++) {
        result[i] = (data[i] - min_val) / range;
    }
}

// Operation 5: Normalization (Z-score)
void NormalizeZScore(float* data, float* result, int size) {
    // Calculate mean
    float sum = 0;
    for (int i = 0; i < size; i++) sum += data[i];
    float mean = sum / size;
    
    // Calculate standard deviation
    float variance = 0;
    for (int i = 0; i < size; i++) {
        variance += (data[i] - mean) * (data[i] - mean);
    }
    float stddev = sqrt(variance / size);
    if (stddev == 0) stddev = 1;  // Avoid division by zero
    
    // Normalize: (x - mean) / stddev
    for (int i = 0; i < size; i++) {
        result[i] = (data[i] - mean) / stddev;
    }
}

// Operation 6: Reshape (conceptual - just reorganize indices)
// Note: In actual TensorFlow, reshape is a graph operation
// Here we show how data is reinterpreted

void Reshape2Dto1D(float* data_2d, float* data_1d, int rows, int cols) {
    // Convert 2D (rows × cols) to 1D
    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_1d[idx++] = data_2d[i * cols + j];
        }
    }
}

void Reshape1Dto2D(float* data_1d, float* data_2d, int rows, int cols) {
    // Convert 1D to 2D (rows × cols)
    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_2d[i * cols + j] = data_1d[idx++];
        }
    }
}

// Operation 7: Transpose (2D only)
void Transpose2D(float* data, float* result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j * rows + i] = data[i * cols + j];
        }
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "Lab 3: Basic Tensor Operations" << endl;
    cout << "========================================" << endl;

    // =============================================
    // Exercise 1: Element-wise Addition
    // =============================================
    cout << "\n--- EXERCISE 1: Element-wise Addition ---" << endl;
    cout << "Scenario: Adding two sensor readings" << endl;
    
    float sensor_a[5] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f};
    float sensor_b[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    float result_add[5];
    
    ElementwiseAdd(sensor_a, sensor_b, result_add, 5);
    
    PrintTensor("Sensor A", sensor_a, 5);
    PrintTensor("Sensor B", sensor_b, 5);
    PrintTensor("Result (A + B)", result_add, 5);

    // =============================================
    // Exercise 2: Element-wise Multiplication
    // =============================================
    cout << "\n--- EXERCISE 2: Element-wise Multiplication ---" << endl;
    cout << "Scenario: Applying weight mask to sensor data" << endl;
    
    float data[5] = {100.0f, 200.0f, 300.0f, 400.0f, 500.0f};
    float weights[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
    float weighted[5];
    
    ElementwiseMultiply(data, weights, weighted, 5);
    
    PrintTensor("Raw Data", data, 5);
    PrintTensor("Weight Mask", weights, 5);
    PrintTensor("Weighted Data", weighted, 5);

    // =============================================
    // Exercise 3: Scalar Multiplication
    // =============================================
    cout << "\n--- EXERCISE 3: Scalar Multiplication ---" << endl;
    cout << "Scenario: Scaling temperature readings by 2.5" << endl;
    
    float temps[4] = {10.0f, 20.0f, 30.0f, 40.0f};
    float scaled[4];
    
    ScalarMultiply(temps, 2.5f, scaled, 4);
    
    PrintTensor("Original Temps", temps, 4);
    cout << "\nMultiplied by scalar: 2.5" << endl;
    PrintTensor("Scaled Temps", scaled, 4);

    // =============================================
    // Exercise 4: Normalization (Min-Max)
    // =============================================
    cout << "\n--- EXERCISE 4: Normalization (Min-Max) ---" << endl;
    cout << "Scenario: Normalizing image pixel values (0-255 → 0-1)" << endl;
    
    float pixels[6] = {0.0f, 64.0f, 128.0f, 192.0f, 255.0f, 127.5f};
    float normalized_minmax[6];
    
    NormalizeMinMax(pixels, normalized_minmax, 6);
    
    PrintTensor("Pixel Values (0-255)", pixels, 6);
    PrintTensor("Normalized (0-1)", normalized_minmax, 6);
    
    cout << "\nFormula: (x - min) / (max - min)" << endl;
    cout << "Min: 0, Max: 255" << endl;

    // =============================================
    // Exercise 5: Normalization (Z-score)
    // =============================================
    cout << "\n--- EXERCISE 5: Normalization (Z-score) ---" << endl;
    cout << "Scenario: Standardizing age data for ML model" << endl;
    
    float ages[5] = {25.0f, 35.0f, 45.0f, 55.0f, 65.0f};
    float normalized_zscore[5];
    
    NormalizeZScore(ages, normalized_zscore, 5);
    
    PrintTensor("Age Data", ages, 5);
    PrintTensor("Z-score Normalized", normalized_zscore, 5);
    
    cout << "\nFormula: (x - mean) / std_dev" << endl;
    cout << "Mean: 45, Std Dev: 14.14" << endl;
    cout << "Result: Values centered around 0, std = 1" << endl;

    // =============================================
    // Exercise 6: Reshape 2-D to 1-D
    // =============================================
    cout << "\n--- EXERCISE 6: Reshape (2-D → 1-D) ---" << endl;
    cout << "Scenario: Flattening image matrix for neural network" << endl;
    
    float matrix_2d[6] = {1, 2, 3, 4, 5, 6};  // 2×3 matrix
    float vector_1d[6];
    
    Reshape2Dto1D(matrix_2d, vector_1d, 2, 3);
    
    cout << "\nOriginal 2-D (2×3):" << endl;
    PrintTensor("Matrix", matrix_2d, 6, 3);
    
    cout << "\nReshaped to 1-D:" << endl;
    PrintTensor("Flattened", vector_1d, 6);

    // =============================================
    // Exercise 7: Reshape 1-D to 2-D
    // =============================================
    cout << "\n--- EXERCISE 7: Reshape (1-D → 2-D) ---" << endl;
    cout << "Scenario: Restructuring vector back to matrix form" << endl;
    
    float vector[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    float matrix[12];
    
    Reshape1Dto2D(vector, matrix, 3, 4);  // 3 rows, 4 cols
    
    cout << "\nOriginal 1-D vector:" << endl;
    PrintTensor("Vector", vector, 12);
    
    cout << "\nReshaped to 2-D (3×4):" << endl;
    PrintTensor("Matrix", matrix, 12, 4);

    // =============================================
    // Exercise 8: Transpose
    // =============================================
    cout << "\n--- EXERCISE 8: Transpose (2-D) ---" << endl;
    cout << "Scenario: Transposing a dataset matrix" << endl;
    
    float data_2d[6] = {1, 2, 3, 4, 5, 6};  // 2×3
    float transposed[6];
    
    Transpose2D(data_2d, transposed, 2, 3);
    
    cout << "\nOriginal 2×3:" << endl;
    PrintTensor("Matrix", data_2d, 6, 3);
    
    cout << "\nTransposed (3×2):" << endl;
    PrintTensor("Transposed", transposed, 6, 2);

    // =============================================
    // Exercise 9: Combined Operations
    // =============================================
    cout << "\n--- EXERCISE 9: Combined Operations ---" << endl;
    cout << "Scenario: Full preprocessing pipeline" << endl;
    cout << "Steps: Load → Normalize → Scale → Ready for ML" << endl;
    
    float raw_data[5] = {100.0f, 200.0f, 150.0f, 250.0f, 180.0f};
    float normalized[5];
    float scaled[5];
    
    // Step 1: Normalize
    cout << "\nStep 1: Normalize (Min-Max)" << endl;
    NormalizeMinMax(raw_data, normalized, 5);
    PrintTensor("Raw Data", raw_data, 5);
    PrintTensor("After Normalize", normalized, 5);
    
    // Step 2: Scale
    cout << "\nStep 2: Scale by 255 (for 0-255 range)" << endl;
    ScalarMultiply(normalized, 255.0f, scaled, 5);
    PrintTensor("After Scale", scaled, 5);

    // =============================================
    // Summary
    // =============================================
    cout << "\n========================================" << endl;
    cout << "LAB 3 COMPLETE" << endl;
    cout << "========================================" << endl;
    cout << "\nYou implemented:" << endl;
    cout << "✓ Element-wise addition" << endl;
    cout << "✓ Element-wise multiplication" << endl;
    cout << "✓ Scalar multiplication" << endl;
    cout << "✓ Min-Max normalization (0-1 range)" << endl;
    cout << "✓ Z-score normalization (mean=0, std=1)" << endl;
    cout << "✓ 2-D → 1-D reshape (flatten)" << endl;
    cout << "✓ 1-D → 2-D reshape (unflatten)" << endl;
    cout << "✓ 2-D transpose" << endl;
    cout << "✓ Combined preprocessing pipeline" << endl;

    return 0;
}
```

---

## Part 3: Normalization Techniques (15 minutes)

### 3.1 Min-Max Normalization (0-1 Range)

**Formula:** `x_normalized = (x - min) / (max - min)`

**Use case:** Image pixels, bounded data

```
Example: [0, 128, 255]
Min: 0, Max: 255, Range: 255

0 → (0 - 0) / 255 = 0.0
128 → (128 - 0) / 255 = 0.50
255 → (255 - 0) / 255 = 1.0

Result: [0.0, 0.5, 1.0]
```

### 3.2 Z-Score Normalization (Standardization)

**Formula:** `x_normalized = (x - mean) / std_dev`

**Use case:** General ML, unbounded data

```
Example: [25, 35, 45, 55, 65]
Mean: 45
Std Dev: 14.14

Result: [-1.41, -0.71, 0, 0.71, 1.41]
```

### 3.3 When to Use Which

| Normalization | Range | Use Case |
|---------------|-------|----------|
| Min-Max | [0, 1] | Images, percentages |
| Min-Max | [-1, 1] | Tanh activation |
| Z-Score | (-∞, +∞) | General ML, Gaussian |
| Log | [0, log(max)] | Skewed data |

---

## Part 4: Build and Run (10 minutes)

### 4.1 Configure Project

Same as Lab 1-2:
- Include directories configured
- Library paths configured
- Libraries linked
- DLLs copied

### 4.2 Build and Run

```
Ctrl+Shift+B  → Build
Ctrl+F5       → Run
```

### 4.3 Expected Output

```
========================================
Lab 3: Basic Tensor Operations
========================================

--- EXERCISE 1: Element-wise Addition ---
Scenario: Adding two sensor readings

[Sensor A]
[10.00, 20.00, 30.00, 40.00, 50.00]

[Sensor B]
[1.00, 2.00, 3.00, 4.00, 5.00]

[Result (A + B)]
[11.00, 22.00, 33.00, 44.00, 55.00]

--- EXERCISE 2: Element-wise Multiplication ---
...
```

---

## Exercises

### Exercise 1: Batch Normalization

**Task:** Implement batch normalization

```cpp
void BatchNormalize(float* data, int batch_size, 
                   float* result) {
    // Calculate mean per sample
    // Calculate std per sample
    // Normalize each sample
}
```

### Exercise 2: Matrix Operations

**Task:** Implement matrix addition (2-D + 2-D)

```cpp
void MatrixAdd(float* a, float* b, float* result,
              int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        result[i] = a[i] + b[i];
    }
}
```

### Exercise 3: Data Pipeline

**Task:** Build complete preprocessing pipeline:

```
Raw Data → Min-Max Norm → Z-Score Norm → Scale → Output
```

---

## Verification Checklist

- [ ] Project compiles without errors
- [ ] All 9 exercises run correctly
- [ ] Element-wise addition works
- [ ] Element-wise multiplication works
- [ ] Scalar multiplication works
- [ ] Min-Max normalization works
- [ ] Z-score normalization works
- [ ] 2-D → 1-D reshape works
- [ ] 1-D → 2-D reshape works
- [ ] Transpose works

---

## Learning Outcomes

By completing this lab, you should:

✅ Understand basic tensor operations
✅ Know when to normalize and why
✅ Understand Min-Max vs Z-score normalization
✅ Know how to reshape tensors
✅ Understand preprocessing pipelines
✅ Be ready for ML model input

---

## Next Lab

Lab 4: Intro to Model Inference (load and run a trained model)

---

## Time Allocation

```
Part 1: Introduction             10 minutes
Part 2: Implement Operations     40 minutes
Part 3: Normalization            15 minutes
Part 4: Build and Run            10 minutes
Exercises (optional)             15+ minutes
─────────────────────────────────
Total:                           60-75 minutes
```

Excellent! You now understand how to preprocess data for ML! 🎉
