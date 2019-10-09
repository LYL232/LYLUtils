#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_COMPLEX_H
#define LYL_UTILS_COMPLEX_H

template<typename NUM_TYPE = double>
class Complex {
public:
    NUM_TYPE r, i;

    Complex() {
        r = 0;
        i = 0;
    }

    Complex(NUM_TYPE xx, NUM_TYPE yy) {
        r = xx;
        i = yy;
    }

    Complex(const Complex &_complex) {
        r = _complex.r;
        i = _complex.i;
    }

    Complex operator*(const Complex &b) const {
        return Complex(r * b.r - i * b.i, r * b.i + i * b.r);
    }

    Complex operator/(const Complex &b) const {
        NUM_TYPE divisor = b.r * b.r + b.i * b.i;
        return Complex((r * b.r + i * b.i) / divisor, (i * b.r - r * b.i) / divisor);
    }

    Complex operator+(const Complex &b) const {
        return Complex(r + b.r, i + b.i);
    }

    Complex operator-(const Complex &b) const {
        return Complex(r - b.r, i - b.i);
    }

    inline void copy(const Complex &_complex) {
        r = _complex.r;
        i = _complex.i;
    }

    inline void assign(NUM_TYPE _x, NUM_TYPE _y) {
        r = _x;
        i = _y;
    }

private:

};


#endif //LYL_UTILS_COMPLEX_H

#pragma clang diagnostic pop