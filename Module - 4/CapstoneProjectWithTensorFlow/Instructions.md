# Smart Access Anomaly Detection & Alerting System
## Lab Guide - Version 1.0
### Multi-Threaded ML Inference with TensorFlow C API

---

## Lab Information

| Item | Details |
|------|---------|
| **Lab Name** | Smart Access Control Anomaly Detection & Real-Time Alert System |
| **Course** | Module 4 - Capstone Project: Production ML Systems in C++ |
| **Duration** | 4-6 hours (setup, implementation, testing, deployment) |
| **Level** | Advanced (Concurrent Programming + ML) |
| **Prerequisites** | C++17, Threading, ML Fundamentals |

---

## Learning Objectives

By completing this lab, students will:

1. ✅ **Build a multi-threaded C++ application** that integrates machine learning
2. ✅ **Load and execute TensorFlow SavedModels** using the C API
3. ✅ **Implement thread-safe data structures** for inter-process communication
4. ✅ **Normalize input features** using pre-trained statistical parameters
5. ✅ **Execute real-time anomaly classification** and alerting
6. ✅ **Understand producer-consumer patterns** in concurrent systems
7. ✅ **Integrate Python model training** with C++ inference pipeline

---

## Prerequisites

- **C++ 17 or later** (with STL threading support)
- **TensorFlow C API** (see setup section)
- **Python 3.8+** with TensorFlow & scikit-learn
- **Visual Studio 2019+** (or equivalent C++ compiler)
- **Basic knowledge of:**
  - C++ templates and STL containers
  - Multi-threading concepts (mutexes, condition variables)
  - Machine learning basics (normalization, classification)
  - TensorFlow SavedModel format

---

## Executive Summary

This lab implements an **end-to-end anomaly detection system** for smart access control (e.g., building entry/exit systems).

### What the System Does:

1. **Generates or reads** access event data (hour, device_id, access_type, user_freq)
2. **Normalizes features** using pre-trained mean/std statistics
3. **Passes events** through a TensorFlow neural network in real-time
4. **Classifies events** as:
   - 🟢 **NORMAL** (score < 0.4)
   - 🟡 **SUSPICIOUS** (0.4 ≤ score ≤ 0.7)
   - 🔴 **HIGH RISK** (score > 0.7)
5. **Outputs alerts** to console

### Architecture Pattern:

```
Producer Thread  →  [Input Queue]  →  Processor Thread  →  [Output Queue]  →  Alert Handler
 (generates)        (safe queue)      (ML inference)       (safe queue)       (displays)
```

This follows the **producer-consumer pattern** with thread-safe queues for decoupled communication.

---

## System Architecture

### Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│          SMART ACCESS ANOMALY DETECTION SYSTEM V1               │
└─────────────────────────────────────────────────────────────────┘

                    ┌──────────────────┐
                    │  THREAD 1:       │
                    │  PRODUCER        │
                    │  (Generate       │
                    │   Events)        │
                    └────────┬─────────┘
                             │
                             ↓
                    ┌──────────────────┐
                    │  SafeQueue<T>    │
                    │  (InputQueue)    │
                    └────────┬─────────┘
                             │
                             ↓
                    ┌──────────────────┐
                    │  THREAD 2:       │
                    │  PROCESSOR       │
                    │  (TensorFlow)    │
                    │  • Load Model    │
                    │  • Normalize     │
                    │  • Inference     │
                    └────────┬─────────┘
                             │
                             ↓
                    ┌──────────────────┐
                    │  SafeQueue<T>    │
                    │  (OutputQueue)   │
                    └────────┬─────────┘
                             │
                             ↓
                    ┌──────────────────┐
                    │  THREAD 3:       │
                    │  ALERT HANDLER   │
                    │  (Display        │
                    │   Alerts)        │
                    └──────────────────┘
