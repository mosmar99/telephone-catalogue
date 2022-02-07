// Laboration 1: Telephone Catalogue (linked-list based solution), Spring 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NOSIZE 10 // all swedish telephone numbers contain 10 digits, "0720-23-13-19"
#define NAMESIZE 20 // length of 20 characters generally suffices for most first names

// A struct which stores contact information, contains name of contact, pointer to the next person
typedef struct Record
{
    char name[NAMESIZE];
    char no[NOSIZE];
    struct Record *next;
} Record;

// global start variable, used to access the linked list telephone catalogue
Record *start = NULL;

// Display the linked list (catalogue) by traversing through all its members
void displayRecords(void) {
    printf("\n ---------Displaying the Telephone Catalogue---------");
    if (start == NULL)
    {
        printf("\n\n ------>EMPTY CATALOGUE<------\n");
    }
    else
    {
        struct Record *ptr;
        ptr = start;
        int i = 1;
        while (ptr != NULL) // prints all the records to console
        {
            printf("\n %d.", i);
            printf("\n Name: %s", ptr->name);
            printf("\n Number: %s", ptr->no);
            puts("");
            ptr = ptr->next;
            i++;
        }
    }
}

// inputs a character array, checks whether or not name exclusively contains english letters
// returns true if it does, otherwise false
bool isValidName(char name[]) {
    for(int i = 0; i < NAMESIZE; i++) {
        int chr = (int) name[i];
        if(chr == 0) {
            break;
        } 
        if((chr < 65 || chr > 90) && chr != 32 && (chr < 97 || chr > 122)) {
            return false;
        }
    }
    return true;
}

// inputs a character array, checks whether or not name exclusively contains digits, 
//     is 10 digit long and starts with "07" (as all Swedish numbers do)
// returns true if it does, otherwise false
bool isValidNo(char no[]) {
    int i;
    for(i = 0; i < NOSIZE && no[i] != '\0'; i++) {
        if(isdigit(no[i]) == 0) {
            return false;
        }
        if(i == 0 && no[i] != '0' || i == 1 && no[i] != '7') {
            return false;
        } 
    }
    if(i < 10) { // every swedish number has 10 digits, starting with 07
        return false;
    }
    return true;
}

// Prints error messages for validity of number function depending on the error
void numberErrMsg(char no[]) {
    int i; bool foundWhichErr = false;
    for(i = 0; i < NOSIZE && no[i] != '\0' && foundWhichErr != true; i++) {
        if(isdigit(no[i]) == 0) {
            fprintf(stderr, " --> Problem: Contains characters not found in the English alphabet\n");
            foundWhichErr = true;
        }
        if(i == 0 && no[i] != '0' || i == 1 && no[i] != '7') {
            fprintf(stderr, " --> Problem: All Swedish numbers begin with a \"07\"\n");
            foundWhichErr = true;
        } 
    }
    if(i < 10 && !foundWhichErr) { // every swedish number has 10 digits, starting with 07
        fprintf(stderr, " --> Problem: All Swedish numbers contain exactly 10 digits\n");
    }
}

// inputs call by reference char pointer, usually an array, fills it with a valid name
void getName(char *name) {
    // get phone number for new contact
    printf(" Enter your contacts name: ");
    fflush(stdin);
    scanf("%[^\n]", name); 

    // check if phone number is valid, 0700000002
    if(isValidName(name)) {
        return;
    } else {
        while(!isValidName(name)) {
            fprintf(stderr, " Please, re-enter a valid name: ");
            fflush(stdin);
            scanf("%[^\n]", name);
        }
        return;
    }        
}

// inputs call by reference char pointer, usually an array, fills it with a valid number
void getPhoneNr(char *no) {
    // get phone number for new contact
    printf(" Enter the phone number of the contact, on format 07XXXXXXXX: ");
    fflush(stdin);
    scanf("%[^\n]", no);

    // check if phone number is valid, 0700000002
    if(isValidNo(no)) {
        return;
    } else {
        while(!isValidNo(no)) {
            numberErrMsg(no);
            fprintf(stderr, " Please, re-enter a valid number, 07XXXXXXXX: ");
            fflush(stdin);
            scanf("%[^\n]", no);
        }
        return;
    }    
}

