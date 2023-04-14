// Header file for customer.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

int currID;
int option;

typedef struct query
{
    int query_num;
    int val;
    float money;

} query;

typedef struct customer
{
    int id;
    char name[30];
    char password[10];
    char details[100];
} customer;

typedef struct seller
{
    int id;
    char name[30];
    char password[10];
    char details[100];
} seller;

typedef struct admin
{
    int id;
    char uname[30];
    char password[10];

} admin;

void customer_menu(int socket_descriptor);
void seller_menu(int socket_descriptor);
void admin_menu(int socket_descriptor);
void buy_product(int socket_descriptor);
// void order_inquiry(int socket_descriptor);
void change_password(int socket_descriptor);
void view_details(int socket_descriptor);
void add_account(int socket_descriptor);
void delete_account(int socket_descriptor);
void modify_account(int socket_descriptor);
void search_account(int socket_descriptor);
void menu(int socket_descriptor);
void options(int socket_descriptor);
