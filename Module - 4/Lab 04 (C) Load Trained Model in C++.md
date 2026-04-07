# LAB 4 (C): LOADING PYTHON-TRAINED MODELS IN C++

## 📚 Complete Lab Guide with Code

---

## 🎯 Learning Objectives

By completing this lab, you will:

✅ Understand Python → C++ model deployment workflow
✅ Load SavedModel format in C++
✅ Implement feature normalization in C++
✅ Run inference on trained neural networks
✅ Handle errors professionally
✅ Deploy machine learning models in production C++
✅ Bridge Python ML training with C++ inference

---

## 📋 Lab Overview

| Item | Details |
|------|---------|
| **Scenario** | Train model in Python, deploy in C++ |
| **Python Framework** | TensorFlow/Keras |
| **C++ Framework** | TensorFlow C API |
| **Model Format** | SavedModel |
| **Use Case** | Diabetes classification inference |
| **Data Flow** | Patient data → Normalize → Predict |
| **Duration** | 45-60 minutes |
| **Difficulty** | Intermediate |

---

## 🔄 WORKFLOW: Python to C++

```
Python Phase (Training)
└── Train Neural Network
    ├── Input: 442 diabetes samples, 10 features each
    ├── Model: Dense(32) → Dense(16) → Dense(1, sigmoid)
    ├── Output: Trained weights
    └── Export SavedModel
        ├── saved_model.pb (architecture)
        ├── variables/ (trained weights)
        └── Configuration files
            ├── feature_means.txt
            ├── feature_stds.txt
            └── feature_names.txt

C++ Phase (Inference)
└── Load & Deploy Model
    ├── Load SavedModel from Python
    ├── Load normalization parameters
    ├── Get patient data
    ├── Normalize using training statistics
    ├── Run inference
    └── Output: Diabetes probability
```

---

## 💾 PYTHON PHASE: What Was Already Done

### Model Training (Python)

```python
# Simplified Python training code (already completed)

import tensorflow as tf
from sklearn.preprocessing import StandardScaler

# 1. Train neural network
model = tf.keras.Sequential([
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

# 2. Fit model
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
model.fit(X_train_scaled, y_train, epochs=20, validation_data=(X_test_scaled, y_test))

# 3. Save model
model.save('SavedModels/DiabetesNN', save_format='tf')

# 4. Save normalization parameters
np.savetxt('SavedModels/DiabetesNN/feature_means.txt', scaler.mean_)
np.savetxt('SavedModels/DiabetesNN/feature_stds.txt', scaler.scale_)

# 5. Save feature names
with open('SavedModels/DiabetesNN/feature_names.txt', 'w') as f:
    for name in features:
        f.write(name + '\n')
```

### What Python Creates

```
SavedModels/DiabetesNN/
├── saved_model.pb              ← Neural network architecture
├── variables/                  ← Trained weights
│   ├── variables.data-...
│   └── variables.index
├── keras_metadata.pb           ← Model metadata
├── feature_means.txt           ← For C++ normalization
├── feature_stds.txt            ← For C++ normalization
└── feature_names.txt           ← Feature labels
```

---

## 🔄 C++ PHASE: What We Do

### Complete C++ Pipeline (5 Steps)

```
Step 1: Load Configuration Files
        └─ Read feature_means.txt, feature_stds.txt

Step 2: Load SavedModel
        └─ Load neural network trained by Python

Step 3: Prepare Input Data
        └─ Patient medical information

Step 4: Normalize Features
        └─ Apply (X - mean) / std (must match Python!)

Step 5: Run Inference
        └─ Get diabetes prediction probability
```

---

## 📖 CODE EXPLANATION

## SECTION 1: INCLUDES & SETUP

### Code

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "tensorflow/c/c_api.h"

