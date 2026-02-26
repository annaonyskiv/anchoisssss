#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* ==================================================================
   ДОПОМІЖНА ФУНКЦІЯ
   ================================================================== */

/*
 * GetLastName
 * Витягує прізвище автора — останнє слово рядка fullName.
 * lastName повинен мати розмір не менше 50 байт.
 */
static void GetLastName(const char* fullName, char* lastName) {
    const char* p = strrchr(fullName, ' ');
    if (p)  strncpy(lastName, p + 1, 49);
    else    strncpy(lastName, fullName, 49);
    lastName[49] = '\0';
}

/* ==================================================================
   ВСТАВКА У ВІДСОРТОВАНИЙ СПИСОК
   ================================================================== */

/*
 * InsertSortedByLastName
 * Створює новий вузол із даними data та вставляє його у список head
 * у позицію, що зберігає алфавітний порядок за прізвищем автора.
 * Повертає нову голову списку.
 *
 * ЗМІНА: після *node = data одразу обнуляємо node->pNext —
 * поле pNext копіюється разом із структурою і може містити
 * сміттєвий вказівник, що призводить до UB.
 */
Book* InsertSortedByLastName(Book* head, Book data) {
    Book* node = (Book*)malloc(sizeof(Book));
    if (!node) {
        fprintf(stderr, "Error: malloc failed in InsertSortedByLastName\n");
        return head;
    }
    *node = data;
    node->pNext = NULL; /* обнуляємо скопійований вказівник */

    char newLast[50];
    GetLastName(data.author, newLast);

    /* список порожній — новий вузол стає головою */
    if (!head) return node;

    /* вставка перед поточною головою */
    char headLast[50];
    GetLastName(head->author, headLast);
    if (strcmp(newLast, headLast) < 0) {
        node->pNext = head;
        return node;
    }

    /* пошук місця вставки всередині списку */
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

/* ==================================================================
   ВИДАЛЕННЯ АВТОРІВ ЗА ПЕРШОЮ ЛІТЕРОЮ ПРІЗВИЩА
   ================================================================== */

/*
 * DeleteByInitial
 * Видаляє з списку всі вузли, де прізвище автора починається
 * на A (А), B (Б), V (В), G (Г) — транслітерація за правописом.
 * Повертає нову голову списку.
 */
Book* DeleteByInitial(Book* head) {
    char last[50];

    /* Крок 1: видаляємо вузли з голови, поки вони підпадають під умову */
    while (head) {
        GetLastName(head->author, last);
        char c = last[0];
        if (c == 'A' || c == 'B' || c == 'V' || c == 'G') {
            Book* toFree = head;
            head = head->pNext;
            free(toFree);
        } else {
            break;
        }
    }

    /* Крок 2: видаляємо вузли всередині та в хвості списку */
    Book* cur = head;
    while (cur && cur->pNext) {
        GetLastName(cur->pNext->author, last);
        char c = last[0];
        if (c == 'A' || c == 'B' || c == 'V' || c == 'G') {
            Book* toFree = cur->pNext;
            cur->pNext   = toFree->pNext;
            free(toFree);
        } else {
            cur = cur->pNext;
        }
    }

    return head;
}

/* ==================================================================
   ЧИТАННЯ З ФАЙЛУ
   ================================================================== */

/*
 * ReadFromFile
 * Відкриває CSV-файл filename (кома як роздільник),
 * зчитує рядки та додає коректні записи у відсортований список *ppHead.
 *
 * Формат рядка: автор,назва,рік,сторінки,ціна
 *
 * Повертає кількість успішно зчитаних записів,
 * або -1 якщо файл не вдалося відкрити.
 *
 * ЗМІНА: додано повернення коду помилки (-1 / кількість записів),
 * перевірку кожного токена на NULL, strncpy замість strcpy,
 * коректну ініціалізацію temp = {0} та явне temp.pNext = NULL
 * перед вставкою — це усуває UB від сміттєвого вказівника.
 */
int ReadFromFile(Book** ppHead, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        return -1;
    }

    char line[256];
    int  count = 0;
    int  lineNum = 0;

    while (fgets(line, sizeof(line), f)) {
        lineNum++;
        /* пропускаємо порожні рядки */
        if (strlen(line) < 3) continue;
        line[strcspn(line, "\n")] = '\0';

        Book temp = {0}; /* ініціалізуємо всі поля нулями */

        /* --- автор --- */
        char* token = strtok(line, ",");
        if (!token) {
            fprintf(stderr, "Warning: line %d — missing 'author', skipped\n", lineNum);
            continue;
        }
        strncpy(temp.author, token, sizeof(temp.author) - 1);

        /* --- назва --- */
        token = strtok(NULL, ",");
        if (!token) {
            fprintf(stderr, "Warning: line %d — missing 'title', skipped\n", lineNum);
            continue;
        }
        strncpy(temp.name, token, sizeof(temp.name) - 1);

        /* --- рік --- */
        token = strtok(NULL, ",");
        if (!token) {
            fprintf(stderr, "Warning: line %d — missing 'year', skipped\n", lineNum);
            continue;
        }
        temp.year = atoi(token);

        /* --- сторінки --- */
        token = strtok(NULL, ",");
        if (!token) {
            fprintf(stderr, "Warning: line %d — missing 'pages', skipped\n", lineNum);
            continue;
        }
        temp.pages = atoi(token);

        /* --- ціна --- */
        token = strtok(NULL, ",");
        if (!token) {
            fprintf(stderr, "Warning: line %d — missing 'price', skipped\n", lineNum);
            continue;
        }
        temp.price  = atof(token);
        temp.pNext  = NULL;

        *ppHead = InsertSortedByLastName(*ppHead, temp);
        count++;
    }

    fclose(f);
    return count;
}

