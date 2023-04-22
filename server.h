/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <Item.h>

typedef struct Query
{
	int query_num;
	Order cart[25];			// If update, then only cart[0] is used.
} Query;

// Customer --> User
typedef struct Customer
{
	int id;
	char name[30];
	char password[10];
	char details[100];
} Customer;

// Admin
typedef struct Admin
{
	int id;
	char uname[30];
	char password[10];

} Admin;

Customer getCustomer(int ID);
Admin getAdmin(int ID);
bool checkCustomer(Customer u);
bool checkAdmin(Admin u);
// bool depositMoney(int acc_type, int ID, float amnt);
// bool withdrawMoney(int acc_type, int ID, float amnt);
// float getBalance(int acc_type, int ID);
bool alterPass(int acc_type, int ID, char newPwd[10]);
bool addCustomer(Customer r);
bool deleteCustomer(int ID);
bool modifyCustomer(Customer n);
void server(int new_sd);
void *connection(void *nsd);
