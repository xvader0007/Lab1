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
    free(value);
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

Field_info *field_info_get_int(void*)
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
    free(value);
}

static int complex_equals(void* a, void* b)
{
    if(!a || !b) return 0;
    return complex_equal(*(Complex*)a, *(Complex*)b);
}

static void complex_print_field(void* value)
{
    if (!value) return;
    return complex_print(*(Complex*)value);
}

Field_info *field_info_get_complex(void*)
{
    if(!field_info_complex)
    {
        field_info_complex = malloc(sizeof(Field_info));
        if(!field_info_complex) return NULL;

        field_info_complex->size_el = sizeof(Complex);
        field_info_complex->clone = complex_clone;
        field_info_complex->free = complex_free;
        field_info_complex->equal = complex_equals;
        field_info_complex->print = complex_print_field;
    }

    return field_info_complex;
}

int field_info_compare(Field_info *a, Field_info *b)
{
    return a == b;
}