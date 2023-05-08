/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include "my_macros.h"
#include <time.h>

#include "Query.h"
#ifndef USER_H
#define USER_H


// Customer --> User
typedef struct Customer
{
	int id;
	char name[30];
	char password[15];
	Product cart[25];
	char details[50];
} Customer;

// Customer actions.
Product* getAllProducts(Product p_arr[]);
Product getProductById(int product_id);
Product* getCartByCustomer(int customer_id, Product p_arr[]);
Product addProductToCart(Product product, int customer_id);
Product updateProductInCart(Product product, int customer_id);
void payment(int nsd, Query q);
int generateReciept(Query q, int total, Product p_arr[]);

// Miscellaneous.
Customer getCustomer(int customer_id);

#endif
