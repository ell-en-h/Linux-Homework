#include <iostream>
#include <vector>
#include "complex/Complex.h"
#include "sort/Sort.h"

int main() {
    std::vector<Complex> numbers = {
        Complex(3, 4),
        Complex(1, 1),
        Complex(0, 2),
        Complex(5, -1)
    };

    std::cout << "Before sorting:\n";
    for (auto& c : numbers) {
        c.print();
        std::cout << " | abs=" << c.abs() << "\n";
    }

    sortComplex(numbers);

    std::cout << "\nAfter sorting:\n";
    for (auto& c : numbers) {
        c.print();
        std::cout << " | abs=" << c.abs() << "\n";
    }

    return 0;
}
