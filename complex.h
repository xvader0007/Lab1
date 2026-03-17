#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdio.h>

typedef struct // структура косплкексного числа
{
    double real_part;
    double imag_part;
}Complex;

int complex_equal(Complex a, Complex b); //сравнение комплексных чисел
void complex_print(Complex a); //вывод комплексного числа
Complex complex_add(Complex a, Complex b); //сложение комплексных чисел
Complex complex_mult(Complex a, Complex b); //умножение комплексных чисел
Complex complex_scal_mult(Complex a, double scalar); //умножение комплексного числа на скаляр

#endif //COMPLEX_H
