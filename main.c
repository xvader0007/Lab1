#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex.h"
#include "field_info.h"
#include "polynomial.h"
#include "tests.h"
#include <windows.h>

void clear_input(void)
{
    int input;
    while ((input = getchar()) != '\n' && input != EOF);
}

Polynomial* create_polynomial_menu(void)
{
    Polynomial *poly = NULL;
    Complex value;
    int choice, coef, coef_count;

    printf("Выберите тип многочлена: \n");
    printf("1) int\n");
    printf("2) complex\n");
    printf("Ввод: ");

    if(scanf("%d", &choice) != 1)
    {
        clear_input();
        printf("\nОшибка ввода!\n");
        return NULL;
    }

    if(choice == 1) poly = polynomial_create(field_info_get_int());
    else if(choice == 2) poly = polynomial_create(field_info_get_complex());
    else
    {
        printf("\nНеверный выбор!\n");
        return NULL;
    }

    if(!poly)
    {
        printf("\nОшибка создания многочлена!\n");
        return NULL;
    }

    printf("Введите количество коэффициентов: ");
    if(scanf("%d", &coef_count) != 1)
    {
        clear_input();
        printf("\nОшибка ввода!\n");
        polynomial_destroy(poly);
        return NULL;
    }

    for(int i = 0; i < coef_count; i++)
    {
        if(choice == 1)
        {
            printf("\nВведите коэффициент: ");
            if(scanf("%d", &coef) != 1)
            {
                clear_input();
                printf("\nОшибка ввода!\n");
                polynomial_destroy(poly);
                return NULL;
            }
            polynomial_set_coef(poly, i, &coef);
        }
        else if(choice == 2)
        {
            printf("\nДействительная часть: ");
            if(scanf("%d", &coef) != 1)
            {
                clear_input();
                printf("\nОшибка ввода!\n");
                polynomial_destroy(poly);
                return NULL;
            }
            value.real_part = coef;

            printf("\nМнимая часть: ");
            if(scanf("%d", &coef) != 1)
            {
                clear_input();
                printf("\nОшибка ввода!\n");
                polynomial_destroy(poly);
                return NULL;
            }
            value.imag_part = coef;

            polynomial_set_coef(poly, i, &value);
        }
    }

    printf("\nМногочлен: ");

    polynomial_print(poly);

    return poly;
}

void add_polynomials(Polynomial *poly1, Polynomial *poly2)
{
    if(!poly1 || !poly2)
    {
        printf("Ошибка - не хватает 1 или 2 многочленов!\n");
        return;
    }

    Polynomial *result = polynomial_create(poly1->type_info);

    if(polynomial_add(result, poly1, poly2) == 0)
    {
        printf("P1 = ");
        polynomial_print(poly1);

        printf("P2 = ");
        polynomial_print(poly2);
        printf("P1 + P2 = ");
        polynomial_print(result);
    }
    else
    {
        printf("Ошибка сложения!\n");
    }

    polynomial_destroy(result);
}

void mult_polynomials(Polynomial *poly1, Polynomial *poly2)
{
    if(!poly1 || !poly2)
    {
        printf("Ошибка - не хватает 1 или 2 многочленов!\n");
        return;
    }

    Polynomial *result = polynomial_create(poly1->type_info);

    if(polynomial_mult(result, poly1, poly2) == 0)
    {
        printf("P1 = ");
        polynomial_print(poly1);

        printf("P2 = ");
        polynomial_print(poly2);
        printf("P1 * P2 = ");
        polynomial_print(result);
    }
    else
    {
        printf("Ошибка умножения!\n");
    }

    polynomial_destroy(result);
}

void mult_scal_menu(Polynomial* poly)
{
    if(!poly)
    {
        printf("Ошибка - нет многочлена!\n");
        return;
    }

    if(poly->type_info == field_info_get_int())
    {
        int scal;

        printf("Введите скаляр (int): ");
        if(scanf("%d", &scal) != 1)
        {
            clear_input();
            printf("Ошибка ввода!\n");
            return;
        }
        polynomial_mult_scal(poly, &scal);
    }
    else
    {
        int scal;
        Complex value;

        printf("Введите скаляр (Complex): \n");
        printf("\nДействительная часть: ");
        if(scanf("%d", &scal) != 1)
        {
            clear_input();
            printf("\nОшибка ввода!\n");
        }
        value.real_part = scal;

        printf("\nМнимая часть: ");
        if(scanf("%d", &scal) != 1)
        {
            clear_input();
            printf("\nОшибка ввода!\n");
        }
        value.imag_part = scal;

        polynomial_mult_scal(poly, &value);
    }

    printf("P * скаляр = ");
    polynomial_print(poly);
}

