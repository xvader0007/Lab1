#ifndef LAB1_POLYNOMIAL_H
#define LAB1_POLYNOMIAL_H

#include "field_info.h"
#include <stdio.h>

typedef struct polynomial
{
    void *coeff; //динамический массив коэф
    size_t degree; //степень многочлена
    size_t capacity; //вместимость дин массива для элементов
    Field_info *type_info; //тип коэф
} Polynomial;

//создвние пустого многочлена с типом коэффициентов
Polynomial *polynomial_create(Field_info *type_info);

void polynomial_destroy(Polynomial *poly);

void polynomial_clear(Polynomial *poly);

int polynomial_set_coef(Polynomial *poly, size_t index, const void* value);

int polynomial_get_coef(const Polynomial *poly, size_t index, void* value);

size_t polynomial_get_degree(const Polynomial *poly);

int polynomial_add(Polynomial *result, const Polynomial *a, const Polynomial *b);

int polynomial_mult(Polynomial *result, const Polynomial *a, const Polynomial *b);

int polynomial_mult_scal(Polynomial *result, const void* scalar);

int polynomial_evaluate(const Polynomial *poly, const void* x, void* result);

void polynomial_print(const Polynomial *poly);

Polynomial *polynomial_clone(const Polynomial *poly);

int polynomial_is_empty(const Polynomial *poly);

#endif //LAB1_POLYNOMIAL_H*