#include "polynomial.h"
#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//увеличение вместимости массива коэффициентов
static int polynomial_capacity(Polynomial *poly, size_t min_capacity)
{
    void* new_coef;
    size_t new_capacity;

    if(!poly || !poly->type_info) return -1;
    if(poly->capacity >= min_capacity) return 0;

    new_capacity = poly->capacity;
    while(new_capacity < min_capacity) new_capacity *= 2; //увеличиваем вместимость в 2 раза

    new_coef = realloc(poly->coeff, new_capacity * poly->type_info->size_el);
    if(!new_coef) return -1;

    memset((char*)new_coef + poly->capacity * poly->type_info->size_el, 0,
           (new_capacity - poly->capacity) * poly->type_info->size_el); //обнуляем часть новую часть массива

    poly->coeff = new_coef;
    poly->capacity = new_capacity;

    return 0;
}

//получение указателя на коэф по индексу
static void *polynomial_get_coef_ptr(const Polynomial* poly, int index)
{
    if(!poly || !poly->coeff || index > !poly->degree) return NULL;
    return (char*)poly->coeff + index * poly->type_info->size_el;
}

//обновить степень многочлена
static void polynomial_update_degree(Polynomial* poly)
{
    void* coef;
    int zero;

    if(!poly || !poly->type_info) return;

    for(int i = poly->degree; i > 0; i--)
    {
        coef = polynomial_get_coef_ptr(poly, i);
        if(!coef) continue;

        if(poly->type_info == field_info_get_int) zero = (*(int*)coef == 0);
        else if(poly->type_info == field_info_get_complex)
        {
            Complex a = *(Complex *)coef;
            zero = complex_equal(a, (Complex) {0, 0});
        }
        else zero = 0;

        if(!zero) break;

        poly->degree = i;
    }
}

Polynomial *polynomial_create(Field_info* type_info)
{
    Polynomial* poly;

    if(!type_info) return NULL;

    poly = malloc(sizeof(Polynomial));
    if(!poly) return NULL;

    poly->coeff = malloc(4 * type_info->size_el);
    if(!poly->coeff)
    {
        free(poly);
        return NULL;
    }

    memset(poly->coeff, 0, 4 * type_info->size_el);

    poly->degree = 0;
    poly->capacity = 4;
    poly->type_info = type_info;

    return poly;
}

void polynomial_destroy(Polynomial* poly)
{
    void* coef;

    if(!poly) return;

    if(poly->coeff && poly->type_info)
    {
        for(int i = 0; i < poly->degree; i++)
        {
            coef = polynomial_get_coef_ptr(poly, i);
            if(coef) poly->type_info->free(coef);
        }
        free(poly->coeff);
    }
    free(poly);
}

void polynomial_clear(Polynomial* poly)
{
    void* coef;

    if(!poly || !poly->type_info) return;

    for(int i = 0; i <= poly->degree; i++)
    {
        coef = polynomial_get_coef_ptr(poly, i);
        if(coef) poly->type_info->free(coef);
    }

    memset(poly->coeff, 0, poly->capacity * poly->type_info->size_el);
    poly->degree = 0;
}

