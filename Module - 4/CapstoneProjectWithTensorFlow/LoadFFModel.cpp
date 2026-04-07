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
// THREAD-SAFE QUEUE
// ============================================================

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
        cv.notify_one();
    }

    T pop() {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [&]() { return !q.empty(); });
        T val = q.front();
        q.pop();
        return val;
    }
};

// ============================================================
// GLOBAL QUEUES
// ============================================================

SafeQueue<vector<float>> inputQueue;
SafeQueue<float> outputQueue;

// ============================================================
// LOAD FILE
// ============================================================

vector<float> LoadFloatFile(string path) {
    vector<float> vals;
    ifstream file(path);
    float x;
    while (file >> x) vals.push_back(x);
    return vals;
}

// ============================================================
// NORMALIZATION
// ============================================================

vector<float> Normalize(vector<float> input, vector<float> mean, vector<float> std) {
    vector<float> out(input.size());
    for (int i = 0; i < input.size(); i++) {
        out[i] = (input[i] - mean[i]) / std[i];
    }
    return out;
}

// ============================================================
// ERROR CHECK
// ============================================================

void CheckStatus(TF_Status* status) {
    if (TF_GetCode(status) != TF_OK) {
        cerr << "TF Error: " << TF_Message(status) << endl;
        exit(1);
    }
}

// ============================================================
// THREAD 1: PRODUCER
// ============================================================

void producer() {
    while (true) {
        vector<float> event = {
            static_cast<float>(rand() % 24),        // hour
            static_cast<float>(rand() % 5),         // device_id
            static_cast<float>(rand() % 2),         // access_type
            static_cast<float>(rand() % 10)         // user_freq
        };

        cout << "[Producer] Event generated\n";

        inputQueue.push(event);

        this_thread::sleep_for(chrono::seconds(1));
    }
}

// ============================================================
// THREAD 2: ML PROCESSOR
// ============================================================

void processor(string model_dir) {

    // Load normalization
    vector<float> mean = LoadFloatFile(model_dir + "/feature_means.txt");
    vector<float> std = LoadFloatFile(model_dir + "/feature_stds.txt");

    // TensorFlow setup
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

    while (true) {

        vector<float> input = inputQueue.pop();

        vector<float> norm = Normalize(input, mean, std);

        // Create tensor
        int64_t dims[2] = { 1, 4 };
        TF_Tensor* input_tensor = TF_AllocateTensor(TF_FLOAT, dims, 2, sizeof(float) * 4);

        float* data = (float*)TF_TensorData(input_tensor);
        for (int i = 0; i < 4; i++) data[i] = norm[i];

        // ⚠️ UPDATE BASED ON YOUR MODEL
        TF_Output input_op = {
            TF_GraphOperationByName(graph, "serve_keras_tensor"), 0
        };

        TF_Output output_op = {
            TF_GraphOperationByName(graph, "StatefulPartitionedCall"), 0
        };

        if (!input_op.oper || !output_op.oper) {
            cerr << "❌ Operation error\n";
            exit(1);
        }

        TF_Tensor* output_tensor = nullptr;

        TF_SessionRun(session,
            nullptr,
            &input_op, &input_tensor, 1,
            &output_op, &output_tensor, 1,
            nullptr, 0, nullptr,
            status);

        CheckStatus(status);

        float result = *(float*)TF_TensorData(output_tensor);

        outputQueue.push(result);

        TF_DeleteTensor(input_tensor);
        TF_DeleteTensor(output_tensor);
    }
}

// ============================================================
// THREAD 3: ALERT HANDLER
// ============================================================

void alertHandler() {
    while (true) {
        float result = outputQueue.pop();

        if (result > 0.7)
            cout << "🚨 HIGH RISK ALERT (" << result << ")\n";
        else if (result > 0.4)
            cout << "⚠ Suspicious (" << result << ")\n";
        else
            cout << "✅ Normal (" << result << ")\n";
    }
}

// ============================================================
// MAIN
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