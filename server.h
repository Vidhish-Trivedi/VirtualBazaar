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

#include "Order.h"

typedef struct Query
{
	int query_num;
	int user_id;
	Product product;
} Query;

// Customer --> User
typedef struct Customer
{
	int id;
	char name[30];
	char password[10];
	Product cart[25];
	char details[50];
} Customer;

// Admin
typedef struct Admin
{
	int id;
	char uname[30];
	char password[15];

} Admin;

// Customer actions.
Product* getAllProducts(Product p_arr[]);
Product getProductById(int product_id);
Product* getCartByCustomer(int customer_id);
bool addProductToCart(int product_id, int customer_id, int quantity);
bool updateProductInCart(int customer_id, Product product);		// 0 on success.

// Admin actions.
bool addCustomer(Customer r);
bool addProduct(Product product);
Product deleteProduct(int product_id);
Product modifyProduct(Product product);		// set field to -1 when you don't want to update it.
// bool generateLog();

// Miscellaneous.
Customer getCustomer(int customer_id);
Admin getAdmin(int ID);

// Utils for connection.
void server(int new_sd);
void *connection(void *nsd);
