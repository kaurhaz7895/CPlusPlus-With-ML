# Lab 1: C++ and TensorFlow Setup Verification

## Objective
Confirm Visual Studio integration, TensorFlow headers/libs, and runtime functionality

## Expected Outcome
Participants can build and run a basic TensorFlow C++ program and troubleshoot setup

---

## Duration: 30-45 minutes

## Prerequisites
- Visual Studio 2019+ installed
- TensorFlow C API installed locally
- Basic C++ knowledge

---

## Part 1: Environment Verification (10 minutes)

### 1.1 Verify TensorFlow Installation

**Check if TensorFlow C API is installed:**

```cmd
# Navigate to TensorFlow installation folder
cd C:\path\to\tensorflow

# List the contents
dir
```

**You should see:**
```
bin\        (DLL files)
include\    (Header files)
lib\        (Library files)
```

### 1.2 Check Include Files

**Verify header files exist:**

```cmd
dir include\tensorflow
dir include\tensorflow\c
```

**Expected files:**
```
c_api.h
c_api_experimental.h
tf_datatype.h
tf_status.h
```

### 1.3 Check Library Files

**Verify library files exist:**

```cmd
dir lib\
```

**Expected files:**
```
tensorflow.lib
tensorflow_cc.lib (if C++ API)
c10.lib (if LibTorch-like)
```

### 1.4 Check Runtime Libraries

**Verify DLL files exist:**

```cmd
dir bin\
```

**Expected files:**
```
tensorflow.dll
tensorflow_cc.dll (if present)
```

---

## Part 2: Visual Studio Project Setup (15 minutes)

### 2.1 Create New Project

1. Open Visual Studio
2. **File** → **New** → **Project**
3. Select **Empty C++ Project**
4. Name: `Lab1_TensorFlow_Setup`
5. Location: Your preferred path
6. Click **Create**

### 2.2 Create Source File

1. Right-click **Source Files** in Solution Explorer
2. **Add** → **New Item**
3. Select **C++ File (.cpp)**
4. Name: `main.cpp`

### 2.3 Add Test Code

Copy this code into `main.cpp`:

```cpp
#include <iostream>
#include <tensorflow/c/c_api.h>

using namespace std;

int main() {
    cout << "================================" << endl;
    cout << "TensorFlow C API Setup Verification" << endl;
    cout << "================================" << endl << endl;

    // Test 1: Check if TensorFlow is loaded
    cout << "[TEST 1] TensorFlow library loaded" << endl;
    cout << "Status: ✓ Successfully included tensorflow/c/c_api.h" << endl << endl;

    // Test 2: Create and inspect a simple tensor
    cout << "[TEST 2] Creating a tensor" << endl;
    
    float data[3] = {1.0f, 2.0f, 3.0f};
    int64_t dims[1] = {3};
    
    // Create tensor
    TF_Tensor* tensor = TF_NewTensor(
        TF_FLOAT,
        dims,
        1,
        data,
        sizeof(data),
        [](void* data, size_t, void*) {},  // No-op deallocator
        nullptr
    );
    
    if (tensor == nullptr) {
        cout << "Status: ✗ Failed to create tensor" << endl;
        return 1;
    }
    cout << "Status: ✓ Tensor created successfully" << endl;
    cout << "  - Data Type: TF_FLOAT (32-bit floating point)" << endl;
    cout << "  - Dimensions: 1" << endl;
    cout << "  - Dimension Size: " << TF_Dim(tensor, 0) << endl;
    cout << "  - Total Elements: " << TF_Dim(tensor, 0) << endl;
    cout << "  - Byte Size: " << TF_TensorByteSize(tensor) << " bytes" << endl << endl;

    // Test 3: Access tensor data
    cout << "[TEST 3] Accessing tensor data" << endl;
    float* tensor_data = static_cast<float*>(TF_TensorData(tensor));
    cout << "Status: ✓ Data accessed successfully" << endl;
    cout << "  - Values: [";
    for (int i = 0; i < 3; i++) {
        cout << tensor_data[i];
        if (i < 2) cout << ", ";
    }
    cout << "]" << endl << endl;

    // Test 4: Cleanup
    cout << "[TEST 4] Resource cleanup" << endl;
    TF_DeleteTensor(tensor);
    cout << "Status: ✓ Tensor deleted successfully" << endl << endl;

    // Summary
    cout << "================================" << endl;
    cout << "SETUP VERIFICATION COMPLETE" << endl;
    cout << "================================" << endl;
    cout << "All tests passed! ✓" << endl;
    cout << "TensorFlow C API is properly configured." << endl;

    return 0;
}
```

### 2.4 Configure Include Directories

1. Right-click **project** → **Properties**
2. **Configuration:** All Configurations
3. **Platform:** x64
4. Go to **VC++ Directories**
5. **Include Directories** → Click **...**
6. Add: `C:\path\to\tensorflow\include`
7. Click **OK**

### 2.5 Configure Library Directories

1. Still in **Properties**
2. **VC++ Directories**
3. **Library Directories** → Click **...**
4. Add: `C:\path\to\tensorflow\lib`
5. Click **OK**

