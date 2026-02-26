#ifndef LIST_H
#define LIST_H
#include <stdio.h>

typedef struct Book {
    char author[50];
    char name[100];
    int year;
    int pages;
    double price;
    struct Book* pNext;
} Book;

Book* InsertSortedByLastName(Book* head, Book data);
Book* DeleteByInitial(Book* head);
void  ReadFromFile(Book** ppHead, const char* filename);
void  PrintList(Book* head);
void  FreeList(Book** ppHead);
double AveragePrice(Book* head);
Book* BuildBelowAverageList(Book* head, double avg);
Book* AddBookFromKeyboard(Book* head); /* НОВА */

#endif
