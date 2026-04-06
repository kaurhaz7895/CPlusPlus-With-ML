# Lab 4: Introduction to Model Inference

## Objective
Load a trained model and run single prediction

## Expected Outcome
Participants understand full inference pipeline from data to prediction

---

## Duration: 60-90 minutes

## Prerequisites
- Labs 1-3 completed
- Understanding of tensors and operations
- Python knowledge (to create test model)
- Access to Python environment (optional)

---

## Part 1: Understanding Model Inference (15 minutes)

### 1.1 What is Model Inference?

**Inference** = Using a trained model to make predictions on new data

```
Training Phase (Python):           Inference Phase (C++):
=====================================  =========================
Raw Data                          Input Data (preprocessed)
    ↓                                  ↓
Preprocessing                     Create Tensor
    ↓                                  ↓
Train Model                       Load Saved Model
    ↓                                  ↓
Save Model (model.pb)             Run Inference
    ↓                                  ↓
Evaluate                          Get Predictions
    ↓                                  ↓
Production Ready                  Return Results
```

### 1.2 Inference Pipeline

```
1. Load Data
   ↓
2. Create Input Tensor
   ↓
3. Load Pre-trained Model
   ↓
4. Run Inference (Forward Pass)
   ↓
5. Get Output Tensor
   ↓
6. Interpret Results
```

### 1.3 Why Do This in C++?

- **Performance:** C++ is faster than Python
- **Production:** Deploy without Python dependency
- **Integration:** Embed in C++ applications
- **Real-time:** Low latency for time-sensitive tasks
- **Deployment:** Edge devices, servers, embedded systems

---

## Part 2: Create a Test Model (20 minutes)

### 2.1 Create Model in Python (If You Have Python)

**File: `create_model.py`**

```python
import tensorflow as tf
import numpy as np

# Create a simple neural network
model = tf.keras.Sequential([
    tf.keras.layers.Dense(10, activation='relu', input_shape=(2,)),
    tf.keras.layers.Dense(5, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

# Compile model
model.compile(optimizer='adam',
              loss='binary_crossentropy',
              metrics=['accuracy'])

# Create dummy training data
X = np.random.randn(100, 2)
y = np.random.randint(0, 2, 100)

# Train
model.fit(X, y, epochs=10, verbose=0)

# Save model
model.save('simple_model.h5')
print("Model saved as simple_model.h5")

# Convert to SavedModel format (TensorFlow format)
tf.saved_model.save(model, 'simple_model')
print("Model saved as SavedModel format in 'simple_model' folder")

# Also test inference
test_input = np.array([[0.5, -0.3]], dtype=np.float32)
prediction = model.predict(test_input)
print(f"Test prediction: {prediction[0][0]:.4f}")
```

**Run:**
```cmd
python create_model.py
```

### 2.2 If You Don't Have Python

**Alternative: Use Pre-trained Models**

You can download pre-trained TensorFlow models from:
- TensorFlow Hub: https://tfhub.dev/
- ONNX Model Zoo: https://github.com/onnx/models
- Kaggle Models: https://www.kaggle.com/models

For this lab, we'll create a simplified C++ model simulation.

---

## Part 3: Implement Model Inference in C++ (30 minutes)

Create file: `main.cpp`

