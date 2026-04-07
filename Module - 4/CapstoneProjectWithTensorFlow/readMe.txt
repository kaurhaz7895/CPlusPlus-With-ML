================================================================================
LAB GUIDE: SMART ACCESS ANOMALY DETECTION & ALERTING SYSTEM
Version 1.0 - Multi-Threaded ML Inference with TensorFlow C API
================================================================================
 
LAB NAME
────────────────────────────────────────────────────────────────────────────
Smart Access Control Anomaly Detection & Real-Time Alert System
 
COURSE
──────
Module 4 - Capstone Project: Production ML Systems in C++
 
DURATION
────────
4-6 hours (including setup, implementation, testing, and deployment)
 
================================================================================
LEARNING OBJECTIVES
================================================================================
 
By completing this lab, students will:
 
1. ✅ Build a multi-threaded C++ application that integrates machine learning
2. ✅ Load and execute TensorFlow SavedModels using the C API
3. ✅ Implement thread-safe data structures for inter-process communication
4. ✅ Normalize input features using pre-trained statistical parameters
5. ✅ Execute real-time anomaly classification and alerting
6. ✅ Understand producer-consumer patterns in concurrent systems
7. ✅ Integrate Python model training with C++ inference pipeline
 
================================================================================
PREREQUISITES
================================================================================
 
✓ C++ 17 or later (with STL threading support)
✓ TensorFlow C API (see setup section)
✓ Python 3.8+ with TensorFlow & scikit-learn
✓ Visual Studio 2019+ (or equivalent C++ compiler)
✓ Basic knowledge of:
  - C++ templates and STL containers
  - Multi-threading concepts (mutexes, condition variables)
  - Machine learning basics (normalization, classification)
  - TensorFlow SavedModel format
 
================================================================================
EXECUTIVE SUMMARY
================================================================================
 
This lab implements an end-to-end anomaly detection system for smart access
control (e.g., building entry/exit systems). The system:
 
1. Generates or reads access event data (hour, device_id, access_type, user_freq)
2. Normalizes features using pre-trained mean/std statistics
3. Passes events through a TensorFlow neural network in real-time
4. Classifies events as NORMAL (score < 0.4), SUSPICIOUS (0.4-0.7), or
   HIGH RISK (> 0.7)
5. Outputs alerts to console
 
ARCHITECTURE PATTERN:
  Producer Thread    →    [Input Queue]    →    Processor Thread    →    [Output Queue]    →    Alert Handler
  (generates events)      (safe queue)       (ML inference)           (safe queue)         (displays alerts)
 
This follows the producer-consumer pattern with thread-safe queues for
decoupled communication.
 
================================================================================
SYSTEM ARCHITECTURE
================================================================================
 
┌─────────────────────────────────────────────────────────────────────────┐
│                         SMART ACCESS SYSTEM V1                          │
└─────────────────────────────────────────────────────────────────────────┘
 
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
 
================================================================================
DETAILED COMPONENT EXPLANATION
================================================================================
 
1. SAFE QUEUE (THREAD-SAFE COMMUNICATION)
   ──────────────────────────────────────────
 
   Problem: Multiple threads accessing queue simultaneously → race conditions
   Solution: Protect with mutex + condition_variable
 
   Key Features:
   • push(T value)  - Thread-safe push, notifies waiting consumers
   • pop()          - Blocks until data available, auto-locks
 
   Template allows any data type:
   - SafeQueue<vector<float>>  for input events
   - SafeQueue<float>          for predictions
 
   Synchronization:
   • unique_lock(mutex)        - Ensures exclusive access
   • condition_variable        - Wakes up blocking threads
   • cv.notify_one()          - Signals data is available
 
2. PRODUCER THREAD
   ────────────────
 
   Function: Generates random access events
   
   Event Structure:
     [hour(0-24), device_id(1-5), access_type(0-1), user_freq(1-10)]
   
   Logic:
     while(true):
       • Generate 4 random integers matching data ranges
       • Push to inputQueue
       • Sleep 1 second (simulates real event stream)
   
   Casting: rand() % N returns int, converted to float for vector<float>:
     static_cast<float>(rand() % 24)
   
   Purpose: Simulates real smart access control system ingesting events
 
