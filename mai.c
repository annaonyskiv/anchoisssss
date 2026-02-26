#include <stdio.h>
#include "list.h"

int main() {
    Book* head;
    ListInit(&head);

    // 1?? Зчитування книг з файлу та сортування
    ReadFromFile(&head, "list.txt");
    printf("After reading and sorting from file:\n");
    PrintList(head);

    // 2?? Вставка окремої книги після сортування
    Book newBook = { "Cormac McCarthy", "Child of God", 1973, 208, 14.75, NULL };
    head = InsertSortedByLastName(head, newBook);

    printf("\nAfter inserting separate book:\n");
    PrintList(head);

    // 3?? Видалення авторів на A, B, V, G (по прізвищу)
    head = DeleteByInitial(head);
    printf("\nAfter deleting authors A, B, V, G:\n");
    PrintList(head);

    // 4?? Звільнення пам'яті
    FreeList(&head);
    return 0;
}
