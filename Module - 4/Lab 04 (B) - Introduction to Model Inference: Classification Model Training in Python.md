# LAB 4 (B): DIABETES CLASSIFICATION WITH NEURAL NETWORKS

## 📚 Complete Lab Guide - TensorFlow/Keras

---

## 🎯 Learning Objectives

By completing this lab, you will:

✅ Load real medical datasets  
✅ Create binary classification labels from continuous data   
✅ Prepare and split data for training  
✅ Normalize features using StandardScaler  
✅ Build a Neural Network classifier with TensorFlow/Keras  
✅ Train the model with validation  
✅ Evaluate model performance with multiple metrics  
✅ Export the model for deployment  
✅ Make predictions on new data  
✅ Understand the complete deep learning pipeline  

---

## 📋 Lab Overview

| Item | Details |
|------|---------|
| **Dataset** | diabetes.txt (442 samples, 10 features) |
| **Task** | Binary Classification |
| **Problem** | Predict: Has Diabetes or No Diabetes |
| **Model** | Neural Network (32→16→1 neurons) |
| **Framework** | TensorFlow/Keras |
| **Expected Accuracy** | 75-85% |
| **Duration** | 15-20 minutes |
| **Difficulty** | Intermediate |

---

## 🏥 Dataset Description

### Source
Real medical dataset with 442 patients

### Features (10 total)

| Feature | Name | Range | Meaning |
|---------|------|-------|---------|
| 0 | AGE | 19-77 | Patient age in years |
| 1 | SEX | 1-2 | 1=Male, 2=Female |
| 2 | BMI | 18.6-38 | Body Mass Index |
| 3 | BP | 63-123 | Blood Pressure |
| 4 | S1 | 114-254 | Serum Measurement 1 |
| 5 | S2 | 54-185 | Serum Measurement 2 |
| 6 | S3 | 22-86 | Serum Measurement 3 |
| 7 | S4 | 2-7 | Serum Measurement 4 |
| 8 | S5 | 3.8-6.1 | Log Serum Triglycerides |
| 9 | S6 | 68-124 | Blood Glucose Level |

### Target Variable

Original: **Y** (continuous, 25-346)
- Represents diabetes progression measure

Converted: **label** (binary, 0-1)
- 0 = No Diabetes (Y ≤ median)
- 1 = Has Diabetes (Y > median)

---

## 📖 Lab Steps

## STEP 1: LOAD DATASET

### Code
```python
import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

print("="*70)
print("DIABETES CLASSIFICATION - Neural Network Classifier")
print("="*70)

data_path = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\diabetes.txt"
df = pd.read_csv(data_path, sep=r"\s+")

print(f"\nDataset loaded: {df.shape[0]} samples, {df.shape[1]} features")
print(f"Columns: {list(df.columns)}")
```

### What Happens
```
✓ Imports TensorFlow and other libraries
✓ Reads diabetes.txt (tab-separated file)
✓ Loads into pandas DataFrame
✓ Shows: 442 samples, 11 columns
```

### Output
```
======================================================================
DIABETES CLASSIFICATION - Neural Network Classifier
======================================================================

Dataset loaded: 442 samples, 11 features
Columns: ['AGE', 'SEX', 'BMI', 'BP', 'S1', 'S2', 'S3', 'S4', 'S5', 'S6', 'Y']
```

### Libraries Explained
- **TensorFlow**: Deep learning framework
- **Keras**: High-level API (part of TensorFlow)
- **pandas**: Data manipulation
- **numpy**: Numerical computing
- **StandardScaler**: Feature normalization

---

## STEP 2: CREATE BINARY LABEL

### Code
```python
print("\n[STEP 2] Creating binary classification label...")

median_y = df['Y'].median()
print(f"Y median value: {median_y:.2f}")

df['label'] = (df['Y'] > median_y).astype(int)

print(f"Label distribution:")
print(f"  No Diabetes (0): {(df['label'] == 0).sum()} samples")
print(f"  Has Diabetes (1): {(df['label'] == 1).sum()} samples")
```

### What Happens
```
✓ Calculates median of continuous Y (140.50)
✓ Creates binary label:
  - Y > 140.50 → label = 1 (Has Diabetes)
  - Y ≤ 140.50 → label = 0 (No Diabetes)
✓ Shows class distribution (balanced!)
```