// creates a new conact (node) with valid name and number, inputs nothing and outputs a pointer to the contact
Record *createNewContact() {
    Record *newContact = NULL;
    char name[NAMESIZE]; 
    char number[NOSIZE];

    printf("\n ---------Creating new Contact---------\n");
        
    // allocate dynamic memory for contact    
    newContact = (Record *) malloc(sizeof(Record));
    
    // return name for new contact
    getName(name); // is now filled with name
    strcpy(newContact->name, name);

    // return telephone number for new contact
    getPhoneNr(number); // is now filled with number
    strcpy(newContact->no, number);

    return newContact;
}

// inputs char and int pointer (usually an array and reference to int variable)
// traverses the catalogue of linked contacts until number is matched, generally returns the previous record to the one searched
// returns NULL if the found node is the first one (flag == 1) or if the directory is empty (flag == 0)
Record *searchByPhone(char *no, int *flag) {
    Record *prevContact = NULL;
    if (start == NULL) {
        // list is empty
        *flag = false;
        return prevContact;
    }
    else
    {

        // set flag, flag = true, contact exists in current catalogue
        Record *ptr = start, *prePtr = start;
        int ctr = 0;
        while(true) { 
            
            if (strcmp(no, ptr->no) == 0) // if string number matches that of a contact
            {
                if (ctr == 0) // if its the first contact, return null and set flag to true (1)
                {
                   *flag = true;
                    prevContact = NULL;
                    return prevContact;
                }
                
                *flag = true;
                prevContact = prePtr;
                return prevContact;
            }
            
            if(ptr->next == NULL) // if last node is hit and contact is not found, return null and set flag to false (0)
            {
                *flag = false;
                prevContact = prePtr;
                return prevContact;
            } 

            prePtr = ptr; // always keeping prePtr pointing one record behind pointer
            ptr = ptr->next;

            ctr++;
        }
    }
}

// isnerts a new contact in the beginning of the catalogue
// the new contact is passed by pointer through the input parameters
// function returns whether or not the insertion of the new contact was successful
// -1 = failed, 0 = successfull
int insertBeg(Record *newContact) {
    int *flag;
    char number[] = {0};
    strcpy(number, newContact->no);
    Record *prevRecord = searchByPhone(number, flag);

    if (*flag == 1) 
    {
        return -1; // node already exists in the directory
    }
    else 
    {
        if (start == NULL) // if the directory is empty
        {
            newContact->next = NULL;
            start = newContact;
        } 
        else // if the directory is not empty
        {
            newContact->next = start;
            start = newContact;
        } 
        
        return 0; // node successfully added to directory
    }
}

// deletes a contact specified by the char *input pointer
// returns whether the operation was successful (0) or failed (-1)
int deleteByPhone(char *no) {
    int *flag;
    Record *prevRecord, *delRecord; // deleted record is not necessary although it makes the code more readiable
    prevRecord = searchByPhone(no, flag);

    if (*flag == 0) // if contact does not exist in directory
    {
        return -1; 
    }
    else // finds and deleted contact
    {
        if (prevRecord == NULL) // deletes the 1st node
        {
            delRecord = start; // saved 1st node
            start = start->next; // start points to 2nd node
            free(delRecord); // free first node
        }

        // prevRecord = (n-1) -> (n) -> null = prevRecord->next->next
        else if (prevRecord->next->next == NULL) // deleting the last node
        {
            delRecord = prevRecord->next;
            free(delRecord);
            prevRecord->next = NULL;
        }

        else // delete node between first and last one
        {
            delRecord = prevRecord->next;
            prevRecord->next = prevRecord->next->next;
            free(delRecord);
        }
    }
        return 0; 
}

// inputs nothing, returns nothins, prints string which demands user to make a binary choice, used in getFlag()
void textPromptGetFlag() {
    // get phone number for new contact
    printf(" Quering the directory to find contact\n");
    printf(" Enter: \"0\" to query catalogue by name\n");
    printf(" Enter: \"1\" to query catalogue by telephone number\n");
    printf(" ?: ");
}

