#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
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

void saveAccountToFile(FILE *ptr, struct User *u, struct Record *r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r->id,
	    u->id,
	    u->name,
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
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

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

int freshRun = 1; // to fix appearing account report on screen
noAccount:
    if (freshRun) { system("clear"); freshRun = 0; }
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            if (!freshRun) {
                system("clear");
            }
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, &u, &r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}


// code injections
#define BUFFER_SIZE 1000
void deleteLine(FILE *src, FILE *temp, const int line);
void printFile(FILE *fptr);
int findLineNumberToRemove(struct User u);
void removeAccount(const int line);
void removeExistingAccount(struct User u);

const char *TEMPRECORDS = "./data/temprecords.tmp";
// "const int line" is line number in file where account data placed
void removeAccount(const int line){
   // printf("%d account will be removed", line);
   FILE *src;
   FILE *temp;
   char ch;
   src = fopen(RECORDS, "r");
   temp = fopen(TEMPRECORDS, "w");
   if (src == NULL || temp == NULL){
      printf("Unable to open file.\n");
      exit(EXIT_FAILURE);
   }
   // printf("\nFile contents before removing line.\n\n");
   // printFile(src);
   
   // Move src file pointer to beginning
   rewind(src);
   // Delete given line from file.
   deleteLine(src, temp, line);
   /* Close all open files */
   fclose(src);
   fclose(temp);
   /* Delete src file and rename temp file as src */
   remove(RECORDS);
   rename(TEMPRECORDS, RECORDS);
   // printf("\n\n\nFile contents after removing %d line.\n\n", line);
   // Open source file and print its contents
//    src = fopen(RECORDS, "r");
//    printFile(src);
//    fclose(src);
}

void removeExistingAccount(struct User u){
    int acnum = 0;
    int strnum = 0;
    int acfound = 0;
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== %s, enter your account number to remove =====\n\n", u.name);
    scanf("%d",&acnum);


    while (getAccountFromFile(pf, userName, &r))
    {
        strnum++;
        if (strcmp(userName, u.name) == 0 && r.accountNbr == acnum)
        {
            // printf("acnum %d r.accountNbr %d strnum %d\n", acnum, r.accountNbr, strnum);
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            acfound = 1;
        }
    }
    fclose(pf);
    if (acfound){
        removeAccount(strnum*2-1); // to remove account
        removeAccount(strnum*2-1); // to remove empty line after account
        printf("Account number %d was removed!\n", acnum);
    }else{
        printf("Account number %d was not found!\n", acnum);
    }
    success(u);
}

// https://www.tutorialspoint.com/c-program-to-remove-a-line-from-the-file#

int rawmain(){
   FILE *src;
   FILE *temp;
   char ch;
   char path[100];
   int line;
   src=fopen("cprogramming.txt","w");
   printf("enter the text.press cntrl Z:\n");
   while((ch = getchar())!=EOF){
      putc(ch,src);
   }
   fclose(src);
   printf("Enter file path: ");
   scanf("%s", path);
   printf("Enter line number to remove: ");
   scanf("%d", &line);
   src = fopen(path, "r");
   temp = fopen("delete.tmp", "w");
   if (src == NULL || temp == NULL){
      printf("Unable to open file.\n");
      exit(EXIT_FAILURE);
   }
   printf("\nFile contents before removing line.\n\n");
   printFile(src);
   // Move src file pointer to beginning
   rewind(src);
   // Delete given line from file.
   deleteLine(src, temp, line);
   /* Close all open files */
   fclose(src);
   fclose(temp);
   /* Delete src file and rename temp file as src */
   remove(path);
   rename("delete.tmp", path);
   printf("\n\n\nFile contents after removing %d line.\n\n", line);
   // Open source file and print its contents
   src = fopen(path, "r");
   printFile(src);
   fclose(src);
   return 0;
}
void printFile(FILE *fptr){
   char ch;
   while((ch = fgetc(fptr)) != EOF)
   putchar(ch);
}
void deleteLine(FILE *src, FILE *temp, const int line){
   char buffer[BUFFER_SIZE];
   int count = 1;
   while ((fgets(buffer, BUFFER_SIZE, src)) != NULL){
      if (line != count)
         fputs(buffer, temp);
      count++;
   }
}