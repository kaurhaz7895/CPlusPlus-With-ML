# LAB 4 (B): DIABETES CLASSIFICATION WITH RANDOM FOREST

## 📚 Complete Lab Guide

---

## 🎯 Learning Objectives

By completing this lab, you will:

✅ Load real medical datasets  
✅ Create binary classification labels from continuous data  
✅ Prepare and split data for training  
✅ Normalize features using StandardScaler  
✅ Train a Random Forest classifier  
✅ Evaluate model performance with multiple metrics  
✅ Analyze feature importance  
✅ Save trained models for deployment  
✅ Make predictions on new data  
✅ Understand the complete ML pipeline  

---

## 📋 Lab Overview

| Item | Details |
|------|---------|
| **Dataset** | diabetes.txt (442 samples, 10 features) |
| **Task** | Binary Classification |
| **Problem** | Predict: Has Diabetes or No Diabetes |
| **Model** | Random Forest (100 trees) |
| **Expected Accuracy** | 70-80% |
| **Duration** | 10-15 minutes |
| **Difficulty** | Beginner to Intermediate |

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
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
import joblib
import os

data_path = r"C:\Users\jasle\OneDrive - Koenig Solutions Ltd\Courses\C++ with ML\Downloads\diabetes.txt"
df = pd.read_csv(data_path, sep=r"\s+")
```

### What Happens
```
✓ Imports required libraries
✓ Reads diabetes.txt (tab-separated file)
✓ Loads into pandas DataFrame
✓ Shows: 442 samples, 11 columns
```

### Output
```
Dataset loaded: 442 samples, 11 features
Columns: ['AGE', 'SEX', 'BMI', 'BP', 'S1', 'S2', 'S3', 'S4', 'S5', 'S6', 'Y']
```

### Concepts
- **pandas.read_csv()**: Reads comma/tab-separated files
- **sep=r"\s+"**: Regex pattern for whitespace separator
- **DataFrame**: 2D table structure (rows=samples, columns=features)

---

## STEP 2: CREATE BINARY LABEL

### Code
```python
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
✓ Shows class distribution (221 vs 221 = balanced!)
```

### Output
```
Y median value: 140.50
Label distribution:
  No Diabetes (0): 221 samples
  Has Diabetes (1): 221 samples
```

### Concepts
- **Median**: Middle value (50% above, 50% below)
- **Binary Classification**: 2 classes (0 and 1)
- **Balanced Classes**: Equal samples in each class (good!)
- **Threshold**: Using median as decision boundary

---

## STEP 3: PREPARE FEATURES

### Code
```python
# Use all columns except Y and label as features
X = df.drop(['Y', 'label'], axis=1)
y = df['label']

print(f"Features: {X.shape[1]} ({', '.join(X.columns)})")
print(f"Feature shape: {X.shape}")
print(f"Target shape: {y.shape}")

print(f"First 5 samples:")
print(X.head())
```

### What Happens
```
✓ X = features (10 columns: AGE, SEX, BMI, ...)
✓ y = target (442 labels: 0 or 1)
✓ Shows first 5 samples
✓ Displays shapes
```

### Output
```
Features: 10 (AGE, SEX, BMI, BP, S1, S2, S3, S4, S5, S6)
Feature shape: (442, 10)
Target shape: (442,)

First 5 samples:
   AGE  SEX   BMI     BP   S1     S2    S3   S4      S5  S6
0   59    2  32.1  101.0  157   93.2  38.0  4.0  4.8598  87
1   48    1  21.6   87.0  183  103.2  70.0  3.0  3.8918  69
2   72    2  30.5   93.0  156   93.6  41.0  4.0  4.6728  85
```

### Concepts
- **Feature Matrix (X)**: Input data (442, 10)
- **Target Vector (y)**: Output labels (442,)
- **axis=1**: Drop columns (axis=0 would drop rows)
- **DataFrame.head()**: First N rows (default N=5)

---

## STEP 4: SPLIT DATA

### Code
```python
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