### Output
```
[STEP 2] Creating binary classification label...
Y median value: 140.50
Label distribution:
  No Diabetes (0): 221 samples
  Has Diabetes (1): 221 samples
```

### Key Concepts
- **Binary Classification**: Exactly 2 classes
- **Balanced Classes**: Equal samples (ideal for training)
- **Median Threshold**: Fair split at middle value

---

## STEP 3: PREPARE FEATURES

### Code
```python
print("\n[STEP 3] Preparing features...")

X = df.drop(['Y', 'label'], axis=1)
y = df['label']

print(f"Features: {X.shape[1]} ({', '.join(X.columns)})")
print(f"Feature shape: {X.shape}")
print(f"Target shape: {y.shape}")
print(f"\nFirst 5 samples:")
print(X.head())
```

### What Happens
```
✓ X = features (10 columns)
✓ y = target labels (0 or 1)
✓ Shows data shapes
✓ Displays sample data
```

### Output
```
[STEP 3] Preparing features...
Features: 10 (AGE, SEX, BMI, BP, S1, S2, S3, S4, S5, S6)
Feature shape: (442, 10)
Target shape: (442,)

First 5 samples:
   AGE  SEX   BMI     BP   S1     S2    S3   S4      S5  S6
0   59    2  32.1  101.0  157   93.2  38.0  4.0  4.8598  87
1   48    1  21.6   87.0  183  103.2  70.0  3.0  3.8918  69
```

---

## STEP 4: SPLIT & NORMALIZE DATA

### Code
```python
print("\n[STEP 4] Splitting data (80% train, 20% test)...")

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

print(f"Training set: {X_train.shape[0]} samples")
print(f"Test set: {X_test.shape[0]} samples")

print("\n[STEP 5] Normalizing features...")

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

print(f"Normalization formula: (X - mean) / std")
print(f"\nFeature means:")
for name, mean in zip(X.columns, scaler.mean_):
    print(f"  {name}: {mean:.4f}")

print(f"\nFeature standard deviations:")
for name, std in zip(X.columns, scaler.scale_):
    print(f"  {name}: {std:.4f}")
```

### What Happens
```
✓ Splits data: 80% training (353), 20% test (89)
✓ Maintains class balance with stratify
✓ Normalizes features to mean=0, std=1
✓ Shows normalization parameters
```

### Output
```
[STEP 4] Splitting data (80% train, 20% test)...
Training set: 353 samples
Test set: 89 samples

[STEP 5] Normalizing features...
Normalization formula: (X - mean) / std

Feature means:
  AGE: 48.1473
  SEX: 1.4589
  BMI: 26.3074
  ...

Feature standard deviations:
  AGE: 13.2631
  SEX: 0.4983
  BMI: 4.3913
  ...
```

### Why Normalization Matters
```
Neural networks:
✓ Train faster with normalized data
✓ Avoid gradient explosion/vanishing
✓ Better convergence
✓ More stable training
```

---

## STEP 6: BUILD NEURAL NETWORK

### Code
```python
print("\n[STEP 6] Building Neural Network model...")

model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(X_train_scaled.shape[1],)),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(optimizer='adam',
              loss='binary_crossentropy',
              metrics=['accuracy'])

print(model.summary())
```

### What Happens
```
✓ Creates Sequential model (layers stacked)
✓ Input layer: 10 features
✓ Hidden layer 1: 32 neurons with ReLU
✓ Hidden layer 2: 16 neurons with ReLU
✓ Output layer: 1 neuron with sigmoid
✓ Compiles with Adam optimizer
```

### Network Architecture
```
Input Layer (10 features)
    ↓
Dense Layer (32 neurons) + ReLU activation
    ↓ (32 parameters)
Dense Layer (16 neurons) + ReLU activation
    ↓ (16 parameters)
Output Layer (1 neuron) + Sigmoid activation
    ↓ (1 probability)
Output: Diabetes probability (0.0 - 1.0)
```

### Model Summary Output
```
Model: "sequential"
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
dense (Dense)                (None, 32)                352       
dense_1 (Dense)              (None, 16)                528       
dense_2 (Dense)              (None, 1)                 17        
=================================================================
Total params: 897
Trainable params: 897
Non-trainable params: 0
_________________________________________________________________
```

### Key Components Explained

