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
#include <Order.h>

typedef struct Query
{
	int query_num;
	int user_type;
	Product product;
	Order order;
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

// Customer actions.
Product* getAllProducts();
Product getProductById(int product_id);
Cart getCartByCustomer(int customer_id);
bool addProductToCart(int customer_id);
bool updateProductInCart(int customer_id, Product product);
bool alterPass(int acc_type, int ID, char newPwd[10]);

// Admin actions.
bool addCustomer(Customer r);
bool deleteCustomer(int customer_id);
bool modifyCustomer(Customer n);
bool addProduct(Product product);
bool deleteProduct(int product_id);
bool modifyProduct(Product product);		// set field to -1 when you don't want to update it.
bool generateLog();

// Miscellaneous.
Customer getCustomer(int customer_id);
Admin getAdmin(int ID);
bool checkCustomer(Customer u);
bool checkAdmin(Admin u);

// Utils for connection.
void server(int new_sd);
void *connection(void *nsd);

// bool depositMoney(int acc_type, int ID, float amnt);
// bool withdrawMoney(int acc_type, int ID, float amnt);
// float getBalance(int acc_type, int ID);