```cpp
#include <iostream>
#include <fstream>
#include <tensorflow/c/c_api.h>
#include <vector>

using namespace std;

// =============================================
// NOTE: TensorFlow C API is very low-level
// For production, use LibTorch or TensorFlow C++ API
// This lab demonstrates CONCEPTS using C API
// =============================================

// Simulated trained model weights
// In reality, these would be loaded from a saved model
struct SimpleModel {
    // Layer 1: 2 inputs → 3 hidden neurons
    float weight1[6] = {
        0.5f, -0.3f,   // Neuron 1 weights
        -0.2f, 0.7f,   // Neuron 2 weights
        0.4f, 0.1f     // Neuron 3 weights
    };
    float bias1[3] = {0.1f, -0.2f, 0.05f};
    
    // Layer 2: 3 hidden → 1 output
    float weight2[3] = {0.6f, -0.4f, 0.3f};
    float bias2[1] = {0.2f};
    
    // ReLU activation
    float ReLU(float x) {
        return x > 0 ? x : 0;
    }
    
    // Sigmoid activation
    float Sigmoid(float x) {
        return 1.0f / (1.0f + exp(-x));
    }
    
    // Forward pass (inference)
    float Predict(float* input) {
        // Layer 1: 2 inputs → 3 hidden neurons with ReLU
        float hidden[3];
        for (int i = 0; i < 3; i++) {
            hidden[i] = bias1[i];
            for (int j = 0; j < 2; j++) {
                hidden[i] += input[j] * weight1[i * 2 + j];
            }
            hidden[i] = ReLU(hidden[i]);
        }
        
        // Layer 2: 3 hidden → 1 output with Sigmoid
        float output = bias2[0];
        for (int i = 0; i < 3; i++) {
            output += hidden[i] * weight2[i];
        }
        output = Sigmoid(output);
        
        return output;
    }
};

// Helper function to create input tensor
TF_Tensor* CreateInputTensor(float* data, int size) {
    int64_t dims[1] = {size};
    
    float* tensor_data = new float[size];
    for (int i = 0; i < size; i++) {
        tensor_data[i] = data[i];
    }
    
    TF_Tensor* tensor = TF_NewTensor(
        TF_FLOAT, dims, 1, tensor_data, size * sizeof(float),
        [](void* data, size_t, void*) { delete[](float*)data; },
        nullptr
    );
    
    return tensor;
}

// Helper function to print tensor
void PrintTensor(const string& name, TF_Tensor* tensor) {
    cout << "\n[" << name << "]" << endl;
    cout << "Shape: (";
    for (int i = 0; i < TF_NumDims(tensor); i++) {
        cout << TF_Dim(tensor, i);
        if (i < TF_NumDims(tensor) - 1) cout << ", ";
    }
    cout << ")" << endl;
    
    cout << "Values: [";
    float* data = static_cast<float*>(TF_TensorData(tensor));
    for (int i = 0; i < TF_TensorByteSize(tensor) / sizeof(float); i++) {
        cout << fixed << setprecision(4) << data[i];
        if (i < (TF_TensorByteSize(tensor) / sizeof(float) - 1)) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "Lab 4: Introduction to Model Inference" << endl;
    cout << "========================================\n" << endl;

    // =============================================
    // Exercise 1: Create Input Data
    // =============================================
    cout << "--- EXERCISE 1: Create Input Data ---" << endl;
    cout << "Scenario: Binary classification input (2 features)" << endl;
    
    float sample1[2] = {0.5f, -0.3f};
    float sample2[2] = {-0.2f, 0.7f};
    float sample3[2] = {0.4f, 0.1f};
    
    cout << "\nInput Sample 1: [" << sample1[0] << ", " << sample1[1] << "]" << endl;
    cout << "Input Sample 2: [" << sample2[0] << ", " << sample2[1] << "]" << endl;
    cout << "Input Sample 3: [" << sample3[0] << ", " << sample3[1] << "]" << endl;

    // =============================================
    // Exercise 2: Create Input Tensors
    // =============================================
    cout << "\n--- EXERCISE 2: Create Input Tensors ---" << endl;
    cout << "Converting raw data to TensorFlow tensors" << endl;
    
    TF_Tensor* input1 = CreateInputTensor(sample1, 2);
    TF_Tensor* input2 = CreateInputTensor(sample2, 2);
    TF_Tensor* input3 = CreateInputTensor(sample3, 2);
    
    PrintTensor("Input Tensor 1", input1);
    PrintTensor("Input Tensor 2", input2);
    PrintTensor("Input Tensor 3", input3);

    // =============================================
    // Exercise 3: Load Pretrained Model
    // =============================================
    cout << "\n--- EXERCISE 3: Load Pretrained Model ---" << endl;
    cout << "Creating in-memory model (simulated loading)" << endl;
    
    SimpleModel model;
    cout << "✓ Model loaded successfully" << endl;
    cout << "  - Layer 1: 2 inputs → 3 hidden (ReLU)" << endl;
    cout << "  - Layer 2: 3 hidden → 1 output (Sigmoid)" << endl;
    cout << "  - Total Parameters: " << (6 + 3 + 3 + 1) << endl;

    // =============================================
    // Exercise 4: Run Inference (Single Sample)
    // =============================================
    cout << "\n--- EXERCISE 4: Run Inference ---" << endl;
    cout << "Running forward pass through model" << endl;
    
    float prediction1 = model.Predict(sample1);
    cout << "\nSample 1: [" << sample1[0] << ", " << sample1[1] << "]" << endl;
    cout << "Prediction: " << fixed << setprecision(4) << prediction1 << endl;
    cout << "Interpretation: ";
    if (prediction1 > 0.5f) {
        cout << "Class 1 (confidence: " << prediction1 * 100 << "%)" << endl;
    } else {
        cout << "Class 0 (confidence: " << (1 - prediction1) * 100 << "%)" << endl;
    }

    // =============================================
    // Exercise 5: Batch Inference
    // =============================================
    cout << "\n--- EXERCISE 5: Batch Inference ---" << endl;
    cout << "Running inference on multiple samples" << endl;
    
    float predictions[3];
    float* samples[3] = {sample1, sample2, sample3};
    
    cout << "\nProcessing 3 samples:" << endl;
    for (int i = 0; i < 3; i++) {
        predictions[i] = model.Predict(samples[i]);
        cout << "\nSample " << (i + 1) << ": [" 
             << samples[i][0] << ", " << samples[i][1] << "]" << endl;
        cout << "  Prediction: " << fixed << setprecision(4) << predictions[i] << endl;
        cout << "  Class: " << (predictions[i] > 0.5f ? "1" : "0") << endl;
    }

    // =============================================
    // Exercise 6: Postprocess Results
    // =============================================
    cout << "\n--- EXERCISE 6: Postprocess Results ---" << endl;
    cout << "Interpreting model outputs" << endl;
    
    cout << "\nSummary of predictions:" << endl;
    cout << "Sample | Raw Score | Class | Confidence" << endl;
    cout << "-------|-----------|-------|----------" << endl;
    
    for (int i = 0; i < 3; i++) {
        float conf = predictions[i] > 0.5f ? predictions[i] : (1 - predictions[i]);
        cout << i + 1 << "      | " 
             << fixed << setprecision(4) << predictions[i] << "    | "
             << (predictions[i] > 0.5f ? "1" : "0") << "     | "
             << fixed << setprecision(2) << (conf * 100) << "%" << endl;
    }

    // =============================================
    // Exercise 7: Model Metrics
    // =============================================
    cout << "\n--- EXERCISE 7: Calculate Metrics ---" << endl;
    cout << "Analyzing prediction results" << endl;
    
    int class_0_count = 0, class_1_count = 0;
    float avg_confidence = 0;
    
    for (int i = 0; i < 3; i++) {
        if (predictions[i] > 0.5f) {
            class_1_count++;
            avg_confidence += predictions[i];
        } else {
            class_0_count++;
            avg_confidence += (1 - predictions[i]);
        }
    }
    avg_confidence /= 3;
    
    cout << "\nResults:" << endl;
    cout << "  - Predicted Class 0: " << class_0_count << " samples" << endl;
    cout << "  - Predicted Class 1: " << class_1_count << " samples" << endl;
    cout << "  - Average Confidence: " << fixed << setprecision(2) 
         << (avg_confidence * 100) << "%" << endl;

    // =============================================
    // Exercise 8: Complete Inference Pipeline
    // =============================================
    cout << "\n--- EXERCISE 8: Complete Pipeline ---" << endl;
    cout << "Full end-to-end inference" << endl;
    
    cout << "\nPipeline Steps:" << endl;
    cout << "1. Read raw input" << endl;
    cout << "   Input: [0.5, -0.3]" << endl;
    
    cout << "2. Preprocess (normalize)" << endl;
    float preproc[2] = {sample1[0] / 10, sample1[1] / 10};
    cout << "   Preprocessed: [" << preproc[0] << ", " << preproc[1] << "]" << endl;
    
    cout << "3. Create tensor" << endl;
    TF_Tensor* final_input = CreateInputTensor(preproc, 2);
    PrintTensor("Input Tensor", final_input);
    
    cout << "4. Load model" << endl;
    cout << "   Model: Binary Classification Network" << endl;
    
    cout << "5. Run inference" << endl;
    float final_pred = model.Predict(preproc);
    cout << "   Output: " << fixed << setprecision(4) << final_pred << endl;
    
    cout << "6. Postprocess" << endl;
    cout << "   Predicted Class: " << (final_pred > 0.5f ? "1" : "0") << endl;
    cout << "   Confidence: " << fixed << setprecision(2) 
         << (final_pred > 0.5f ? final_pred : 1 - final_pred) * 100 << "%" << endl;

    // =============================================
    // Cleanup
    // =============================================
    TF_DeleteTensor(input1);
    TF_DeleteTensor(input2);
    TF_DeleteTensor(input3);
    TF_DeleteTensor(final_input);

    // =============================================
    // Summary
    // =============================================
    cout << "\n========================================" << endl;
    cout << "LAB 4 COMPLETE" << endl;
    cout << "========================================" << endl;
    cout << "\nYou learned:" << endl;
    cout << "✓ Understanding inference pipeline" << endl;
    cout << "✓ Creating input tensors from data" << endl;
    cout << "✓ Loading pre-trained models" << endl;
    cout << "✓ Running forward passes (inference)" << endl;
    cout << "✓ Batch processing multiple samples" << endl;
    cout << "✓ Postprocessing model outputs" << endl;
    cout << "✓ Calculating metrics and confidence" << endl;
    cout << "✓ Complete end-to-end inference pipeline" << endl;

    cout << "\n\nNEXT STEPS:" << endl;
    cout << "1. Load real TensorFlow models" << endl;
    cout << "2. Use LibTorch for PyTorch models (easier!)" << endl;
    cout << "3. Deploy models in production C++ applications" << endl;
    cout << "4. Optimize inference for performance" << endl;

    return 0;
}
```

