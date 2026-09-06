#include "header.h"

const char *RECORDS = "./data/records.txt"; // route to records address

int getAccountFromFile(FILE *ptr, char name[50],
                       struct Record *r) // ptr for pointer to an open file
                                         // (gets a previously opened file)
// structifies all data in a struct record
// opens, gets a file, reads all fields of an account in a struct record
{
  return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                // file to program info reading
                &r->id, &r->userId, name, &r->accountNbr, &r->deposit.month,
                &r->deposit.day, &r->deposit.year, r->country, r->phone,
                &r->amount, r->accountType) == 11;
}

int isValidAccountType(char accountType[]) {
  if (strcmp(accountType, "saving") == 0 ||
    strcmp(accountType, "current") == 0 ||
    strcmp(accountType, "fixed01") == 0 ||
    strcmp(accountType, "fixed02") == 0 ||
    strcmp(accountType, "fixed03") == 0)
{
    return 1;
}
return 0;
}

int isValidPhone(char phone[]) {
    if (strlen(phone) != 10) {
        return 0;
    }

    for (int i = 0; phone[i] != '\0'; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }

    return 1;
}
// for all data modifications such as update info, transaction, transfer, remove
void writeRecordToFile(FILE *file, const char userName[], struct Record r){
  fprintf(file,
            "%d %d %s %d %d/%d/%d %s %s %.2lf %s\n\n",
            r.id,
            r.userId,
            userName,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

// for saving data from scratch, new accounts.
void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
  r.userId = u.id;
  writeRecordToFile(ptr, u.name, r);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
  int option;
  if (notGood == 0) {
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
      exit(EXIT_SUCCESS);
    else {
      printf("Insert a valid operation!\n");
      goto invalid;
    }
  } else {
    printf("\nEnter 1 to go to the main menu and 0 to exit:");
    scanf("%d", &option);
  }
  if (option == 1) {
    system("clear");
    mainMenu(u);
  } else {
    system("clear");
    exit(EXIT_SUCCESS);
  }
}

void waitForEnter(void) {
  printf("Press enter to continue");
  getchar();
  getchar();
}

void success(struct User u) {
  int option;
  printf("\n✔ Success!\n\n");
invalid:
  printf("Enter 1 to go to the main menu and 0 to exit!\n");
  scanf("%d", &option);
  system("clear");
  if (option == 1) {
    mainMenu(u);
  } else if (option == 0) {
    exit(EXIT_SUCCESS);
  } else {
    printf("Insert a valid operation!\n");
    goto invalid;
  }
}


/*
cosas a mejorar:
- r.id no tiene valor asignado
- accountType no se valida
- phone como int es discutible
- usa goto
*/
void createNewAcc(struct User u) {
  struct Record r;  // new account creation
  struct Record cr; // temporary record to compare with existing accounts
  char userName[50];

  int lastId = -1;
  int accountAvailable = 0;
  int validPhone = 0;
  int validType = 0;

  FILE *pf = fopen(RECORDS, "a+");

  if (pf == NULL) {
    printf("Error opening records file\n");
    exit(EXIT_FAILURE);
  }

  system("clear");
  printf("\t\t\t===== New record =====\n");

  while (!accountAvailable) {
    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d",
          &r.deposit.month,
          &r.deposit.day,
          &r.deposit.year);

    int validAccountNbr = 0;

    while (!validAccountNbr) {
      printf("\nEnter the account number:");

      if (scanf("%d", &r.accountNbr) == 1) {
        validAccountNbr = 1;
      } else {
        printf("Invalid account number. Please enter numbers only.\n");

        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        } // discard all useless chars until enter
      }
    }

    accountAvailable = 1;

    lastId = -1;
    rewind(pf);

    while (getAccountFromFile(pf, userName, &cr)) {
      lastId = cr.id;

      if (strcmp(userName, u.name) == 0 &&
          cr.accountNbr == r.accountNbr) {
        accountAvailable = 0;

        printf("✖ This account already exists for this user\n\n");
        waitForEnter();

        break;
      }
    }
  }

  r.id = lastId + 1;

  printf("\nEnter the country:");
  scanf("%s", r.country);

  while (!validPhone) {
    printf("\nEnter the phone number:");
    scanf("%s", r.phone);

    if (isValidPhone(r.phone)) {
      validPhone = 1;
    } else {
      printf("Invalid phone number. Please enter exactly 10 digits.\n");
    }
  }

  printf("\nEnter amount to deposit: $");
  scanf("%lf", &r.amount);

  while (!validType) {
    printf("\nChoose the type of account:\n");
    printf("\t-> saving\n");
    printf("\t-> current\n");
    printf("\t-> fixed01(for 1 year)\n");
    printf("\t-> fixed02(for 2 years)\n");
    printf("\t-> fixed03(for 3 years)\n");

    printf("\nEnter your choice:");
    scanf("%s", r.accountType);

    if (isValidAccountType(r.accountType)) {
      validType = 1;
    } else {
      printf("Invalid account type. Please try again.\n");
    }
  }

  saveAccountToFile(pf, u, r);

  fclose(pf);
  success(u);
}