### 2.6 Link Libraries

1. Still in **Properties**
2. **Linker** → **Input**
3. **Additional Dependencies** → Click **...**
4. Add: `tensorflow.lib`
5. Click **OK**

### 2.7 Copy DLL Files

1. Go to: `C:\path\to\tensorflow\bin\`
2. Copy all `.dll` files
3. Paste into your project folder
4. Or set Windows PATH environment variable

---

## Part 3: Build and Run (10 minutes)

### 3.1 Build Project

1. Press **Ctrl+Shift+B** to build
2. Check **Output** window for success message
3. Should see: "Build succeeded"

### 3.2 Run Program

1. Press **Ctrl+F5** to run without debugging
2. A console window should appear

### 3.3 Expected Output

```
================================
TensorFlow C API Setup Verification
================================

[TEST 1] TensorFlow library loaded
Status: ✓ Successfully included tensorflow/c/c_api.h

[TEST 2] Creating a tensor
Status: ✓ Tensor created successfully
  - Data Type: TF_FLOAT (32-bit floating point)
  - Dimensions: 1
  - Dimension Size: 3
  - Total Elements: 3
  - Byte Size: 12 bytes

[TEST 3] Accessing tensor data
Status: ✓ Data accessed successfully
  - Values: [1, 2, 3]

[TEST 4] Resource cleanup
Status: ✓ Tensor deleted successfully

================================
SETUP VERIFICATION COMPLETE
================================
All tests passed! ✓
TensorFlow C API is properly configured.
```

---

## Troubleshooting Guide

### Issue 1: "fatal error C1083: Cannot open include file: 'tensorflow/c/c_api.h'"

**Solution:**
1. Check include path in Properties → VC++ Directories
2. Verify path matches your TensorFlow installation
3. Make sure it ends with `\include`
4. Rebuild project

### Issue 2: "unresolved external symbol TF_NewTensor"

**Solution:**
1. Check library path in Properties → VC++ Directories
2. Verify path ends with `\lib`
3. Check if `tensorflow.lib` is listed in Additional Dependencies
4. Rebuild project

### Issue 3: "tensorflow.dll not found" (runtime error)

**Solution:**
1. Copy DLL files from `\bin\` to project folder
2. Or add to Windows PATH environment variable
3. Restart Visual Studio
4. Run again

### Issue 4: Project builds but crashes immediately

**Solution:**
1. Make sure DLL files are in correct location
2. Check that tensor creation didn't return nullptr
3. Verify data size calculation: `sizeof(data)` is correct
4. Check array bounds when accessing data

---

## Verification Checklist

- [ ] TensorFlow installation folder exists
- [ ] `include\tensorflow\` folder contains headers
- [ ] `lib\` folder contains `.lib` files
- [ ] `bin\` folder contains `.dll` files
- [ ] Visual Studio project created
- [ ] Include directories configured
- [ ] Library directories configured
- [ ] Libraries linked (tensorflow.lib)
- [ ] DLL files copied or PATH set
- [ ] Project builds without errors
- [ ] Program runs without errors
- [ ] All tests pass (4/4) ✓

---

## Learning Outcomes

By completing this lab, you should:

✅ Understand TensorFlow C API structure
✅ Know how to configure Visual Studio for TensorFlow
✅ Understand tensor creation and deletion
✅ Know how to access tensor data safely
✅ Be able to troubleshoot basic setup issues
✅ Understand memory management (deallocators)
✅ Know what TensorFlow C API files do what

---

## Advanced Tasks (Optional)

### Task 1: Create Multiple Tensors

Modify the code to create:
- A 2D tensor (2×3 matrix)
- A tensor with different data type (TF_INT32)
- A larger tensor (100+ elements)

### Task 2: Verify Runtime Libraries

Write code that:
- Checks TensorFlow version
- Reports all tensor properties
- Validates data integrity after creation

### Task 3: Error Handling

Add error handling for:
- Failed tensor creation
- Invalid tensor data
- Null pointer access

---

## Summary

**Lab 1 Goal:** Verify that TensorFlow C API is properly integrated into your Visual Studio project

**What You Did:**
1. ✓ Verified TensorFlow installation
2. ✓ Configured Visual Studio project
3. ✓ Created and tested tensor operations
4. ✓ Verified memory management
5. ✓ Confirmed runtime setup

**Next Lab:** Tensor Fundamentals - Create and manipulate tensors of various shapes and types

---

## References

- TensorFlow C API Documentation: https://github.com/tensorflow/tensorflow/tree/master/tensorflow/c
- C API Header: `tensorflow/c/c_api.h`
- Data Types: `tensorflow/c/tf_datatype.h`

---

## Time Allocation

```
Part 1: Environment Verification  10 minutes
Part 2: Visual Studio Setup       15 minutes
Part 3: Build and Run             10 minutes
Troubleshooting (if needed)       10-20 minutes
─────────────────────────────────
Total:                            30-45 minutes
```

Good luck! If you complete this lab successfully, your TensorFlow C API setup is ready for advanced work! 🎉
