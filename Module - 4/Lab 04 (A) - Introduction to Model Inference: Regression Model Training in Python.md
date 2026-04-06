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

**File: `create_LR_model.py`**

```python
import os
import zipfile
import urllib.request
import json
import pandas as pd
import numpy as np
import tensorflow as tf
from tensorflow import keras

# Reproducibility
np.random.seed(42)
tf.random.set_seed(42)

# ================== 1. Download ==================
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/00360/AirQualityUCI.zip"
zip_path = "air_quality.zip"
data_dir = "air_quality_data"

if not os.path.exists(zip_path):
    print("Downloading dataset...")
    try:
        urllib.request.urlretrieve(url, zip_path)
    except Exception as e:
        raise RuntimeError(f"Failed to download dataset: {e}")

# ================== 2. Extract ==================
if not os.path.exists(data_dir):
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(data_dir)
    except Exception as e:
        raise RuntimeError(f"Failed to extract dataset: {e}")

# CSV uses ';' and has some odd columns
csv_path = os.path.join(data_dir, "AirQualityUCI.csv")

# ================== 3. Load & Clean ==================
df = pd.read_csv(csv_path, sep=';', decimal=',')

# Drop completely empty columns
df = df.dropna(axis=1, how='all')

# Replace -200 (missing value marker) with NaN
df = df.replace(-200, np.nan)

# Keep only useful numeric columns (Temperature, Humidity, CO)
cols = ["T", "RH", "CO(GT)"]
df = df[cols].dropna()

# Rename for clarity
df.columns = ["temp", "humidity", "co"]

# ================== 4. Prepare data ==================
# Predict CO from temperature and humidity -> do not include CO as an input feature
X = df[["temp", "humidity"]].values.astype(np.float32)
# Target
y = df["co"].values.astype(np.float32).reshape(-1, 1)

# Simple train/test split
perm = np.random.permutation(len(X))
n_train = int(len(X) * 0.8)
train_idx = perm[:n_train]
test_idx = perm[n_train:]
X_train, X_test = X[train_idx], X[test_idx]
y_train, y_test = y[train_idx], y[test_idx]

# Compute normalization using training data and save parameters for later
eps = 1e-8
X_mean = X_train.mean(axis=0)
X_std = X_train.std(axis=0)
y_mean = y_train.mean(axis=0)
y_std = y_train.std(axis=0)

X_train = (X_train - X_mean) / (X_std + eps)
X_test = (X_test - X_mean) / (X_std + eps)
y_train = (y_train - y_mean) / (y_std + eps)
y_test = (y_test - y_mean) / (y_std + eps)

# Save scaler params so C++ or other consumers can reproduce preprocessing
np.savez("scaler.npz", X_mean=X_mean, X_std=X_std, y_mean=y_mean, y_std=y_std)

# ================== 5. Train model ==================
# Small network with one hidden layer and early stopping
model = keras.Sequential([
    keras.layers.Dense(16, activation='relu', input_shape=(2,)),
    keras.layers.Dense(1)
])

model.compile(optimizer='adam', loss='mse')
early = keras.callbacks.EarlyStopping(patience=10, restore_best_weights=True)
model.fit(X_train, y_train, validation_data=(X_test, y_test), epochs=200, callbacks=[early], verbose=1)

# Evaluate
mse = model.evaluate(X_test, y_test, verbose=0)
print(f"Test MSE (normalized target): {mse:.6f}")

# ================== 6. Export model and final layer weights ==================
# Save full model (TensorFlow SavedModel)
save_dir = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\SavedModels\LR"
os.makedirs(save_dir, exist_ok=True)

model_path = os.path.join(save_dir, "LR.keras")
model.save(model_path)

# Save final layer weights and bias (useful for small deployments)
final_w, final_b = model.layers[-1].get_weights()
np.savetxt(os.path.join(save_dir, "final_weights.txt"), final_w)
np.savetxt(os.path.join(save_dir, "final_bias.txt"), final_b)

print(f"Done. Saved model to {model_path}, final weights/bias to {save_dir}, and scaler.npz")

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
### Python Training Code - Complete Explanation

## Overview

This Python script trains a **regression neural network** to predict air quality (CO levels) from temperature and humidity.

**Purpose:** Generate a trained model that C++ can load and use for inference.

---

## Part 1: Imports & Setup

```python
import os
import zipfile
import urllib.request
import json
import pandas as pd
import numpy as np
import tensorflow as tf
from tensorflow import keras

