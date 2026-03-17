#include <stdio.h>
#include "complex.h"
#include "field_info.h"
#include "polynomial.h"

//печать результата теста
static void test_print_result(const char* test_name, int passed)
{
    if(passed) printf("(+) %s passed\n", test_name);
    else printf("(-) %s failed\n", test_name);
}

static void test_polynomial_create_int(void)
{
    Polynomial* poly;
    int passed = 1;

    poly = polynomial_create(field_info_get_int());

    passed = passed && (poly != NULL);
    passed = passed && (poly->degree == 0);
    passed = passed && (poly->type_info == field_info_get_int(poly));

    if(poly) polynomial_destroy(poly);

    test_print_result("test_polynomial_create_int", passed);
}

static void test_polynomial_create_complex(void)
{
    Polynomial* poly;
    int passed = 1;

    poly = polynomial_create(field_info_get_complex(poly));

    passed = passed && (poly != 0);
    passed = passed && (poly->type_info == field_info_get_complex(poly));

    if(poly) polynomial_destroy(poly);

    test_print_result("test_polynomial_create_complex", passed);
};

static void test_set_get_coef_int(void)
{
    Polynomial* poly;
    int value, result;
    int passed;

    poly = polynomial_create(field_info_get_int(poly));

    value = 1;
    polynomial_set_coef(poly, 0, &value);

    value = 2;
    polynomial_set_coef(poly, 1, &value);

    value = 3;
    polynomial_set_coef(poly, 2, &value);

    //Должно быть P(x) = 3x^2 + 2x + 1

    polynomial_get_coef(poly, 0, &result);
    passed = passed && (result == 1);

    polynomial_get_coef(poly, 1, &result);
    passed = passed && (result == 2);

    polynomial_get_coef(poly, 2, &result);
    passed = passed && (result == 3);

    //проверим коэф за пределами (0)
    polynomial_get_coef(poly, 17, &result);
    passed = passed && (result == 0);

    polynomial_destroy(poly);

    test_print_result("test_set_get_coef_int", passed);
}

static void test_set_get_coef_complex(void)
{
    Polynomial* poly;
    Complex value, result;
    int passed;

    poly = polynomial_create(field_info_get_complex(poly));

    value = (Complex){3, 4};
    polynomial_set_coef(poly, 0, &value);

    value = (Complex){1, 2};
    polynomial_set_coef(poly, 1, &value);
    //Должны получить P(x) = (1 + 2i)x + (3 + 4i)

    polynomial_get_coef(poly, 0, &result);
    passed = passed && (complex_equal(result, (Complex){3, 4}));

    polynomial_get_coef(poly, 1, &result);
    passed = passed && (complex_equal(result, (Complex){1, 2}));

    polynomial_destroy(poly);

    test_print_result("test_set_get_coef_complex", passed);
}

static void test_polynomial_add_int(void)
{
    Polynomial *a, *b, *result;
    int coef;
    int passed = 1;

    a = polynomial_create(field_info_get_int(a));
    b = polynomial_create(field_info_get_int(b));
    result = polynomial_create(field_info_get_int(result));

    // a = x + 2 | b = 3x + 4 | a + b = 4x + 5
    coef = 1;
    polynomial_set_coef(a, 0, &coef);

    coef = 2;
    polynomial_set_coef(a, 1, &coef);

    coef = 3;
    polynomial_set_coef(b, 0, &coef);

    coef = 4;
    polynomial_set_coef(b, 1, &coef);

    polynomial_add(result, a, b);

    polynomial_get_coef(result, 0, &coef);
    passed = passed && (coef == 4);

    polynomial_get_coef(result, 1, &coef);
    passed = passed && (coef == 5);

    polynomial_destroy(a);
    polynomial_destroy(b);
    polynomial_destroy(result);

    test_print_result("test_polynomial_add", passed);
}

static void test_polynomial_complex(void)
{
    Polynomial *a, *b, *result;
    Complex coef;
    int passed = 1;

    a = polynomial_create(field_info_get_complex(a));
    b = polynomial_create(field_info_get_complex(b));
    result = polynomial_create(field_info_get_complex(result));

    coef = (Complex){1, 2};
    polynomial_set_coef(a, 1, &coef);

    coef = (Complex){3, 4};
    polynomial_set_coef(b, 1, &coef);
    // (1 + 2i)x + (3 + 4i)x = 4 + 5i

    polynomial_add(result, a, b);

    polynomial_get_coef(result, 0, &coef);
    passed = passed && (complex_equal(coef, (Complex){4,5}));

    polynomial_destroy(a);
    polynomial_destroy(b);
    polynomial_destroy(result);

    test_print_result("test_polynomial_add_complex", passed);
}

