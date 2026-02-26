#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* ---------- допоміжна: отримати прізвище (останнє слово) ---------- */
static void GetLastName(const char* fullName, char* lastName) {
    const char* p = strrchr(fullName, ' ');
    if (p) strcpy(lastName, p + 1);
    else   strcpy(lastName, fullName);
}

/* ---------- вставка у відсортований список за прізвищем ----------- */
Book* InsertSortedByLastName(Book* head, Book data) {
    Book* node = (Book*)malloc(sizeof(Book));
    if (!node) return head;
    *node = data;
    node->pNext = NULL;   /* обнуляємо вказівник, скопійований з data */

    char newLast[50];
    GetLastName(data.author, newLast);

    /* список порожній */
    if (!head) return node;

    /* вставка перед головою */
    char headLast[50];
    GetLastName(head->author, headLast);
    if (strcmp(newLast, headLast) < 0) {
        node->pNext = head;
        return node;
    }

    /* пошук місця вставки */
    Book* cur = head;
    while (cur->pNext) {
        char nextLast[50];
        GetLastName(cur->pNext->author, nextLast);
        if (strcmp(newLast, nextLast) < 0) break;
        cur = cur->pNext;
    }

    node->pNext = cur->pNext;
    cur->pNext  = node;
    return head;
}

/* ---------- видалення авторів з прізвищем на A, B, V, G ----------- */
Book* DeleteByInitial(Book* head) {
    char last[50];

    /* видаляємо з голови поки підпадають під умову */
    while (head) {
        GetLastName(head->author, last);
        char c = last[0];
        if (c == 'A' || c == 'B' || c == 'V' || c == 'G') {
            Book* t = head;
            head = head->pNext;
            free(t);
        } else break;
    }

    /* видаляємо з середини/хвоста */
    Book* cur = head;
    while (cur && cur->pNext) {
        GetLastName(cur->pNext->author, last);
        char c = last[0];
        if (c == 'A' || c == 'B' || c == 'V' || c == 'G') {
            Book* t = cur->pNext;
            cur->pNext = t->pNext;
            free(t);
        } else cur = cur->pNext;
    }

    return head;
}

/* ---------- зчитування з файлу ------------------------------------ */
void ReadFromFile(Book** ppHead, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line[256];
    Book temp = {0};    /* ВИПРАВЛЕНО: коректна ініціалізація всіх полів */

    while (fgets(line, sizeof(line), f)) {
        if (strlen(line) < 3) continue;
        line[strcspn(line, "\n")] = 0;

        char* token = strtok(line, ",");
        if (!token) continue;
        strncpy(temp.author, token, sizeof(temp.author) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(temp.name, token, sizeof(temp.name) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        temp.year = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        temp.pages = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        temp.price = atof(token);

        temp.pNext = NULL;
        *ppHead = InsertSortedByLastName(*ppHead, temp);
    }

    fclose(f);
}

/* ---------- вивід списку ------------------------------------------ */
void PrintList(Book* head) {
    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
    printf("| Author                    | Title                                    | Year | Pages | Price  |\n");
    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
    while (head) {
        printf("| %-25s | %-40s | %4d | %5d | %6.2f |\n",
               head->author, head->name, head->year, head->pages, head->price);
        head = head->pNext;
    }
    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
}

/* ---------- звільнення пам'яті ------------------------------------ */
void FreeList(Book** ppHead) {
    Book* cur = *ppHead;
    while (cur) {
        Book* tmp = cur;
        cur = cur->pNext;
        free(tmp);
    }
    *ppHead = NULL;
}

/* ---------- середня ціна ------------------------------------------ */
double AveragePrice(Book* head) {
    double sum = 0;
    int count  = 0;
    while (head) {
        sum += head->price;
        count++;
        head = head->pNext;
    }
    /* ВИПРАВЛЕНО: захист від ділення на нуль */
    return (count > 0) ? sum / count : 0.0;
}

/* ---------- список книг дешевших за середню ----------------------- */
Book* BuildBelowAverageList(Book* head, double avg) {
    Book* newHead = NULL;
    while (head) {
        if (head->price < avg)
            newHead = InsertSortedByLastName(newHead, *head);
        head = head->pNext;
    }
    return newHead;
}

/* ---------- НОВА: введення книги з клавіатури --------------------- */
/*
 * Приймає поточну голову списку, зчитує дані з клавіатури,
 * вставляє новий запис у відсортований список і повертає нову голову.
 */
Book* AddBookFromKeyboard(Book* head) {
    Book temp = {0};

    printf("\n--- Add new book ---\n");

    printf("Author (Firstname Lastname): ");
    if (!fgets(temp.author, sizeof(temp.author), stdin)) return head;
    temp.author[strcspn(temp.author, "\n")] = 0;

    printf("Title: ");
    if (!fgets(temp.name, sizeof(temp.name), stdin)) return head;
    temp.name[strcspn(temp.name, "\n")] = 0;

    printf("Year: ");
    if (scanf("%d", &temp.year) != 1) return head;

    printf("Pages: ");
    if (scanf("%d", &temp.pages) != 1) return head;

    printf("Price: ");
    if (scanf("%lf", &temp.price) != 1) return head;

    /* очищаємо буфер після scanf перед наступним fgets */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    temp.pNext = NULL;
    head = InsertSortedByLastName(head, temp);
    printf("Book added successfully.\n");
    return head;
}

