#include "User.h"

#ifndef ADMIN_H
#define ADMIN_H

// Admin
typedef struct Admin
{
	int id;
	char uname[30];
	char password[15];

} Admin;

// Admin actions.
Customer addCustomer(Customer r);
Product addProduct(Product product);
Product deleteProduct(int product_id);
Product modifyProduct(Product product);		// set field to -1 when you don't want to update it.
// bool generateLog();

Admin getAdmin(int ID);

#endif
