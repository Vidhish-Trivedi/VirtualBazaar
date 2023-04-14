#include "customer.h"

// Initial Menu for type of user login
void options(int socket_descriptor)
{

    while (1)
    {

        write(1, "1: Customer User Login\n", sizeof("1: Customer User Login\n"));
        write(1, "2: Seller Account Login\n", sizeof("2: Seller Account Login\n"));
        write(1, "3: Admin Login\n", sizeof("3: Admin Login\n"));

        write(1, "Enter your choice\n", sizeof("Enter your choice\n"));

        int choice;
        scanf("%d", &choice);
        option = choice;

        if (choice == 1)
        {
            customer_menu(socket_descriptor);
            break;
        }
        if (choice == 2)
        {
            seller_menu(socket_descriptor);
            break;
        }
        if (choice == 3)
        {
            admin_menu(socket_descriptor);
            break;
        }
    }
}
///////////////////////////////////////////////////////////////////
// The following 3 functions work on the same basis :-
// Menu takes an user input, sends the input to server side via
// socket descriptor and accepts an response

void customer_menu(int socket_descriptor)
{

    bool res;
    struct customer user;
    write(1, "Enter User ID : ", sizeof("Enter User ID : "));
    scanf("%d", &user.id);
    currID = user.id;
    write(1, "Password : ", sizeof("Password : "));
    scanf("%s", user.password);

    write(socket_descriptor, &option, sizeof(int));
    write(socket_descriptor, &user, sizeof(customer));

    read(socket_descriptor, &res, sizeof(res));

    if (!res)
    {
        write(1, "Invalid Login\n", sizeof("Invalid Login\n"));
        options(socket_descriptor);
    }
    else
    {
        write(1, "Successfully logged in\n", sizeof("Successfully logged in\n"));
    }
    return;
}

// ! TODO
void seller_menu(int socket_descriptor){
}

void admin_menu(int socket_descriptor)
{
    // Same as customer menu
    bool res;
    struct admin user;
    write(1, "Enter User ID : ", sizeof("Enter User ID : "));
    scanf("%d", &user.id);
    currID = user.id;
    write(1, "Password : ", sizeof("Password : "));
    scanf("%s", user.password);

    write(socket_descriptor, &option, sizeof(int));
    write(socket_descriptor, &user, sizeof(customer));

    read(socket_descriptor, &res, sizeof(res));

    if (!res)
    {
        write(1, "Invalid Login\n", sizeof("Invalid Login\n"));
        options(socket_descriptor);
    }
    else
    {
        write(1, "Successfully logged in\n", sizeof("Successfully logged in\n"));
    }
    return;
}

// Standard Menu after successful authentication. A single function catering to all type of users

void menu(int socket_descriptor)
{
    int choice;
    if (option == 1 || option == 2)
    {
        write(1, "1 : Buy Product\n", sizeof("1 : Buy Product\n"));
        write(1, "2 : Change Password\n", sizeof("2 : Change Password\n"));
        write(1, "3 : View Details\n", sizeof("3 : View Details\n"));
        write(1, "4 : Exit\n", sizeof("6 : Exit\n"));

        write(1, "Choose an option : ", sizeof("Choose an option : "));
        scanf("%d", &choice);
        printf("Option : %d\n", choice);

        switch (choice)
        {
        case 1:
            buy_product(socket_descriptor);
            break;
        case 2:
            change_password(socket_descriptor);
            break;
        case 3:
            view_details(socket_descriptor);
            break;
        case 4:
            write(socket_descriptor, &choice, sizeof(int));
            write(1, "Thank you\n", sizeof("Thank you\n"));
            exit(0);
        default:
            write(1, "Invalid option\n", sizeof("Invalid option\n"));
            menu(socket_descriptor);
            break;
        }
    }
    else if (option == 3)
    {
        write(1, "1 : Add Account\n", sizeof("1 : Add Account\n"));
        write(1, "2 : Delete Account\n", sizeof("2 : Delete Account\n"));
        write(1, "3 : Modify Account\n", sizeof("3 : Modify Account\n"));
        write(1, "4 : Search Account\n", sizeof("4 : Search Account\n"));
        write(1, "5 : Exit\n", sizeof("5 : Exit\n"));

        write(1, "Choose an option : ", sizeof("Choose an option : "));
        scanf("%d", &choice);
        printf("Option : %d\n", choice);

        switch (choice)
        {
        case 1:
            add_account(socket_descriptor);
            break;
        case 2:
            delete_account(socket_descriptor);
            break;
        case 3:
            modify_account(socket_descriptor);
            break;
        case 4:
            search_account(socket_descriptor);
            break;
        case 5:
            write(socket_descriptor, &choice, sizeof(int));
            write(1, "Thank you\n", sizeof("Thank you\n"));
            exit(0);
        default:
            write(1, "Invalid option\n", sizeof("Invalid option\n"));
            menu(socket_descriptor);
            break;
        }
    }
}
/////////////////////////////////////////////////////////////////////////