void updateAccount(struct User u) {
  struct Record r; // current reading account on each loop
  char userName[100];

  int accountNbr; // user input
  int option;     // choose between phone or country
  int found = 0;  // finding flag
  int validPhone = 0;

  system("clear");
  printf("\t\t\t===== Update account =====\n");

  printf("\nEnter the account number you want to update: ");
  scanf("%d", &accountNbr);

  FILE *oldFile = fopen(RECORDS, "r"); // reading only records.txt

  if (oldFile == NULL) {
    printf("Error opening records file\n");
    exit(1);
  }

  FILE *tempFile = fopen("./data/temp.txt",
                         "w"); // edit version, new file with modified info

  if (tempFile == NULL) {
    printf("Error creating temporary file\n");
    fclose(oldFile);
    exit(1);
  }

  while (getAccountFromFile(oldFile, userName, &r)) {
    if (strcmp(userName, u.name) == 0 &&
        r.accountNbr ==
            accountNbr) // if username matches with logged user, and the account
                        // number matches with the asked to be modified account
    {
      found = 1;
      int validOption = 0;

      while (!validOption) {
        printf("\nWhat do you want to update?\n");
        printf("1. Country\n");
        printf("2. Phone number\n");

        scanf("%d", &option);

        switch (option) {
        case 1:
          printf("Enter new country: ");
          scanf("%s", r.country);
          validOption = 1;
          break;

        case 2:
        while(!validPhone) {
          printf("Enter new phone number: ");
          scanf("%s", r.phone);
        
          if (isValidPhone(r.phone)) {
            validPhone = 1;
          } else {
            printf("Invalid phone number. Please enter exactly 10 digits.\n");
          }
        }
          validOption = 1;
          break;

        default:
          printf("Invalid option\n");
          break;
        }
      }
    }
    writeRecordToFile(tempFile, userName, r);
  }
  fclose(oldFile);
  fclose(tempFile);

  if (found == 0) {
    remove("./data/temp.txt");
    printf("Account not found\n");
    waitForEnter();
    return;
  }
  remove("./data/records.txt");
  rename("./data/temp.txt", "./data/records.txt");

  success(u);
}