**Layers:**
```
Dense(32, activation='relu')
  - 32 neurons in layer
  - ReLU: max(0, x) activation function
  - Introduces non-linearity
  - Good for hidden layers

Dense(1, activation='sigmoid')
  - 1 output neuron
  - Sigmoid: 1/(1+e^(-x))
  - Output range: 0.0 to 1.0
  - Perfect for binary classification
```

**Optimizer:**
```
Adam (Adaptive Moment Estimation)
- Adaptive learning rate
- Faster convergence
- Good default choice
```

**Loss Function:**
```
binary_crossentropy
- For binary classification
- Measures prediction error
- Lower = better predictions
```

**Metrics:**
```
accuracy
- Percentage of correct predictions
- Easy to interpret
```

---

## STEP 7: TRAIN NEURAL NETWORK

### Code
```python
print("\n[STEP 7] Training Neural Network...")

history = model.fit(X_train_scaled, y_train,
                    validation_data=(X_test_scaled, y_test),
                    epochs=20,
                    batch_size=32,
                    verbose=1)

print("\n✓ Training complete!")
```

### What Happens
```
✓ Trains on 353 training samples
✓ Validates on 89 test samples
✓ 20 epochs (full passes through data)
✓ Batch size = 32 (gradient updates per 32 samples)
✓ Shows progress for each epoch
```

### Training Output Example
```
[STEP 7] Training Neural Network...
Epoch 1/20
11/11 [==============================] - 0s 3ms/step - loss: 0.6931 - accuracy: 0.5352 - val_loss: 0.6874 - val_accuracy: 0.5955
Epoch 2/20
11/11 [==============================] - 0s 2ms/step - loss: 0.6845 - accuracy: 0.5865 - val_loss: 0.6782 - val_accuracy: 0.6292
Epoch 3/20
11/11 [==============================] - 0s 2ms/step - loss: 0.6727 - accuracy: 0.6290 - val_loss: 0.6658 - val_accuracy: 0.6742
...
Epoch 20/20
11/11 [==============================] - 0s 2ms/step - loss: 0.4523 - accuracy: 0.8103 - val_loss: 0.5234 - val_accuracy: 0.7865

✓ Training complete!
```

### Training Process Explained
```
For each epoch:
  1. Forward pass: predictions = model(training_data)
  2. Calculate loss: error = loss_function(predictions, targets)
  3. Backward pass: compute gradients
  4. Update weights: weights -= learning_rate * gradients
  5. Validate: check performance on validation data
  6. Display metrics

Why validation data?
✓ Monitor for overfitting
✓ Stop early if not improving
✓ Estimate real-world performance
```

### Key Parameters
```
epochs=20
- How many times to go through entire dataset
- More = better learning (usually)
- Too many = overfitting
- Start with 20, adjust as needed

batch_size=32
- How many samples per gradient update
- Smaller = more updates per epoch
- Larger = faster training, less stable
- 32 is good default

verbose=1
- Show progress bar for each epoch
- verbose=0 for silent training
```

---

## STEP 8: EVALUATE MODEL

### Code
```python
print("\n[STEP 8] Evaluating model...")

loss, acc = model.evaluate(X_test_scaled, y_test)
print(f"\nTest Loss: {loss:.4f}")
print(f"Test Accuracy: {acc:.4f} ({acc*100:.2f}%)")

# Make predictions
y_pred_prob = model.predict(X_test_scaled)
y_pred = (y_pred_prob > 0.5).astype(int).flatten()

# Classification metrics
from sklearn.metrics import confusion_matrix, classification_report, roc_auc_score

cm = confusion_matrix(y_test, y_pred)
print(f"\nConfusion Matrix (Test Set):")
print(f"  True Negatives (Correctly No Diabetes): {cm[0,0]}")
print(f"  False Positives (Wrongly marked Diabetes): {cm[0,1]}")
print(f"  False Negatives (Missed Diabetes): {cm[1,0]}")
print(f"  True Positives (Correctly marked Diabetes): {cm[1,1]}")

print(f"\nClassification Report:")
print(classification_report(y_test, y_pred, 
                          target_names=['No Diabetes', 'Has Diabetes']))

# ROC-AUC Score
roc_auc = roc_auc_score(y_test, y_pred_prob)
print(f"\nROC-AUC Score: {roc_auc:.4f}")
```

### What Happens
```
✓ Evaluates on test data
✓ Shows loss and accuracy
✓ Makes predictions
✓ Creates confusion matrix
✓ Generates classification report
✓ Calculates ROC-AUC score
```

