# C++ Lab Guide --- Working with `std::vector`

## Objective

Understand how to use vectors in C++ including: - Declaring vectors -
Adding elements using `push_back()` and `emplace_back()` - Accessing
elements - Traversing vectors - Removing elements - Observing
differences between `push_back()` and `emplace_back()`

------------------------------------------------------------------------

## What is a Vector?

A **vector** in C++ is a dynamic array from the Standard Template
Library (STL).

Key properties:

-   Automatically resizes when elements are added
-   Stores elements in contiguous memory
-   Supports fast random access
-   Manages memory automatically

Header required:

``` cpp
#include <vector>
```

Declaration:

``` cpp
vector<int> numbers;
```

------------------------------------------------------------------------

# Example 1 --- Creating and Using a Vector

``` cpp
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> numbers;

    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    cout << numbers[0] << endl;
    cout << numbers[1] << endl;
    cout << numbers[2] << endl;

    return 0;
}
```

### Output

    10
    20
    30

Explanation: - `push_back()` adds elements to the end of the vector. -
Indexing starts at **0**.

------------------------------------------------------------------------

# Example 2 --- Traversing a Vector

``` cpp
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> numbers = {5,10,15,20};

    for(int i = 0; i < numbers.size(); i++)
    {
        cout << numbers[i] << " ";
    }

    return 0;
}
```

### Output

    5 10 15 20

Explanation:

-   `size()` returns number of elements.
-   Loop iterates through all elements.

------------------------------------------------------------------------

# Example 3 --- Range Based Loop

``` cpp
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> numbers = {1,2,3,4};

    for(int value : numbers)
    {
        cout << value << " ";
    }

    return 0;
}
```

### Output

    1 2 3 4

Explanation: Range-based loops simplify iteration.

------------------------------------------------------------------------

# Example 4 --- push_back vs emplace_back

``` cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Student
{
public:
    string name;
    int age;

    Student(string n, int a)
    {
        name = n;
        age = a;
    }
};

int main()
{
    vector<Student> students;

    students.push_back(Student("Aman",21));
    students.emplace_back("Riya",22);

    cout << students[0].name << endl;
    cout << students[1].name << endl;
}
```

### Output

    Aman
    Riya

Explanation:

`push_back()`: - Object created first - Then inserted into vector

`emplace_back()`: - Object constructed directly inside vector

------------------------------------------------------------------------

# Example 5 --- Removing Elements

``` cpp
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> numbers = {10,20,30,40};

    numbers.pop_back();

    for(int n : numbers)
        cout << n << " ";
}
```

### Output

    10 20 30

Explanation:

`pop_back()` removes the last element.

------------------------------------------------------------------------

# Useful Vector Functions

  Function           Purpose
  ------------------ ----------------------------
  `push_back()`      Add element at end
  `emplace_back()`   Construct element in-place
  `pop_back()`       Remove last element
  `size()`           Number of elements
  `clear()`          Remove all elements
  `empty()`          Check if vector is empty

------------------------------------------------------------------------

# Practice Exercise

Write a program that:

1.  Creates a vector of integers
2.  Inserts five numbers using `push_back()`
3.  Prints the numbers using a loop
4.  Removes the last element using `pop_back()`
5.  Prints the vector again

Expected output format:

    Original Vector:
    10 20 30 40 50

    After pop_back:
    10 20 30 40

------------------------------------------------------------------------

# Key Takeaways

-   `vector` is a dynamic array in C++
-   `push_back()` inserts elements at the end
-   `emplace_back()` constructs objects inside the vector
-   `size()` returns number of elements
-   Vectors provide fast random access
