#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

#include <iostream>
#include <cmath>

class ComplexNumber {
private:
    double real;
    double imag;

public:
    ComplexNumber(double r = 0, double i = 0);

    double getReal() const;
    double getImag() const;

    ComplexNumber operator+(const ComplexNumber& other) const;
    ComplexNumber operator-(const ComplexNumber& other) const;
    ComplexNumber operator*(double scalar) const;
    double abs() const;

    void print() const;
};

#endif
