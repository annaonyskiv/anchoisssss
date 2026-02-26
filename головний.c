#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "list.h"

int main(void) {

    Book* list1 = NULL; /* повний відсортований список із файлу   */
    Book* list2 = NULL; /* список книг дешевше за середню ціну    */

    /* ----------------------------------------------------------
       1. Зчитування з файлу
    ---------------------------------------------------------- */
    int readCount = ReadFromFile(&list1, "list.txt");
    if (readCount < 0) {
        fprintf(stderr, "Fatal: could not read input file. Exiting.\n");
        return 1;
    }
    printf("Read %d book(s) from list.txt\n", readCount);

    printf("\n All books sorted by last name\n");
    PrintList(list1);

    /* ----------------------------------------------------------
       2. Середня ціна та побудова відфільтрованого списку
    ---------------------------------------------------------- */
    double avg = AveragePrice(list1);
    printf("Average price of all books: %.2f\n", avg);

    list2 = BuildBelowAverageList(list1, avg);

    if (!list2) {
        printf("Warning: no books below average price — result list is empty.\n");
    }

    /* ----------------------------------------------------------
       3. Вставка нової книги у відсортований список list2
    ---------------------------------------------------------- */
    Book newBook = {
        .author = "Anna Onyskiv",
        .name   = "Software Engineering",
        .year   = 2030,
        .pages  = 350,
        .price  = 8.50,
        .pNext  = NULL
    };
    list2 = InsertSortedByLastName(list2, newBook);

    printf("\n Books with price < average + new book\n");
    PrintList(list2);

    /* ----------------------------------------------------------
       4. Видалення авторів A, B, V, G
    ---------------------------------------------------------- */
    list2 = DeleteByInitial(list2);

    printf("\n After deleting authors A, B, V, G\n");
    PrintList(list2);

    /* ----------------------------------------------------------
       5. Запис результату у файл
    ---------------------------------------------------------- */
    int writeCount = WriteToFile(list2, "result.txt");
    if (writeCount < 0) {
        fprintf(stderr, "Error: failed to write result.txt\n");
    } else {
        printf("\n%d book(s) written to result.txt\n", writeCount);
    }

    /* ----------------------------------------------------------
       6. Звільнення пам'яті
    ---------------------------------------------------------- */
    FreeList(&list1);
    FreeList(&list2);

    return 0;
}