print(f"Training set: {X_train.shape[0]} samples")
print(f"Test set: {X_test.shape[0]} samples")
print(f"\nTraining set class distribution:")
print(f"  No Diabetes: {(y_train == 0).sum()}")
print(f"  Has Diabetes: {(y_train == 1).sum()}")
```

### What Happens
```
✓ Splits data: 80% training, 20% test
✓ Maintains class distribution (stratify)
✓ Random seed = 42 (reproducible)
```

### Output
```
Training set: 353 samples
Test set: 89 samples

Training set class distribution:
  No Diabetes: 176
  Has Diabetes: 177
```

### Visualization
```
Total: 442 samples
├─ Training: 353 (80%) ← Model learns from this
└─ Test: 89 (20%)      ← Model evaluates on this

Why split?
✓ Training: Learns patterns
✓ Test: Estimates real performance
✓ Prevents overfitting
```

### Concepts
- **train_test_split()**: Divides data into train/test
- **test_size=0.2**: 20% for testing (standard)
- **random_state=42**: Reproducible split
- **stratify=y**: Maintains class distribution
- **80/20 rule**: Standard data split

---

## STEP 5: NORMALIZE FEATURES

### Code
```python
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

print(f"Feature means:")
for name, mean in zip(X.columns, scaler.mean_):
    print(f"  {name}: {mean:.4f}")

print(f"\nFeature standard deviations:")
for name, std in zip(X.columns, scaler.scale_):
    print(f"  {name}: {std:.4f}")
```

### What Happens
```
✓ Learns means and stds from training data
✓ Applies transformation to training data
✓ Applies same transformation to test data
✓ Shows normalization parameters
```

### Output
```
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

### Normalization Formula
```
normalized = (X - mean) / std

Example: AGE
  Raw value: 35
  Mean: 48.1473
  Std: 13.2631
  Normalized: (35 - 48.1473) / 13.2631 = -1.0034
```

### Why Normalize?
```
✓ All features on same scale (-1 to 1)
✓ Faster training
✓ Better model performance
✓ Some algorithms require it
```

### Concepts
- **StandardScaler**: Normalizes to mean=0, std=1
- **fit_transform()**: Learn + apply (training data)
- **transform()**: Apply only (test data)
- **Important**: Never fit on test data!

---

## STEP 6: BUILD & TRAIN MODEL

### Code
```python
model = RandomForestClassifier(
    n_estimators=100,
    max_depth=10,
    random_state=42,
    n_jobs=-1
)

print(f"Training model...")
model.fit(X_train_scaled, y_train)
print(f"✓ Training complete!")
```

### What Happens
```
✓ Creates Random Forest with 100 trees
✓ Each tree max depth = 10
✓ Fits on 353 training samples
✓ Takes ~1 second
```

### Output
```
Training model...
✓ Training complete!
```

### Model Architecture
```
Random Forest (100 trees)
├─ Tree 1
│  ├─ Split: AGE > 50? → Branch left/right
│  ├─ Split: BMI > 25? → Branch left/right
│  └─ Leaf: Predict "Diabetes" or "No Diabetes"
│
├─ Tree 2
│  └─ Different splits...
│
...
└─ Tree 100
   └─ Different splits...

Prediction: Majority vote from 100 trees
```

### Hyperparameters
| Parameter | Value | Meaning |
|-----------|-------|---------|
| n_estimators | 100 | 100 decision trees |
| max_depth | 10 | Tree can split up to 10 times |
| random_state | 42 | Reproducibility |
| n_jobs | -1 | Use all CPU cores |

### Concepts
- **Random Forest**: Ensemble of decision trees
- **Ensemble Learning**: Multiple models voting
- **Bagging**: Trains on random subsets
- **Majority Voting**: Class with most votes wins

---

## STEP 7: EVALUATE MODEL