3. PROCESSOR THREAD (ML INFERENCE ENGINE)
   ──────────────────────────────────────
 
   Responsibilities:
   a) Load Pre-trained Model
      • TensorFlow SavedModel format
      • Contains: computational graph + weights
      • Loaded with TF_LoadSessionFromSavedModel()
   
   b) Load Normalization Parameters
      • feature_means.txt  - Mean of each feature during training
      • feature_stds.txt   - Std dev of each feature during training
      • Used for Z-score normalization: (x - mean) / std
   
   c) Process Events in Real-Time
      For each event:
      • Pop from inputQueue (blocks if empty)
      • Normalize using loaded statistics
      • Create TensorFlow tensor (1x4 float matrix)
      • Execute model inference: TF_SessionRun()
      • Push result to outputQueue
   
   d) Error Handling
      • CheckStatus() verifies TensorFlow operations
      • Logs errors to stderr and exits if critical
   
   TensorFlow C API Workflow:
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
 
4. ALERT HANDLER THREAD
   ──────────────────────
 
   Function: Consumes predictions and displays alerts
   
   Classification Rules:
   • score > 0.7  →  🚨 HIGH RISK ALERT
   • 0.4 < score ≤ 0.7  →  ⚠ SUSPICIOUS
   • score ≤ 0.4  →  ✅ NORMAL
   
   Output Format:
     🚨 HIGH RISK ALERT (0.85)
     ⚠ Suspicious (0.55)
     ✅ Normal (0.25)
   
   Logic:
     while(true):
       • Pop from outputQueue (blocks if empty)
       • Apply thresholds
       • Print alert to console
 
5. NORMALIZATION FUNCTION
   ───────────────────────
 
   Why Normalize?
   • Training data had mean=μ, std=σ
   • Raw input has different scale
   • Model expects normalized input
   • Formula: z = (x - μ) / σ  (Z-score normalization)
   
   Example:
     hour = 23, μ_hour = 12, σ_hour = 8
     normalized = (23 - 12) / 8 = 1.375
   
   Benefits:
   • Brings all features to same scale
   • Stabilizes neural network training
   • Improves generalization
 
================================================================================
WORKFLOW DIAGRAM
================================================================================
 
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
 
================================================================================
FEATURE ENGINEERING NOTES
================================================================================
 
Input Features (4 dimensions):
┌──────────────────────────────────────────────────────────┐
│ Feature       │ Range  │ Meaning                          │
├──────────────────────────────────────────────────────────┤
│ hour          │ 0-23   │ Time of access (0=midnight)     │
│ device_id     │ 1-4    │ Which door/sensor triggered     │
│ access_type   │ 0-1    │ Entry (0) vs Exit (1)           │
│ user_freq     │ 1-9    │ How often user accesses (1-10)  │
└──────────────────────────────────────────────────────────┘
 
Anomaly Patterns Detected:
• Late-night access (hour < 5) + high frequency (freq > 6) = ANOMALY
• Unusual device combinations
• Access patterns inconsistent with training distribution
 
================================================================================
SETUP INSTRUCTIONS
================================================================================
 
STEP 1: INSTALL TENSORFLOW C API
─────────────────────────────────
 
Windows:
1. Download from: https://www.tensorflow.org/install/lang_c
2. Extract to: C:\tensorflow (or your preferred location)
3. Download TensorFlow C library (.dll + .lib files)
 
Linux:
  $ apt-get install libtensorflow libtensorflow-dev
 
STEP 2: CONFIGURE VISUAL STUDIO PROJECT
────────────────────────────────────────
 
Right-click Project → Properties:
 
C/C++ → General → Additional Include Directories:
  Add: C:\tensorflow\include
 
Linker → General → Additional Library Directories:
  Add: C:\tensorflow\lib
 
Linker → Input → Additional Dependencies:
  Add: tensorflow.lib
 