using namespace std;
```

### What It Means

```
iostream:  Input/output operations
vector:    Store arrays of data
string:    Handle text
fstream:   Read configuration files
iomanip:   Format output (decimal places)
tensorflow/c/c_api.h:  TensorFlow C API
           - Single header file!
           - All we need for inference
           - Simpler than C++ API
```

### Why TensorFlow C API?

```
Comparison:
C API (This approach):
✅ Single header
✅ Lightweight
✅ Stable
✅ Production standard
✅ Easier to compile

C++ API:
❌ Multiple headers
❌ Heavy dependencies
❌ Complex compilation
❌ Overkill for inference
```

---

## SECTION 2: ERROR HANDLING

### Code

```cpp
void CheckStatus(TF_Status* status) {
    if (TF_GetCode(status) != TF_OK) {
        cerr << "❌ TensorFlow Error: " << TF_Message(status) << endl;
        exit(1);
    }
}
```

### How It Works

```
TensorFlow operations return a status object
CheckStatus() checks if operation succeeded

If Error:
1. Print error message
2. Exit program
3. Prevent silent failures

If Success:
1. Continue execution
2. No output (good!)
```

### Usage Pattern

```cpp
// After ANY TensorFlow operation:
TF_LoadSessionFromSavedModel(..., status);
CheckStatus(status);  // ← Always check!

// If we reach here, it worked!
```

---

## SECTION 3: FILE LOADING

### Load Float File

```cpp
vector<float> LoadFloatFile(const string& filename) {
    vector<float> values;
    ifstream file(filename);

    float val;
    while (file >> val) values.push_back(val);

    return values;
}
```

**Purpose:** Load feature_means.txt and feature_stds.txt

**Example File:**
```
48.1473
1.4589
26.3074
94.9235
188.5666
114.9365
50.0283
4.0417
4.6282
90.7819
```

**Returns:** `[48.1473, 1.4589, 26.3074, ...]`

### Load String File

```cpp
vector<string> LoadStringFile(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    string line;
    while (getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }

    return lines;
}
```

**Purpose:** Load feature_names.txt

**Example File:**
```
AGE
SEX
BMI
BP
S1
S2
S3
S4
S5
S6
```

**Returns:** `["AGE", "SEX", "BMI", "BP", ...]`

---

## SECTION 4: MODEL CONFIGURATION CLASS

### Purpose

Manages everything related to model metadata and normalization:
- Load configuration files from Python training
- Store normalization parameters
- Normalize new patient data using training statistics

### Complete Code

```cpp
class ModelConfig {
public:
    vector<float> means;           // Feature means from training
    vector<float> stds;            // Feature stds from training
    vector<string> feature_names;  // Feature names

    bool Load(const string& dir) {
        // Load all configuration files from Python
        means = LoadFloatFile(dir + "/feature_means.txt");
        stds = LoadFloatFile(dir + "/feature_stds.txt");
        feature_names = LoadStringFile(dir + "/feature_names.txt");

        // Validate
        if (means.size() != 10 || stds.size() != 10) {
            cerr << "❌ Invalid normalization files!" << endl;
            return false;
        }

        cout << "✓ Configuration loaded\n";
        return true;
    }

    vector<float> Normalize(const vector<float>& input) {
        vector<float> out(input.size());

        // Apply normalization: (X - mean) / std
        for (int i = 0; i < input.size(); i++) {
            out[i] = (input[i] - means[i]) / stds[i];
        }

        return out;
    }
};
```

### Load Method Explained

```cpp
bool Load(const string& dir) {
    // 1. Load feature means from Python training
    means = LoadFloatFile(dir + "/feature_means.txt");
    
    // 2. Load feature standard deviations
    stds = LoadFloatFile(dir + "/feature_stds.txt");
    
    // 3. Load feature names
    feature_names = LoadStringFile(dir + "/feature_names.txt");

    // 4. Validate sizes are correct
    if (means.size() != 10 || stds.size() != 10) {
        cerr << "❌ Invalid normalization files!" << endl;
        return false;  // Error!
    }

    cout << "✓ Configuration loaded\n";
    return true;  // Success!
}
```

### Normalize Method Explained

```cpp
vector<float> Normalize(const vector<float>& input) {
    // Create output array same size as input
    vector<float> out(input.size());

    // For each feature
    for (int i = 0; i < input.size(); i++) {
        // Apply normalization formula
        out[i] = (input[i] - means[i]) / stds[i];
    }

    return out;
}
```

### Normalization Math

```
Why normalize?
- Neural networks trained with normalized data
- Expect input range: approximately [-1, 1]
- Without normalization: garbage output (silent failure!)

