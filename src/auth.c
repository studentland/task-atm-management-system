#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "header.h"

const char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    // printf("inside getpasswordcode");
    FILE *fp;
    struct User userChecker;
    // printf("before if fp open auth.c line 42\n");
    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    // printf("before while fscanf auth.c line 47\n");
    while (fscanf(fp, "%i %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        // printf("before strcomp auth.c line 50\n");
        // printf("\nuserChecker.name ");
        // printf("%s",userChecker.name);
        // printf("\nuserChecker.password ");
        // printf("%s",userChecker.password);
        // printf("\n");
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            // printf("%s", buff);
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

// register new user implementation

int isUserNameUnique(char a[50]){
    // printf("inside getpasswordcode");
    FILE *fp;
    struct User userChecker;
    // printf("before if fp open auth.c line 42\n");
    if ((fp = fopen(USERS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    // printf("before while fscanf auth.c line 47\n");
    while (fscanf(fp, "%i %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        // printf("before strcomp auth.c line 50\n");
        // printf("\nuserChecker.name ");
        // printf("%s",userChecker.name);
        // printf("\nuserChecker.password ");
        // printf("%s",userChecker.password);
        // printf("\n");
        if (strcmp(userChecker.name, a) == 0)
        {
            fclose(fp);
            return 0; // the name is already in database
        }
    }

    fclose(fp);
    return 1; // the name is unique and can be used
}

// register new user menu
void registerMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n");
registerAgain:
    printf("\n\t\t\t\t\t Enter new User Login:");
    scanf("%s", a);

    if (isUserNameUnique(a)){ // name is unique , can use it

    }else{ // name is already in database
        system("clear");
        printf("Login is not unique. Use another\n");
        goto registerAgain;
    }

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    printf("\n\n\t\t\t\tEnter the new password:");
    char pass0[50];
    scanf("%s", pass0);

    // second password attempt , to compare
    printf("\n\t\t\t\tRepeat the new password:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    if (strcmp(pass0,pass) != 0) {
        system("clear");
        printf("Passwords was not identical");
        goto registerAgain;
    }

};