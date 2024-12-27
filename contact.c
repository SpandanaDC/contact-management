#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define HASH_SIZE 26

// Structure for a contact
typedef struct Contact {
    char name[50];
    char phone[15];
    struct Contact *next; // For linked list
} Contact;

// Hash table to store contacts
Contact *hashTable[HASH_SIZE];

// Function to calculate hash index based on the first letter of the name
int hashFunction(char *name) {
    return (toupper(name[0]) - 'A') % HASH_SIZE;
}

// Function to search for a contact
Contact *searchContact(char *name) {
    int index = hashFunction(name);
    Contact *current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to check if a phone number is valid
bool isValidPhone(char *phone) {
    if (strlen(phone) != 10) {
        return false;
    }
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) {
            return false;
        }
    }
    return true;
}

// Function to check if a contact name is unique
bool isUniqueName(char *name) {
    return searchContact(name) == NULL;
}

// Function to create a new contact
Contact *createContact(char *name, char *phone) {
    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->next = NULL;
    return newContact;
}

// Function to insert a contact into the hash table
void insertContact(char *name, char *phone) {
    if (!isValidPhone(phone)) {
        printf("Invalid phone number. Must be 10 digits.\n");
        return;
    }
    if (!isUniqueName(name)) {
        printf("Contact name already exists.\n");
        return;
    }
    
    int index = hashFunction(name);
    Contact *newContact = createContact(name, phone);

    newContact->next = hashTable[index];
    hashTable[index] = newContact;
    printf("Contact added: %s - %s\n", name, phone);
}

// Function to display all contacts in sorted order
void displayContacts() {
    Contact *allContacts[1000];
    int count = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        Contact *current = hashTable[i];
        while (current != NULL) {
            allContacts[count++] = current;
            current = current->next;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(allContacts[j]->name, allContacts[j + 1]->name) > 0) {
                Contact *temp = allContacts[j];
                allContacts[j] = allContacts[j + 1];
                allContacts[j + 1] = temp;
            }
        }
    }

    printf("\nContacts List:\n");
    for (int i = 0; i < count; i++) {
        printf("%s - %s\n", allContacts[i]->name, allContacts[i]->phone);
    }
}

// Function to delete a contact
void deleteContact(char *name) {
    int index = hashFunction(name);
    Contact *current = hashTable[index];
    Contact *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Contact deleted: %s\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Contact not found: %s\n", name);
}

// Main menu for the Contact Management System
void menu() {
    int choice;
    char name[50], phone[15];

    do {
        printf("\nContact Management System\n");
        printf("1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Display Contacts\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Name: ");
                scanf("%s", name);
                printf("Enter Phone: ");
                scanf("%s", phone);
                insertContact(name, phone);
                break;

            case 2:
                printf("Enter Name to Search: ");
                scanf("%s", name);
                Contact *contact = searchContact(name);
                if (contact) {
                    printf("Contact Found: %s - %s\n", contact->name, contact->phone);
                } else {
                    printf("Contact not found.\n");
                }
                break;

            case 3:
                displayContacts();
                break;

            case 4:
                printf("Enter Name to Delete: ");
                scanf("%s", name);
                deleteContact(name);
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);
}

int main() {
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable[i] = NULL;
    }

    menu();
    return 0;
}
