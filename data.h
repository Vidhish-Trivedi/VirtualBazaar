#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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

int get_customer();
int get_seller();
int get_admin();
