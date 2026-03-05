#include <stdio.h>
#include <math.h>
#include "complex.c"

int main(void)
{
    Complex a, b;
    a.real_part = 1.23456789;
    a.imag_part = 2.12345678;
    b.real_part = 3.14;
    b.imag_part = -2.71;
    complex_print(a);
    complex_print(b);

    return 0;
}