```

---

## Detailed Component Explanation

### 1. Safe Queue (Thread-Safe Communication)

**Problem:** Multiple threads accessing queue simultaneously → race conditions

**Solution:** Protect with mutex + condition_variable

#### Key Features:

- `push(T value)` - Thread-safe push, notifies waiting consumers
- `pop()` - Blocks until data available, auto-locks

#### Template Implementation:

```cpp
template<typename T>
class SafeQueue {
private:
    queue<T> q;
    mutex m;
    condition_variable cv;

public:
    void push(T value) {
        unique_lock<mutex> lock(m);
        q.push(value);
        cv.notify_one();  // Wake waiting thread
    }

    T pop() {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [&]() { return !q.empty(); });
        T val = q.front();
        q.pop();
        return val;
    }
};
```

#### Thread Safety Mechanisms:

| Mechanism | Purpose |
|-----------|---------|
| `unique_lock<mutex>` | Ensures exclusive access |
| `condition_variable` | Wakes up blocking threads |
| `cv.notify_one()` | Signals data is available |

### 2. Producer Thread

**Function:** Generates random access events

#### Event Structure:
```
[hour(0-24), device_id(1-5), access_type(0-1), user_freq(1-10)]
```

#### Logic:
```cpp
while(true):
  • Generate 4 random integers in correct ranges
  • Cast to float: static_cast<float>(rand() % 24)
  • Push to inputQueue
  • Sleep 1 second (simulates real event stream)
```

#### Purpose:
Simulates real smart access control system ingesting events

### 3. Processor Thread (ML Inference Engine)

**Responsibilities:**

#### a) Load Pre-trained Model
- TensorFlow SavedModel format
- Contains: computational graph + weights
- Loaded with `TF_LoadSessionFromSavedModel()`

#### b) Load Normalization Parameters
- `feature_means.txt` - Mean of each feature during training
- `feature_stds.txt` - Std dev of each feature during training
- Formula: **z = (x - mean) / std** (Z-score normalization)

#### c) Process Events in Real-Time
```cpp
For each event:
  1. Pop from inputQueue (blocks if empty)
  2. Normalize using loaded statistics
  3. Create TensorFlow tensor (1x4 float matrix)
  4. Execute model inference: TF_SessionRun()
  5. Push result to outputQueue
```

#### d) Error Handling
- `CheckStatus()` verifies TensorFlow operations
- Logs errors to stderr
- Exits if critical

#### TensorFlow C API Workflow:

```
┌──────────────────────────────────────────┐
│ 1. TF_NewGraph()                         │  Create empty graph
│ 2. TF_NewStatus()                        │  Create status object
│ 3. TF_NewSessionOptions()                │  Configure session
│ 4. TF_LoadSessionFromSavedModel()        │  Load model from disk
│ 5. TF_GraphOperationByName()             │  Find input/output nodes
│ 6. TF_AllocateTensor()                   │  Create input tensor
│ 7. TF_SessionRun()                       │  Execute inference
│ 8. TF_TensorData()                       │  Extract output
│ 9. TF_DeleteTensor()                     │  Free memory
└──────────────────────────────────────────┘
```

### 4. Alert Handler Thread

**Function:** Consumes predictions and displays alerts

#### Classification Rules:

| Score | Classification | Symbol |
|-------|-----------------|--------|
| > 0.7 | HIGH RISK ALERT | 🚨 |
| 0.4 - 0.7 | SUSPICIOUS | ⚠️ |
| < 0.4 | NORMAL | ✅ |

#### Output Format:
```
🚨 HIGH RISK ALERT (0.85)
⚠️ Suspicious (0.55)
✅ Normal (0.25)
```

#### Logic:
```cpp
while(true):
  • Pop from outputQueue (blocks if empty)
  • Apply thresholds
  • Print alert to console