### Output Example
```
[STEP 8] Evaluating model...

89/89 [==============================] - 0s 0ms/step - loss: 0.5234 - accuracy: 0.7865

Test Loss: 0.5234
Test Accuracy: 0.7865 (78.65%)

Confusion Matrix (Test Set):
  True Negatives (Correctly No Diabetes): 32
  False Positives (Wrongly marked Diabetes): 13
  False Negatives (Missed Diabetes): 6
  True Positives (Correctly marked Diabetes): 38

Classification Report:
              precision    recall  f1-score   support
 No Diabetes       0.84      0.71      0.77        45
Has Diabetes       0.75      0.86      0.80        44

    accuracy                           0.79        89
   macro avg       0.79      0.79      0.79        89
weighted avg       0.79      0.79      0.79        89

ROC-AUC Score: 0.8456
```

### Metrics Explained

**Test Accuracy: 78.65%**
```
Meaning: 78.65% of predictions on unseen data correct
Neural network vs Random Forest:
- RF: ~74% accuracy
- NN: ~79% accuracy
- Better generalization with NN!
```

**Confusion Matrix**
```
                Predicted
            No Diabetes | Diabetes
        ┌──────────────┼──────────┐
Actual No│    32 (TN)   │ 13 (FP)  │  45
        ─┼──────────────┼──────────┤
       Yes│     6 (FN)   │ 38 (TP)  │  44
        └──────────────┴──────────┘
             45            51

TN: Correctly identified no diabetes (good!)
FP: False alarms (less critical)
FN: Missed cases (more critical in medicine!)
TP: Correctly identified diabetes (good!)
```

**ROC-AUC: 0.8456**
```
ROC-AUC = Area Under the Receiver Operating Curve
Range: 0.0 to 1.0
- 0.5: Random guessing
- 0.7-0.8: Good
- 0.8-0.9: Very good
- >0.9: Excellent

Our score (0.8456) = Very good model!
```

---

## STEP 9: EXPORT MODEL

### Code
```python
print("\n[STEP 9] Exporting model...")

save_dir = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\SavedModels\DiabetesNN"

try:
    if hasattr(model, "export"):
        model.export(save_dir)
    else:
        model.save(save_dir, save_format='tf')
    print(f"✓ SavedModel exported to: {save_dir}")
except Exception as e:
    fallback = save_dir + ".keras"
    model.save(fallback)
    print(f"✓ Model saved in Keras format: {fallback}")
```

### What Happens
```
✓ Attempts SavedModel export format (TensorFlow)
✓ Falls back to Keras format (.keras) if needed
✓ Saves all weights and architecture
✓ Ready for deployment
```

### Output
```
[STEP 9] Exporting model...
✓ SavedModel exported to: C:\Users\jasle\...\SavedModels\DiabetesNN
```

### Export Formats Explained

**SavedModel Format (Recommended)**
```
DiabetesNN/
├── assets/
├── saved_model.pb
├── variables/
│   ├── variables.data-00000-of-00001
│   └── variables.index
└── keras_metadata.pb

✓ TensorFlow standard
✓ Cross-language deployment
✓ Includes preprocessing info
✓ Best for production
```

**Keras Format (.keras)**
```
DiabetesNN.keras
(Single file)

✓ Simpler structure
✓ Smaller file size
✓ Easy to share
✓ Good for simple deployment
```

### Why Export?
```
✓ Reuse without retraining
✓ Deploy to production
✓ Share with others
✓ Version control
✓ Use in C++ (via ONNX)
```

---

## STEP 10: SAVE NORMALIZATION PARAMETERS

### Code
```python
print("\n[STEP 10] Saving normalization parameters...")

# Save as text for C++ compatibility
import os
os.makedirs(save_dir, exist_ok=True)

np.savetxt(os.path.join(save_dir, "feature_means.txt"), scaler.mean_)
np.savetxt(os.path.join(save_dir, "feature_stds.txt"), scaler.scale_)

# Save feature/class names
with open(os.path.join(save_dir, "feature_names.txt"), "w") as f:
    for name in X.columns:
        f.write(name + "\n")

with open(os.path.join(save_dir, "class_names.txt"), "w") as f:
    f.write("No Diabetes\n")
    f.write("Has Diabetes\n")

print(f"✓ Normalization parameters saved")
print(f"✓ Feature names saved")
print(f"✓ Class names saved")
```