Formula: normalized = (raw - mean) / std

Example (Feature 0: AGE):
  Raw age: 45
  Mean age from training: 48.1473
  Std from training: 13.2631
  
  Normalized = (45 - 48.1473) / 13.2631
             = -3.1473 / 13.2631
             = -0.2373

CRITICAL: Must use EXACT same means/stds as Python training!
```

---

## SECTION 5: TENSORFLOW MODEL CLASS

### Purpose

Wrapper around TensorFlow C API for:
- Loading SavedModel from Python
- Running inference
- Managing resources properly

### Complete Code

```cpp
class TFModel {
private:
    TF_Graph* graph;        // Computation graph from Python
    TF_Session* session;    // Execution session
    TF_Status* status;      // Error tracking

public:
    TFModel() {
        graph = TF_NewGraph();
        status = TF_NewStatus();
    }

    bool LoadModel(const string& model_dir) {
        TF_SessionOptions* opts = TF_NewSessionOptions();
        const char* tags[] = { "serve" };
        
        session = TF_LoadSessionFromSavedModel(
            opts,               // Session options
            nullptr,            // Run options (advanced)
            model_dir.c_str(),  // Path to SavedModel
            tags,               // "serve" = inference mode
            1,                  // Number of tags
            graph,              // Output: computation graph
            nullptr,            // Metadata (optional)
            status              // Output: error status
        );

        CheckStatus(status);
        cout << "✓ Model loaded successfully\n";
        return true;
    }

    float Predict(const vector<float>& input) {
        // ===== PART 1: CREATE INPUT TENSOR =====
        
        // Define shape: [1, 10]
        // 1 = one sample
        // 10 = ten features
        int64_t dims[2] = { 1, 10 };

        // Allocate tensor
        TF_Tensor* input_tensor = TF_AllocateTensor(
            TF_FLOAT,            // Data type (32-bit float)
            dims,                // Shape [1, 10]
            2,                   // 2 dimensions
            sizeof(float) * 10   // Size in bytes
        );

        // Get pointer to data and fill with input
        float* data = static_cast<float*>(TF_TensorData(input_tensor));

        for (int i = 0; i < 10; i++) {
            data[i] = input[i];
        }

        // ===== PART 2: GET INPUT/OUTPUT OPERATIONS =====
        
        // ⚠️ IMPORTANT: These names must match your SavedModel!
        // Use: saved_model_cli show --dir SavedModels/DiabetesNN --all
        
        TF_Output input_op = {
             TF_GraphOperationByName(graph, "serving_default_keras_tensor"), 0
         };

         TF_Output output_op = {
             TF_GraphOperationByName(graph, "StatefulPartitionedCall_1"), 0
         };

        // ===== PART 3: RUN INFERENCE =====
        
        TF_Tensor* output_tensor = nullptr;

        TF_SessionRun(
            session,                 // Which session
            nullptr,                 // Run options
            &input_op,               // Input operation
            &input_tensor,           // Input data
            1,                       // Number of inputs
            &output_op,              // Output operation
            &output_tensor,          // Where to store output
            1,                       // Number of outputs
            nullptr,                 // Target operations
            0,                       // Number of targets
            nullptr,                 // Statistics
            status                   // Error status
        );

        CheckStatus(status);

        // ===== PART 4: EXTRACT RESULT =====
        
        float result = *(float*)TF_TensorData(output_tensor);

        // ===== CLEANUP =====
        
        TF_DeleteTensor(input_tensor);
        TF_DeleteTensor(output_tensor);

        return result;
    }