# Reproducibility
np.random.seed(42)
tf.random.set_seed(42)
```

### What Each Import Does

| Import | Purpose |
|--------|---------|
| `os` | File/directory operations |
| `zipfile` | Handle .zip files |
| `urllib.request` | Download from internet |
| `pandas` | Read/manipulate CSV data |
| `numpy` | Numerical operations |
| `tensorflow` | ML framework |
| `keras` | High-level neural network API |

### Seeds (Reproducibility)

```python
np.random.seed(42)
tf.random.set_seed(42)
```

**Why?** Makes results reproducible - same random numbers every time you run.

---

## Part 2: Download Dataset

```python
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/00360/AirQualityUCI.zip"
zip_path = "air_quality.zip"
data_dir = "air_quality_data"

if not os.path.exists(zip_path):
    print("Downloading dataset...")
    try:
        urllib.request.urlretrieve(url, zip_path)
    except Exception as e:
        raise RuntimeError(f"Failed to download dataset: {e}")
```

### What Happens

**Step 1: Define paths**
```
url = download location
zip_path = "air_quality.zip" (where to save)
data_dir = "air_quality_data" (where to extract)
```

**Step 2: Check if already downloaded**
```python
if not os.path.exists(zip_path):
    # Only download if file doesn't exist
```

**Step 3: Download**
```python
urllib.request.urlretrieve(url, zip_path)
```
Downloads the file from URL to `zip_path`

**Step 4: Error handling**
```python
try:
    # download code
except Exception as e:
    raise RuntimeError(...)
```
If download fails, shows error message

### Output
```
Downloading dataset...
```

---

## Part 3: Extract ZIP File

```python
if not os.path.exists(data_dir):
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(data_dir)
    except Exception as e:
        raise RuntimeError(f"Failed to extract dataset: {e}")

csv_path = os.path.join(data_dir, "AirQualityUCI.csv")
```

### What Happens

**Step 1: Check if extracted**
```python
if not os.path.exists(data_dir):
    # Only extract if folder doesn't exist
```

**Step 2: Extract ZIP**
```python
with zipfile.ZipFile(zip_path, 'r') as zip_ref:
    zip_ref.extractall(data_dir)
```
- Opens zip file
- Extracts all files to `data_dir`

**Step 3: Prepare CSV path**
```python
csv_path = os.path.join(data_dir, "AirQualityUCI.csv")
```
Creates path: `air_quality_data/AirQualityUCI.csv`

---

## Part 4: Load & Clean Data

```python
df = pd.read_csv(csv_path, sep=';', decimal=',')

# Drop completely empty columns
df = df.dropna(axis=1, how='all')

# Replace -200 (missing value marker) with NaN
df = df.replace(-200, np.nan)

# Keep only useful numeric columns (Temperature, Humidity, CO)
cols = ["T", "RH", "CO(GT)"]
df = df[cols].dropna()

