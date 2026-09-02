#include "header.h"

void mainMenu(struct User u)
{
    int option;
    int running = 1; // =1 means menu continues to be executed

    while(running){

    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check account details\n");
    printf("\n\t\t[4]- Check list of owned accounts\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccount(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeExistingAccount(u);
        break;
    case 7:
        transferOwner(u);
        break;
    case 8:
        running = 0;
        break;
    default:
        printf("Invalid operation!\n");
    }
}
}

void initMenu(struct User *u)
{
    int r = 0;
    int option;
  
    while (!r)
    {

        system("clear");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");

        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password); // pointer : follow the arrow and enter to name/password field
            if (checkCredentials(u))
            {
                printf("\nLogin successful!\n");
            }
            else
            {
                printf("\nInvalid username or password\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            registerMenu(u->name, u->password);
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
}

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
    // order is important here, there is no simultaneous execution
    // first initMeni task is compelted, then mainMenu task starts
}