    ~TFModel() {
        TF_DeleteSession(session, status);
        TF_DeleteGraph(graph);
        TF_DeleteStatus(status);
    }
};
```

### LoadModel Method Explained

```
Purpose: Load SavedModel exported from Python

Steps:
1. Create session options
2. Set tag = "serve" (inference mode)
3. Load SavedModel from directory
4. Store graph and session
5. Check for errors
6. Ready for predictions!

Parameters:
- opts: Session configuration
- tags: Model tags ("serve" is standard)
- model_dir: Where SavedModel is located
- graph: Output - computation graph gets stored here
- status: Output - error info gets stored here
```

### Predict Method Explained

```
Purpose: Run inference on normalized input

PART 1: Create Input Tensor
- Allocate memory for tensor
- Set shape [1, 10]
- Fill with normalized input data

PART 2: Get Operations
- Find input operation in graph
- Find output operation in graph
- ⚠️ Names must match your SavedModel!

PART 3: Run Inference
- Execute the computation graph
- Pass input tensor
- Receive output tensor

PART 4: Extract Result
- Get probability from output tensor
- Clean up memory
- Return result

Returns: float between 0.0 and 1.0
- < 0.5: No Diabetes
- ≥ 0.5: Has Diabetes
```

### Important: Operation Names

```
⚠️ CRITICAL: Names must match YOUR SavedModel!

Example names (may differ):
- Input: "serving_default_input"
- Output: "StatefulPartitionedCall"

How to find correct names:
$ saved_model_cli show --dir SavedModels/DiabetesNN --all

If wrong:
❌ "Operation not found" error
✅ Update string in code

This is the most common error!
```

### Destructor (Cleanup)

```cpp
~TFModel() {
    TF_DeleteSession(session, status);  // Free session
    TF_DeleteGraph(graph);              // Free graph
    TF_DeleteStatus(status);            // Free status
}
```

**Why Important:**
- Free TensorFlow resources
- Prevent memory leaks
- Called automatically when object destroyed
- Professional C++ practice

---

## SECTION 6: MAIN PROGRAM

### Step 1: Setup & Load Configuration

```cpp
int main() {
    cout << "\n=== VERSION 3: DIABETES PREDICTION (C++ + TF) ===\n";

    string model_dir = "C:\\Users\\jasle\\OneDrive - Koenig Solutions Ltd\\Courses\\C++ with ML\\Downloads\\SavedModels\\DiabetesNN";

    // STEP 1: Load configuration
    ModelConfig config;

    if (!config.Load(model_dir)) {
        return -1;  // Error loading configuration
    }
```

**What Happens:**
1. Load feature_means.txt (10 values)
2. Load feature_stds.txt (10 values)
3. Load feature_names.txt (10 names)
4. Validate all loaded correctly

### Step 2: Load SavedModel

```cpp
    // STEP 2: Load TensorFlow model
    TFModel model;
    model.LoadModel(model_dir);
```

**What Happens:**
1. Load SavedModel from Python
2. Extract computation graph
3. Create inference session
4. Ready for predictions!

### Step 3: Prepare Input Data

```cpp
    // STEP 3: Input patient data
    vector<float> patient = {
        45, 1, 25.0, 100, 180, 120, 50, 3, 4.5, 85
    };

    cout << "\nRaw Input:\n";
    for (int i = 0; i < patient.size(); i++) {
        cout << patient[i] << " ";
    }
    cout << endl;
```

**Patient Data Explanation:**
```
Index | Feature | Value  | Unit
------|---------|--------|----------
  0   | AGE     | 45     | years
  1   | SEX     | 1      | 1=Male, 2=Female
  2   | BMI     | 25.0   | kg/m²
  3   | BP      | 100    | mmHg
  4   | S1      | 180    | serum level
  5   | S2      | 120    | serum level
  6   | S3      | 50     | serum level
  7   | S4      | 3      | serum level
  8   | S5      | 4.5    | serum level
  9   | S6      | 85     | glucose mg/dL
```

### Step 4: Normalize Features

```cpp
    // STEP 4: Normalize
    vector<float> normalized = config.Normalize(patient);

    cout << "\nNormalized Input:\n";
    for (float v : normalized) {
        cout << fixed << setprecision(4) << v << " ";
    }
    cout << endl;
```

**What Normalization Does:**
```
Raw: [45, 1, 25, 100, ...]
Normalized: [-0.2373, 1.0791, -0.2405, 0.3779, ...]

Formula applied to each feature:
normalized[i] = (raw[i] - means[i]) / stds[i]

Why?
✓ Neural network trained with normalized input
✓ Expects values in range [-1, 1]
✓ MUST use training means/stds!
✓ Skip = garbage prediction
```

### Step 5: Run Prediction

```cpp
    // STEP 5: Predict
    float prob = model.Predict(normalized);

    cout << "\nPrediction Probability: " << prob << endl;

    if (prob > 0.5)
        cout << "⚠ Diabetes Detected\n";
    else
        cout << "✓ No Diabetes\n";

    cout << "\n=== DONE ===\n";

    return 0;
}
```

**Result Interpretation:**
```
Probability range: 0.0 to 1.0

