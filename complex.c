#include <stdio.h>
#include <math.h>
#include "complex.h"

Complex complex_add(Complex a, Complex b)
{
    Complex result;

    result.real_part = a.real_part + b.real_part;
    result.imag_part = a.imag_part + b.imag_part;

    return result;
}

Complex complex_mult(Complex a, Complex b)
{
    Complex result;

    result.real_part = a.real_part * b.real_part - a.imag_part * b.imag_part;
    result.imag_part = a.real_part * b.imag_part + a.imag_part * b.real_part;

    return result;
}

Complex complex_scal_mult(Complex a, double scalar)
{
    Complex result;

    result.real_part = a.real_part * scalar;
    result.imag_part = a.imag_part * scalar;

    return result;
}

int complex_equal(Complex a, Complex b)
{
    const double e = 1e-9;
    return (fabs(a.real_part - b.real_part) < e) && (fabs(a.imag_part - b.imag_part) < e);
}

void complex_print(Complex a)
{
    if(a.real_part != 0 && a.imag_part != 0)
    {
        if(a.imag_part > 0) printf("%.2f+%.2fi", a.real_part, a.imag_part);
        else printf("%.2f%.2fi", a.real_part, a.imag_part);
    }
    else if(a.real_part == 0 && a.imag_part != 0) printf("%.2fi", a.imag_part);
    else if(a.real_part != 0 && a.imag_part == 0) printf("%.2f", a.real_part);
    else printf("%d", 0);
}