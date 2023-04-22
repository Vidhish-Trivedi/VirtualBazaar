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
#include "Order.h"

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
                write(1, "4.) Update cart\n", sizeof("4.) Update cart\n"));
                write(1, "5.) Exit\n", sizeof("5.) Exit\n"));

                scanf("%d", &option);
                write(sckfd, &option, sizeof(option));
                if (option == 1) // List all products.
                {
                    Product product_array[30];
                    read(sckfd, product_array, sizeof(product_array));
                    write(1, "--------------------------------------------------------------------\n", sizeof("--------------------------------------------------------------------\n"));
                    write(1, "ProductId        ProductName        Cost        QuantityAvailable\n", sizeof("ProductId        ProductName        Cost        QuantityAvailable\n"));
                    for (int idx = 0; idx < 30; idx++)
                    {
                        if(product_array[idx].id == -10){
                            break;
                        }
                        printf("%3d %20s %5d %4d\n", product_array[idx].id, product_array[idx].name, product_array[idx].quantity, product_array[idx].price); // For formatting.

                        // if (product_array[idx].id < 0)
                        // {
                        //     break;
                        // }
                        // else
                        // {
                        //     printf("%3d %20s %5d %4d\n", product_array[idx].id, product_array[idx].name, product_array[idx].quantity, product_array[idx].price); // For formatting.
                        // }
                    }
                }
                else if (option == 2) // List products in cart
                {
                }
                else if (option == 3) // Add to cart (pid, quantity)
                {
                }
                else if (option == 4) // Update cart
                {
                }
                else if (option == 5) // Exit
                {
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

                    scanf("%d %s %d %d", &a, buf, &b, &c);
                    // Product p = {a, buf, b, c};
                    Product p;
                    p.id = a;
                    strcpy(p.name, buf);
                    p.price = b;
                    p.quantity = c;

                    Query q = {1, 2, p};
                    
                    write(sckfd, &q, sizeof(Query));

                    bool res;
                    read(sckfd, &res, sizeof(bool));
                    if (res)
                    {
                        write(1, "Added product\n", sizeof("Added product\n"));
                        Product r_p;
                        write(1, "ok\n", sizeof("ok\n"));
                        read(sckfd, &r_p, sizeof(Product));
                        printf("%d,\t%s,\t%d,\t%d\n", r_p.id, r_p.name, r_p.price, r_p.quantity);
                    }
                    else
                    {
                        write(1, "Try again!\n", sizeof("Try again!\n"));
                    }
                }
                else if (option == 2) // Delete a product
                {
                    Product p = {1, "mobile", 100, 20};

                    Query q = {2, 2, p};
                    write(sckfd, &q, sizeof(Query));

                    bool res;
                    read(sckfd, &res, sizeof(bool));
                    if (res)
                    {
                        write(1, "Deleted product\n", sizeof("Deleted product\n"));
                    }
                    else
                    {
                        write(1, "Try again!\n", sizeof("Try again!\n"));
                    }
                }
                else if (option == 3) // Update product details.
                {
                }
                else if (option == 4) // Exit, generate product stock log.
                {
                    // ! TODO: Generate log file.
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
            close(sckfd);
            exit(0);
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