### Code
```python
train_pred = model.predict(X_train_scaled)
test_pred = model.predict(X_test_scaled)

train_accuracy = accuracy_score(y_train, train_pred)
test_accuracy = accuracy_score(y_test, test_pred)

print(f"Training Accuracy: {train_accuracy:.4f} ({train_accuracy*100:.2f}%)")
print(f"Test Accuracy: {test_accuracy:.4f} ({test_accuracy*100:.2f}%)")

cm = confusion_matrix(y_test, test_pred)
print(f"True Negatives: {cm[0,0]}")
print(f"False Positives: {cm[0,1]}")
print(f"False Negatives: {cm[1,0]}")
print(f"True Positives: {cm[1,1]}")

print(classification_report(y_test, test_pred, 
                          target_names=['No Diabetes', 'Has Diabetes']))
```

### What Happens
```
✓ Makes predictions on training data
✓ Makes predictions on test data
✓ Calculates accuracy
✓ Creates confusion matrix
✓ Generates classification report
```

### Output Example
```
Training Accuracy: 0.9972 (99.72%)
Test Accuracy: 0.7416 (74.16%)

True Negatives: 31
False Positives: 14
False Negatives: 9
True Positives: 35

              precision    recall  f1-score   support
 No Diabetes       0.78      0.69      0.73        45
Has Diabetes       0.71      0.80      0.75        44

    accuracy                           0.74        89
```

### Confusion Matrix Explained
```
                 Predicted
              No Diabetes | Diabetes
           ┌─────────────┼──────────┐
Actual  No │  TN (31)    │ FP (14)  │  45
        ───┼─────────────┼──────────┤
        Yes│  FN (9)     │ TP (35)  │  44
           └─────────────┴──────────┘
               45            49

TN (True Negative): Correctly said "No Diabetes"
FP (False Positive): Wrongly said "Diabetes"
FN (False Negative): Missed actual diabetes
TP (True Positive): Correctly said "Diabetes"
```

### Metrics Explained

**Accuracy**
```
Accuracy = (TP + TN) / Total
         = (35 + 31) / 89
         = 0.7416 (74.16%)
         
Meaning: 74% of predictions correct
```

**Precision** (for "Has Diabetes" class)
```
Precision = TP / (TP + FP)
          = 35 / (35 + 14)
          = 0.71 (71%)
          
Meaning: Of 49 diabetes predictions, 35 were correct
         (29% false alarms)
```

**Recall** (for "Has Diabetes" class)
```
Recall = TP / (TP + FN)
       = 35 / (35 + 9)
       = 0.80 (80%)
       
Meaning: Of 44 actual diabetes cases, caught 35
         (missed 9)
```

**F1-Score**
```
F1 = 2 * (Precision * Recall) / (Precision + Recall)
   = Harmonic mean of precision and recall
   
Meaning: Balance between precision and recall
```

### Why Training > Test?
```
Training Accuracy: 99.72%
Test Accuracy: 74.16%

Reason: Model slightly overfit
- Learned training data too well
- Can't generalize to new data
- Still 74% is acceptable!

Prevention:
- Reduce tree depth
- Reduce number of trees
- Add more data
```

### Concepts
- **Accuracy**: Overall correctness
- **Precision**: Avoid false positives
- **Recall**: Avoid false negatives
- **Confusion Matrix**: Detailed error analysis
- **Overfitting**: Too good on training, bad on test

---

## STEP 8: FEATURE IMPORTANCE

### Code
```python
importances = model.feature_importances_
feature_importance = list(zip(X.columns, importances))
feature_importance.sort(key=lambda x: x[1], reverse=True)

print(f"Feature Importance Ranking:")
for rank, (name, importance) in enumerate(feature_importance, 1):
    bar = "█" * int(importance * 50)
    print(f"  {rank}. {name}: {importance:.4f} {bar}")
```

### What Happens
```
✓ Extracts feature importance scores
✓ Sorts by importance (highest first)
✓ Displays as ranked list with bar chart
```