0.0 - 0.5: NO DIABETES ✓
  - 0.234 (23.4%) = Low risk
  - 0.412 (41.2%) = Moderate-low risk

0.5 - 1.0: HAS DIABETES ⚠
  - 0.623 (62.3%) = Moderate-high risk
  - 0.892 (89.2%) = High risk

Threshold: 0.5
  - < 0.5: Predict "No Diabetes"
  - ≥ 0.5: Predict "Has Diabetes"
```

---

## 📤 SAMPLE OUTPUT

### Expected Output When Program Runs

```
=== VERSION 3: DIABETES PREDICTION (C++ + TF) ===

✓ Configuration loaded

✓ Model loaded successfully

Raw Input:
45 1 25 100 180 120 50 3 4.5 85 

Normalized Input:
-0.2373 1.0791 -0.2405 0.3779 -0.2436 -0.4316 -0.0002 -0.3203 -0.2447 -0.4833 

Prediction Probability: 0.4523

✓ No Diabetes

=== DONE ===
```

### What Each Line Means

```
Configuration loaded:
  ✓ feature_means.txt loaded
  ✓ feature_stds.txt loaded
  ✓ feature_names.txt loaded

Model loaded successfully:
  ✓ SavedModel from Python loaded
  ✓ Computation graph extracted
  ✓ Session created

Raw Input:
  Patient's original medical measurements

Normalized Input:
  Features after (X - mean) / std transformation
  Ready for neural network

Prediction Probability: 0.4523
  Probability of diabetes: 45.23%
  Below threshold (0.5)
  Therefore: NO DIABETES
```

---

## 🔧 COMPLETE CODE

### Full Listing

```cpp
// ============================================================
// VERSION 3: PRODUCTION-READY TENSORFLOW C++ INFERENCE
// Combines correctness + structure + real ML pipeline
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "tensorflow/c/c_api.h"

using namespace std;

// ============================================================
// UTILITY: Error Handling
// ============================================================

void CheckStatus(TF_Status* status) {
    if (TF_GetCode(status) != TF_OK) {
        cerr << "❌ TensorFlow Error: " << TF_Message(status) << endl;
        exit(1);
    }
}

// ============================================================
// LOAD FILE HELPERS
// ============================================================

