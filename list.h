#ifndef LIST_H
#define LIST_H

#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Структура книги                                                     */
/* ------------------------------------------------------------------ */
typedef struct Book {
    char   author[50];
    char   name[100];
    int    year;
    int    pages;
    double price;
    struct Book* pNext;
} Book;

/* ------------------------------------------------------------------ */
/*  Прототипи функцій                                                   */
/* ------------------------------------------------------------------ */

/* Вставка книги у список, відсортований за прізвищем автора */
Book* InsertSortedByLastName(Book* head, Book data);

/* Видалення авторів, прізвища яких починаються на A, B, V, G */
Book* DeleteByInitial(Book* head);

/* Зчитування книг з CSV-файлу у відсортований список */
int   ReadFromFile(Book** ppHead, const char* filename);

/* Запис поточного списку у файл result.txt у форматі CSV */
int   WriteToFile(Book* head, const char* filename);

/* Вивід списку у вигляді таблиці в консоль */
void  PrintList(Book* head);

/* Звільнення всієї пам'яті списку */
void  FreeList(Book** ppHead);

/* Обчислення середньої ціни книг у списку */
double AveragePrice(Book* head);

/* Побудова відсортованого списку книг із ціною нижче середньої */
Book* BuildBelowAverageList(Book* head, double avg);

/* Введення нової книги з клавіатури та вставка у список */
Book* AddBookFromKeyboard(Book* head);

#endif /* LIST_H */
