#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "list.h"

int main() {
    Book* list1 = NULL;
    Book* list2 = NULL;

    ReadFromFile(&list1, "list.txt");
    printf("\n All books sorted by last name\n");
    PrintList(list1);

    double avg = AveragePrice(list1);
    printf("Average price of all books: %.2f\n", avg);

    list2 = BuildBelowAverageList(list1, avg);

    /* ВИПРАВЛЕНО: іменовані ініціалізатори — не залежать від порядку полів */
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

    /* НОВА функція: додавання з клавіатури */
    list2 = AddBookFromKeyboard(list2);

    printf("\n Books after manual entry\n");
    PrintList(list2);

    list2 = DeleteByInitial(list2);
    printf("\n After deleting authors A, B, V, G\n");
    PrintList(list2);

    FreeList(&list1);
    FreeList(&list2);

    return 0;
}