vector<float> LoadFloatFile(const string& filename) {
    vector<float> values;
    ifstream file(filename);

    float val;
    while (file >> val) values.push_back(val);

    return values;
}

vector<string> LoadStringFile(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    string line;
    while (getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }

    return lines;
}

// ============================================================
// MODEL CONFIG (Normalization + Metadata)
// ============================================================

class ModelConfig {
public:
    vector<float> means;
    vector<float> stds;
    vector<string> feature_names;

    bool Load(const string& dir) {
        means = LoadFloatFile(dir + "/feature_means.txt");
        stds = LoadFloatFile(dir + "/feature_stds.txt");
        feature_names = LoadStringFile(dir + "/feature_names.txt");

        if (means.size() != 10 || stds.size() != 10) {
            cerr << "❌ Invalid normalization files!" << endl;
            return false;
        }

        cout << "✓ Configuration loaded\n";
        return true;
    }

    vector<float> Normalize(const vector<float>& input) {
        vector<float> out(input.size());

        for (int i = 0; i < input.size(); i++) {
            out[i] = (input[i] - means[i]) / stds[i];
        }

        return out;
    }
};

// ============================================================
// TENSORFLOW MODEL WRAPPER (C API)
// ============================================================

class TFModel {
private:
    TF_Graph* graph;
    TF_Session* session;
    TF_Status* status;

public:
    TFModel() {
        graph = TF_NewGraph();
        status = TF_NewStatus();
    }

    bool LoadModel(const string& model_dir) {
        TF_SessionOptions* opts = TF_NewSessionOptions();
        const char* tags[] = { "serve" };
        
        session = TF_LoadSessionFromSavedModel(
            opts,
            nullptr,
            model_dir.c_str(),
            tags,
            1,
            graph,
            nullptr,
            status
        );

        CheckStatus(status);
        cout << "✓ Model loaded successfully\n";
        return true;
    }

    float Predict(const vector<float>& input) {
        // Create input tensor [1, 10]
        int64_t dims[2] = { 1, 10 };

        TF_Tensor* input_tensor = TF_AllocateTensor(
            TF_FLOAT, dims, 2, sizeof(float) * 10
        );

        float* data = static_cast<float*>(TF_TensorData(input_tensor));

        for (int i = 0; i < 10; i++) {
            data[i] = input[i];
        }

        // Get operations
        TF_Output input_op = {
            TF_GraphOperationByName(graph, "serving_default_keras_tensor"), 0
        };

        TF_Output output_op = {
            TF_GraphOperationByName(graph, "StatefulPartitionedCall_1"), 0
        };

        TF_Tensor* output_tensor = nullptr;

        // Run inference
        TF_SessionRun(
            session,
            nullptr,
            &input_op, &input_tensor, 1,
            &output_op, &output_tensor, 1,
            nullptr, 0,
            nullptr,
            status
        );

        CheckStatus(status);

        // Extract result
        float result = *(float*)TF_TensorData(output_tensor);

        // Cleanup
        TF_DeleteTensor(input_tensor);
        TF_DeleteTensor(output_tensor);

        return result;
    }

    ~TFModel() {
        TF_DeleteSession(session, status);
        TF_DeleteGraph(graph);
        TF_DeleteStatus(status);
    }
};

// ============================================================
// MAIN PROGRAM
// ============================================================