void checkAccountDetails(struct User u) {
  struct Record r;
  char userName[100];

  int accountNbr;
  int found = 0;

  FILE *file = fopen(RECORDS, "r"); // no need to write or edit in this func

  if (file == NULL) 
  {
    printf("Error! opening file");
    exit(1);
  }

  system("clear");

  printf("Enter the account number to see details: ");
  scanf("%d", &accountNbr);
  
while (getAccountFromFile(file, userName, &r))
 {
  if(strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
  {
    found = 1;
    double interestRate = 0.0;

    printf("_____________________\n");
    printf("\nAccount number: %d\n"
       "Deposit Date: %d/%d/%d\n"
       "Country: %s\n"
       "Phone number: %s\n"
       "Amount deposited: $%.2f\n"
       "Type Of Account: %s\n",
       r.accountNbr,
       r.deposit.day,
       r.deposit.month,
       r.deposit.year,
       r.country,
       r.phone,
       r.amount,
       r.accountType);

       if (strcmp(r.accountType, "current") == 0) {
        printf("You will not get interests because the account is of type current\n");
       }
       else if (strcmp(r.accountType, "saving") == 0) {
        interestRate = 0.07;
       }
       else if (strcmp(r.accountType, "fixed01") == 0) {
        interestRate = 0.04;
       }
       else if (strcmp(r.accountType, "fixed02") == 0) {
        interestRate = 0.05;
       }
       else if (strcmp(r.accountType, "fixed03") == 0) {
        interestRate = 0.08;
       }
       else {
        printf("Unknown account type\n");
       }

       if (interestRate > 0)
       {
        double interest = r.amount * interestRate / 12;
        printf("You will get $%.2f as interest on day %d of every month\n", interest, r.deposit.day);
       }
  }
}
fclose(file);
if (found == 0)
{
  printf("Account not found\n");
  waitForEnter();
  return;
}
success(u);
}

void checkAllAccounts(struct User u) {
  char userName[100];
  struct Record r;
  int found = 0;

  FILE *pf = fopen(RECORDS, "r"); // only reading mode

  system("clear"); // same command as if it was typed in termina, just clears
                   // terminal space
  printf("\t\t====== All accounts from user, %s =====\n\n",
         u.name); // shows current user name

  while (getAccountFromFile(pf, userName,
                            &r)) // while the function can read another record
                                 // in records.txt, continue the loop
  // in each loop, r is filled with a different account
  {
    if (strcmp(userName, u.name) == 0) // if the account belongs to u, print
                                       // data, else continue with next record
    {
      found = 1;
      printf("_____________________\n");
      printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone "
             "number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
             r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
             r.country, r.phone, r.amount, r.accountType);
    }
  }
  fclose(pf);
  if (found == 0)
{
  printf("No accounts found\n");
  waitForEnter();
  return;
}
  success(u);
}

void makeTransaction(struct User u) {
  struct Record r;
  char userName[100];

  int accountNbr;
  int option;
  int found = 0;
  double transactionAmount;
  int transactionDone = 0;


  system("clear");
  printf("\t\t\t===== Make transaction =====\n");

  printf("\nEnter the account number to make a transaction: ");
  scanf("%d", &accountNbr);

  FILE *oldFile = fopen(RECORDS, "r"); // reading only records.txt

  if (oldFile == NULL) {
    printf("Error opening records file\n");
    exit(1);
  }

  FILE *tempFile = fopen("./data/temp.txt", "w"); // edit version, new file with modified info

  if (tempFile == NULL) {
    printf("Error creating temporary file\n");
    fclose(oldFile);
    exit(1);
  }  

  while (getAccountFromFile(oldFile, userName, &r)) {
    if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
      found = 1;
      int transactionAllowed = 1;

      if (strcmp(r.accountType, "fixed01") == 0 ||
          strcmp(r.accountType, "fixed02") == 0 ||
          strcmp(r.accountType, "fixed03") == 0)
          {
            transactionAllowed = 0;
            printf("Transactions are not allowed for fixed accounts\n");
          }

      if (transactionAllowed) {
        printf("What transaction do you want to make?\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        // possibility to add money transfer in the future

        scanf("%d", &option);

        switch (option) {
          case 1:
            printf("Enter the amount to deposit: ");
            scanf("%lf", &transactionAmount);
            if (transactionAmount <= 0) {
              printf("invalid amount\n");
              break;
            }
            r.amount = r.amount + transactionAmount;
            transactionDone = 1;
            printf("Your new balance is €%.2lf", r.amount);
            break;

          case 2:
            printf("Enter the amount to withdraw: ");
            scanf("%lf", &transactionAmount);
            if (transactionAmount <= 0) {
              printf("Invalid amount\n");
              break;
            }

            if (transactionAmount > r.amount) {
              printf("Not enough funds\n");
              printf("Your balance is €%.2lf", r.amount);
            } else {
              r.amount = r.amount - transactionAmount;
              transactionDone = 1;
              printf("Transaction successfully made\n");
              printf("Your new balance is €%.2lf", r.amount);
            }
            break;

          default:
            printf("Invalid option\n");
            break;
        }
      }
    }
    writeRecordToFile(tempFile, userName, r);
  }
  fclose(oldFile);
  fclose(tempFile);

  if (found == 0) {
    remove("./data/temp.txt");
    printf("Account not found\n");
    waitForEnter();
    return;
  }

  if (transactionDone == 0) {
    remove("./data/temp.txt");
    printf("\nTransaction was not completed\n");
    waitForEnter();
    return;
  }
  remove("./data/records.txt");
  rename("./data/temp.txt", "./data/records.txt");

  success(u); 
}

