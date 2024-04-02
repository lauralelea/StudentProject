#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_BOOKS 100
#define MAX_NUM_LOANS 10
#define MAX_NAME_LENGTH 50
#define MAX_TITLE_LENGTH 50
#define MAX_AUTHOR_LENGTH 50

// Structure to represent a book
typedef struct {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int copies_available;
} Book;

// Structure to represent a person
typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    Book loans[MAX_NUM_LOANS];
    int num_loans;
} Person;

// Library structure to hold books and manage operations
typedef struct {
    Book books[MAX_NUM_BOOKS];
    int num_books;
} Library;

// Function prototypes
void login(Person* person, const char* name, const char* surname);
void borrow_books(Person* person, Library* library);
void return_books(Person* person, Library* library);
void view_loans(Person* person);
void search_books(Library* library);
void initialize_library(Library* library);

int main() {
    Person person;
    Library library;
    char choice;

    // Initialize the library with some books
    initialize_library(&library);

    // Simulate login
    login(&person, "John", "Doe");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Borrow books\n");
        printf("2. Return books\n");
        printf("3. View your loans\n");
        printf("4. Search for books\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf_s(" %c", &choice, sizeof(choice));

        switch (choice) {
        case '1':
            borrow_books(&person, &library);
            break;
        case '2':
            return_books(&person, &library);
            break;
        case '3':
            view_loans(&person);
            break;
        case '4':
            search_books(&library);
            break;
        case '5':
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Login function
void login(Person* person, const char* name, const char* surname) {
    strcpy_s(person->name, sizeof(person->name), name);
    strcpy_s(person->surname, sizeof(person->surname), surname);
    person->num_loans = 0;
}

// Borrow books function
void borrow_books(Person* person, Library* library) {
    int i, num_books;
    printf("Enter the number of books to borrow: ");
    scanf_s("%d", &num_books);

    if (person->num_loans + num_books > MAX_NUM_LOANS) {
        printf("You can't borrow more than %d books at a time.\n", MAX_NUM_LOANS);
        return;
    }

    printf("Enter the titles and authors of the books to borrow:\n");
    for (i = 0; i < num_books; i++) {
        printf("Book %d:\n", i + 1);
        printf("Title: ");
        scanf_s("%s", person->loans[person->num_loans + i].title, sizeof(person->loans[person->num_loans + i].title));
        printf("Author: ");
        scanf_s("%s", person->loans[person->num_loans + i].author, sizeof(person->loans[person->num_loans + i].author));

        int j, found = 0;
        for (j = 0; j < library->num_books; j++) {
            if (strcmp(person->loans[person->num_loans + i].title, library->books[j].title) == 0 &&
                strcmp(person->loans[person->num_loans + i].author, library->books[j].author) == 0 &&
                library->books[j].copies_available > 0) {
                found = 1;
                library->books[j].copies_available--;
                break;
            }
        }

        if (!found) {
            printf("Book not found or no copies available: %s by %s\n",
                person->loans[person->num_loans + i].title,
                person->loans[person->num_loans + i].author);
            person->num_loans--;
        }
    }

    person->num_loans += num_books;
}

// Return books function
void return_books(Person* person, Library* library) {
    int i, num_books;
    printf("Enter the number of books to return: ");
    scanf_s("%d", &num_books);

    printf("Enter the titles and authors of the books to return:\n");
    for (i = 0; i < num_books; i++) {
        printf("Book %d:\n", i + 1);
        printf("Title: ");
        char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH];
        scanf_s("%s", title, sizeof(title));
        printf("Author: ");
        scanf_s("%s", author, sizeof(author));

        int j;
        for (j = 0; j < person->num_loans; j++) {
            if (strcmp(title, person->loans[j].title) == 0 && strcmp(author, person->loans[j].author) == 0) {
                int k;
                for (k = 0; k < library->num_books; k++) {
                    if (strcmp(title, library->books[k].title) == 0 && strcmp(author, library->books[k].author) == 0) {
                        library->books[k].copies_available++;
                        break;
                    }
                }
                if (k == library->num_books) {
                    strcpy_s(library->books[library->num_books].title, sizeof(library->books[library->num_books].title), title);
                    strcpy_s(library->books[library->num_books].author, sizeof(library->books[library->num_books].author), author);
                    library->books[library->num_books].copies_available = 1;
                    library->num_books++;
                }
                person->loans[j] = person->loans[person->num_loans - 1];
                person->num_loans--;
                break;
            }
        }
        if (j == person->num_loans)
            printf("Book not found in your loans: %s by %s\n", title, author);
    }
}

// View loans function
void view_loans(Person* person) {
    if (person->num_loans == 0) {
        printf("You haven't borrowed any books.\n");
        return;
    }

    printf("Your current loans:\n");
    int i;
    for (i = 0; i < person->num_loans; i++) {
        printf("%s by %s\n", person->loans[i].title, person->loans[i].author);
    }
}

// Search books function
void search_books(Library* library) {
    char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH];
    printf("Enter the title or author of the book to search: ");
    scanf_s("%s", title, sizeof(title));

    int i, found = 0;
    printf("Search results:\n");
    for (i = 0; i < library->num_books; i++) {
        if (strstr(library->books[i].title, title) || strstr(library->books[i].author, title)) {
            printf("%s by %s - Copies available: %d\n", library->books[i].title,
                library->books[i].author, library->books[i].copies_available);
            found = 1;
        }
    }

    if (!found)
        printf("No matching books found.\n");
}

// Initialize library with some books
void initialize_library(Library* library) {
    strcpy_s(library->books[0].title, sizeof(library->books[0].title), "Book1");
    strcpy_s(library->books[0].author, sizeof(library->books[0].author), "Author1");
    library->books[0].copies_available = 5;

    strcpy_s(library->books[1].title, sizeof(library->books[1].title), "Book2");
    strcpy_s(library->books[1].author, sizeof(library->books[1].author), "Author2");
    library->books[1].copies_available = 3;

    strcpy_s(library->books[2].title, sizeof(library->books[2].title), "Book3");
    strcpy_s(library->books[2].author, sizeof(library->books[2].author), "Author3");
    library->books[2].copies_available = 2;

    library->num_books = 3;
}



