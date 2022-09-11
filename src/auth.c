#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

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
    printf("inside getpasswordcode");
    FILE *fp;
    struct User userChecker;
printf("before if fp open auth.c line 42\n");
    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
printf("before while fscanf auth.c line 47\n");
    while (fscanf(fp, "%i %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        printf("before strcomp auth.c line 50\n");
        printf("\nuserChecker.name ");
        printf("%s",userChecker.name);
        printf("\nuserChecker.password ");
        printf("%s",userChecker.password);
        printf("\n");
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            printf("%s", buff);
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}