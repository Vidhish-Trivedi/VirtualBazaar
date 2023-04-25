/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/client_menu.h"

void loginMenu()
{
    write(1, "----------------Login Menu----------------\n", sizeof("----------------Login Menu----------------\n"));
    write(1, "1.) Customer\n", sizeof("1.) Customer\n"));
    write(1, "2.) Admin\n", sizeof("2.) Admin\n"));
    write(1, "3.) Exit application\n", sizeof("3.) Exit application\n"));
}

void customerMenu()
{
    write(1, "1.) List all products\n", sizeof("1.) List all products\n"));
    write(1, "2.) List products in cart\n", sizeof("2.) List products in cart\n"));
    write(1, "3.) Add to cart\n", sizeof("3.) Add to cart\n"));
    write(1, "4.) Update cart\n", sizeof("4.) Update cart\n")); // set updated quantity to -1 to remove.
    write(1, "5.) Payment\n", sizeof("5.) Payment\n"));
    write(1, "6.) Exit\n", sizeof("6.) Exit\n"));
}

void adminMenu()
{
    write(1, "1.) Add a product\n", sizeof("1.) Add a product\n"));
    write(1, "2.) Delete a product\n", sizeof("2.) Delete a product\n"));
    write(1, "3.) Update product details\n", sizeof("3.) Update product details\n"));
    write(1, "4.) Exit\n", sizeof("4.) Exit\n"));
}
