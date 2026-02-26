#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "list.h"

int main() {
    Book* list1 = NULL;
    Book* list2 = NULL;

    ReadFromFile(&list1, "list.txt");
    printf("\n All books sorted by last name\n");
    PrintList(list1);

    // обчислення та вивід середньої ціни
    double avg = AveragePrice(list1);
    printf("Average price of all books: %.2lf\n", avg);

    list2 = BuildBelowAverageList(list1, avg); // книги < середньої, вже відсортовані

    // Вставка нової книги у відсортований список
    Book newBook = { "Anna Onyskiv", " Software Engineering", 2030, 350, 8.50, NULL };
    list2 = InsertSortedByLastName(list2, newBook);

    printf("\n Books with price < average + new book\n");
    PrintList(list2);

    list2 = DeleteByInitial(list2); // видалення авторів A,B,V,G
    printf("\n After deleting authors A, B, V, G\n");
    PrintList(list2);

    FreeList(&list1);
    FreeList(&list2);

    return 0;
}

