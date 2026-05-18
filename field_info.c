#include "field_info.h"
#include "complex.h"

#include <stdio.h>
#include <stdlib.h>

/*
===================================
=                                 =
=       Реализация под int        =
=                                 =
===================================
*/

static Field_info *field_info_int = NULL;

static void *int_clone(void *value)
{
    int *result;

    if(!value) return NULL;

    result = malloc(sizeof(int));
    if(!result) return NULL;

    *result = *(int*)value; //приводим к int и достаем значение из памяти

    return result;
}

static void int_free(void* value)
{
    (void)value;
}

static int int_equal(void* a, void* b)
{
    if(!a || !b) return 0; //защита от NULL
    return *(int*)a == *(int*)b;
}

static void int_print(void* value)
{
    if(!value) return;
    printf("%d", *(int*)value);
}

//кольцо

static void int_add(void *a, void *b, void *result)
{
    if(!a || !b || !result) return;
    *(int*)result = *(int*)a + *(int*)b;
}

static void int_mult(void *a, void *b, void *result)
{
    if(!a || !b || !result) return;
    *(int*)result = *(int*)a * *(int*)b;
}

static void int_mult_scal(void *a, void *scalar)
{
    if(!a || !scalar) return;
    *(int*)a = *(int*)a * *(int*)scalar;
}

static void int_zero(void *a) {*(int*)a = 0;}
static void int_one(void *a) {*(int*)a = 1;}
static int int_is_zero(void *a) { return *(int*)a == 0;}


Field_info *field_info_get_int(void)
{
    if(!field_info_int) //проверка на первый вызов
    {
        field_info_int = malloc(sizeof(Field_info));
        if(!field_info_int) return NULL;

        field_info_int->size_el = sizeof(int);
        field_info_int->clone = int_clone;
        field_info_int->free = int_free;
        field_info_int->equal = int_equal;
        field_info_int->print = int_print;

        field_info_int->add = int_add;
        field_info_int->mult = int_mult;
        field_info_int->mult_scal = int_mult_scal;
        field_info_int->zero = int_zero;
        field_info_int->one = int_one;
        field_info_int->is_zero = int_is_zero;
    }
    return field_info_int;
}

/*
=======================================
=                                     =
=       Реализация под complex        =
=                                     =
=======================================
*/

static Field_info *field_info_complex = NULL;

static void *complex_clone(void* value)
{
    Complex *result;

    if(!value) return NULL;

    result = malloc(sizeof(Complex));
    if(!result) return NULL;

    *result = *(Complex*)value;

    return result;
}

static void complex_free(void* value)
{
    (void)value;
}

static int complex_equal_field(void* a, void* b)
{
    if(!a || !b) return 0;
    return complex_equal(*(Complex*)a, *(Complex*)b);
}

static void complex_print_field(void* value)
{
    if (!value) return;
    return complex_print(*(Complex*)value);
}

//кольцо

static void complex_add_field(void *a, void *b, void *result)
{
    if(!a || !b || !result) return;

    Complex *r = (Complex*)result;
    Complex *x = (Complex*)a;
    Complex *y = (Complex*)b;

    r->real_part = x->real_part + y->real_part;
    r->imag_part = x->imag_part + y->imag_part;
}

static void complex_mult_field(void *a, void *b, void *result)
{
    if(!a || !b || !result) return;

    Complex *r = (Complex*)result;
    Complex *x = (Complex*)a;
    Complex *y = (Complex*)b;

    r->real_part = x->real_part * y->real_part - x->imag_part * y->imag_part;
    r->imag_part = x->real_part * y->imag_part + x->imag_part * y->real_part;
}

static void complex_mult_scal_field(void *a, void *scalar)
{
    if(!a || !scalar) return;

    Complex *c = (Complex*)a;
    Complex *s = (Complex*)scalar;
    *c = complex_mult(*c, *s);
}

static void complex_zero(void *a)
{
    Complex *c = (Complex*)a;
    c->real_part = 0;
    c->imag_part = 0;
}

static void complex_one(void *a)
{
    Complex *c = (Complex*)a;
    c->real_part = 1;
    c->imag_part = 0;
}

static int complex_is_zero_field(void *a)
{
    Complex *c = (Complex*)a;

    return complex_equal(*c, (Complex){0,0});
}

Field_info *field_info_get_complex(void)
{
    if(!field_info_complex)
    {
        field_info_complex = malloc(sizeof(Field_info));
        if(!field_info_complex) return NULL;

        field_info_complex->size_el = sizeof(Complex);
        field_info_complex->clone = complex_clone;
        field_info_complex->free = complex_free;
        field_info_complex->equal = complex_equal_field;
        field_info_complex->print = complex_print_field;

        field_info_complex->add = complex_add_field;
        field_info_complex->mult = complex_mult_field;
        field_info_complex->mult_scal = complex_mult_scal_field;
        field_info_complex->zero = complex_zero;
        field_info_complex->one = complex_one;
        field_info_complex->is_zero = complex_is_zero_field;
    }

    return field_info_complex;
}

int field_info_compare(Field_info *a, Field_info *b)
{
    return a == b;
}