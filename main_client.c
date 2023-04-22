/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

/* Client */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Item.h"

int main()
{
    int sckfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sckfd == -1)
    {
        perror(" ");
    }
    struct sockaddr_in sddr;
    sddr.sin_port = htons(8080);
    sddr.sin_family = AF_INET;
    sddr.sin_addr.s_addr = INADDR_ANY;
    int cntfd = connect(sckfd, (struct sockaddr *)&sddr, sizeof(sddr));
    if (cntfd == -1)
    {
        perror(" ");
    }

    int userType = -1;
    while (userType <= 0)
    {
        write(1, "----------------Login Menu----------------\n", sizeof("----------------Login Menu----------------\n"));
        write(1, "1.) Customer\n", sizeof("1.) Customer\n"));
        write(1, "2.) Admin\n", sizeof("2.) Admin\n"));
        write(1, "3.) Exit application\n", sizeof("3.) Exit application\n"));

        read(0, &userType, sizeof(userType));

        if (userType == 1)
        {
            // Customer
            while (1)
            {
                int option;
                write(1, "1.) List all products\n", sizeof("1.) List all products\n"));
                write(1, "2.) List products in cart\n", sizeof("2.) List products in cart\n"));
                write(1, "3.) Add to cart\n", sizeof("3.) Add to cart\n"));
                write(1, "4.) Update cart\n", sizeof("4.) Update cart\n"));
                write(1, "5.) Exit\n", sizeof("5.) Exit"));
                read(0, &option, sizeof(option));
                if (option == 1) // List all products.
                {
                    int val = 1;
                    write(sckfd, &val, 4);
                    Product product_array[100];
                    read(sckfd, product_array, sizeof(product_array));
                    write(1, "ProductId        ProductName        Cost        QuantityAvailable\n", sizeof("ProductId        ProductName        Cost        QuantityAvailable\n"));
                    for (int idx = 0; idx < 100; idx++)
                    {
                        if (product_array[idx].id != 0 && product_array[idx].quantity >= 0)
                        {
                            break;
                        }
                        else
                        {
                            printf("%3d %20s %5d %4d\n", product_array[idx].id, product_array[idx].name, product_array[idx].quantity, product_array[idx].price); // For formatting.
                        }
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
                else{
                    write(1, "Invalid Choice... Try again!\n", sizeof("Invalid Choice... Try again!\n"));
                }
            }
        }
        else if (userType == 2)
        {
            // Admin
            while (1)
            {
                int option;
                write(1, "1.) Add a product\n", sizeof("1.) Add a product\n"));
                write(1, "2.) Delete a product\n", sizeof("2.) Delete a product\n"));
                write(1, "3.) Update product details\n", sizeof("3.) Update product details\n"));
                write(1, "4.) Exit\n", sizeof("4.) Exit\n"));
                read(0, &option, sizeof(option));
                if (option == 1)    // Add a product
                {

                }
                else if(option == 2) // Delete a product
                {

                }
                else if(option == 3) // Update product details.
                {

                }
                else if(option == 4) // Exit, generate product stock log.
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
            write(1, "Invalid Choice... Try again!\n", sizeof("Invalid Choice... Try again!\n"));
        }
    }

    close(sckfd);
    return (0);
}