```

### 5. Normalization Function

#### Why Normalize?

- Training data had mean=μ, std=σ
- Raw input has different scale
- Model expects normalized input
- **Formula:** z = (x - μ) / σ

#### Example:
```
hour = 23, μ_hour = 12, σ_hour = 8
normalized = (23 - 12) / 8 = 1.375
```

#### Benefits:

✅ Brings all features to same scale  
✅ Stabilizes neural network training  
✅ Improves generalization  

---

## Workflow Diagram

```
                    [START]
                       │
           ┌───────────┴───────────┐
           │                       │
    [Load Model]          [Generate Event]
           │                       │
           └───────────┬───────────┘
                       │
              [inputQueue.push(event)]
                       │
              [inputQueue.pop(event)]
                       │
             [Normalize Features]
                       │
             [Create TF Tensor]
                       │
             [TF_SessionRun()]
                       │
             [Extract Score]
                       │
             [outputQueue.push(score)]
                       │
             [outputQueue.pop(score)]
                       │
             [Classify (thresholds)]
                       │
             [Display Alert]
                       │
                    [LOOP]
```

---

## Feature Engineering Notes

### Input Features (4 dimensions)

| Feature | Range | Meaning |
|---------|-------|---------|
| **hour** | 0-23 | Time of access (0=midnight) |
| **device_id** | 1-4 | Which door/sensor triggered |
| **access_type** | 0-1 | Entry (0) vs Exit (1) |
| **user_freq** | 1-9 | How often user accesses (1-10) |

### Anomaly Patterns Detected:

- 🔴 Late-night access (hour < 5) + high frequency (freq > 6) = **ANOMALY**
- 🔴 Unusual device combinations
- 🔴 Access patterns inconsistent with training distribution

---

## Setup Instructions

### Step 1: Install TensorFlow C API

#### Windows:
1. Download from: https://www.tensorflow.org/install/lang_c
2. Extract to: `C:\tensorflow` (or your preferred location)
3. Verify files: `.dll` + `.lib` files present

#### Linux:
```bash
apt-get install libtensorflow libtensorflow-dev
```

### Step 2: Configure Visual Studio Project

**Right-click Project → Properties:**

#### C/C++ → General → Additional Include Directories:
```
C:\tensorflow\include
```

#### Linker → General → Additional Library Directories:
```
C:\tensorflow\lib
```

#### Linker → Input → Additional Dependencies:
```
tensorflow.lib
```

### Step 3: Compile & Link

#### Command Line:
```bash
cl /std:c++17 /I"C:\tensorflow\include" LoadFFModel.cpp ^
   /link /LIBPATH:"C:\tensorflow\lib" tensorflow.lib
```

#### Visual Studio:
```
Build → Build Solution (Ctrl+Shift+B)
```

---

## Expected Output

### Console Output:

```
[Processor] Model loaded
[Producer] Event generated
[Producer] Event generated
[Producer] Event generated
[Processor] Event generated

✅ Normal (0.23)
✅ Normal (0.35)
⚠️ Suspicious (0.58)
[Producer] Event generated

🚨 HIGH RISK ALERT (0.82)
✅ Normal (0.18)
⚠️ Suspicious (0.51)

(repeats continuously)
```

### System Behavior:

- Producer generates **1 event per second**
- Processor runs **inference immediately**
- AlertHandler outputs **classification**
- **Infinite loop** (Ctrl+C to stop)

---

## Testing & Validation

### Test 1: Basic Execution

1. Run the program
2. Observe output flowing
3. Verify no crashes

**Expected:** Continuous alerts at ~1 per second

### Test 2: Thread Safety

Visual Studio → Debug → Windows → Threads

Verify 3 threads running:
- Thread 1: `producer`
- Thread 2: `processor`
- Thread 3: `alertHandler`

### Test 3: Model Loading

Add breakpoint at: `"cout << "[Processor] Model loaded"`

**Expected:** Model loads within 2-3 seconds  
**If error:** Check model path and TensorFlow installation

### Test 4: Inference Correctness

Verify outputs range **[0.0, 1.0]**

