#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// import replacement
#define BUFFER_SIZE 1000

void successOwner(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

int getAccountFromFileOwner(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}




// account transfer ownership implementation

int findFreeAccountNumber(FILE *ptr, char *userName, struct Record *r){
    int acnum = 1; // first account number for check
    int free = 0; // account number "acnum" is free for user, not used in system
    char *acname; // gap for scanf testing
    
    while (free == 0){ //do while free number not found
        free = 1;
        rewind(ptr); // jump to the beginning of the stream
        while (getAccountFromFileOwner(ptr, acname, r)) {
            if(r->accountNbr == acnum && strcmp(acname, userName) == 0){
                free =  0;
                break;
            }
        }
        if(free == 1){
            break;
        }else{acnum++;}
    }
    if (free == 1){return acnum;}else{return 0;}
}



// to check new owner name is in the system
const char *USERSOWNER = "./data/users.txt";
// to manage accounts
const char *RECORDSOWNER = "./data/records.txt";
// to manage template file
const char *TEMPRECORDSOWNER = "./data/temprecords.tmp";

// the user is registered in the system at the moment
int isUser(char *name)
{
    FILE *fp;
    struct User userChecker;
    if ((fp = fopen(USERSOWNER, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fp, "%i %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, name) == 0)
        {
            fclose(fp);
            return 1; // user  found
        }
    }
    fclose(fp);
    return 0;
}


// update account implementation

// update account

// update user name of account
// void updateAccountOwnerName(struct Record *r, struct User *u){
//     printf("\nOld owner: %s", r->name);
//     strcpy(r->name, u->name);
//     printf("\nNew owner: %s", r->name);
// }


void updateLineOwner(FILE *src, FILE *temp, const int line, struct Record *r, struct User u){
    // printf("\n------r.name is %s", u.name);
    
    char buffer[BUFFER_SIZE];
    int count = 1;
    while ((fgets(buffer, BUFFER_SIZE, src)) != NULL){
        if (line != count){
            fputs(buffer, temp);
        }else{
            char* sline;
            asprintf(&sline, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r->id,
	    r->userId,
	    u.name, // dot because passed by value but not by reference
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
            fputs(sline, temp);
        }
        count++;
    }
}

// "const int line" is line number in file where account data placed
void updateAccountOwner(const int line, struct Record r, struct User u){
   FILE *src;
   FILE *temp;
   char ch;
   src = fopen(RECORDSOWNER, "r");
   temp = fopen(TEMPRECORDSOWNER, "w");
   if (src == NULL || temp == NULL){
      printf("Unable to open file.\n");
      exit(EXIT_FAILURE);
   }
   
   // Move src file pointer to beginning
   rewind(src);
   // Update given line inside file.
   updateLineOwner(src, temp, line, &r, u);
   /* Close all open files */
   fclose(src);
   fclose(temp);
   /* Delete src file and rename temp file as src */
   remove(RECORDSOWNER);
   rename(TEMPRECORDSOWNER, RECORDSOWNER);
}

// transfer ownership upper level function
void transferOwnership(struct User u){
    int acnum = 0; // account number property(structure field)
    int strnum = 0; // number of line of the account data in the file
    int acfound = 0; // boolean simulation
    char userName[100];
    struct Record r; // to manage account data properly

    FILE *pf = fopen(RECORDSOWNER, "r");

    system("clear");
    printf("\t\t====== %s, enter your account number to transfer ownership =====\n\n", u.name);
    scanf("%d",&acnum);

    // checking/searching of requested account among of logged in user accounts
    while (getAccountFromFileOwner(pf, userName, &r))
    {
        strnum++;
        if (strcmp(userName, u.name) == 0 && r.accountNbr == acnum)
        {
            // printf("acnum %d r.accountNbr %d strnum %d\n", acnum, r.accountNbr, strnum);
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            acfound = 1; // account found , break the while loop
            break;
        }
    }
    fclose(pf);
    // check the new user name for account and calculate new account number free for new owner
    if (acfound){
        char newowner[100]; // for desirable name for the new account owner
        nameagain:
        printf("\nEnter the new owner name for your account %d: ", acnum);
        
        fscanf(stdin, "%s", newowner);
        // if no user with given name found then request new name
        if(!isUser(newowner)){
            printf("User %s not found", newowner);
            goto nameagain;
        }

        
        char oldowner[100]; // previous owner of account
        strcpy(oldowner, userName);

        // find free account number, among the owned accounts of new owner
        struct Record rn; // for managing data
        FILE *pf = fopen(RECORDSOWNER, "r");
        int newnum = findFreeAccountNumber(pf, newowner, &rn); //for new owner
        fclose(pf);
        if (newnum){ // new owner is registered user and have free number for new account
            strcpy(u.name, newowner); // u.name used just to not refactor code, r.name can be used too
            r.accountNbr = newnum;
            updateAccountOwner(strnum*2-1, r, u); // to update account data
            strcpy(u.name, oldowner);
            system("clear");
            printf("\naccount %d old owner: %s", acnum, oldowner);
            printf("\naccount %d new owner: %s", acnum, newowner);

            printf("\nAccount number %d was updated!\n", acnum);
        }else{
            printf("User %s do not have free numbers for new account number", newowner);
        }
    }else{
        printf("\nAccount number %d was not found!\n", acnum);
    }
    successOwner(u);
}
