#include "ComplexNumber.h"
#include "Sort.h"
#include <vector>
#include <iostream>

int main() {
    std::vector<ComplexNumber> arr {
        ComplexNumber(3, 4),
        ComplexNumber(1, -1),
        ComplexNumber(0, 2),
        ComplexNumber(-2, -2)
    };

    for(const auto& c : arr) {
        c.print();
        std::cout << " ";
    }
    std::cout << "\n";

    sortByAbs(arr);

    for(const auto& c : arr) {
        c.print();
        std::cout << " ";
    }
    std::cout << "\n";

    return 0;
}