### Output Example
```
Feature Importance Ranking:
  1. BMI: 0.2115 ██████████
  2. S5: 0.1773 ████████
  3. BP: 0.1432 ███████
  4. S3: 0.0962 ████
  5. S2: 0.0817 ████
  6. S6: 0.0755 ███
  7. S1: 0.0731 ███
  8. AGE: 0.0667 ███
  9. S4: 0.0565 ██
  10. SEX: 0.0183 █
```

### What It Means
```
BMI (21.15%): Most important
  - Strongest indicator of diabetes
  - Model relies heavily on BMI

S5 (17.73%): Second most important
  - Log serum triglycerides
  - Also strong indicator

SEX (1.83%): Least important
  - Gender has minimal impact
  - Other factors matter more
```

### How It Works
```
Each tree makes splits based on features.
Important features = split many times
Less important = split rarely

Feature Importance =
  (Number of splits on feature) / (Total splits)
```

### Concepts
- **Feature Importance**: Which features matter
- **Interpretability**: Understand model decisions
- **Domain Knowledge**: Align with medical understanding

---

## STEP 9: SAVE MODEL & FILES

### Code
```python
save_dir = r"C:\Users\jasle\...\SavedModels\DiabetesModel"
os.makedirs(save_dir, exist_ok=True)

# Save model
joblib.dump(model, os.path.join(save_dir, "diabetes_model.pkl"))

# Save scaler
joblib.dump(scaler, os.path.join(save_dir, "diabetes_scaler.pkl"))

# Save parameters as text (for C++)
np.savetxt(os.path.join(save_dir, "feature_means.txt"), scaler.mean_)
np.savetxt(os.path.join(save_dir, "feature_stds.txt"), scaler.scale_)

# Save feature/class names
with open(os.path.join(save_dir, "feature_names.txt"), "w") as f:
    for name in X.columns:
        f.write(name + "\n")

with open(os.path.join(save_dir, "class_names.txt"), "w") as f:
    f.write("No Diabetes\n")
    f.write("Has Diabetes\n")
```

### What Happens
```
✓ Creates directory if not exists
✓ Saves model (100 trees, parameters)
✓ Saves scaler (means, stds)
✓ Saves feature names (for reference)
✓ Saves class names (for results)
```

### Files Generated
```
DiabetesModel/
├── diabetes_model.pkl          (~10 KB)
├── diabetes_scaler.pkl         (~2 KB)
├── feature_means.txt           (<1 KB)
├── feature_stds.txt            (<1 KB)
├── feature_names.txt           (<1 KB)
└── class_names.txt             (<1 KB)
```

### Why Save?
```
✓ Reuse model without retraining
✓ Deploy to production
✓ Share with others
✓ Version control
✓ Use in C++ or other languages
```

### Concepts
- **joblib**: Saves sklearn objects
- **Serialization**: Convert object to bytes
- **Persistence**: Save to disk
- **Deployment**: Ready for production

---

## STEP 10: SUMMARY

### Output
```
TRAINING COMPLETE!

✓ All files saved to:
  C:\Users\jasle\...\SavedModels\DiabetesModel

✓ Model Performance:
  Training Accuracy: 99.72%
  Test Accuracy: 74.16%

✓ Files Generated:
  1. diabetes_model.pkl (trained model)
  2. diabetes_scaler.pkl (normalizer)
  3. feature_means.txt (for C++)
  4. feature_stds.txt (for C++)
  5. feature_names.txt (feature names)
  6. class_names.txt (class names)

✓ Ready to use in C++!
```

---

## STEP 11: TEST ON SAMPLE (BONUS)