---

## Part 4: Understanding Model Formats (15 minutes)

### 4.1 TensorFlow Model Formats

| Format | Extension | Use Case |
|--------|-----------|----------|
| SavedModel | `/saved_model` | Production, C++ loading |
| Frozen Graph | `.pb` | Optimized inference |
| H5/HDF5 | `.h5` | Keras models |
| ONNX | `.onnx` | Cross-framework |

### 4.2 Loading Models in C++ API

```cpp
// TensorFlow C++ API (higher level than C API)
#include <tensorflow/cc/client/client_session.h>

// Load SavedModel
tensorflow::SavedModelBundle bundle;
tensorflow::LoadSavedModel(session, tags, 
                          export_dir, &bundle);

// Create input tensor
tensorflow::Tensor input(tensorflow::DT_FLOAT, 
                        tensorflow::TensorShape({1, 2}));

// Run inference
vector<tensorflow::Output> output_names = 
    {bundle.graph.FindNodeByName("output_node")->output(0)};

vector<Tensor> outputs;
session->Run(input_tensors, output_names, {}, &outputs);
```

### 4.3 Inference Best Practices

1. **Preprocess input** - Match training preprocessing
2. **Batch when possible** - Process multiple samples together
3. **Cache model** - Don't reload for each prediction
4. **Handle errors** - Check for invalid inputs
5. **Optimize** - Use appropriate data types

