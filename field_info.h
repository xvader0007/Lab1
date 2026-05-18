#ifndef LAB1_FIELD_INFO_H
#define LAB1_FIELD_INFO_H

#include <stddef.h>

typedef struct
{
    size_t size_el;
    void *(*clone)(void*); //для копирования
    void (*free)(void*); //для освобождения памяти
    void (*print)(void*); //вывод
    int (*equal)(void*, void*); //сравнение

    //операции кольца
    void (*add)(void *a, void *b, void *result);
    void (*mult)(void *a, void *b, void *result);
    void (*mult_scal)(void *a, void *scalar);
    void (*zero)(void *a);
    void (*one)(void *a);
    int (*is_zero)(void *a);
} Field_info;

Field_info *field_info_get_int(void);
Field_info *field_info_get_complex(void);
int field_info_compare(Field_info *a, Field_info *b); //сравнение типов

#endif //LAB1_FIELD_INFO_H