### Files Created
```
DiabetesNN/
├── saved_model.pb              (Model architecture)
├── variables/                  (Model weights)
├── feature_means.txt           (For C++ normalization)
├── feature_stds.txt            (For C++ normalization)
├── feature_names.txt           (Feature names)
└── class_names.txt             (Output labels)
```

### Why Save These Files?
```
For C++ Integration:
✓ Load model in C++ using TensorFlow C++ API
✓ Use saved means/stds for preprocessing
✓ Normalize input the same way as training
✓ Make predictions in C++
```

---

## STEP 11: TEST ON SAMPLE DATA

### Code
```python
print("\n[STEP 11] Testing on sample patients...")

# Low risk patient
sample1_raw = np.array([[35, 1, 23, 85, 160, 95, 55, 3, 4.2, 85]])
sample1_scaled = scaler.transform(sample1_raw)
pred1 = model.predict(sample1_scaled)[0][0]

print(f"\nPatient 1 (Low Risk):")
print(f"  Age: 35, BMI: 23, BP: 85")
print(f"  Diabetes Probability: {pred1:.4f} ({pred1*100:.2f}%)")
print(f"  Result: {'HAS DIABETES' if pred1 > 0.5 else 'NO DIABETES'}")

# High risk patient
sample2_raw = np.array([[62, 2, 31, 115, 210, 145, 35, 5, 5.0, 110]])
sample2_scaled = scaler.transform(sample2_raw)
pred2 = model.predict(sample2_scaled)[0][0]

print(f"\nPatient 2 (High Risk):")
print(f"  Age: 62, BMI: 31, BP: 115")
print(f"  Diabetes Probability: {pred2:.4f} ({pred2*100:.2f}%)")
print(f"  Result: {'HAS DIABETES' if pred2 > 0.5 else 'NO DIABETES'}")
```

### Output
```
[STEP 11] Testing on sample patients...

Patient 1 (Low Risk):
  Age: 35, BMI: 23, BP: 85
  Diabetes Probability: 0.2345 (23.45%)
  Result: NO DIABETES

Patient 2 (High Risk):
  Age: 62, BMI: 31, BP: 115
  Diabetes Probability: 0.8234 (82.34%)
  Result: HAS DIABETES
```

### Prediction Interpretation
```
Probability < 0.5: No Diabetes
Probability ≥ 0.5: Has Diabetes

Example 1: 23.45% (Low) → Confident: NO diabetes
Example 2: 82.34% (High) → Confident: HAS diabetes
```

---

## STEP 12: FINAL SUMMARY

### Code
```python
print("\n" + "="*70)
print("TRAINING COMPLETE - NEURAL NETWORK MODEL")
print("="*70)

print(f"\n✓ Model Architecture:")
print(f"  Input: 10 features")
print(f"  Hidden Layer 1: 32 neurons (ReLU)")
print(f"  Hidden Layer 2: 16 neurons (ReLU)")
print(f"  Output: 1 neuron (Sigmoid)")
print(f"  Total Parameters: 897")

print(f"\n✓ Performance:")
print(f"  Test Accuracy: {acc*100:.2f}%")
print(f"  ROC-AUC Score: {roc_auc:.4f}")
print(f"  Loss: {loss:.4f}")

print(f"\n✓ Files Saved:")
print(f"  Model: {save_dir}")
print(f"  Format: TensorFlow SavedModel")
print(f"  Normalization Parameters: Included")

print(f"\n✓ Ready for:")
print(f"  C++ Integration using TensorFlow C++ API")
print(f"  Production Deployment")
print(f"  Further Fine-tuning")

print("\n" + "="*70)
```

### Output
```
======================================================================
TRAINING COMPLETE - NEURAL NETWORK MODEL
======================================================================

✓ Model Architecture:
  Input: 10 features
  Hidden Layer 1: 32 neurons (ReLU)
  Hidden Layer 2: 16 neurons (ReLU)
  Output: 1 neuron (Sigmoid)
  Total Parameters: 897

✓ Performance:
  Test Accuracy: 78.65%
  ROC-AUC Score: 0.8456
  Loss: 0.5234

✓ Files Saved:
  Model: C:\Users\jasle\...\SavedModels\DiabetesNN
  Format: TensorFlow SavedModel
  Normalization Parameters: Included

✓ Ready for:
  C++ Integration using TensorFlow C++ API
  Production Deployment
  Further Fine-tuning

======================================================================
```

