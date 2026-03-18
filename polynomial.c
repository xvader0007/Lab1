#include "polynomial.h"
#include "complex.h"
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

    //обнуляем часть новую часть массива
    memset((char*)new_coef + poly->capacity * poly->type_info->size_el, 0, (new_capacity - poly->capacity) * poly->type_info->size_el);

    poly->coeff = new_coef;
    poly->capacity = new_capacity;

    return 0;
}

//получение указателя на коэф по индексу
static void *polynomial_get_coef_ptr(const Polynomial* poly, int index)
{
    if(!poly || !poly->coeff) return NULL;
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

        if(poly->type_info == field_info_get_int()) zero = (*(int*)coef == 0);
        else if(poly->type_info == field_info_get_complex())
        {
            Complex a = *(Complex *)coef;
            zero = complex_equal(a, (Complex) {0, 0});
        }
        else zero = 0;

        if(!zero) {
            poly->degree = i;
            return;
        }
    }

    // Если все коэффициенты нулевые
    poly->degree = 0;
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
    if(!poly) return;
    if(poly->coeff) {
        free(poly->coeff);
    }
    free(poly);
}


void polynomial_clear(Polynomial* poly)
{
    if(!poly || !poly->type_info) return;
    memset(poly->coeff, 0, poly->capacity * poly->type_info->size_el);
    poly->degree = 0;
}

int polynomial_set_coef(Polynomial *poly, int index, const void *value)
{
    void *target;
    void *cloned_value;

    if (!poly || !value || !poly->type_info || !poly->type_info->clone)
        return -1;

    if (polynomial_capacity(poly, index + 1) != 0)
        return -1;

    // Клонируем новое значение
    cloned_value = poly->type_info->clone((void *)value);
    if (!cloned_value)
        return -1;

    // Копируем в массив (перезаписываем старое значение)
    target = polynomial_get_coef_ptr(poly, index);
    memcpy(target, cloned_value, poly->type_info->size_el);

    // Освобождаем ВРЕМЕННУЮ копию (которую создал clone)
    poly->type_info->free(cloned_value);

    if (index > poly->degree)
        poly->degree = index;

    polynomial_update_degree(poly);
    return 0;
}

int polynomial_get_coef(const Polynomial* poly, int index, void* out)
{
    void *coef;

    if (!poly || !out || !poly->type_info) return -1;

    if (index > poly->degree) {
        memset(out, 0, poly->type_info->size_el);

        return 0;
    }

    coef = polynomial_get_coef_ptr(poly, index);
    if(!coef) return -1;

    memcpy(out, coef, poly->type_info->size_el);

    return 0;
}

size_t polynomial_get_degree(const Polynomial* poly)
{
    if(!poly) return 0;
    return poly->degree;
}

int polynomial_add(Polynomial* result, const Polynomial* a, const Polynomial* b)
{
    int max_degree;
    void* coef_a;
    void* coef_b;
    void* coef_result;

    if(!result || !a || !b) return -1;

    if(!field_info_compare(a->type_info, b->type_info)) return -1;

    if(a->degree > b->degree) max_degree = a->degree;
    else max_degree = b->degree;

    if(polynomial_capacity(result, max_degree + 1) != 0) return -1;

    for(int i = 0; i <= max_degree; i++)
    {
        if(i <= a->degree) coef_a = polynomial_get_coef_ptr(a,  i);
        else coef_a = NULL;

        if(i <= b->degree) coef_b = polynomial_get_coef_ptr(b, i);
        else coef_b = NULL;

        coef_result = polynomial_get_coef_ptr(result, i);

        if(!coef_result) return -1;

        if(result->type_info == field_info_get_int())
        {
            int int_a = coef_a ? *(int*)coef_a : 0;
            int int_b = coef_b ? *(int*)coef_b : 0;
            int sum = int_a + int_b;
            memcpy(coef_result, &sum, sizeof(int));
        }
        else if(result->type_info == field_info_get_complex())
        {
            Complex c_a = coef_a ? *(Complex*)coef_a : (Complex){0,0};
            Complex c_b = coef_b ? *(Complex*)coef_b : (Complex){0,0};
            Complex sum = complex_add(c_a, c_b);
            memcpy(coef_result, &sum, sizeof(Complex));
        }
    }

    result->degree = max_degree;
    polynomial_update_degree(result);

    return 0;
}

int polynomial_mult(Polynomial* result, const Polynomial* a, const Polynomial* b)
{
    int result_degree;
    void* coef_result;

    if(!result || !a || !b) return -1;

    if(!field_info_compare(a->type_info, b->type_info)) return -1;

    result_degree = a->degree + b->degree;

    if(polynomial_capacity(result, result_degree + 1) != 0) return -1;

    //обнуляем результат
    memset(result->coeff, 0, result->capacity * result->type_info->size_el);
    result->degree = result_degree;

    for(int i = 0; i <= a->degree; i++)
    {
        for(int j = 0; j <= b->degree; j++)
        {
            void* coef_a = polynomial_get_coef_ptr(a, i);
            void* coef_b = polynomial_get_coef_ptr(b, j);
            void* coef_result = polynomial_get_coef_ptr(result, i + j);

            if(!coef_a || !coef_b || !coef_result) continue;

            if(result->type_info == field_info_get_int())
            {
                int int_a = *(int*)coef_a;
                int int_b = *(int*)coef_b;
                int int_res = *(int*)coef_result;
                int res_mult = int_res + int_a * int_b;

                memcpy(coef_result, &res_mult, sizeof(int));
            }
            else if(result->type_info == field_info_get_complex())
            {
                Complex c_a = *(Complex*)coef_a;
                Complex c_b = *(Complex*)coef_b;
                Complex c_res = *(Complex*)coef_result;
                Complex c_mult = complex_add(c_res, complex_mult(c_a, c_b));

                memcpy(coef_result, &c_mult, sizeof(Complex));
            }
        }
    }

    polynomial_update_degree(result);

    return 0;
}