/* ==================================================================
   ЗАПИС У ФАЙЛ
   ================================================================== */

/*
 * WriteToFile
 * Записує всі книги зі списку head у файл filename у форматі CSV
 * (той самий формат, що й вхідний файл).
 * Один рядок = один запис: автор,назва,рік,сторінки,ціна
 *
 * Повертає кількість записаних рядків, або -1 при помилці відкриття.
 *
 * ЗМІНА: нова функція — забезпечує збереження результату на диск
 * без зміни логіки роботи програми.
 */
int WriteToFile(Book* head, const char* filename) {
    if (!head) {
        fprintf(stderr, "Warning: WriteToFile — list is empty, '%s' will not be written\n",
                filename);
        return 0;
    }

    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: cannot open file '%s' for writing\n", filename);
        return -1;
    }

    int count = 0;
    while (head) {
        fprintf(f, "%s,%s,%d,%d,%.2f\n",
                head->author,
                head->name,
                head->year,
                head->pages,
                head->price);
        count++;
        head = head->pNext;
    }

    fclose(f);
    return count;
}

/* ==================================================================
   ВИВІД ТАБЛИЦІ В КОНСОЛЬ
   ================================================================== */

/*
 * PrintList
 * Виводить список у вигляді форматованої таблиці.
 * Якщо список порожній — виводить відповідне повідомлення.
 *
 * ЗМІНА: додано перевірку порожнього списку з виводом повідомлення —
 * раніше функція мовчки виводила лише рядки таблиці без записів.
 */
void PrintList(Book* head) {
    if (!head) {
        printf("  (list is empty)\n");
        return;
    }

    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
    printf("| Author                    | Title                                    | Year | Pages | Price  |\n");
    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");

    while (head) {
        printf("| %-25s | %-40s | %4d | %5d | %6.2f |\n",
               head->author, head->name,
               head->year,   head->pages, head->price);
        head = head->pNext;
    }

    printf("+---------------------------+------------------------------------------+------+-------+--------+\n");
}

/* ==================================================================
   ЗВІЛЬНЕННЯ ПАМ'ЯТІ
   ================================================================== */

/*
 * FreeList
 * Звільняє всі вузли списку та встановлює голову в NULL.
 */
void FreeList(Book** ppHead) {
    Book* cur = *ppHead;
    while (cur) {
        Book* tmp = cur;
        cur = cur->pNext;
        free(tmp);
    }
    *ppHead = NULL;
}

/* ==================================================================
   СЕРЕДНЯ ЦІНА
   ================================================================== */

/*
 * AveragePrice
 * Повертає середню ціну книг у списку.
 * Повертає 0.0 якщо список порожній (захист від ділення на нуль).
 *
 * ЗМІНА: додано перевірку count > 0 — раніше був ризик ділення на нуль.
 */
double AveragePrice(Book* head) {
    double sum  = 0.0;
    int    count = 0;
    while (head) {
        sum += head->price;
        count++;
        head = head->pNext;
    }
    return (count > 0) ? sum / count : 0.0;
}

/* ==================================================================
   ПОБУДОВА СПИСКУ КНИГ НИЖЧЕ СЕРЕДНЬОЇ ЦІНИ
   ================================================================== */

/*
 * BuildBelowAverageList
 * Перебирає список head, копіює книги з ціною < avg
 * у новий відсортований список і повертає його голову.
 *
 * Вихідний список head не змінюється.
 */
Book* BuildBelowAverageList(Book* head, double avg) {
    Book* newHead = NULL;
    while (head) {
        if (head->price < avg)
            newHead = InsertSortedByLastName(newHead, *head);
        head = head->pNext;
    }
    return newHead;
}

/* ==================================================================
   ВВЕДЕННЯ КНИГИ З КЛАВІАТУРИ
   ================================================================== */

/*
 * AddBookFromKeyboard
 * Зчитує дані нової книги з клавіатури, вставляє запис у список head
 * і повертає нову голову відсортованого списку.
 *
 * ЗМІНА: очищення буфера stdin після scanf запобігає
 * "пожиранню" символу '\n' наступним fgets.
 */
Book* AddBookFromKeyboard(Book* head) {
    Book temp = {0};

    printf("\n--- Add new book ---\n");

    printf("Author (Firstname Lastname): ");
    if (!fgets(temp.author, sizeof(temp.author), stdin)) return head;
    temp.author[strcspn(temp.author, "\n")] = '\0';

    printf("Title: ");
    if (!fgets(temp.name, sizeof(temp.name), stdin)) return head;
    temp.name[strcspn(temp.name, "\n")] = '\0';

    printf("Year: ");
    if (scanf("%d", &temp.year) != 1) return head;

    printf("Pages: ");
    if (scanf("%d", &temp.pages) != 1) return head;

    printf("Price: ");
    if (scanf("%lf", &temp.price) != 1) return head;

    /* очищаємо буфер після scanf перед наступним можливим fgets */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    temp.pNext = NULL;
    head = InsertSortedByLastName(head, temp);
    printf("Book added successfully.\n");
    return head;
}