# Rename for clarity
df.columns = ["temp", "humidity", "co"]
```

### Step-by-Step Breakdown

**Step 1: Read CSV**
```python
df = pd.read_csv(csv_path, sep=';', decimal=',')
```
- `sep=';'` = columns separated by semicolon
- `decimal=','` = decimal point is comma (European format)

**Step 2: Remove completely empty columns**
```python
df = df.dropna(axis=1, how='all')
```
- `axis=1` = work on columns
- `how='all'` = drop if ALL values are missing

**Step 3: Clean missing values**
```python
df = df.replace(-200, np.nan)
```
The dataset uses `-200` to indicate missing data. Replace with `NaN` (proper missing value).

**Step 4: Select useful columns**
```python
cols = ["T", "RH", "CO(GT)"]
df = df[cols].dropna()
```
- Keep only: Temperature (T), Humidity (RH), CO levels
- Drop rows with any NaN values

**Step 5: Rename columns**
```python
df.columns = ["temp", "humidity", "co"]
```
Make names clearer for downstream code.

### Result
Clean dataframe with 3 columns:
- `temp`: Temperature
- `humidity`: Humidity  
- `co`: CO level (target to predict)

---

## Part 5: Prepare Training Data

```python
# Predict CO from temperature and humidity -> do not include CO as an input feature
X = df[["temp", "humidity"]].values.astype(np.float32)
# Target
y = df["co"].values.astype(np.float32).reshape(-1, 1)

# Simple train/test split
perm = np.random.permutation(len(X))
n_train = int(len(X) * 0.8)
train_idx = perm[:n_train]
test_idx = perm[n_train:]
X_train, X_test = X[train_idx], X[test_idx]
y_train, y_test = y[train_idx], y[test_idx]
```

### What Happens

**Step 1: Separate input (X) and output (y)**
```python
X = df[["temp", "humidity"]].values.astype(np.float32)
# Input: [temp, humidity]

y = df["co"].values.astype(np.float32).reshape(-1, 1)
# Output: [co]
```

**Visualization:**
```
Data:
temp  humidity  co
20.5  45.2     150
21.3  46.1     155
...

X (inputs):        y (target):
[20.5, 45.2]  →    [150]
[21.3, 46.1]  →    [155]
```

**Step 2: Shuffle data**
```python
perm = np.random.permutation(len(X))
# perm = [5, 2, 8, 1, 9, ...]  (random order)
```

**Step 3: Split 80/20**
```python
n_train = int(len(X) * 0.8)
train_idx = perm[:n_train]    # First 80%
test_idx = perm[n_train:]     # Last 20%
```

**Step 4: Create train/test sets**
```python
X_train = X[train_idx]  # Training inputs
X_test = X[test_idx]    # Test inputs
y_train = y[train_idx]  # Training targets
y_test = y[test_idx]    # Test targets
```

### Example Numbers
```
Total samples: 100
80% train = 80 samples
20% test = 20 samples
```

---

## Part 6: Normalize Data (IMPORTANT FOR C++)

```python
# Compute normalization using training data and save parameters for later
eps = 1e-8
X_mean = X_train.mean(axis=0)
X_std = X_train.std(axis=0)
y_mean = y_train.mean(axis=0)
y_std = y_train.std(axis=0)

X_train = (X_train - X_mean) / (X_std + eps)
X_test = (X_test - X_mean) / (X_std + eps)
y_train = (y_train - y_mean) / (y_std + eps)
y_test = (y_test - y_mean) / (y_std + eps)

# Save scaler params so C++ or other consumers can reproduce preprocessing
np.savez("scaler.npz", X_mean=X_mean, X_std=X_std, y_mean=y_mean, y_std=y_std)
```

### What Happens

**Step 1: Calculate statistics from TRAINING data only**
```python
X_mean = X_train.mean(axis=0)
# [mean_temp, mean_humidity]

X_std = X_train.std(axis=0)
# [std_temp, std_humidity]
```

**Example:**
```
X_train temperatures: [10, 20, 30]
X_mean = (10+20+30)/3 = 20
X_std = sqrt(variance) = 8.16
```

**Step 2: Normalize train data**
```python
X_train = (X_train - X_mean) / (X_std + eps)
```

**Formula:** `(value - mean) / std`

**Example:**
```
Original: 10
Normalized: (10 - 20) / 8.16 = -1.22
```

**Why `+ eps`?** Avoid division by zero if std = 0

**Step 3: Apply SAME normalization to test data**
```python
X_test = (X_test - X_mean) / (X_std + eps)
```
Use training mean/std, NOT test mean/std!

**Step 4: Save parameters**
```python
np.savez("scaler.npz", X_mean=X_mean, X_std=X_std, y_mean=y_mean, y_std=y_std)
```
Saves normalization parameters to file for C++ to load and use!

### Why This Matters for C++

C++ will do:
```cpp
// In C++
X_normalized = (X_raw - X_mean) / (X_std + eps)
// Using the saved parameters from Python!
```

---

## Part 7: Train Neural Network

```python
# Small network with one hidden layer and early stopping
model = keras.Sequential([
    keras.layers.Dense(16, activation='relu', input_shape=(2,)),
    keras.layers.Dense(1)
])

