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
#include "my_macros.h"

#include "Order.h"

#ifndef SERVER_H

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
	char password[15];
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
Product* getCartByCustomer(int customer_id, Product p_arr[]);
Product addProductToCart(Product product, int customer_id);
Product updateProductInCart(Product product, int customer_id);
void payment(int nsd, Query q);

// Admin actions.
Customer addCustomer(Customer r);
Product addProduct(Product product);
Product deleteProduct(int product_id);
Product modifyProduct(Product product);		// set field to -1 when you don't want to update it.
// bool generateLog();

// Miscellaneous.
Customer getCustomer(int customer_id);
Admin getAdmin(int ID);

// Utils for connection.
void server(int new_sd);
void *connection(void *nsd);

#endif

