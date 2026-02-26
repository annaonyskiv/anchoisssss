#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// отримати прізвище автора
static void GetLastName(const char* fullName, char* lastName) {
    const char* p = strrchr(fullName, ' ');
    if (p) strcpy(lastName, p + 1);
    else strcpy(lastName, fullName);
}

// вставка у відсортований список за прізвищем
Book* InsertSortedByLastName(Book* head, Book data) {
    Book* node = (Book*)malloc(sizeof(Book));
    if (!node) return head;
    *node = data;
    node->pNext = NULL;

    char newLast[50];
    GetLastName(data.author, newLast);

    if (!head) return node;

    char headLast[50];
    GetLastName(head->author, headLast);

    if (strcmp(newLast, headLast) < 0) {
        node->pNext = head;
        return node;
    }

    Book* cur = head;
    while (cur->pNext) {
        char nextLast[50];
        GetLastName(cur->pNext->author, nextLast);
        if (strcmp(newLast, nextLast) < 0) break;
        cur = cur->pNext;
    }

    node->pNext = cur->pNext;
    cur->pNext = node;
    return head;
}

// вставка Фоменка в кінець
Book* InsertAtEnd(Book* head, Book data) {
    Book* node = (Book*)malloc(sizeof(Book));
    if (!node) return head;
    *node = data;
    node->pNext = NULL;

    if (!head) return node;  

    Book* cur = head;
    while (cur->pNext) cur = cur->pNext; 
    cur->pNext = node;
    return head;
}

// видалення прізвищ авторів з прізвищем на A, B, V, G
Book* DeleteByInitial(Book* head) {
    char last[50];

    while (head) {
        GetLastName(head->author, last);
        char c = last[0];
        if (c == 'A' || c == 'B' || c == 'V' || c == 'G') {
            Book* t = head;
            head = head->pNext;
            free(t);
        }
        else break;
    }

    Book* cur = head;
    while (cur && cur->pNext) {
        GetLastName(cur->pNext->author, last);
        char c = last[0];
        if (c == 'A' || c == 'B' || c == 'V' || c == 'G') {
            Book* t = cur->pNext;
            cur->pNext = t->pNext;
            free(t);
        }
        else cur = cur->pNext;
    }

    return head;
}

// зчитування з файлу
void ReadFromFile(Book** ppHead, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line[256];
    Book temp ; {};

    while (fgets(line, sizeof(line), f)) {
        if (strlen(line) < 3) continue;
        line[strcspn(line, "\n")] = 0;

        char* token = strtok(line, ",");
        if (!token) continue;
        strcpy(temp.author, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(temp.name, token);

        token = strtok(NULL, ",");
        temp.year = atoi(token);

        token = strtok(NULL, ",");
        temp.pages = atoi(token);

        token = strtok(NULL, ",");
        temp.price = atof(token);

        *ppHead = InsertSortedByLastName(*ppHead, temp);
    }

    fclose(f);
}

void PrintList(Book* head) {
    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
    printf("| Author                    | Title                                    | Year | Pages | Price  |\n");
    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");

    while (head) {
        printf("| %-25s | %-40s | %4d | %5d | %6.2lf |\n",
            head->author, head->name, head->year, head->pages, head->price);
        head = head->pNext;
    }

    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
}

// звільнення пам’яті
void FreeList(Book** ppHead) {
    Book* cur = *ppHead;
    while (cur) {
        Book* tmp = cur;
        cur = cur->pNext;
        free(tmp);
    }
    *ppHead = NULL;
}

// обчислення середньої ціни
double AveragePrice(Book* head) {
    double sum = 0;
    int count = 0;
    while (head) {
        sum += head->price;
        count++;
        head = head->pNext;
    }
    return sum / count ;
}

// формування списку книг нижче середньої ціни та сортування
Book* BuildBelowAverageList(Book* head, double avg) {
    Book* newHead = NULL;
    while (head) {
        if (head->price < avg) {
            newHead = InsertSortedByLastName(newHead, *head);
        }
        head = head->pNext;
    }
    return newHead;
}