model.compile(optimizer='adam', loss='mse')
early = keras.callbacks.EarlyStopping(patience=10, restore_best_weights=True)
model.fit(X_train, y_train, validation_data=(X_test, y_test), epochs=200, callbacks=[early], verbose=1)

# Evaluate
mse = model.evaluate(X_test, y_test, verbose=0)
print(f"Test MSE (normalized target): {mse:.6f}")
```

### Architecture

```
Inputs (2)
  ↓
Dense Layer (16 neurons, ReLU)
  ↓
Dense Layer (1 neuron, Linear)
  ↓
Output (1 value: predicted CO)
```

**Diagram:**
```
temp ──┐
       ├─→ [Dense 16] ──→ [Dense 1] ──→ CO_prediction
humidity─┤
```

### Building Model

```python
model = keras.Sequential([
    keras.layers.Dense(16, activation='relu', input_shape=(2,)),
    keras.layers.Dense(1)
])
```

| Layer | What it does |
|-------|-------------|
| Dense(16, relu) | 2 inputs → 16 neurons with ReLU activation |
| Dense(1) | 16 neurons → 1 output (predicted CO) |

### Compile

```python
model.compile(optimizer='adam', loss='mse')
```
- `optimizer='adam'` = Algorithm to update weights
- `loss='mse'` = Mean Squared Error (good for regression)

### Early Stopping

```python
early = keras.callbacks.EarlyStopping(patience=10, restore_best_weights=True)
```
- Stop training if test loss doesn't improve for 10 epochs
- Keep best weights found

### Train

```python
model.fit(X_train, y_train, 
          validation_data=(X_test, y_test), 
          epochs=200, 
          callbacks=[early], 
          verbose=1)
```

| Parameter | Meaning |
|-----------|---------|
| `X_train, y_train` | Training data |
| `validation_data` | Test data to monitor |
| `epochs=200` | Maximum 200 training iterations |
| `callbacks=[early]` | Use early stopping |
| `verbose=1` | Print progress |

### Output Example
```
Epoch 1/200
24/24 [==============================] - 0s 2ms/step - loss: 2.1234 - val_loss: 2.0456
Epoch 2/200
24/24 [==============================] - 0s 2ms/step - loss: 1.9876 - val_loss: 1.9234
...
Epoch 45/200
24/24 [==============================] - 0s 2ms/step - loss: 0.0234 - val_loss: 0.0256

Test MSE (normalized target): 0.025634
```

### Evaluate

```python
mse = model.evaluate(X_test, y_test, verbose=0)
print(f"Test MSE (normalized target): {mse:.6f}")
```
Prints final test loss (lower is better)

---

## Part 8: Export Model & Weights

```python
# Save full model (TensorFlow SavedModel)
save_dir = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\SavedModels\LR"
os.makedirs(save_dir, exist_ok=True)

model_path = os.path.join(save_dir, "LR.keras")
model.save(model_path)

# Save final layer weights and bias (useful for small deployments)
final_w, final_b = model.layers[-1].get_weights()
np.savetxt(os.path.join(save_dir, "final_weights.txt"), final_w)
np.savetxt(os.path.join(save_dir, "final_bias.txt"), final_b)

