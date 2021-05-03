# Array3D

![GitHub](https://img.shields.io/github/license/A9Radeus/Array3D)
![GitHub](https://img.shields.io/badge/language-C%2B%2B17-blue)

## Integration

Simply add "Array3D.h" file to your project and include it:
```cpp 
#include "Array3D.h"
```

## Examples

Basic usage:

```cpp
rch::Array3D<int, 4,2,3> arr{}; // default initialisation

if (arr[{0, 0, 0}] == 0 && arr[{3, 1, 2}] == 0) {
    std::cout << "These have the default value. Now, let's change them.\n";
}

arr[{0, 0, 0}] = 2;
arr[{3, 1, 2}] = -5;

if (arr[{0, 0, 0}] == 2 && arr[{3, 1, 2}] == -5) {
    std::cout << "Done!\n";
}
```

Returning a given value when indices are out of bounds:

```cpp
rch::Array3D<float, 1,2,3> arr{};

arr[{0, 0, 1}] = 2.2f;

if (arr.dget(-1.3f, {0, 0, 1}) == 2.2f) {
    std::cout << "OK! {0, 0, 1} does not go out of bounds.\n";
}

if (arr.dget(-1.3f, {111, 222, 333}) == -1.3f) {
    std::cout << "Indices are out of bounds and '-1.3' is returned.\n";
}
```

Using static constexpr functions to check whether given indices don't go out of bounds. Useful when dealing with heavy index calculations that could be done at compile-time:

```cpp
if constexpr (rch::Array3D<int, 2,4,3>::in_bounds_signed(1, 2, 1) == true) {
    std::cout << "These indices are within the 3D boundaries which means the if statement doesn't have to be evaluated at runtime!\n"; 
}

if constexpr (rch::Array3D<int, 2,4,3>::in_bounds_signed(123, 123, 123) == true) {
    std::cout << "This won't even get compiled!\n"; 
}
```

## Tips

* When often dealing the same Array3D type, like inside of another class, you can use "using":

```cpp

class A {
public:
    using AData = rch::Array3D<float, 34, 23, 5>;

    const AData& get_data() const { return m_data; }
    void merge_data(const AData& other_data) { /*...*/ }

private:
    AData m_data;
};

```

## Testing

The repository also contains VisualStudio solution with "Tests" project in which you can find GoogleTest tests useful for further development of Array3D. 