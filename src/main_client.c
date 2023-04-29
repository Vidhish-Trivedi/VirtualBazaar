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
#include "./../header/my_macros.h"
#include "./../header/Query.h"
#include "./../header/client_menu.h"

int main()
{
    struct sockaddr_in server;
    int sckfd;
    sckfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);
    connect(sckfd, (struct sockaddr *)&server, sizeof(server));
    Product prod_ref;
    prod_ref.id = -1;
    strcpy(prod_ref.name, "==");
    prod_ref.price = -1;
    prod_ref.quantity = -1;

    int userType = -1;
    while (userType <= 0)
    {
        loginMenu();
        scanf("%d", &userType);

        if (userType == 1)
        {
            // Customer
            while (1)
            {
                write(sckfd, &userType, sizeof(userType));
                int option;
                customerMenu();

                scanf("%d", &option);
                if (option == 1) // List all products.
                {
                    Product product_array[MAX_PRODUCTS];
                    Query q = {option, 1, prod_ref};
                    write(sckfd, &q, sizeof(Query));
                    read(sckfd, product_array, sizeof(Product) * MAX_PRODUCTS);
                    listProducts(product_array);
                }
                else if (option == 2) // List products in cart
                {
                    Product product_array[MAX_CART_SIZE];

                    // ! TODO: set user id.
                    Query q = {option, 1, prod_ref};
                    write(sckfd, &q, sizeof(Query));
                    read(sckfd, product_array, sizeof(Product) * MAX_CART_SIZE);
                    listCart(product_array);
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

                    // ! TODO: set user id.
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
                        write(1, "Add unsuccessful... cart may be full or product already in cart or product not found!\n", sizeof("Add unsuccessful... cart may be full or product already in cart or product not found!\n"));
                    }
                }
                else if (option == 4) // Update cart (pid, quantity), to remove, set quantity to -1.
                {
                    write(1, "Enter product id to modify in cart:\n", sizeof("Enter product id to modify in cart:\n"));
                    int a, b;
                    scanf("%d", &a);
                    write(1, "Enter new quantity (-1 to remove from cart):\n", sizeof("Enter new quantity (-1 to remove from cart):\n"));
                    scanf("%d", &b);

                    Product p;
                    p.id = a;
                    p.quantity = b;
                    p.price = -1;
                    strcpy(p.name, "==");

                    // ! TODO: set user id.
                    Query q = {4, 1, p};
                    write(sckfd, &q, sizeof(Query));

                    Product res;
                    read(sckfd, &res, sizeof(Product));
                    if (res.id > 0)
                    {
                        printf("Updated product with Id: %d to cart\n", res.id);
                    }
                    else
                    {
                        write(1, "Update unsuccessful... cart may not have that product added!\n", sizeof("Update unsuccessful... cart may not have that product added!\n"));
                    }
                }
                else if (option == 5) // Payment.
                {
                    // Lock PRODUCT_FILE.
                    // Check quantity of all products from PRODUCT_FILE.
                    // Display list of products which can be bought.
                    // Calculate total, display total.
                    // Get confirmation.
                    // If success, empty cart, update quantities in PRODUCT_FILE.
                    // Unlock PRODUCT_FILE.
                    // Generate receipt.

                    // ! TODO: set user id.
                    write(1, "Products in cart available in sufficient quantity are:\n", sizeof("Products in cart available in sufficient quantity are:\n"));
                    Product product_array[MAX_CART_SIZE];

                    Query q = {option, 1, prod_ref};
                    write(sckfd, &q, sizeof(Query));

                    read(sckfd, product_array, sizeof(Product) * MAX_CART_SIZE);
                    
                    listCart(product_array);

                    write(1, "\tPrices may vary from the time you added a product to your cart.\n", sizeof("\tPrices may vary from the time you added a product to your cart.\n"));
                    write(1, "\nOther products in cart which are not available in sufficient quantity:\n", sizeof("\nOther products in cart which are not available in sufficient quantity:\n"));

                    
                    read(sckfd, product_array, sizeof(Product) * MAX_CART_SIZE);
                    
                    listCart(product_array);

                    int cnt_available = 0;
                    read(sckfd, &cnt_available, sizeof(int));

                    if (cnt_available > 0)
                    {
                        int total_cost = 0;
                        read(sckfd, &total_cost, sizeof(int));
                        printf("\tTotal amount to pay: %5d\n", total_cost);

                        write(1, "Enter the amount displayed above to confirm:\n", sizeof("Enter the amount displayed above to confirm:\n"));
                        int cost_confirm = -1;
                        scanf("%d", &cost_confirm);
                        write(sckfd, &cost_confirm, sizeof(int));

                        char buf[50];
                        int ret = read(sckfd, buf, sizeof(buf));
                        write(1, buf, ret);
                    }
                    else
                    {
                        char buf[75];
                        int ret = read(sckfd, buf, sizeof(buf));
                        write(1, buf, ret);
                    }
                }
                else if (option == 6) // Exit
                {
                    Query q = {-1, -1, prod_ref};
                    write(sckfd, &q, sizeof(Query));
                    userType = -1;
                    break;
                }
                else
                {
                    Query q = {-1, -1, prod_ref};
                    write(sckfd, &q, sizeof(Query));
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
                adminMenu();

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

                    Query q = {1, 1, p};

                    write(sckfd, &q, sizeof(Query));

                    Product res;
                    read(sckfd, &res, sizeof(Product));
                    if (res.id > 0)
                    {
                        printf("Added product with Id: %d\n", res.id);
                    }
                    else
                    {
                        write(1, "Add unsuccessful... product may already exist, try updating it!\n", sizeof("Add unsuccessful... product may already exist, try updating it!\n"));
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
                    Query q = {4, 2, prod_ref};
                    write(sckfd, &q, sizeof(Query));
                    char buf[200];
                    int ret = read(sckfd, buf, sizeof(buf));
                    write(1, buf, ret);
                    userType = -1;
                    break;
                }
                else
                {
                    Query q = {-1, -1, prod_ref};
                    write(sckfd, &q, sizeof(Query));
                    write(1, "Invalid Choice... Try again!\n", sizeof("Invalid Choice... Try again!\n"));
                }
            }
        }
        else if (userType == 3)
        {
            // Exit application
            Query q = {-1, -1, prod_ref};
            userType = -1;
            write(sckfd, &q, sizeof(Query));
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