int main() {

    cout << "\n=== VERSION 3: DIABETES PREDICTION (C++ + TF) ===\n";

    string model_dir = "C:\\Users\\jasle\\OneDrive - Koenig Solutions Ltd\\Courses\\C++ with ML\\Downloads\\SavedModels\\DiabetesNN";

    // --------------------------------------------------------
    // STEP 1: Load configuration
    // --------------------------------------------------------

    ModelConfig config;

    if (!config.Load(model_dir)) {
        return -1;
    }

    // --------------------------------------------------------
    // STEP 2: Load TensorFlow model
    // --------------------------------------------------------

    TFModel model;
    model.LoadModel(model_dir);

    // --------------------------------------------------------
    // STEP 3: Input patient data
    // --------------------------------------------------------

    vector<float> patient = {
        45, 1, 25.0, 100, 180, 120, 50, 3, 4.5, 85
    };

    cout << "\nRaw Input:\n";
    for (int i = 0; i < patient.size(); i++) {
        cout << patient[i] << " ";
    }
    cout << endl;

    // --------------------------------------------------------
    // STEP 4: Normalize
    // --------------------------------------------------------

    vector<float> normalized = config.Normalize(patient);

    cout << "\nNormalized Input:\n";
    for (float v : normalized) {
        cout << fixed << setprecision(4) << v << " ";
    }
    cout << endl;

    // --------------------------------------------------------
    // STEP 5: Predict
    // --------------------------------------------------------

    float prob = model.Predict(normalized);

    cout << "\nPrediction Probability: " << prob << endl;

    if (prob > 0.5)
        cout << "⚠ Diabetes Detected\n";
    else
        cout << "✓ No Diabetes\n";

    cout << "\n=== DONE ===\n";

    return 0;
}
```

---

## 🔧 COMPILATION

### Prerequisites

```bash
# Install TensorFlow C library
# Linux: sudo apt-get install libtensorflow-dev
# macOS: brew install tensorflow
# Windows: Download from tensorflow.org
```

### Compile Commands

**Linux/macOS:**
```bash
g++ -std=c++17 \
    -I/usr/include/tensorflow \
    -L/usr/lib \
    your_code.cpp \
    -ltensorflow \
    -o diabetes_inference
```

**Windows (with Visual Studio):**
```bash
g++ -std=c++17 ^
    -I"C:\tensorflow\include" ^
    -L"C:\tensorflow\lib" ^
    your_code.cpp ^
    -ltensorflow ^
    -o diabetes_inference.exe
```

**Using pkg-config (Recommended):**
```bash
g++ -std=c++17 \
    $(pkg-config --cflags tensorflow) \
    your_code.cpp \
    $(pkg-config --libs tensorflow) \
    -o diabetes_inference
```

### Running

```bash
./diabetes_inference
# or on Windows:
.\diabetes_inference.exe
```

---

## ⚠️ COMMON ISSUES

### Issue 1: "Operation not found"

**Cause:** Wrong operation name in code

**Solution:**
```bash
# Find correct names
saved_model_cli show --dir SavedModels/DiabetesNN --all

# Update in code
TF_GraphOperationByName(graph, "correct_name_here")
```

### Issue 2: "Cannot load tensorflow library"

**Cause:** TensorFlow C not installed

**Solution:**
```bash
# Linux
sudo apt-get install libtensorflow-dev

# macOS
brew install tensorflow

# Windows
Download from tensorflow.org/install/lang_c
```

### Issue 3: Segmentation fault

**Cause:** Tensor size mismatch or wrong paths

**Solution:**
```cpp
// Check tensor size
if (input.size() != 10) {
    cerr << "Error: Expected 10 features!" << endl;
    return -1;
}

// Check paths
cout << "Model dir: " << model_dir << endl;
```

### Issue 4: NaN or Inf predictions

**Cause:** Wrong normalization parameters

**Solution:**
```cpp
// Verify means and stds loaded
cout << "Mean[0]: " << config.means[0] << endl;
cout << "Std[0]: " << config.stds[0] << endl;

// Check normalized values
for (auto v : normalized) {
    if (isnan(v)) cout << "NaN!" << endl;
    if (isinf(v)) cout << "Inf!" << endl;
}
```

---

## 🎓 KEY CONCEPTS

### Python → C++ Deployment

```
Python Phase (One-time):
├─ Train neural network
├─ Validate on test data
├─ Export as SavedModel
└─ Save normalization parameters