int polynomial_mult_scal(Polynomial* poly, const void* scalar)
{
    void* coef;

    if(!poly || !scalar) return -1;

    for(int i = 0; i <= poly->degree; i++)
    {
        coef = polynomial_get_coef_ptr(poly, i);
        if(!coef) continue;

        if(poly->type_info == field_info_get_int())
        {
            int int_coef = *(int*)coef;
            int int_scalar = *(int*)scalar;

            int_coef = int_coef * int_scalar;

            memcpy(coef, &int_coef, sizeof(int));
        }
        else if(poly->type_info == field_info_get_complex())
        {
            Complex c_coef = *(Complex*)coef;
            Complex c_scalar = *(Complex*)scalar;

            c_coef = complex_mult(c_coef, c_scalar);

            memcpy(coef, &c_coef, sizeof(Complex));
        }
    }

    polynomial_update_degree(poly);

    return 0;
}

int polynomial_evaluate(const Polynomial *poly, const void* a, void* result)
{
    void* coef;

    if(!poly || !a || !result) return -1;

    //инициализируем результат старшим коэф
    if(poly->degree == 0)
    {
        coef = polynomial_get_coef_ptr(poly, 0);
        if(!coef) return -1;

        memcpy(result, coef, poly->type_info->size_el);

        return 0;
    }

    //старший коэф
    coef = polynomial_get_coef_ptr(poly, poly->degree);
    if(!coef) return -1;
    memcpy(result, coef, poly->type_info->size_el);

    //Схема горнера P(x) = a_0 + x(a_1 + x(a_2 + ... + x(a_n)))
    for(int i = poly->degree; i > 0; i--)
    {
        if(poly->type_info == field_info_get_int())
        {
            int int_result = *(int*)result;
            int int_a = *(const int*)a;

            int_result = int_result * int_a;

            memcpy(result, &int_result, sizeof(int));
        }
        else if(poly->type_info == field_info_get_complex())
        {
            Complex c_result = *(Complex*)result;
            Complex c_a = *(const Complex*)a;

            c_result = complex_mult(c_result, c_a);

            memcpy(result, &c_result, sizeof(Complex));
        }

        //result = result + a_(i-1)
        coef = polynomial_get_coef_ptr(poly, i - 1);
        if(coef)
        {
            if(poly->type_info == field_info_get_int())
            {
                int int_result = *(int*)result;
                int int_a = *(int*)coef;

                int_result = int_result + int_a;

                memcpy(result, &int_result, sizeof(int));
            }
            else if(poly->type_info == field_info_get_complex())
            {
                Complex c_result = *(Complex*)result;
                Complex c_a = *(Complex*)coef;

                c_result = complex_add(c_result, c_a);

                memcpy(result, &c_result, sizeof(Complex));
            }
        }
    }

    return 0;
}

void polynomial_print(const Polynomial* poly)
{
    void* coef;
    int first = 1;

    if(!poly)
    {
        printf("NULL\n");
        return;
    }

    if(poly->degree == 0)
    {
        coef = polynomial_get_coef_ptr(poly, 0);
        if(coef)
        {
            poly->type_info->print(coef);
            printf("\n");
        }
        else printf("0\n");

        return;
    }

    for(int i = poly->degree; i >= 0; i--)
    {
        coef = polynomial_get_coef_ptr(poly, i);
        if(!coef) return;

        if(poly->type_info == field_info_get_int())
        {
            if(*(int*)coef == 0 && i > 0) continue;
        }
        else if(poly->type_info == field_info_get_complex())
        {
            Complex c_coef = *(Complex*)coef;
            if(complex_equal(c_coef, (Complex){0,0}) && i > 0) continue;
        }

        if(!first) printf(" + ");
        first = 0;

        printf("(");
        poly->type_info->print(coef);
        printf(")");

        if(i > 0)
        {
            printf("x");
            if(i > 1) printf("^%d", (int)i);
        }
    }

    printf("\n");
}

Polynomial* polynomial_clone(const Polynomial* poly)
{
    Polynomial* clone;
    void* coef;

    if(!poly) return NULL;

    clone = polynomial_create(poly->type_info);
    if(!clone) return NULL;

    if(polynomial_capacity(clone, poly->degree + 1) != 0)
    {
        polynomial_destroy(clone);
        return NULL;
    }

    for(int i = 0; i <= poly->degree; i++)
    {
        coef = polynomial_get_coef_ptr(poly, i);

        if(coef)
        {
            void* cloned = poly->type_info->clone(coef);
            if(cloned) memcpy((char*)clone->coeff + i * poly->type_info->size_el, cloned, poly->type_info->size_el);
            poly->type_info->free(cloned);
        }
    }

    clone->degree = poly->degree;

    return clone;
}

int polynomial_is_empty(const Polynomial* poly)
{
    if(!poly) return 1;
    return poly->degree == 0;
}