STEP 3: COMPILE & LINK
──────────────────────
 
Command Line:
  cl /std:c++17 /I"C:\tensorflow\include" LoadFFModel.cpp ^
     /link /LIBPATH:"C:\tensorflow\lib" tensorflow.lib
 
Visual Studio:
  Build → Build Solution (Ctrl+Shift+B)
 
================================================================================
EXPECTED OUTPUT
================================================================================
 
Console Output:
────────────────
[Processor] Model loaded
[Producer] Event generated
[Producer] Event generated
[Producer] Event generated
[Processor] Event generated
 
✅ Normal (0.23)
✅ Normal (0.35)
⚠ Suspicious (0.58)
[Producer] Event generated
 
🚨 HIGH RISK ALERT (0.82)
✅ Normal (0.18)
⚠ Suspicious (0.51)
 
(repeats continuously)
 
System Behavior:
• Producer generates 1 event per second
• Processor runs inference immediately
• AlertHandler outputs classification
• Infinite loop (Ctrl+C to stop)
 
================================================================================
TESTING & VALIDATION
================================================================================
 
TEST 1: Basic Execution
───────────────────────
Step 1: Run the program
Step 2: Observe output flowing
Step 3: Verify no crashes
Expected: Continuous alerts at ~1 per second
 
TEST 2: Thread Safety
─────────────────────
Visual Studio → Debug → Windows → Threads
Verify 3 threads running:
  • Thread 1: producer
  • Thread 2: processor
  • Thread 3: alertHandler
 
TEST 3: Model Loading
──────────────────────
Add breakpoint at: "cout << "[Processor] Model loaded"
Expected: Model loads within 2-3 seconds
If error: Check model path and TensorFlow installation
 
TEST 4: Inference Correctness
──────────────────────────────
Verify outputs range [0.0, 1.0]
Expected: mix of normal/suspicious/high-risk
If always normal: Check normalization parameters
 
================================================================================
COMMON ERRORS & SOLUTIONS
================================================================================
 
ERROR 1: "unresolved external symbol _imp_TF_NewGraph"
───────────────────────────────────────────────────────
Cause: TensorFlow library not linked
Fix:
  1. Download TensorFlow C API
  2. Add tensorflow.lib to Linker → Input → Additional Dependencies
  3. Add include path to C/C++ → General → Additional Include Directories
 
ERROR 2: "Failed to load model from SavedModel"
────────────────────────────────────────────────
Cause: Incorrect model path or missing files
Fix:
  1. Verify path exists: C:\Users\...\SavedModels\FFModel\
  2. Check files present:
     - saved_model.pb
     - variables/ (directory)
     - assets/ (directory)
  3. Update model_dir in main()
 
ERROR 3: "Operation error" (input_op or output_op is null)
──────────────────────────────────────────────────────────
Cause: Node names don't match your SavedModel
Fix:
  1. Run Python to inspect model:
     import tensorflow as tf
     model = tf.saved_model.load("SavedModels/FFModel")
     print(model.signatures)
  2. Update node names in C++:
     "serving_default_keras_tensor"  (input)
     "StatefulPartitionedCall"       (output)
 
ERROR 4: Alerts always "Normal" or always "High Risk"
──────────────────────────────────────────────────────
Cause: Incorrect normalization parameters
Fix:
  1. Regenerate model: python PythonApplication1.py
  2. Verify feature_means.txt and feature_stds.txt contain 4 floats each
  3. Check normalization logic applies mean/std correctly
 
================================================================================
PERFORMANCE METRICS
================================================================================
 
Typical Performance (on modern hardware):
• Model Load Time:        ~1-2 seconds
• Inference Latency:      ~10-50 ms per event
• Throughput:             ~20-100 events/sec
• Memory Usage:           ~100-300 MB (model + buffers)
• Thread Creation Time:   ~1-2 ms per thread
 
Bottlenecks:
1. TensorFlow inference (CPU) - major bottleneck
2. File I/O - only on startup
3. Queue operations - negligible (~1 μs)
 