void removeExistingAccount (struct User u) {
  struct Record r; 
  char userName[100];

  int accountNbr;
  int found = 0;

  system("clear");
  printf("\t\t\t===== Remove existing account =====\n");

  printf("\nEnter the account number to remove: ");
  scanf("%d", &accountNbr);

  FILE *oldFile = fopen(RECORDS, "r"); // reading only records.txt

  if (oldFile == NULL) {
    printf("Error opening records file\n");
    exit(1);
  }

  FILE *tempFile = fopen("./data/temp.txt", "w"); // edit version, new file with modified info

  if (tempFile == NULL) {
    printf("Error creating temporary file\n");
    fclose(oldFile);
    exit(1);
  }

  while(getAccountFromFile(oldFile, userName, &r)) {
    if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
      found = 1;
      continue;
    }
    writeRecordToFile(tempFile, userName, r);   
  }
  fclose(oldFile);
  fclose(tempFile);

  if (found == 0) {
    remove("./data/temp.txt");
    printf("Account not found\n");
    waitForEnter();
    return;
  }
  remove("./data/records.txt");
  rename("./data/temp.txt", "./data/records.txt");
printf("\nYour account was successfully removed\n");

success(u);
}

void transferOwner(struct User u) {
  struct Record r; 
  char userName[100];
  char newOwnerName[100];

  int accountNbr;
  int userFound = 0;
  int accountFound = 0;

  system("clear");
  printf("\t\t\t===== Transfer an account =====\n");


  printf("\nEnter the new owner's name: ");
  scanf("%s", newOwnerName); // destinatario

  // NEW OWNER'S USERNAME VALIDATION
  FILE *usersFile = fopen("./data/users.txt", "r");
  if (usersFile == NULL) {
    printf("Error opening users file\n");
    exit(1);
  }
struct User userChecker;
int newOwnerId = -1;

while (fscanf(usersFile, "%d %s %s",
              &userChecker.id,
              userChecker.name,
              userChecker.password) == 3)
{
    if (strcmp(userChecker.name, newOwnerName) == 0)
    {
        userFound = 1;
        newOwnerId = userChecker.id;
        break;
    }
}
fclose(usersFile);

if(userFound == 0) {
  printf("Unable to find the username\n");
  waitForEnter();
  return;
}

if (strcmp(newOwnerName, u.name) == 0)
{
    printf("You already own this account\n");
    waitForEnter();
    return;
}

  printf("\nEnter the account number to transfer: ");
  scanf("%d", &accountNbr);
  
  // ACCOUNT'S SEARCH AND TRANSFER
  FILE *oldFile = fopen(RECORDS, "r"); // reading only records.txt

  if (oldFile == NULL) {
    printf("Error opening records file\n");
    exit(1);
  }

  FILE *tempFile = fopen("./data/temp.txt", "w"); // edit version, new file with modified info

  if (tempFile == NULL) {
    printf("Error creating temporary file\n");
    fclose(oldFile);
    exit(1);
  } 

  while (getAccountFromFile(oldFile, userName, &r)) {
    if (strcmp(userName, u.name) == 0 &&
    r.accountNbr == accountNbr) {
      accountFound = 1;
      r.userId = newOwnerId;
      strcpy(userName, newOwnerName);
    }
    writeRecordToFile(tempFile, userName, r);     
  }
  fclose(oldFile);
  fclose(tempFile);

    if (accountFound == 0) {
    remove("./data/temp.txt");
    printf("Account not found\n");
    waitForEnter();
    return;
  }
  remove("./data/records.txt");
  rename("./data/temp.txt", "./data/records.txt");
  printf("\nYour account was successfully transferred to %s\n", newOwnerName);

success(u);
}