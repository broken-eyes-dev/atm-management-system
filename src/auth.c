#include "header.h"
#include <termios.h> // unix library to control terminal things

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50]) {
  struct termios oflags, nflags;

  system("clear");
  printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
  scanf("%s", a);

  // disabling echo
  tcgetattr(fileno(stdin), &oflags);
  nflags = oflags;
  nflags.c_lflag &=
      ~ECHO; // deactivates keyboard echo (hide password when typing)
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
  printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
  scanf("%s", pass);

  // restore terminal
  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
};

void registerMenu(char name[50], char password[50]) {
  struct User userChecker;
  int lastId = -1;
  int usernameAvailable = 0; // false, unavailable or not valid

  FILE *fp = fopen("./data/users.txt", "a+");

 

  system("clear");
  printf("\t\t\t===== Register =====\n");

  while (!usernameAvailable) {
    printf("\nChoose your username: ");
    scanf("%s", name);

    rewind(fp);
    lastId = -1;
    usernameAvailable = 1; // true, available

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                  userChecker.password) == 3) {
      lastId = userChecker.id;

      if (strcmp(userChecker.name, name) == 0) {
        printf("username not available\n");
        usernameAvailable = 0;
        break;
      }
    }
  }

  printf("\nCreate your password: ");
  scanf("%s", password);

  int newId = lastId + 1;

  fprintf(fp, "%d %s %s\n", newId, name, password);
  fclose(fp);

  printf("\t\t\t===== Registration completed =====\n");
};

int checkCredentials(struct User *u)
/*
recieve user credentials
open users.txt
read each user
compare username and password
return 1 if credentials match
return 0 otherwise
*/
{
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen("./data/users.txt", "r")) ==
      NULL)
  {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) == 3) {
    if (strcmp(userChecker.name, u->name) ==
        0 && strcmp(userChecker.password, u->password) == 0)
    {
      u->id = userChecker.id;
      fclose(fp);
      return 1;
    }
  }

  fclose(fp);
  return 0;
}