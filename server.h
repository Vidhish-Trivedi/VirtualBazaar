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

typedef struct Query
{
	int query_num;
	int val;
	float money;

} Query;

typedef struct Customer
{
	int id;
	char name[30];
	char password[10];
	char details[100];
} Customer;

typedef struct Seller
{
	int id;
	char name[30];
	char password[10];
	char details[100];
} Seller;

typedef struct Admin
{
	int id;
	char uname[30];
	char password[10];

} Admin;

Customer getNormalUser(int ID);
Seller getJointUser(int ID);
Admin getAdmin(int ID);
bool checkNormalUser(Customer u);
bool checkJointUser(Seller u);
bool checkAdmin(Admin u);
bool depositMoney(int acc_type, int ID, float amnt);
bool withdrawMoney(int acc_type, int ID, float amnt);
float getBalance(int acc_type, int ID);
bool alterPass(int acc_type, int ID, char newPwd[10]);
bool addNormalUser(Customer r);
bool addJointUser(Seller j);
bool deleteNormalUser(int ID);
bool deleteJointUser(int ID);
bool modifyNormalUser(Customer n);
bool modifyJointUser(Seller j);
void server(int new_sd);
void *connection(void *nsd);