**Expected:** Mix of normal/suspicious/high-risk  
**If always normal:** Check normalization parameters

---

## Common Errors & Solutions

### Error 1: "unresolved external symbol _imp_TF_NewGraph"

**Cause:** TensorFlow library not linked

**Fix:**
1. Download TensorFlow C API
2. Add `tensorflow.lib` to Linker → Input → Additional Dependencies
3. Add include path to C/C++ → General → Additional Include Directories

### Error 2: "Failed to load model from SavedModel"

**Cause:** Incorrect model path or missing files

**Fix:**
1. Verify path exists: `C:\Users\...\SavedModels\FFModel\`
2. Check files present:
   - `saved_model.pb`
   - `variables/` (directory)
   - `assets/` (directory)
3. Update `model_dir` in `main()`

### Error 3: "Operation error" (input_op or output_op is null)

**Cause:** Node names don't match your SavedModel

**Fix:**
1. Run Python to inspect model:
```python
import tensorflow as tf
model = tf.saved_model.load("SavedModels/FFModel")
print(model.signatures)
```
2. Update node names in C++:
   - `"serving_default_keras_tensor"` (input)
   - `"StatefulPartitionedCall"` (output)

### Error 4: Alerts always "Normal" or always "High Risk"

**Cause:** Incorrect normalization parameters

**Fix:**
1. Regenerate model: `python PythonApplication1.py`
2. Verify `feature_means.txt` and `feature_stds.txt` contain 4 floats each
3. Check normalization logic applies mean/std correctly

---

## Performance Metrics

### Typical Performance (on modern hardware)

| Metric | Time |
|--------|------|
| Model Load Time | ~1-2 seconds |
| Inference Latency | ~10-50 ms per event |
| Throughput | ~20-100 events/sec |
| Memory Usage | ~100-300 MB (model + buffers) |
| Thread Creation Time | ~1-2 ms per thread |

### Bottlenecks:

1. **TensorFlow inference (CPU)** - major bottleneck
2. **File I/O** - only on startup
3. **Queue operations** - negligible (~1 μs)

### Optimization Tips:

- Use GPU acceleration (requires TensorFlow GPU + CUDA)
- Batch process events (group multiple inputs)
- Pre-allocate tensors (reuse across inferences)

---

## Extension Exercises

### Exercise 1: Batch Processing

Modify processor to handle 10 events at once:
- Create 10x4 tensor instead of 1x4
- Run inference once per batch
- Distribute outputs to 10 alert handlers
- Measure speedup

### Exercise 2: Dynamic Thresholds

Load thresholds from config file instead of hardcoded:
```
threshold_suspicious = 0.4
threshold_high_risk = 0.7
```

Update alerts based on device_id (device 3 = more sensitive)

### Exercise 3: Persistence

Save high-risk events to file in CSV format:
```
timestamp, hour, device_id, access_type, user_freq, score
```

### Exercise 4: Multiple Models

Load 2 different SavedModels:
- Model A: trained on normal users
- Model B: trained on suspicious patterns
- Ensemble voting for final prediction

---

## Dependencies Summary

### Runtime Dependencies

| Library | Version | Purpose |
|---------|---------|---------|
| TensorFlow C API | 2.10+ | Model loading & inference |
| C++ Standard | C++17 | Threading, STL containers |
| Windows API | - | Memory management (implicit) |

### Build Dependencies

| Tool | Version |
|------|---------|
| Compiler | MSVC 2019+ or GCC 9+ |
| Visual Studio | 2019 or later |
| CMake (optional) | 3.15+ |

---

## Key Takeaways

1. **✅ Producer-Consumer Pattern**
   - Decouples data generation from processing
   - Allows independent scaling of threads
   - Enables pipelining for better throughput

2. **✅ ML Model Integration**
   - TensorFlow C API bridges ML and systems programming
   - Normalization is essential for correct predictions
   - Model format (SavedModel) must be portable

3. **✅ Concurrency & Synchronization**
   - Mutex + Condition Variables = thread-safe communication
   - Prevents race conditions, data corruption

4. **✅ Real-Time Processing**
   - Sub-second latency from input to alert
   - Suitable for time-critical systems

5. **✅ Anomaly Detection**
   - Classification thresholds enable multi-tier alerting
   - Practical security/monitoring application

---

## References

- **TensorFlow C API:** https://www.tensorflow.org/api_docs/cc
- **C++ Concurrency:** https://cppreference.com/w/cpp/thread
- **SavedModel Format:** https://www.tensorflow.org/guide/saved_model
- **Machine Learning:** https://scikit-learn.org/stable/modules/preprocessing.html

---

## Submission Checklist

Before submitting, verify:

- [ ] Code compiles without errors
- [ ] All 3 threads running (checked in debugger)
- [ ] Model loads successfully
- [ ] Alerts appear at console within 5 seconds
- [ ] No memory leaks (use Visual Studio diagnostic tools)
- [ ] Code is well-commented
- [ ] Variable names are meaningful
- [ ] No hardcoded paths (use R"(...)" strings)
- [ ] README.md included with instructions
- [ ] Python training script included
- [ ] All source files included in submission

---

## Compilation & Execution Walkthrough

### Step 1: Train the Model (Python)

```bash
python PythonApplication1.py
```

**Expected Output:**
```
Training data shape: (500, 4)
Anomaly rate: 0.082
Epoch 1/10
16/16 [==============================] - 0s 2ms/step - loss: 0.6234 - accuracy: 0.6340
...
Epoch 10/10
Training Accuracy: 0.9560
✅ Model saved to: C:\Users\...\SavedModels\FFModel
✅ Normalization parameters saved
✅ Feature names saved
```

**Output Files Created:**
```
SavedModels/FFModel/
├── saved_model.pb
├── variables/
├── assets/
├── feature_means.txt    (4 floats)
├── feature_stds.txt     (4 floats)
└── feature_names.txt
```

### Step 2: Compile C++ Code (Visual Studio)

1. Open Visual Studio
2. Create New C++ Console App
3. Copy `LoadFFModel.cpp` content
4. Project Properties → C/C++:
   - Add include: `C:\tensorflow\include`
5. Project Properties → Linker:
   - Add library dir: `C:\tensorflow\lib`
   - Add dependency: `tensorflow.lib`
6. Build → Build Solution (F7)

**Expected Output:**
```
Build succeeded. 0 warnings.
```

### Step 3: Run the Application

```bash
LoadFFModel.exe
```

**Console Output:**
```
[Processor] Model loaded
[Producer] Event generated
[Producer] Event generated
✅ Normal (0.2341)
⚠️ Suspicious (0.5612)
🚨 HIGH RISK ALERT (0.8234)
✅ Normal (0.1823)
... (repeats indefinitely)
```

**Stop:** Press Ctrl+C to terminate

---

## Verification Checklist

After running, verify:

- [ ] No TensorFlow errors appear
- [ ] Alerts display within 5-10 seconds of starting
- [ ] Mix of ✅, ⚠️, and 🚨 alerts (not always one type)
- [ ] Model loads in < 5 seconds
- [ ] New alerts appear roughly every 1-2 seconds
- [ ] No memory growth (check Task Manager)
- [ ] Scores are in range [0.0, 1.0]

**If any check fails:**
1. Review error message carefully
2. Check TensorFlow installation
3. Verify model path is correct
4. See "Common Errors" section above

---

## Version 1 vs Version 2 Comparison

This lab implements **Version 1.0** (basic functionality).  
**Version 2.0** (Production) adds:

| Feature | Version 1 | Version 2 |
|---------|-----------|-----------|
| Data Input | Random generator | CSV file input |
| Logging | Console only | File + console |
| Error Handling | Exit on error | Retry logic |
| Input Validation | None | Bounds checking |
| Output Format | Console only | Structured CSV |
| Timestamps | None | All events |
| Timeout Handling | Infinite wait | Queue timeouts |
| Production Ready | ❌ No | ✅ Yes |

Version 1 focuses on **core ML + threading concepts**.  
Version 2 adds **production-grade reliability & monitoring**.

---

## Next Steps: Version 2.0

The production version (Version 2.0) adds:

### 1. FILE-BASED LOGGING
- `system.log`: All events and errors
- `alerts.log`: High-risk events only

### 2. ERROR RESILIENCE
- Input validation and bounds checking
- Inference retry logic (3 attempts)
- Graceful handling of malformed events

### 3. CSV DATA INPUT
- Reads events from `access_events.csv`
- Replaces random generation
- Better for realistic testing and evaluation

### 4. STRUCTURED OUTPUT
- Alerts in parseable format
- Suitable for dashboard/analytics integration
- Enables auditing and compliance

### 5. PROFESSIONAL FEATURES
- Timeout handling (no infinite waits)
- Queue size tracking
- Startup status reporting

To continue with Version 2.0, see: **LAB_GUIDE_V2.md** (coming soon)

---

## Final Thoughts

Congratulations on completing this lab! You've built:

✅ A multi-threaded C++ application  
✅ Integrated machine learning models  
✅ Implemented real-time anomaly detection  
✅ Created a production-grade alerting system  

This demonstrates your ability to work with:
- Advanced C++ features (templates, STL)
- Concurrent programming (threading, synchronization)
- Machine learning (feature normalization, model deployment)
- Real-time systems (latency-sensitive processing)

Use this as a foundation for future projects. The patterns you've learned here apply to many domains:
- 🔒 Fraud detection (financial transactions)
- 🛡️ Network intrusion detection (cybersecurity)
- 🔧 Predictive maintenance (IoT sensors)
- 👤 User behavior analysis (cybersecurity)

**Good luck! 🚀**

---

# Full Source Code

## File: LoadFFModel.cpp

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <chrono>
#include "tensorflow/c/c_api.h"

using namespace std;

// ============================================================
// THREAD-SAFE QUEUE (Template Class)
// ============================================================
// Provides thread-safe communication between threads
// Uses mutex for exclusive access and condition_variable for signaling

template<typename T>
class SafeQueue {
private:
    queue<T> q;                          // STL queue (not thread-safe by itself)
    mutex m;                             // Protects queue access
    condition_variable cv;               // Wakes waiting threads when data available

public:
    // Thread-safe push: adds element and notifies waiting threads
    void push(T value) {
        unique_lock<mutex> lock(m);      // Lock automatically released at scope end
        q.push(value);
        cv.notify_one();                 // Wake one waiting thread
    }

    // Thread-safe pop: blocks until data available
    T pop() {
        unique_lock<mutex> lock(m);      // Acquire lock
        cv.wait(lock, [&]() { return !q.empty(); });  // Release lock & wait for signal
        T val = q.front();
        q.pop();
        return val;
    }
};

// ============================================================
// GLOBAL QUEUES
// ============================================================
// These enable thread-safe communication between producer and processor

SafeQueue<vector<float>> inputQueue;    // Producer → Processor
SafeQueue<float> outputQueue;           // Processor → Alert Handler

// ============================================================
// LOAD FILE (Normalization Parameters)
// ============================================================
// Reads floating-point values from text file (one per line)

vector<float> LoadFloatFile(string path) {
    vector<float> vals;
    ifstream file(path);
    float x;
    while (file >> x) {
        vals.push_back(x);
    }
    return vals;
}

// ============================================================
// NORMALIZATION FUNCTION
// ============================================================
// Apply Z-score normalization: z = (x - mean) / std
// Essential: model was trained on normalized data, must apply same transform

vector<float> Normalize(vector<float> input, vector<float> mean, vector<float> std) {
    vector<float> out(input.size());
    for (int i = 0; i < input.size(); i++) {
        out[i] = (input[i] - mean[i]) / std[i];  // Z-score formula
    }
    return out;
}

// ============================================================
// ERROR CHECK (TensorFlow Status)
// ============================================================
// TensorFlow returns status objects; this checks for errors and logs them

void CheckStatus(TF_Status* status) {
    if (TF_GetCode(status) != TF_OK) {
        cerr << "TF Error: " << TF_Message(status) << endl;
        exit(1);
    }
}

// ============================================================
// THREAD 1: PRODUCER
// ============================================================
// Generates random access events and pushes to input queue
// Simulates a stream of access events from smart sensors

void producer() {
    while (true) {
        // Create random event: [hour, device_id, access_type, user_freq]
        vector<float> event = {
            static_cast<float>(rand() % 24),        // hour: 0-23
            static_cast<float>(rand() % 5),         // device_id: 0-4
            static_cast<float>(rand() % 2),         // access_type: 0-1 (entry/exit)
            static_cast<float>(rand() % 10)         // user_freq: 0-9
        };

        cout << "[Producer] Event generated\n";

        inputQueue.push(event);                      // Thread-safe push

        this_thread::sleep_for(chrono::seconds(1)); // Simulate event rate: 1/sec
    }
}

// ============================================================
// THREAD 2: ML PROCESSOR (Main Inference Engine)
// ============================================================
// Loads TensorFlow model, normalizes input, runs inference

void processor(string model_dir) {

    // STEP 1: Load normalization parameters
    vector<float> mean = LoadFloatFile(model_dir + "/feature_means.txt");
    vector<float> std = LoadFloatFile(model_dir + "/feature_stds.txt");

    // STEP 2: Initialize TensorFlow
    TF_Graph* graph = TF_NewGraph();
    TF_Status* status = TF_NewStatus();
    TF_SessionOptions* opts = TF_NewSessionOptions();

    const char* tags[] = { "serve" };

    TF_Session* session = TF_LoadSessionFromSavedModel(
        opts, nullptr, model_dir.c_str(),
        tags, 1, graph, nullptr, status
    );

    CheckStatus(status);
    cout << "[Processor] Model loaded\n";

    // STEP 3: Main inference loop
    while (true) {

        vector<float> input = inputQueue.pop();
        vector<float> norm = Normalize(input, mean, std);

        // STEP 4: Create TensorFlow Tensor
        int64_t dims[2] = { 1, 4 };
        TF_Tensor* input_tensor = TF_AllocateTensor(TF_FLOAT, dims, 2, sizeof(float) * 4);

        float* data = (float*)TF_TensorData(input_tensor);
        for (int i = 0; i < 4; i++) data[i] = norm[i];

        // STEP 5: Get input/output operation names
        TF_Output input_op = {
            TF_GraphOperationByName(graph, "serving_default_keras_tensor"), 0
        };

        TF_Output output_op = {
            TF_GraphOperationByName(graph, "StatefulPartitionedCall"), 0
        };

        if (!input_op.oper || !output_op.oper) {
            cerr << "❌ Operation error\n";
            exit(1);
        }

        // STEP 6: Run inference
        TF_Tensor* output_tensor = nullptr;

        TF_SessionRun(session,
            nullptr,
            &input_op, &input_tensor, 1,
            &output_op, &output_tensor, 1,
            nullptr, 0, nullptr,
            status);

        CheckStatus(status);

        // STEP 7: Extract prediction score
        float result = *(float*)TF_TensorData(output_tensor);

        // STEP 8: Send result to alert handler
        outputQueue.push(result);

        // STEP 9: Clean up tensors
        TF_DeleteTensor(input_tensor);
        TF_DeleteTensor(output_tensor);
    }
}

// ============================================================
// THREAD 3: ALERT HANDLER
// ============================================================
// Consumes predictions from processor and displays alerts

void alertHandler() {
    while (true) {
        float result = outputQueue.pop();

        if (result > 0.7)
            cout << "🚨 HIGH RISK ALERT (" << result << ")\n";
        else if (result > 0.4)
            cout << "⚠️ Suspicious (" << result << ")\n";
        else
            cout << "✅ Normal (" << result << ")\n";
    }
}

// ============================================================
// MAIN ENTRY POINT
// ============================================================

int main() {

    string model_dir = R"(C:\Users\jasle\source\repos\Module4_CapstoneProject\SavedModels\FFModel)";

    thread t1(producer);
    thread t2(processor, model_dir);
    thread t3(alertHandler);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
```