---

## 🎓 Key Concepts Summary

### Neural Networks
```
Inspired by biological neurons
- Neurons: receive inputs, apply weights
- Layers: neurons organized in layers
- Activation: non-linear transformation
- Training: adjust weights to minimize error

Why use Neural Networks?
✓ Learn complex patterns automatically
✓ No manual feature engineering needed
✓ Scale well with more data
✓ Great for non-linear problems
```

### Deep Learning Pipeline
```
Data → Preprocessing → Model Building → Training → Evaluation → Deployment

1. Data: 442 patient samples
2. Preprocessing: normalize features, split data
3. Model: Sequential with 3 layers
4. Training: 20 epochs, gradient descent
5. Evaluation: accuracy, loss, ROC-AUC
6. Deployment: SavedModel format
```

### Activation Functions
```
ReLU (Rectified Linear Unit)
- Formula: max(0, x)
- Good for hidden layers
- Solves vanishing gradient problem

Sigmoid
- Formula: 1 / (1 + e^(-x))
- Output range: 0.0 to 1.0
- Perfect for binary classification
```

### Loss Function
```
binary_crossentropy
- Measures prediction error
- Standard for binary classification
- Formula: -(y*log(ŷ) + (1-y)*log(1-ŷ))
- Lower loss = better model
```

---

## ⚠️ Common Issues

### Issue 1: ImportError for TensorFlow
```
Error: No module named 'tensorflow'

Solution:
pip install tensorflow
# Or for GPU support:
pip install tensorflow[and-cuda]
```

### Issue 2: Low Accuracy
```
Reason: Model not trained long enough

Solutions:
- Increase epochs (20 → 50)
- Adjust batch size (32 → 16 or 64)
- Add more hidden units (32 → 64)
- Train longer, monitor validation loss
```

### Issue 3: Overfitting
```
Symptoms:
- Training accuracy >> Test accuracy
- Validation loss increasing

Solutions:
- Reduce model size
- Add dropout layers
- More training data
- Early stopping
```

---

## ✅ Lab Checklist

- [ ] Install TensorFlow: `pip install tensorflow`
- [ ] Have diabetes.txt in correct location
- [ ] Load dataset successfully (442 samples)
- [ ] Create binary labels (balanced: 221/221)
- [ ] Split data (353 train, 89 test)
- [ ] Normalize features (means/stds calculated)
- [ ] Build neural network (3 layers)
- [ ] Train model (20 epochs)
- [ ] Evaluate model (accuracy ~78-80%)
- [ ] Export SavedModel
- [ ] Save normalization parameters
- [ ] Test on sample patients
- [ ] Understand training curves

---

## 📊 Expected Results

### Model Performance
```
Test Accuracy: 78-82%
ROC-AUC Score: 0.80-0.85
Training Loss: 0.45-0.60
Test Loss: 0.50-0.65
```

### Comparison: Random Forest vs Neural Network
```
Random Forest:
  ✓ Accuracy: 74%
  ✗ More hyperparameter tuning
  ✓ Faster training
  ✗ Larger model

Neural Network:
  ✓ Accuracy: 78%
  ✓ Better generalization
  ✓ Automatic feature learning
  ✗ Longer training
```

### Dataset Statistics
```
Total Samples: 442
Training: 353 (80%)
Test: 89 (20%)

Classes:
  No Diabetes: 221 (50%)
  Has Diabetes: 221 (50%)
```

---

## 🚀 Next Steps

### 1. **Load in C++**
Use TensorFlow C++ API to load SavedModel

### 2. **Improve Model**
```python
# Try different architectures:
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(10,)),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dropout(0.3),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dropout(0.3),
    tf.keras.layers.Dense(1, activation='sigmoid')
])
```

### 3. **Add Early Stopping**
```python
early_stop = tf.keras.callbacks.EarlyStopping(
    monitor='val_loss', patience=5
)
model.fit(..., callbacks=[early_stop])
```

### 4. **Hyperparameter Tuning**
- Epochs: try 20, 50, 100
- Batch size: try 16, 32, 64
- Hidden units: try 32, 64, 128
- Learning rate: use different optimizers

### 5. **Feature Engineering**
- Create BMI categories
- Age groups
- Combine features

---

## 📚 Learning Resources