// inputs a char array as input, which is used when retriving a binary answer from user in console
// returns whether or not the character array actually just contains a binary digit or something else
// if something else, it returns false (0), if valid input, then true (1)
bool isValidBinaryInput(char binary[]) {
    for (int i = 0; binary[i] != '\0'; i++) {
        if(i == 1) { // digits are only one character long, so if array contains other input, it is false
            return false; // note: first character (digit) might be correct but not second
        }
        char c = binary[i];
        if (!(binary[i] == '0' || binary[i] == '1')) // if not binary, return false
        {
            return false;
        }
    }
    return true;
}

// inputs nothing, returns either 0 for name, or 1 for telephone number, flag is later passed to QueryDirByNoOrName()
int getFlag() {

    int flag2 = 0; // flag2 to not confuse its name with "flag" in other parts of the code
    char binary[NOSIZE] = {0};

    // get phone number for new contact
    textPromptGetFlag();
    fflush(stdin); 
    scanf("%[^\n]", binary); 

    // check if digit is valid
    if(isValidBinaryInput(binary)) {
        flag2 = strtol(binary, NULL, 10);
        return flag2;
    } 
    else // otherwise demand a valid flag input
    {
        bool loop = true;
        while(loop) {
            fprintf(stderr, "\n You entered invalid input, try again.\n");
            textPromptGetFlag();
            fflush(stdin);
            scanf("%[^\n]", binary);
            if(isValidBinaryInput(binary)) { // loop until a valid flag input is entered
                flag2 = strtol(binary, NULL, 10);
                loop = false;
            }
        }
        return flag2;
    }
}

// inputs char and int pointer (usually an array and reference to int variable)
// traverses the catalogue of linked contacts until name is matched, generally returns the previous record to the one searched
// returns NULL if the found node is the first one (flag == 1) or if the directory is empty (flag == 0)
Record *searchByName(char *name, int *flag) {
    Record *prevContact = NULL;
    if (start == NULL) {
        // list is empty
        *flag = 0;
        return prevContact;
    }
    else
    {

        // set flag, flag = true, contact exists in current catalogue
        Record *ptr = start, *prePtr = start;
        int ctr = 0;
        while(true) { 
            
            if (strcmp(name, ptr->name) == 0) // if string number matches that of a contact
            {
                if (ctr == 0) // if its the first contact, return null and set flag to true (1)
                {
                   *flag = 1;
                    prevContact = NULL;
                    return prevContact;
                } {
                    *flag = 1;
                    prevContact = prePtr;
                    return prevContact;
                }
            }
            
            if(ptr->next == NULL) // if last node is hit and contact is not found, return null and set flag to false (0)
            {
                *flag = 0;
                prevContact = prePtr;
                return prevContact;
            } 

            prePtr = ptr; // always keeping prePtr pointing one record behind pointer
            ptr = ptr->next;

            ctr++;
        }
    }
}

// inputs the queried contact and prints their information to the standard output (console), returns nothing
void printOneRecord(Record *queriedRecord) {
    int tmp = 1;
    fprintf(stdout, "\n %d.", tmp);
    fprintf(stdout, "\n Name: %s", queriedRecord->name);
    fprintf(stdout, "\n Number: %s", queriedRecord->no);
    puts("");
}