static void test_polynomial_mult_int(void)
{
    Polynomial *a, *b, *result;
    int coef;
    int passed = 1;

    a = polynomial_create(field_info_get_int(a));
    b = polynomial_create(field_info_get_int(b));
    result = polynomial_create(field_info_get_int(result));

    // a = x + 1 | b = x + 2 | a * b = x^2 + 3x + 2
    coef = 1;
    polynomial_set_coef(a, 0, &coef);
    polynomial_set_coef(a, 1, &coef);

    coef = 2;
    polynomial_set_coef(b, 0, &coef);

    coef = 1;
    polynomial_set_coef(b, 1, &coef);

    polynomial_mult(result, a, b);

    polynomial_get_coef(result, 0, &coef);
    passed = passed && (coef == 1);

    polynomial_get_coef(result, 1, &coef);
    passed = passed && (coef == 3);

    polynomial_get_coef(result, 2, &coef);
    passed = passed && (coef == 2);

    polynomial_destroy(a);
    polynomial_destroy(b);
    polynomial_destroy(result);

    test_print_result("test_polynomial_mult_int", passed);
}

static void test_polynomial_mult_complex(void)
{
    Polynomial *a, *b, *result;
    Complex coef;
    int passed = 1;

    a = polynomial_create(field_info_get_complex(a));
    b = polynomial_create(field_info_get_complex(b));
    result = polynomial_create(field_info_get_complex(result));

    //(1 + 2i)x * (3 + 4i)x
    coef = (Complex){1, 2};
    polynomial_set_coef(a, 1, &coef);

    coef = (Complex){3,4};
    polynomial_set_coef(b, 1, &coef);

    polynomial_mult(result, a, b);
    //-5x^2 + 10x^2i
}

static void test_polynomial_mult_scal_int(void)
{
    Polynomial *poly;
    int coef, scalar;
    int passed = 1;

    poly = polynomial_create(field_info_get_int(poly));

    // P(x) = 2x + 3 | scalar = 2 | P(x) * scalar = 4x + 6
    coef = 3;
    polynomial_set_coef(poly, 0, &coef);

    coef = 2;
    polynomial_set_coef(poly, 1, &coef);

    scalar = 2;
    polynomial_mult_scal(poly, &scalar);

    polynomial_get_coef(poly, 0, &coef);
    passed = passed && (coef == 6);

    polynomial_get_coef(poly, 1, &coef);
    passed = passed && (coef == 4);

    polynomial_destroy(poly);

    test_print_result("test_polynomial_mult_scal", passed);
}

static void test_polynomial_evalute_int(void)
{
    Polynomial *poly;
    int a, coef, result;
    int passed = 1;

    poly = polynomial_create(field_info_get_int(poly));

    //P(x) = 2x^2 +3x + 1, P(2) = 2*(2)^2 + 3*2 + 1 = 15
    coef = 1;
    polynomial_set_coef(poly, 0, &coef);

    coef = 3;
    polynomial_set_coef(poly, 1, &coef);

    coef = 2;
    polynomial_set_coef(poly, 2, &coef);

    a = 2;
    polynomial_evaluate(poly, &a, &result);

    passed = passed && (result == 15);

    polynomial_destroy(poly);

    test_print_result("test_polynomial_evalute_int", passed);
}

static void test_polynomial_null_checks(void)
{
    Polynomial *poly;
    int passed = 1;

    passed = passed && (polynomial_create(NULL) == NULL);

    poly = polynomial_create(field_info_get_int(poly));

    passed = passed && (polynomial_set_coef(NULL, 0, &poly) != 0);
    passed = passed && (polynomial_get_coef(poly, 0, NULL) != 0);

    polynomial_destroy(poly);

    test_print_result("test_polynomial_null_checks", passed);
}

static void test_polynomial_type_mismatch(void)
{
    Polynomial *int_poly, *complex_poly, *result;
    int passed = 1;

    int_poly = polynomial_create(field_info_get_int(int_poly));
    complex_poly = polynomial_create(field_info_get_complex(complex_poly));
    result = polynomial_create(field_info_get_int(result));

    passed = passed && (polynomial_add(result, int_poly, complex_poly) != 0);

    polynomial_destroy(int_poly);
    polynomial_destroy(complex_poly);
    polynomial_destroy(result);

    test_print_result("test_polynomial_type_mismatch", passed);
}

void tests_run_all(void)
{
    printf("=== Запуск тестов для Вариант 15 (Многочлен) ===\\n\\n");


}