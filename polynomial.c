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

int polynomial_set_coeff(Polynomial* poly, int index, const void* value)
{
    void* target;
    void* clon_value;

    if(!poly || !value || !poly->type_info || !poly->type_info->clone) return -1;

    if(polynomial_capacity(poly, index + 1) != 0) return -1;

    //освобождаем старый коэф
    target = polynomial_get_coef_ptr(poly, index);
    if(target && index <= poly->degree) poly->type_info->free(target);

    //копируем значение
    clon_value = poly->type_info->clone((void*)value);
    if(!clon_value) return -1;

    //копируем в массив
    memcpy(target, clon_value, poly->type_info->size_el);
    poly->type_info->free(clon_value);

    //обновление степени
    if(index > poly->degree) poly->degree = index;

    polynomial_update_degree(poly);

    return 0;
}

int polynomial_get_coef(const Polynomial* poly, size_t index, void* out)
{
    void *coef;

    if (!poly || !out || !poly->type_info) return -1;

    if (index > poly->degree) {
        memset(out, 0, poly->type_info->size_el);

        return 0;
    }
}

size_t polynomial_get_degree(const Polynomial* poly)
{
    if(!poly) return 0;
    return poly->degree;
}

int polynomal_add(Polynomial* result, const Polynomial* a, const Polynomial* b)
{
    int max_degree;
    void* coef_a;
    void* coef_b;
    void* coef_result;

    if(!result || !a || !b) return -1;

    if(!field_info_compare(a->type_info, b->type_info)) return -1;

    max_degree = (a->degree > b->degree);
}

