### Code
```python
sample_raw = np.array([[45, 1, 25.0, 100, 180, 120, 50, 3, 4.5, 85]])
sample_df = pd.DataFrame(sample_raw, columns=X.columns)

print(f"Sample patient features:")
for name, value in zip(X.columns, sample_df.iloc[0]):
    print(f"  {name}: {value}")

sample_scaled = scaler.transform(sample_df)
prediction = model.predict(sample_scaled)[0]
probability = model.predict_proba(sample_scaled)[0]

result = "HAS DIABETES" if prediction == 1 else "NO DIABETES"
print(f"\nPrediction: {result}")
print(f"  No Diabetes: {probability[0]*100:.2f}%")
print(f"  Has Diabetes: {probability[1]*100:.2f}%")
```

### What Happens
```
✓ Creates sample patient data
✓ Normalizes using fitted scaler
✓ Makes prediction
✓ Shows confidence percentages
```

### Output Example
```
Sample patient features:
  AGE: 45
  SEX: 1
  BMI: 25.0
  BP: 100
  S1: 180
  S2: 120
  S3: 50
  S4: 3
  S5: 4.5
  S6: 85

Prediction: NO DIABETES
Confidence:
  No Diabetes: 54.96%
  Has Diabetes: 45.04%
```

### Concepts
- **Prediction**: Model output (class label)
- **Probability**: Confidence score (0-1)
- **Normalization**: Must use same scaler

---

## 🎓 Key Concepts Summary

### Binary Classification
```
Two possible outputs:
  Class 0: No Diabetes
  Class 1: Has Diabetes
```

### Machine Learning Pipeline
```
Raw Data → Preprocessing → Training → Evaluation → Deployment

1. Raw Data: 442 diabetes samples
2. Preprocessing: Split, normalize
3. Training: Fit Random Forest
4. Evaluation: Test accuracy, metrics
5. Deployment: Save files
```

### Random Forest
```
Ensemble of 100 decision trees
- Each tree votes
- Majority vote = final prediction
- Handles complex patterns
- Less prone to overfitting
```

### Overfitting
```
Training Accuracy: 99.72% (very high)
Test Accuracy: 74.16% (lower)

Model memorized training data
but can't generalize well
```

### Normalization
```
Converts features to common scale
Formula: (X - mean) / std

Why: Improves training speed
     and model performance
```

---

## 💡 Common Questions

**Q: Why split data 80/20?**
A: Standard practice. 80% enough to train, 20% enough to test.

**Q: Why normalize?**
A: Features have different scales. Normalization makes them comparable.

**Q: Why is training accuracy higher?**
A: Model can memorize training data. Test data is unseen.

**Q: Can we improve the model?**
A: Yes! Try: more trees, different depth, different hyperparameters.

**Q: What does feature importance mean?**
A: How much each feature contributes to predictions.

**Q: Is 74% accuracy good?**
A: For medical data, yes! Real data is complex.

---

## ✅ Lab Checklist

- [ ] Install required libraries (pandas, scikit-learn, etc.)
- [ ] Have diabetes.txt file in correct location
- [ ] Load dataset successfully (442 samples)
- [ ] Create binary labels (balanced: 221/221)
- [ ] Split data (353 train, 89 test)
- [ ] Normalize features (means/stds calculated)
- [ ] Train Random Forest (100 trees)
- [ ] Evaluate model (accuracy ~74%)
- [ ] Check feature importance (BMI highest)
- [ ] Save all 6 files
- [ ] Test on sample patient
- [ ] Understand confusion matrix
- [ ] Understand feature importance

---

## 📊 Lab Results

### Model Performance
```
Training Accuracy: 99.72%
Test Accuracy: 74.16%
Precision: 0.71
Recall: 0.80
F1-Score: 0.75
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

### Feature Importance
```
Top 3 Features:
  1. BMI: 21.15%
  2. S5: 17.73%
  3. BP: 14.32%