---

## Part 5: Build and Run (10 minutes)

### 5.1 Build Project

```
Ctrl+Shift+B  → Build
```

### 5.2 Run Program

```
Ctrl+F5  → Run
```

### 5.3 Expected Output

```
========================================
Lab 4: Introduction to Model Inference
========================================

--- EXERCISE 1: Create Input Data ---
Scenario: Binary classification input (2 features)

Input Sample 1: [0.5, -0.3]
Input Sample 2: [-0.2, 0.7]
Input Sample 3: [0.4, 0.1]

--- EXERCISE 2: Create Input Tensors ---
Converting raw data to TensorFlow tensors

[Input Tensor 1]
Shape: (2)
Values: [0.5000, -0.3000]

--- EXERCISE 3: Load Pretrained Model ---
Creating in-memory model (simulated loading)

✓ Model loaded successfully
  - Layer 1: 2 inputs → 3 hidden (ReLU)
  - Layer 2: 3 hidden → 1 output (Sigmoid)
  - Total Parameters: 13

--- EXERCISE 4: Run Inference ---
Running forward pass through model

Sample 1: [0.5, -0.3]
Prediction: 0.6234
Interpretation: Class 1 (confidence: 62.34%)

... (more output)

========================================
LAB 4 COMPLETE
========================================
```