// queries the directory by phone number or name
// an option flag that is set to:
// 1 to search by phone number or
// 0 to search by name, the user enters which one they prefer
// return the pointer to the found record, null if not found
Record *QueryDirByNoOrName(int *flag) {
    Record *foundRecord = NULL;

    if(*flag == 0) { // search by name
        int searchFlag; // 1 if the record is found in the list, 0 otherwise
        char name[NAMESIZE]; // stores the name the user searched for
        getName(name); // name is now filled
        Record *prevPtr = searchByName(name, &searchFlag); 
        if(searchFlag == 1) { // if user is found
            if(prevPtr == NULL) { // check if the first element is searched for, remember how searchByName() works
                foundRecord = start;
            } else {
                foundRecord = prevPtr->next; // otherwise return the record
            }
            return foundRecord;
        } else { // if the user is not found in the directory, null is returned
            return foundRecord;
        }
    } 
    else { // search by number
        int searchFlag; // 1 if the record is found in the list, 0 otherwise
        char number[NOSIZE]; // stores the name the user searched for
        getPhoneNr(number); // number is now filled
        Record *prevPtr = searchByPhone(number, &searchFlag);
        if(searchFlag == 1) { // if user is found
            if(prevPtr == NULL) { // check if the first element is searched for, remember how searchByName() works
                foundRecord = start;
            } else {
                foundRecord = prevPtr->next; // otherwise return the record
            }
            return foundRecord;
        } else { // if the user is not found in the directory, null is returned
            return foundRecord;
        }
    }
}

// inputs nothing, prints (outputs) menu option to standard output (console)
void printMenu() {
    printf("\n ======= Menu: Telephone Catalogue =======\n"); 
    printf("\n1. Display Telephone Catalogue");
    printf("\n2. Add new contact at beginning of Catalogue"); // create it, then adds it to the catalogue
    printf("\n3. Delete a contact in Telephone Catalogue (using their number)");
    printf("\n4. Query Catalogue for Contact by name or phone number");
    printf("\n5. Print Main Menu");
    printf("\n6. Quit");
    printf("\n\n?: ");
}

// inputs a menu choice, 1-6, returns nothing, calls on relevant functions to execute choice of the user
void applyChoice(unsigned int choice) {                                                                                              
    int res; // reads whether or not inserions (option 2) and deletions (option 3) were successful or not
    int flag; // prompts user for a digit, 0 for name, 1 for number, in option 4
    char number[NOSIZE]; // stores the number which user is prompted for upon requesting to delete a record in option 4
    Record *queriedRecord = NULL; // stores the queried contact in option 4
    switch (choice)
    {
    case 1:
        // Display Telephone Catalogue
        displayRecords();
        printMenu();
        break;
    case 2:
        // Add new contact
        res = insertBeg(createNewContact());
        if(res == 0) {
            printf(" Contact successfully added.\n");
        } else {
            printf(" Contact already exists in catalogue.\n");
        }
        printMenu();
        break;
    case 3:
        // Delete a contact in Telephone Catalogue
        getPhoneNr(number); // is now filled with number
        res = deleteByPhone(number);
        if(res == 0) {
            printf(" Contact successfully deleted.\n");
        } else {
            printf(" Contact does not exists in catalogue.\n");
        }
        printMenu();
        break;
    case 4:
        flag = getFlag();
        queriedRecord = QueryDirByNoOrName(&flag);
        if(queriedRecord != NULL) {
            printOneRecord(queriedRecord);
        } else {
            printf(" == No Contact with given name or number exists in current Catalogue ==\n");
        }
        printMenu();
        break;
    case 5:
        // Print Main Menu
        printMenu();
        break;
    }
}

int main(void) {

    // some dummy records which facilitate testing
    Record *n1 = (Record *) malloc(sizeof(Record));
    strcpy(n1->name,"Zac");
    strcpy(n1->no,"0700000001");

    Record *n2 = (Record *) malloc(sizeof(Record));
    strcpy(n2->name,"Cody");
    strcpy(n2->no,"0700000002");
    
    Record *n3 = (Record *) malloc(sizeof(Record));
    strcpy(n3->name,"Zed");
    strcpy(n3->no,"0700000003");

    start = n1;
    n1->next = n2;
    n2->next = n3;
    n3->next = NULL;

    printMenu();
    unsigned int choice = 0; 
    while( choice != 6 ) { // loops menu until user specifies that they want to quit
        scanf("%d", &choice);
        if (
        choice == 1 || choice == 2 || choice == 3 || choice == 4 ||
        choice == 5 || choice == 5 || choice == 6) {
            applyChoice(choice);
        }
    }
    system("pause");
    return 0; 
}