// Basic CRUD operations
// These works in the followin way : Create a Query q -> Send the Query q, the server checks the business logic and send s
// an appropriate response . The client processes the response accordingly

void buy_product(int socket_descriptor)
{
    float to_withdraw;
    query q;
    q.query_num = 2;
    bool res;
    write(1, "Enter money to withdraw\n", sizeof("Enter money to withdraw\n"));
    scanf("%f", &to_withdraw);
    q.money = to_withdraw;
    if (to_withdraw <= 0)
    {
        buy_product(socket_descriptor);
        return;
    }
    write(socket_descriptor, &q, sizeof(query));
    read(socket_descriptor, &res, sizeof(int));
    if (!res)
    {
        write(1, "Some error occured while withdrawing your money\n", sizeof("Some error occured while withdrawing your money\n"));
    }
    else
    {
        write(1, "Successfully withdraw\n", sizeof("Successfully withdraw\n"));
    }
    menu(socket_descriptor);
    return;
}

void change_password(int socket_descriptor)
{
    query q;
    q.query_num = 4;
    char newpass[10];
    bool res;
    write(1, "Enter new password(max 10 char) ", 33);
    scanf("%s", newpass);
    write(socket_descriptor, &q, sizeof(query));
    write(socket_descriptor, newpass, sizeof(newpass));
    read(socket_descriptor, &res, sizeof(res));

    if (!res)
    {
        write(1, "Error changing password !\n\n", 28);
    }
    else
    {
        write(1, "Succesfully changed password\n\n", 31);
    }
    menu(socket_descriptor);
    return;
}
void view_details(int socket_descriptor)
{
    query q;
    q.query_num = 5;
    write(socket_descriptor, &q, sizeof(query));
    customer n;
    switch (option)
    {
    case 1:

        read(socket_descriptor, &n, sizeof(n));
        printf("User ID : %d\n", n.id);
        printf("Name : %s\n", n.name);
        // printf("Account Number : %d\n", n.account_number);
        // printf("Available Balance : Rs.%0.2f\n", n.balance);
        printf("Status : %s\n\n", n.details);
        menu(socket_descriptor);
        break;

    default:
        menu(socket_descriptor);
        break;
    }
    return;
}
void add_account(int socket_descriptor)
{
    query q;
    q.query_num = 1;
    write(socket_descriptor, &q, sizeof(query));
    customer n;
    int type_of_account;
    write(1, "Enter the type of account : 1. Normal 2. Joint\n", 48);
    scanf("%d", &type_of_account);
    write(socket_descriptor, &type_of_account, sizeof(type_of_account));
    switch (type_of_account)
    {
    case 1:

        write(1, "Name of the account holder : ", 30);
        scanf(" %[^\n]", n.name);
        write(1, "Password(max 10 char) : ", 25);
        scanf("%s", n.password);
        write(1, "Initial Deposit : Rs.", 22);
        // scanf("%f", &n.balance);
        write(socket_descriptor, &n, sizeof(n));

        break;

    default:
        break;
    }
    int res;
    read(socket_descriptor, &res, sizeof(res));
    if (!res)
    {
        write(1, "There was an error adding the account\n\n", 40);
    }
    else
    {
        write(1, "Account added successfully\n\n", 29);
    }
    menu(socket_descriptor);
    return;
}
void delete_account(int socket_descriptor)
{
    query q;
    q.query_num = 2;
    int type_of_account, ID;
    bool res;

    write(socket_descriptor, &q, sizeof(query));
    write(1, "Enter the type of account : 1. Customer \n 2. Seller\n", sizeof("Enter the type of account : 1. Customer \n 2. Seller\n"));
    scanf("%d", &type_of_account);
    write(socket_descriptor, &type_of_account, sizeof(type_of_account));
    write(1, "User ID to remove : ??", 23);
    scanf("%d", &ID);
    write(socket_descriptor, &ID, sizeof(ID));
    read(socket_descriptor, &res, sizeof(res));
    if (!res)
    {
        write(1, "Error while deleting account!\n\n", 32);
    }
    else
    {
        write(1, "Account successfully deleted\n\n", 31);
    }
    menu(socket_descriptor);
    return;
}
void modify_account(int socket_descriptor)
{
    query q;
    q.query_num = 3;
    write(socket_descriptor, &q, sizeof(query));
    int type_of_account;

    write(1, "Enter the type of account : 1. Normal 2. Joint\n", 48);
    scanf("%d", &type_of_account);
    write(socket_descriptor, &type_of_account, sizeof(type_of_account));
    customer n;
    switch (type_of_account)
    {
    case 1:

        write(1, "User ID : ", 11);
        scanf("%d", &n.id);
        write(1, "Account number : ", 18);
        // scanf("%d", &n.account_number);
        write(1, "New Name of the account holder : ", 34);
        scanf(" %[^\n]", n.name);
        write(1, "New Password(max 10 char) : ", 29);
        scanf("%s", n.password);
        write(1, "New Initial Deposit : Rs.", 26);
        // scanf("%f", &n.balance);
        write(socket_descriptor, &n, sizeof(n));

        break;

    default:
        break;
    }
    int res;
    read(socket_descriptor, &res, sizeof(res));
    if (!res)
    {
        write(1, "There was an error modifying the account\n\n", 40);
    }
    else
    {
        write(1, "Account modified successfully\n\n", 29);
    }
    menu(socket_descriptor);
    return;
}

void search_account(int socket_descriptor)
{
    query q;
    q.query_num = 4;
    int type_of_account, len;
    bool res;
    write(socket_descriptor, &q, sizeof(query));
    write(1, "Enter the type of account : 1. Normal 2. Joint\n", 48);
    scanf("%d", &type_of_account);
    write(socket_descriptor, &type_of_account, sizeof(type_of_account));
    customer n;
    int ID;
    switch (type_of_account)
    {
    case 1:
        len = 0;
        write(1, "User ID : ", 11);
        scanf("%d", &ID);
        write(socket_descriptor, &ID, sizeof(ID));

        len = read(socket_descriptor, &n, sizeof(n));
        if (len == 0 || n.id != ID)
        {
            write(1, "Please re-check the User ID!!\n\n", 32);
        }
        else
        {
            printf("User ID : %d\n", n.id);
            printf("Name : %s\n", n.name);
            // printf("Account Number : %d\n", n.account_number);
            // printf("Available Balance : Rs.%0.2f\n", n.balance);
            printf("Status : %s\n\n", n.details);
        }
        /* code */
        break;

    default:
        break;
    }
    menu(socket_descriptor);
    return;
}
//////////////////////////////////////////////////////////////////////////////