void evaluate_menu(Polynomial *poly)
{
    if(!poly)
    {
        printf("Ошибка - нет многочлена!\n");
        return;
    }

    printf("P(x) = ");
    polynomial_print(poly);

    if(poly->type_info == field_info_get_int())
    {
        int x, result;
        printf("Введите x: ");
        if(scanf("%d", &x) != 1)
        {
            clear_input();
            printf("Ошибка ввода!\n");
            return;
        }
        if(polynomial_evaluate(poly, &x, &result) == 0) printf("P(%d) = %d\n", x, result);
    }
    else
    {
        int x;
        Complex value, result;
        printf("\nДействительная часть: ");
        if(scanf("%d", &x) != 1)
        {
            clear_input();
            printf("Ошибка ввода!\n");
            return;
        }
        value.real_part = x;

        printf("\nМнимая часть: ");
        if(scanf("%d", &x) != 1)
        {
            clear_input();
            printf("Ошибка ввода!\n");
            return;
        }
        value.imag_part = x;

        if(polynomial_evaluate(poly, &value, &result) == 0)
        {
            printf("P(x) = ");
            complex_print(result);
            printf("\n");
        }
    }
}


void print_menu(void)
{
    printf("_______________________________\n");
    printf("|             Меню            |\n");
    printf("|_____________________________|\n");
    printf("| 1. Создать многочллен P1    |\n");
    printf("| 2. Создать многочлен P2     |\n");
    printf("| 3. Показать многочлен P1    |\n");
    printf("| 4. Показать многочлен P2    |\n");
    printf("| 5. P1 + P2                  |\n");
    printf("| 6. P1 * P2                  |\n");
    printf("| 7. Умножить P1 на скаляр    |\n");
    printf("| 8. Умножить P2 на скаляр    |\n");
    printf("| 9. Вычислить P1(x)          |\n");
    printf("|10. Вычислить P2(x)          |\n");
    printf("|11. Запустить тесты          |\n");
    printf("| 0. Выход                    |\n");
    printf("|_____________________________|\n \n");
}
int main(void)
{
    SetConsoleOutputCP(CP_UTF8);

    Polynomial *poly1 = NULL, *poly2 = NULL;
    int choice;

    printf("___________________________________________\n");
    printf("|   Лабораторная работа №1, Вариант 15    | \n");
    printf("|   Многочлен с поддержкой int и complex  |\n");
    printf("| Студент: Михеев Дмитрий, группа Б25-507 |\n");
    printf("|_________________________________________|\n \n");

    do
    {
        print_menu();
        printf("Ваш выбор: ");

        if(scanf("%d", &choice) != 1)
        {
            clear_input();
            printf("Ошибка ввода!\n\n");
            continue;
        }

        switch(choice)
        {
            case 1:
                if(poly1) polynomial_destroy(poly1);
                poly1 = create_polynomial_menu();
                break;

            case 2:
                if(poly2) polynomial_destroy(poly2);
                poly2 = create_polynomial_menu();
                break;

            case 3:
                if(poly1)
                {
                    printf("P1 = ");
                    polynomial_print(poly1);
                    break;
                }
                else printf("P1 не создан!\n");

            case 4:
                if(poly2)
                {
                    printf("P2 = ");
                    polynomial_print(poly2);
                }
                else printf("P2 не создан!\n");
                break;

            case 5:
                add_polynomials(poly1, poly2);
                break;

            case 6:
                mult_polynomials(poly1, poly2);
                break;

            case 7:
                mult_scal_menu(poly1);
                break;

            case 8:
                mult_scal_menu(poly2);
                break;

            case 9:
                evaluate_menu(poly1);
                break;

            case 10:
                evaluate_menu(poly2);
                break;

            case 11:
                tests_run_all();
                break;

            case 0:
                printf("Выход из программы...\n");
                break;

            default:
                printf("Неверный выбор!\n");
        }
    }
    while(choice != 0);

    if(poly1) polynomial_destroy(poly1);
    if(poly2) polynomial_destroy(poly2);

    return 0;
}
