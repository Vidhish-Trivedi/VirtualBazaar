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

void listProducts(Product product_array[])
{
    write(1, "\t-----------------------------------------------------------------------------------\n", sizeof("\t-----------------------------------------------------------------------------------\n"));
    write(1, "\t| ProductId\t\tProductName\t\t Cost\t\tQuantityAvailable |\n", sizeof("\t| ProductId\t\tProductName\t\tCost\t\tQuantityAvailable |\n"));
    for (int idx = 0; idx < MAX_PRODUCTS; idx++)
    {
        if (product_array[idx].id > 0 && product_array[idx].id < MAX_PRODUCTS + 1 && product_array[idx].quantity > 0)
        {
            printf("\t| %9d\t\t%11s\t\t%5d\t\t%17d |\n", product_array[idx].id, product_array[idx].name, product_array[idx].price, product_array[idx].quantity); // For formatting.
        }
    }
    write(1, "\t-----------------------------------------------------------------------------------\n", sizeof("\t-----------------------------------------------------------------------------------\n"));
}

void listCart(Product product_array[])
{
    write(1, "\t-----------------------------------------------------------------------------------\n", sizeof("\t-----------------------------------------------------------------------------------\n"));
    write(1, "\t| ProductId\t\tProductName\t\t Cost\t\t    QuantityAdded |\n", sizeof("\t| ProductId\t\tProductName\t\tCost\t\tQuantityAvailable |\n"));
    for (int idx = 0; idx < MAX_CART_SIZE; idx++)
    {
        if (product_array[idx].id > 0 && product_array[idx].id < MAX_PRODUCTS + 1 && product_array[idx].quantity > 0)
        {
            printf("\t| %9d\t\t%11s\t\t%5d\t\t%17d |\n", product_array[idx].id, product_array[idx].name, product_array[idx].price, product_array[idx].quantity); // For formatting.
        }
    }
    write(1, "\t-----------------------------------------------------------------------------------\n", sizeof("\t-----------------------------------------------------------------------------------\n"));
}
