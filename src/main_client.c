/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

/* Client */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "./../header/Order.h"
#include "./../header/my_macros.h"

typedef struct Query
{
    int query_num;
    int user_type;
    Product product;
    Order order;
} Query;

int main()
{
    struct sockaddr_in server;
    int sckfd;
    sckfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5000);
    connect(sckfd, (struct sockaddr *)&server, sizeof(server));

    int userType = -1;
    while (userType <= 0)
    {
        write(1, "----------------Login Menu----------------\n", sizeof("----------------Login Menu----------------\n"));
        write(1, "1.) Customer\n", sizeof("1.) Customer\n"));
        write(1, "2.) Admin\n", sizeof("2.) Admin\n"));
        write(1, "3.) Exit application\n", sizeof("3.) Exit application\n"));

        scanf("%d", &userType);

        if (userType == 1)
        {
            // Customer
            while (1)
            {
                write(sckfd, &userType, sizeof(userType));
                int option;
                write(1, "1.) List all products\n", sizeof("1.) List all products\n"));
                write(1, "2.) List products in cart\n", sizeof("2.) List products in cart\n"));
                write(1, "3.) Add to cart\n", sizeof("3.) Add to cart\n"));
                write(1, "4.) Update cart\n", sizeof("4.) Update cart\n")); // set updated quantity to -1 to remove.
                write(1, "5.) Exit\n", sizeof("5.) Exit\n"));

                scanf("%d", &option);
                write(sckfd, &option, sizeof(option));
                if (option == 1) // List all products.
                {
                    Product product_array[MAX_PRODUCTS];
                    read(sckfd, product_array, sizeof(Product) * MAX_PRODUCTS);
                    write(1, "--------------------------------------------------------------------\n", sizeof("--------------------------------------------------------------------\n"));
                    write(1, "ProductId        ProductName        Cost        QuantityAvailable\n", sizeof("ProductId        ProductName        Cost        QuantityAvailable\n"));
                    for (int idx = 0; idx < MAX_PRODUCTS; idx++)
                    {
                        if (product_array[idx].id > 0 && product_array[idx].id < MAX_PRODUCTS + 1 && product_array[idx].quantity >= 0)
                        {
                            printf("%3d\t %20s\t %5d\t   %4d\n", product_array[idx].id, product_array[idx].name, product_array[idx].quantity, product_array[idx].price); // For formatting.
                        }
                    }
                }
                else if (option == 2) // List products in cart
                {
                    Product product_array[MAX_CART_SIZE];
                    read(sckfd, product_array, sizeof(Product) * MAX_CART_SIZE);
                    write(1, "--------------------------------------------------------------------\n", sizeof("--------------------------------------------------------------------\n"));
                    write(1, "ProductId        ProductName        Cost        QuantityAvailable\n", sizeof("ProductId        ProductName        Cost        QuantityAvailable\n"));
                    for (int idx = 0; idx < 25; idx++)
                    {
                        if (product_array[idx].id > 0 && product_array[idx].id < MAX_PRODUCTS + 1 && product_array[idx].quantity >= 0)
                        {
                            printf("%3d\t %20s\t %5d\t   %4d\n", product_array[idx].id, product_array[idx].name, product_array[idx].quantity, product_array[idx].price); // For formatting.
                        }
                    }
                }
                else if (option == 3) // Add to cart (pid, quantity)
                {
                    write(1, "Enter p_id, quantity of product to add to cart as space-separated values:\n", sizeof("Enter p_id, quantity of product to add to cart as space-separated values:\n"));
                    int a, b;
                    scanf("%d %d", &a, &b);
                    Product p;
                    p.id = a;
                    p.quantity = b;
                    strcpy(p.name, "==");
                    p.price = -1;

                    // Set user_id to query here.

                    Query q = {3, 1, p};
                    write(sckfd, &q, sizeof(Query));

                    Product res;
                    read(sckfd, &res, sizeof(Product));
                    if (res.id > 0)
                    {
                        printf("Added product with Id: %d to cart\n", res.id);
                    }
                    else
                    {
                        write(1, "Add unsuccessful... cart may be full or product not found!\n", sizeof("Add unsuccessful... cart may be full or product not found!\n"));
                    }
                }
                else if (option == 4) // Update cart
                {
                }
                else if (option == 5) // Exit
                {
                    userType = -1;
                    break;
                }
                else
                {
                    write(1, "Invalid Choice... Try again!\n", sizeof("Invalid Choice... Try again!\n"));
                }
            }
        }
        else if (userType == 2)
        {
            // Admin
            while (1)
            {
                write(sckfd, &userType, sizeof(userType));

                int option;
                write(1, "1.) Add a product\n", sizeof("1.) Add a product\n"));
                write(1, "2.) Delete a product\n", sizeof("2.) Delete a product\n"));
                write(1, "3.) Update product details\n", sizeof("3.) Update product details\n"));
                write(1, "4.) Exit\n", sizeof("4.) Exit\n"));

                scanf("%d", &option);
                if (option == 1) // Add a product
                {
                    int a, b, c;
                    char buf[100];

                    write(1, "Enter p_id, p_name, price and quantity as space-separated values:\n", sizeof("Enter p_id, p_name, price and quantity as space-seperated values:\n"));
                    scanf("%d %s %d %d", &a, buf, &b, &c);

                    Product p;
                    p.id = a;
                    strcpy(p.name, buf);
                    p.price = b;
                    p.quantity = c;

                    Query q = {1, 2, p};

                    write(sckfd, &q, sizeof(Query));

                    Product res;
                    read(sckfd, &res, sizeof(Product));
                    if (res.id > 0)
                    {
                        printf("Added product with Id: %d\n", res.id);
                    }
                    else
                    {
                        write(1, "Add unsuccessful... try again!\n", sizeof("Add unsuccessful... try again!\n"));
                    }
                }
                else if (option == 2) // Delete a product
                {
                    int a;

                    write(1, "Enter p_id of product to delete:\n", sizeof("Enter p_id of product to delete:\n"));
                    scanf("%d", &a);

                    Product p;
                    p.id = a;

                    Query q = {2, 2, p};
                    write(sckfd, &q, sizeof(Query));

                    bool res;
                    read(sckfd, &p, sizeof(Product));
                    if (p.id > 0)
                    {
                        printf("Deleted product %s, with Id: %d\n", p.name, p.id);
                    }
                    else
                    {
                        write(1, "Delete unsuccessful... try again!\n", sizeof("Delete unsuccessful... try again!\n"));
                    }
                }
                else if (option == 3) // Update product details.
                {
                    int a, b, c;

                    write(1, "Enter p_id of product to update:\n", sizeof("Enter p_id of product to update:\n"));
                    scanf("%d", &a);
                    write(1, "Enter new price(enter -1 for no change):\n", sizeof("Enter new price(enter -1 for no change):\n"));
                    scanf("%d", &b);
                    write(1, "Enter new quantity(enter -1 for no change):\n", sizeof("Enter new quantity(enter -1 for no change):\n"));
                    scanf("%d", &c);

                    Product p;
                    p.id = a;
                    p.price = b;
                    p.quantity = c;

                    Query q = {3, 2, p};
                    write(sckfd, &q, sizeof(Query));

                    bool res;
                    read(sckfd, &p, sizeof(Product));
                    if (p.id > 0)
                    {
                        printf("Updated product %s, with Id: %d\nNew price: %d, new quantity: %d\n", p.name, p.id, p.price, p.quantity);
                    }
                    else
                    {
                        write(1, "Update unsuccessful... try again!\n", sizeof("Update unsuccessful... try again!\n"));
                    }
                }
                else if (option == 4) // Exit, generate product stock log.
                {
                    // ! TODO: Generate log file.
                    userType = -1;
                    break;
                }
                else
                {
                    write(1, "Invalid Choice... Try again!\n", sizeof("Invalid Choice... Try again!\n"));
                }
            }
        }
        else if (userType == 3)
        {
            // Exit application
            write(1, "Bye Bye!\n", sizeof("Bye Bye!\n"));
            break;
        }
        else
        {
            // Invalid
            userType = -1;
            write(1, "Invalid Choice of user... Try again!\n", sizeof("Invalid Choice... Try again!\n"));
        }
    }

    close(sckfd);
    return (0);
}