print(f"Done. Saved model to {model_path}, final weights/bias to {save_dir}, and scaler.npz")
```

### What Gets Saved

**Step 1: Create directory**
```python
os.makedirs(save_dir, exist_ok=True)
```
Creates folder if it doesn't exist

**Step 2: Save full model**
```python
model.save(model_path)
```
Saves entire model as `.keras` file

**Step 3: Extract final layer weights**
```python
final_w, final_b = model.layers[-1].get_weights()
```
- `model.layers[-1]` = last layer (output layer)
- `.get_weights()` = get weights and bias

**Step 4: Save weights as text**
```python
np.savetxt(os.path.join(save_dir, "final_weights.txt"), final_w)
np.savetxt(os.path.join(save_dir, "final_bias.txt"), final_b)
```
Saves weights in human-readable format

### Files Created

```
SavedModels/LR/
├── LR.keras              (Full trained model)
├── final_weights.txt     (Output layer weights: 16×1)
├── final_bias.txt        (Output layer bias: 1×1)
└── scaler.npz           (Normalization params: means & stds)
```

---

## Complete Data Flow Diagram

```
1. DOWNLOAD
   ↓
   air_quality.zip (from UCI ML repo)
   
2. EXTRACT
   ↓
   AirQualityUCI.csv
   
3. LOAD & CLEAN
   ↓
   ┌─────────────────┐
   │ temp humidity co│
   │ 20.5  45.2  150│
   │ 21.3  46.1  155│
   │ ...   ...   ...│
   └─────────────────┘
   
4. PREPARE
   ↓
   X = [temp, humidity]
   y = [co]
   Split 80/20
   
5. NORMALIZE
   ↓
   X_normalized = (X - X_mean) / X_std
   y_normalized = (y - y_mean) / y_std
   Save means & stds → scaler.npz
   
6. TRAIN
   ↓
   Model: 2 → 16 → 1
   Train on X_train, y_train
   Validate on X_test, y_test
   Early stopping
   
7. EXPORT
   ↓
   ┌──────────────────────────┐
   │ LR.keras (full model)    │
   │ final_weights.txt        │
   │ final_bias.txt           │
   │ scaler.npz               │
   └──────────────────────────┘
```

---

## How C++ Will Use This

```
C++ Code:
1. Load scaler.npz (get X_mean, X_std, y_mean, y_std)
2. Load LR.keras (get trained model)
3. Read new input [temp, humidity]
4. Normalize: (input - X_mean) / X_std
5. Run through model
6. Get prediction
7. Denormalize: prediction * y_std + y_mean
```

---

## Key Concepts

| Concept | Purpose | Value |
|---------|---------|-------|
| **Reproducibility** | Same results every run | Seeds set to 42 |
| **Train/Test Split** | Evaluate on unseen data | 80/20 |
| **Normalization** | Scale to [-1, 1] range | Saved for C++ |
| **Early Stopping** | Prevent overfitting | Patience = 10 |
| **Model Export** | Use in C++ | .keras format |
| **Weight Extraction** | Manual inference | .txt files |

---

## Output Summary

**Console Output:**
```
Downloading dataset...
[Model training progress...]
Test MSE (normalized target): 0.025634
Done. Saved model to C:\Users\...\LR.keras, final weights/bias to C:\Users\..., and scaler.npz
```

**Files Created:**
```
1. scaler.npz - Normalization parameters
2. LR.keras - Full trained model
3. final_weights.txt - Output layer weights (16x1)
4. final_bias.txt - Output layer bias (1x1)
```

---

## Summary

This script:

1. ✅ Downloads air quality dataset from UCI ML Repository
2. ✅ Cleans and prepares data (temperature, humidity, CO)
3. ✅ Splits into 80% train, 20% test
4. ✅ Normalizes using training statistics (IMPORTANT for C++)
5. ✅ Trains neural network (2 inputs → 16 hidden → 1 output)
6. ✅ Saves trained model and weights
7. ✅ Exports normalization parameters for C++ to use

**The C++ code will load the saved model and use it to make predictions!**

---

## Next Steps (In Lab 4 C++ Part)

```cpp
// C++ will:
1. Load scaler.npz
2. Load LR.keras
3. Read new air quality data
4. Preprocess using saved scaler
5. Run inference
6. Get CO predictions
```