Optimization Tips:
• Use GPU acceleration (requires TensorFlow GPU + CUDA)
• Batch process events (group multiple inputs)
• Pre-allocate tensors (reuse across inferences)
 
================================================================================
EXTENSION EXERCISES
================================================================================
 
Exercise 1: Batch Processing
─────────────────────────────
Modify processor to handle 10 events at once:
  • Create 10x4 tensor instead of 1x4
  • Run inference once per batch
  • Distribute outputs to 10 alert handlers
  • Measure speedup
 
Exercise 2: Dynamic Thresholds
───────────────────────────────
Load thresholds from config file instead of hardcoded:
  threshold_suspicious = 0.4
  threshold_high_risk = 0.7
  
Update alerts based on device_id (device 3 = more sensitive)
 
Exercise 3: Persistence
────────────────────────
Save high-risk events to file:
  CSV format: timestamp, hour, device_id, access_type, user_freq, score
 
Exercise 4: Multiple Models
────────────────────────────
Load 2 different SavedModels:
  • Model A: trained on normal users
  • Model B: trained on suspicious patterns
  • Ensemble voting for final prediction
 
================================================================================
DEPENDENCIES SUMMARY
================================================================================
 
RUNTIME DEPENDENCIES:
┌────────────────────────────────────────────────────────────┐
│ Library          │ Version │ Purpose                        │
├────────────────────────────────────────────────────────────┤
│ TensorFlow C API │ 2.10+   │ Model loading & inference      │
│ C++ Standard     │ C++17   │ Threading, STL containers      │
│ Windows API      │ -       │ Memory management (implicit)   │
└────────────────────────────────────────────────────────────┘
 
BUILD DEPENDENCIES:
├────────────────────────────────────────────────────────────┤
│ Compiler         │ MSVC 2019+ or GCC 9+                   │
│ Visual Studio    │ 2019 or later                          │
│ CMake (optional) │ 3.15+ for automated builds             │
└────────────────────────────────────────────────────────────┘
 
================================================================================
KEY TAKEAWAYS
================================================================================
 
1. ✅ Producer-Consumer Pattern
   Decoupled threads communicate via thread-safe queues
   → Scalable, flexible architecture
 
2. ✅ ML Model Integration
   TensorFlow C API enables inference in production systems
   → Bridge between ML and systems programming
 
3. ✅ Concurrency & Synchronization
   Mutex + Condition Variables = thread-safe communication
   → Prevents race conditions, data corruption
 
4. ✅ Real-Time Processing
   Sub-second latency from input to alert
   → Suitable for time-critical systems
 
5. ✅ Anomaly Detection
   Classification thresholds enable multi-tier alerting
   → Practical security/monitoring application
 
================================================================================
REFERENCES
================================================================================
 
TensorFlow C API:
  https://www.tensorflow.org/api_docs/cc
  https://github.com/tensorflow/tensorflow/tree/master/tensorflow/c
 
C++ Concurrency:
  https://cppreference.com/w/cpp/thread
  "C++ Concurrency in Action" - Anthony Williams
 
Machine Learning:
  https://scikit-learn.org/stable/modules/preprocessing.html
  https://www.tensorflow.org/guide/saved_model
 
SavedModel Format:
  https://www.tensorflow.org/guide/saved_model
 
Thread Safety Best Practices:
  https://herbsutter.com/2012/08/21/internet-face-to-face-meeting-on-c-parallelism/
 
================================================================================
SUBMISSION CHECKLIST
================================================================================
 
Before submitting, verify:
 
[ ] Code compiles without errors
[ ] All 3 threads running (checked in debugger)
[ ] Model loads successfully
[ ] Alerts appear at console within 5 seconds
[ ] No memory leaks (use Visual Studio diagnostic tools)
[ ] Code is well-commented
[ ] Variable names are meaningful
[ ] No hardcoded paths (use R"(...)" strings)
[ ] README.md included with instructions
[ ] Python training script included
[ ] All source files included in submission
 
================================================================================