```

### Files Generated
```
6 files in: SavedModels/DiabetesModel/
```

---

## 🚀 Next Steps

### 1. **Load in C++**
Use DiabetesPrediction_CPP.cpp to load model

### 2. **Improve Model**
Try different hyperparameters:
```python
model = RandomForestClassifier(
    n_estimators=200,    # More trees
    max_depth=8,         # Shallower
    min_samples_split=5
)
```

### 3. **Try Other Models**
- Logistic Regression
- SVM
- Gradient Boosting
- Neural Network

### 4. **Feature Engineering**
Create new features:
- BMI category (underweight, normal, etc.)
- Age groups (young, middle, senior)
- Blood pressure category (normal, high, etc.)

### 5. **Hyperparameter Tuning**
Find best parameters:
```python
from sklearn.grid_search import GridSearchCV
```

---

## 📚 Learning Resources

### Key Libraries
- **pandas**: Data manipulation
- **scikit-learn**: Machine learning
- **joblib**: Model saving
- **numpy**: Numerical computing

### Important Functions
- `train_test_split()`: Split data
- `StandardScaler()`: Normalize features
- `RandomForestClassifier()`: Create model
- `accuracy_score()`: Evaluate model
- `confusion_matrix()`: Error analysis
- `feature_importances_`: Feature importance

---

## 🎓 Summary

This lab taught you:

✅ **Data Loading**: Read CSV with pandas  
✅ **Label Creation**: Convert continuous to binary  
✅ **Data Splitting**: Train/test split  
✅ **Normalization**: StandardScaler  
✅ **Model Training**: Random Forest  
✅ **Model Evaluation**: Accuracy, precision, recall  
✅ **Feature Analysis**: Feature importance  
✅ **Model Saving**: joblib serialization  
✅ **Prediction**: Make predictions on new data  
✅ **Complete Pipeline**: End-to-end ML workflow  

**You now understand the full machine learning pipeline!**

---

## 📝 Appendix: Python Code Summary

### All Code in One Place
```python
# Step 1: Load
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, confusion_matrix, classification_report
import joblib, numpy as np, os

df = pd.read_csv(data_path, sep=r"\s+")

# Step 2: Label
median_y = df['Y'].median()
df['label'] = (df['Y'] > median_y).astype(int)

# Step 3: Features
X = df.drop(['Y', 'label'], axis=1)
y = df['label']

# Step 4: Split
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

# Step 5: Normalize
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Step 6: Train
model = RandomForestClassifier(n_estimators=100, max_depth=10, random_state=42)
model.fit(X_train_scaled, y_train)

# Step 7: Evaluate
train_pred = model.predict(X_train_scaled)
test_pred = model.predict(X_test_scaled)
print(f"Train Acc: {accuracy_score(y_train, train_pred):.4f}")
print(f"Test Acc: {accuracy_score(y_test, test_pred):.4f}")
print(confusion_matrix(y_test, test_pred))
print(classification_report(y_test, test_pred))

# Step 8: Feature Importance
for name, imp in zip(X.columns, model.feature_importances_):
    print(f"{name}: {imp:.4f}")

# Step 9: Save
save_dir = r"C:\...\DiabetesModel"
os.makedirs(save_dir, exist_ok=True)
joblib.dump(model, os.path.join(save_dir, "diabetes_model.pkl"))
joblib.dump(scaler, os.path.join(save_dir, "diabetes_scaler.pkl"))
np.savetxt(os.path.join(save_dir, "feature_means.txt"), scaler.mean_)
np.savetxt(os.path.join(save_dir, "feature_stds.txt"), scaler.scale_)

# Step 11: Test
sample_raw = np.array([[45, 1, 25.0, 100, 180, 120, 50, 3, 4.5, 85]])
sample_df = pd.DataFrame(sample_raw, columns=X.columns)
sample_scaled = scaler.transform(sample_df)
pred = model.predict(sample_scaled)[0]
prob = model.predict_proba(sample_scaled)[0]
print(f"Result: {['No Diabetes', 'Has Diabetes'][pred]}")
print(f"Confidence: {prob[pred]*100:.2f}%")
```

---

**LAB COMPLETE! You've successfully trained a diabetes classification model!** 🎉