C++ Phase (Repeated):
├─ Load SavedModel
├─ Load normalization parameters
├─ For each prediction:
│  ├─ Get raw input
│  ├─ Normalize
│  ├─ Run inference
│  └─ Get probability
└─ Repeat for many predictions
```

### Feature Normalization (Critical!)

```
Python (Training):
  raw = [45, 1, 25, 100, ...]
  scaler.fit_transform(raw)
  → normalized = [-0.2373, 1.0791, -0.2405, 0.3779, ...]
  
  Saves:
  - feature_means.txt: [48.1473, 1.4589, 26.3074, ...]
  - feature_stds.txt: [13.2631, 0.4983, 4.3913, ...]

C++ (Inference):
  Load means and stds (SAME VALUES!)
  raw = [45, 1, 25, 100, ...]
  normalized[0] = (45 - 48.1473) / 13.2631 = -0.2373
  normalized[1] = (1 - 1.4589) / 0.4983 = 1.0791
  
  MUST BE IDENTICAL!
```

### SavedModel Format

```
SavedModel is a standard TensorFlow format:
├─ saved_model.pb: Protobuf definition of graph
├─ variables/: Trained weights
├─ assets/: Supporting files
└─ keras_metadata.pb: Model metadata

C API loads everything:
- Reconstructs computation graph
- Loads weights into memory
- Creates session for inference
```

---

## ✅ LAB CHECKLIST

- [ ] Understand Python → C++ workflow
- [ ] Have SavedModel from Python training
- [ ] Have feature_means.txt and feature_stds.txt
- [ ] Install TensorFlow C library
- [ ] Update model_dir path in code
- [ ] Find correct operation names
- [ ] Compile code successfully
- [ ] Run program
- [ ] Verify correct output
- [ ] Test with different patients
- [ ] Understand error handling
- [ ] Ready for production!

---

## 🚀 NEXT STEPS

### 1. Test Different Patients
```cpp
vector<vector<float>> patients = {
    {45, 1, 25.0, 100, 180, 120, 50, 3, 4.5, 85},   // Low risk
    {62, 2, 31.0, 115, 210, 145, 35, 5, 5.0, 110},  // High risk
    {50, 1, 27.0, 105, 190, 130, 45, 4, 4.7, 95},   // Medium risk
};

for (auto& patient : patients) {
    auto norm = config.Normalize(patient);
    float prob = model.Predict(norm);
    cout << "Prediction: " << prob << endl;
}
```

### 2. Batch Processing
```cpp
// Process multiple patients efficiently
for (const auto& patient : patients) {
    // ... normalize and predict
}
```

### 3. Add Logging
```cpp
cout << "Processing patient: " << patient_id << endl;
cout << "Raw: [" << patient[0] << ", " << patient[1] << ", ...]" << endl;
cout << "Normalized: [" << normalized[0] << ", ...]" << endl;
cout << "Probability: " << prob << endl;
```

### 4. Production Deployment
- Add error recovery
- Log all predictions
- Monitor inference time
- Version control models

---

## 📚 SUMMARY

This lab demonstrates:

✅ **Complete ML Pipeline**
- Python training → Model export
- C++ loading → Feature normalization → Inference

✅ **TensorFlow C API**
- Load SavedModel
- Run inference
- Handle errors

✅ **Production Code**
- Professional error handling
- Resource management
- Clean structure

✅ **Best Practices**
- Match Python preprocessing exactly
- Use training means/stds for normalization
- Proper memory cleanup
- Clear documentation

---

## 🎉 CONCLUSION

You now know how to:
1. ✅ Train models in Python
2. ✅ Export as SavedModel
3. ✅ Load in C++
4. ✅ Run inference
5. ✅ Deploy to production

**This is real-world ML deployment!** 🚀

---

**LAB 4 (C) COMPLETE!** ✨

You have successfully bridged Python ML training with C++ inference deployment!
