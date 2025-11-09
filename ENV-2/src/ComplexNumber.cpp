#include "ComplexNumber.h"

ComplexNumber::ComplexNumber(double r, double i) : real(r), imag(i) {}

double ComplexNumber::getReal() const { 
    return real; }
double ComplexNumber::getImag() const { 
    return imag; }

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
    return ComplexNumber(real + other.real, imag + other.imag);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
    return ComplexNumber(real - other.real, imag - other.imag);
}

ComplexNumber ComplexNumber::operator*(double scalar) const {
    return ComplexNumber(real * scalar, imag * scalar);
}

double ComplexNumber::abs() const {
    return std::sqrt(real * real + imag * imag);
}

void ComplexNumber::print() const {
    std::cout << "(" << real << " + " << imag << "i)";
}