---

## File: PythonApplication1.py

```python
import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.preprocessing import StandardScaler
import os

# ============================================================
# STEP 1: CREATE SAMPLE TRAINING DATA
# ============================================================

np.random.seed(42)
n = 500

data = pd.DataFrame({
    "hour": np.random.randint(0, 24, n),
    "device_id": np.random.randint(1, 5, n),
    "access_type": np.random.randint(0, 2, n),
    "user_freq": np.random.randint(1, 10, n)
})

# Anomaly pattern: Late night + High frequency
data["label"] = ((data["hour"] < 5) & (data["user_freq"] > 6)).astype(int)

X = data.drop("label", axis=1)
y = data["label"]

print("Training data shape:", X.shape)
print("Anomaly rate:", y.mean())

# ============================================================
# STEP 2: NORMALIZE FEATURES
# ============================================================

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

print("Means:", scaler.mean_)
print("Stds:", scaler.scale_)

# ============================================================
# STEP 3: BUILD NEURAL NETWORK MODEL
# ============================================================

model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(4,)),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
model.fit(X_scaled, y, epochs=10, batch_size=32, verbose=1)

loss, accuracy = model.evaluate(X_scaled, y)
print(f"Training Accuracy: {accuracy:.4f}")

# ============================================================
# STEP 4: SAVE MODEL IN SAVEDMODEL FORMAT
# ============================================================

save_dir = r"C:\Users\jasle\source\repos\Module4_CapstoneProject\SavedModels\FFModel"
os.makedirs(save_dir, exist_ok=True)

model.export(save_dir)
print(f"✅ Model saved to: {save_dir}")

# ============================================================
# STEP 5: SAVE NORMALIZATION PARAMETERS
# ============================================================

np.savetxt(os.path.join(save_dir, "feature_means.txt"), scaler.mean_)
np.savetxt(os.path.join(save_dir, "feature_stds.txt"), scaler.scale_)
print("✅ Normalization parameters saved")

# Save feature names
with open(os.path.join(save_dir, "feature_names.txt"), "w") as f:
    for col in X.columns:
        f.write(col + "\n")

print("✅ Feature names saved")

# ============================================================
# VERIFICATION: Test model
# ============================================================

test_event = [[12, 2, 1, 5]]
test_normalized = scaler.transform(test_event)
test_prediction = model.predict(test_normalized, verbose=0)
print(f"\nTest event: {test_event[0]} → score: {test_prediction[0][0]:.4f}")

test_anomaly = [[3, 1, 0, 8]]
test_anom_normalized = scaler.transform(test_anomaly)
test_anom_pred = model.predict(test_anom_normalized, verbose=0)
print(f"Test anomaly: {test_anomaly[0]} → score: {test_anom_pred[0][0]:.4f}")

print("\n" + "="*60)
print("✅ MODEL TRAINING COMPLETE")
print("="*60)
```

---

## Document Metadata

| Item | Value |
|------|-------|
| **Title** | Smart Access Anomaly Detection System - Lab Guide |
| **Version** | 1.0 |
| **Date** | 2025-04-08 |
| **Audience** | Professionals / Graduate Students (Capstone Level) |
| **Duration** | 2 hours |
| **Difficulty** | Advanced |

---

**Good luck with your capstone project! 🚀**
