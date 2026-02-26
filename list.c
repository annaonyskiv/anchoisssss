#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Ініціалізація списку
void ListInit(Book** ppHead) {
    *ppHead = NULL;
}

// Отримати прізвище з автора
void GetLastName(const char* fullName, char* lastName, int size) {
    const char* p = strrchr(fullName, ' ');
    if (p)
        strncpy_s(lastName, size, p + 1, _TRUNCATE);
    else
        strncpy_s(lastName, size, fullName, _TRUNCATE);
}

// Вставка у відсортований список по прізвищу
Book* InsertSortedByLastName(Book* head, Book data) {
    Book* node = (Book*)malloc(sizeof(Book));
    if (!node) return head;

    memset(node, 0, sizeof(Book));
    strcpy_s(node->author, sizeof(node->author), data.author);
    strcpy_s(node->name, sizeof(node->name), data.name);
    node->year = data.year;
    node->pages = data.pages;
    node->price = data.price;
    node->pNext = NULL;

    char dataLast[30];
    GetLastName(data.author, dataLast, sizeof(dataLast));

    if (!head) return node;

    char headLast[30];
    GetLastName(head->author, headLast, sizeof(headLast));

    if (strcmp(dataLast, headLast) < 0) {
        node->pNext = head;
        return node;
    }

    Book* cur = head;
    while (cur->pNext) {
        char nextLast[30];
        GetLastName(cur->pNext->author, nextLast, sizeof(nextLast));
        if (strcmp(dataLast, nextLast) < 0)
            break;
        cur = cur->pNext;
    }

    node->pNext = cur->pNext;
    cur->pNext = node;
    return head;
}

// Зчитування книг з файлу і вставка у відсортований список
void ReadFromFile(Book** ppHead, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line[256];
    Book temp;

    while (fgets(line, sizeof(line), f)) {
        if (strlen(line) < 3) continue;
        line[strcspn(line, "\n")] = 0;

        char* token = strtok(line, ",");
        if (!token) continue;
        strcpy_s(temp.author, sizeof(temp.author), token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy_s(temp.name, sizeof(temp.name), token);

        token = strtok(NULL, ",");
        temp.year = atoi(token);

        token = strtok(NULL, ",");
        temp.pages = atoi(token);

        token = strtok(NULL, ",");
        temp.price = (float)atof(token);

        *ppHead = InsertSortedByLastName(*ppHead, temp);
    }
    fclose(f);
}

// Видалення авторів на A, B, V, G
Book* DeleteByInitial(Book* head) {
    while (head &&
        (head->author[0] == 'A' || head->author[0] == 'B' ||
            head->author[0] == 'V' || head->author[0] == 'G')) {
        Book* tmp = head;
        head = head->pNext;
        free(tmp);
    }

    Book* cur = head;
    while (cur && cur->pNext) {
        char first = cur->pNext->author[0];
        if (first == 'A' || first == 'B' || first == 'V' || first == 'G') {
            Book* tmp = cur->pNext;
            cur->pNext = tmp->pNext;
            free(tmp);
        }
        else {
            cur = cur->pNext;
        }
    }
    return head;
}

// Друк списку у компактній таблиці
void PrintList(Book* head) {
    printf("\n| %-12s | %-20s | %-4s | %-5s | %-5s |\n",
        "Author", "Title", "Year", "Pages", "Price");
    printf("-------------------------------------------------------------\n");
    while (head) {
        printf("| %-12s | %-20s | %4d | %5d | %5.2f |\n",
            head->author, head->name, head->year, head->pages, head->price);
        head = head->pNext;
    }
}

// Звільнення пам'яті
void FreeList(Book** ppHead) {
    Book* cur = *ppHead;
    while (cur) {
        Book* tmp = cur;
        cur = cur->pNext;
        free(tmp);
    }
    *ppHead = NULL;
}

