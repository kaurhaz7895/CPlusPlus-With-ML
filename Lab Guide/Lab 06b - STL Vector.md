# C++ STL Vector — Basic Program

## Objective
Understand the basic operations of the `std::vector` container in C++ including:

- Vector creation
- Adding and removing elements
- Accessing elements
- Checking size and capacity
- Inserting and erasing elements
- Clearing vectors
- Assigning values
- Resizing vectors
- Reserving capacity
- Swapping vectors
- Accessing underlying data

---

# Program: Basic Vector Operations

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {

	// ---------- Vector Creation ----------
	vector<int> v1;                 // empty vector
	vector<int> v2(5);              // size 5, default values (0)
	vector<int> v3(5, 10);          // size 5, all values = 10
	vector<int> v4{ 1,2,3,4,5 };    // initializer list
	vector<int> v5 = { 1,2,3,4,5 }; // another initializer list

	// ---------- push_back ----------
	v5.push_back(6); // adds 6 to the end
	v5.push_back(7); // adds 7 to the end

	cout << "v5 elements after push_back: ";
	for (auto num : v5) {
		cout << num << " ";
	}

	cout << "\nDisplaying addresses of elements in v5:\n";
	cout << &v5[0] << "\n"
	     << &v5[1] << "\n"
	     << &v5[2] << "\n"
	     << &v5[3] << "\n"
	     << &v5[4] << endl;

	// ---------- pop_back ----------
	cout << endl;
	v5.pop_back(); // removes last element

	cout << "v5 after pop_back: ";
	for (auto num : v5) {
		cout << num << " ";
	}

	cout << "\n";

	// ---------- size and capacity ----------
	cout << "Size: " << v5.size() << endl;
	cout << "Capacity: " << v5.capacity() << endl;

	// ---------- element access ----------
	cout << "First element: " << v5.front() << endl;
	cout << "Last element: " << v5.back() << endl;
	cout << "Element at index 2 using at(): " << v5.at(2) << endl;

	// ---------- insert ----------
	v5.insert(v5.begin() + 2, 10);

	cout << "After insert at index 2: ";
	for (auto num : v5)
		cout << num << " ";
	cout << endl;

	// ---------- erase ----------
	v5.erase(v5.begin() + 2);

	cout << "After erase index 2: ";
	for (auto num : v5)
		cout << num << " ";
	cout << endl;

	// ---------- clear ----------
	vector<int> v6 = { 10,20,30 };
	v6.clear();

	cout << "v6 size after clear(): " << v6.size() << endl;

	// ---------- empty ----------
	if (v6.empty())
		cout << "v6 is empty" << endl;

	// ---------- assign ----------
	vector<int> v7;
	v7.assign(5, 50);

	cout << "v7 after assign(): ";
	for (auto num : v7)
		cout << num << " ";
	cout << endl;

	// ---------- resize ----------
	v7.resize(3);

	cout << "v7 after resize(3): ";
	for (auto num : v7)
		cout << num << " ";
	cout << endl;

	// ---------- reserve ----------
	vector<int> v8;
	v8.reserve(10);

	cout << "v8 capacity after reserve(10): " << v8.capacity() << endl;

	// ---------- swap ----------
	vector<int> a = {1,2,3};
	vector<int> b = {10,20};

	a.swap(b);

	cout << "Vector a after swap: ";
	for(auto num : a)
		cout << num << " ";
	cout << endl;

	// ---------- data ----------
	int* ptr = a.data();

	cout << "First element using data(): " << *ptr << endl;

	return 0;
}
```

# Review Questions

## Question 1
What is a `std::vector` in C++?

### Answer
`std::vector` is a dynamic array container from the C++ Standard Template Library (STL).  
It stores elements in **contiguous memory** and automatically resizes when elements are added or removed.

---

## Question 2
What is the difference between `size()` and `capacity()`?

### Answer

| Function | Meaning |
|--------|---------|
| `size()` | Number of elements currently stored in the vector |
| `capacity()` | Total number of elements the vector can hold before reallocating memory |

---

## Question 3
What does `push_back()` do?

### Answer
`push_back()` inserts a new element at the **end of the vector**.

Example:

```cpp
v.push_back(10);
```
This increases the vector size by one.

## Question 4
What does `pop_back()` do?

### Answer
pop_back() removes the last element from the vector.
