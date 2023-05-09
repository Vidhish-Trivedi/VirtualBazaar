/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/
#include <unistd.h>
#include "./../header/Product.h"
#include "./../header/my_macros.h"

#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H

void loginMenu();
int authMenu();
void customerMenu();
void adminMenu();
void listProducts(Product p_arr[]);
void listCart(Product p_arr[]);

#endif
