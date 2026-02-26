#ifndef LIST_H
#define LIST_H

typedef struct Book {
    char author[30];
    char name[30];
    int year;
    int pages;
    float price;
    struct Book* pNext;
} Book;

void ListInit(Book** ppHead);
Book* InsertSortedByLastName(Book* head, Book data);
void ReadFromFile(Book** ppHead, const char* filename);
Book* DeleteByInitial(Book* head);
void PrintList(Book* head);
void FreeList(Book** ppHead);

#endif
