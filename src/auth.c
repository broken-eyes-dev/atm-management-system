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

const char *getPassword(struct User u)
/*
recieve user
open users.txt
read each user
search for matching names
if yes, return password
*/
{
  FILE *fp; // refers to an open file
  struct User userChecker;

  if ((fp = fopen("./data/users.txt", "r")) ==
      NULL) // opens the actual file and gives the path thean read
  {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF) {
    if (strcmp(userChecker.name, u.name) ==
        0) // compares both user and userChecker, if ==0 then perfect match
    {
      fclose(fp);                        // if match, closes file
      char *buff = userChecker.password; // returns found password for user
      return buff;
      // it returns internally the matching password in order to compare with
      // user input
    }
  }

  fclose(fp);
  return "no user found";
}