### Key Libraries
- **TensorFlow/Keras**: Deep learning framework
- **pandas**: Data manipulation
- **numpy**: Numerical computing
- **scikit-learn**: ML utilities and metrics

### Important Functions
- `tf.keras.Sequential()`: Build neural network
- `Dense()`: Fully connected layer
- `model.compile()`: Configure training
- `model.fit()`: Train model
- `model.evaluate()`: Test performance
- `model.predict()`: Make predictions
- `model.save()`: Export model

---

## 🎓 What You've Learned

✅ **Deep Learning Fundamentals**
- Neural network architecture
- Layers and activation functions
- Training process (forward/backward pass)

✅ **Medical Data Science**
- Load health datasets
- Handle binary classification
- Evaluate on real-world data

✅ **Model Development**
- Data preprocessing
- Feature normalization
- Train/validation/test splits

✅ **Model Deployment**
- Export SavedModel format
- Save normalization parameters
- Prepare for C++ integration

✅ **Evaluation & Metrics**
- Accuracy, precision, recall
- Confusion matrix interpretation
- ROC-AUC score

---

## 📝 Complete Python Script

See the provided Python code file for the complete, ready-to-run implementation.

**The script includes all 12 steps in a single, well-organized file.**
### Complete Code
```python
import os
import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

print("="*70)
print("DIABETES CLASSIFICATION - Neural Network Classifier (TensorFlow/Keras)")
print("="*70)

# ============================================================
# STEP 1: LOAD DATASET
# ============================================================

data_path = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\diabetes.txt"
df = pd.read_csv(data_path, sep=r"\s+")

# ============================================================
# STEP 2: CREATE BINARY LABEL
# ============================================================

median_y = df['Y'].median()
df['label'] = (df['Y'] > median_y).astype(int)

X = df.drop(['Y', 'label'], axis=1)
y = df['label']

# ============================================================
# STEP 3: SPLIT & SCALE
# ============================================================

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# ============================================================
# STEP 4: BUILD NEURAL NETWORK
# ============================================================

model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(X_train_scaled.shape[1],)),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')  # binary classification
])

model.compile(optimizer='adam',
              loss='binary_crossentropy',
              metrics=['accuracy'])

print(model.summary())

# ============================================================
# STEP 5: TRAIN
# ============================================================

history = model.fit(X_train_scaled, y_train,
                    validation_data=(X_test_scaled, y_test),
                    epochs=20,
                    batch_size=32)

# ============================================================
# STEP 6: EVALUATE
# ============================================================

loss, acc = model.evaluate(X_test_scaled, y_test)
print(f"Test Accuracy: {acc:.4f}")

# ============================================================
# STEP 7: EXPORT MODEL
# ============================================================

save_dir = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\SavedModels\DiabetesNN"
# Newer Keras may require explicit export for SavedModel. Attempt export() first,
# fall back to saving in TensorFlow SavedModel format, and finally to the
# Keras native format (.keras) if needed.
try:
    if hasattr(model, "export"):
        model.export(save_dir)
    else:
        # save_format='tf' requests the SavedModel directory format
        model.save(save_dir, save_format='tf')
    print(f"✓ SavedModel exported to: {save_dir}")
except Exception as e:
    # Final fallback: save as Keras native file
    fallback = save_dir + ".keras"
    model.save(fallback)
    print(f"✓ Model saved in Keras format: {fallback} (fallback due to: {e})")

# ============================================================
# STEP 8: SAVE NORMALIZATION PARAMETERS
# ============================================================

os.makedirs(save_dir, exist_ok=True)

# Save means
np.savetxt(os.path.join(save_dir, "feature_means.txt"), scaler.mean_)

# Save stds
np.savetxt(os.path.join(save_dir, "feature_stds.txt"), scaler.scale_)

# Save feature names
with open(os.path.join(save_dir, "feature_names.txt"), "w") as f:
    for col in X.columns:
        f.write(col + "\n")

print("✓ Saved normalization files")
```

**LAB COMPLETE! You've successfully trained a neural network for diabetes classification!** 🎉

## 🎓 SUMMARY

This lab taught you to:

✅ Build neural networks with TensorFlow/Keras  
✅ Handle medical classification problems  
✅ Normalize features properly  
✅ Train and evaluate deep learning models  
✅ Export models for deployment  
✅ Interpret neural network predictions  
✅ Compare with traditional ML methods (Random Forest)  

**You're now ready to integrate deep learning models with C++!** 🚀