---

## Exercises

### Exercise 1: Load Real Model

**Task:** Modify code to load a SavedModel from disk

```cpp
// Load from file
tensorflow::SavedModelBundle bundle;
TF_CHECK_OK(tensorflow::LoadSavedModel(
    session, {}, "path/to/model", &bundle));
```

### Exercise 2: Batch Inference

**Task:** Process entire dataset in batches

```cpp
// Process 100 samples at once
for (int batch = 0; batch < total_samples; batch += 100) {
    // Create batch tensor
    // Run inference
    // Save results
}
```

### Exercise 3: Confidence Threshold

**Task:** Filter predictions by confidence

```cpp
float CONFIDENCE_THRESHOLD = 0.75f;
if (confidence > CONFIDENCE_THRESHOLD) {
    // High confidence prediction
} else {
    // Uncertain, might need review
}
```

---

## Verification Checklist

- [ ] Project compiles without errors
- [ ] All 8 exercises run correctly
- [ ] Input tensors created
- [ ] Model loaded
- [ ] Single inference works
- [ ] Batch inference works
- [ ] Results postprocessed
- [ ] Metrics calculated
- [ ] Complete pipeline works

---

## Learning Outcomes

By completing this lab, you should:

✅ Understand inference pipeline
✅ Know how to create input tensors
✅ Know how to run forward passes
✅ Understand batch processing
✅ Know how to interpret outputs
✅ Understand model deployment

---

## Next Steps

1. **LibTorch Alternative:** Use PyTorch models (easier than TensorFlow C API)
2. **Real Models:** Load actual TensorFlow/PyTorch models
3. **Optimization:** Quantization, pruning for faster inference
4. **Deployment:** Integrate into production applications
5. **Edge Devices:** Deploy on mobile, embedded systems

---

## Important Notes

⚠️ **This Lab Used Simplified Approach**

- Real TensorFlow C API is much more complex
- For production, use TensorFlow C++ API or LibTorch
- TensorFlow C API is low-level and verbose
- Better options exist for most use cases

**For your course:**
- ✅ This demonstrates inference concepts
- ✅ Real projects should use LibTorch or C++ API
- ✅ Concepts transfer to any framework

---

## Time Allocation

```
Part 1: Understanding Inference  15 minutes
Part 2: Create Test Model        20 minutes
Part 3: Implement Inference      30 minutes
Part 4: Model Formats            15 minutes
Part 5: Build and Run            10 minutes
Exercises (optional)             15+ minutes
─────────────────────────────────
Total:                           60-90 minutes
```

---

## References

- TensorFlow C API: https://github.com/tensorflow/tensorflow/tree/master/tensorflow/c
- Model Formats: https://tensorflow.org/guide/saved_model
- LibTorch (recommended): https://pytorch.org/cppdocs/
- TensorFlow C++ API: https://www.tensorflow.org/api_guides/cc

---

## Congratulations! 🎉

You've completed all 4 labs:
- ✅ Lab 1: Setup Verification
- ✅ Lab 2: Tensor Fundamentals
- ✅ Lab 3: Tensor Operations
- ✅ Lab 4: Model Inference

You now understand:
- How to set up TensorFlow in C++
- How to create and manipulate tensors
- How to preprocess data
- How to run inference on trained models

**You're ready for production ML applications!** 🚀
