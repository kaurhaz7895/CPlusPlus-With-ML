
import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.preprocessing import StandardScaler
import os

# ============================================================
# STEP 1: CREATE SAMPLE DATA
# ============================================================

np.random.seed(42)

n = 500

data = pd.DataFrame({
    "hour": np.random.randint(0, 24, n),
    "device_id": np.random.randint(1, 5, n),
    "access_type": np.random.randint(0, 2, n),  # 0=entry, 1=exit
    "user_freq": np.random.randint(1, 10, n)
})

# Label: anomaly if late night + high freq
data["label"] = ((data["hour"] < 5) & (data["user_freq"] > 6)).astype(int)

X = data.drop("label", axis=1)
y = data["label"]

# ============================================================
# STEP 2: NORMALIZE
# ============================================================

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# ============================================================
# STEP 3: BUILD MODEL 
# ============================================================
# Simple feedforward network for binary classification
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(4,)),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

model.fit(X_scaled, y, epochs=10, batch_size=32)

# ============================================================
# STEP 4: SAVE MODEL + CONFIG
# ============================================================

save_dir = r"C:\Users\jasle\source\repos\Module4_CapstoneProject\SavedModels\FFModel"
os.makedirs(save_dir, exist_ok=True)

model.export(save_dir)

# Save normalization
np.savetxt(os.path.join(save_dir, "feature_means.txt"), scaler.mean_)
np.savetxt(os.path.join(save_dir, "feature_stds.txt"), scaler.scale_)

# Save feature names
with open(os.path.join(save_dir, "feature_names.txt"), "w") as f:
    for col in X.columns:
        f.write(col + "\n")

print("✅ Model + config saved")