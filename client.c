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
        perror("");
    }

    int option;
    write(1, "1.) List all products\n", sizeof("1.) List all products\n"));
    write(1, "2.) List products in cart\n", sizeof("2.) List products in cart\n"));
    write(1, "3.) Place an order\n", sizeof("3.) Place an order\n"));
    write(1, "4.) Update cart\n", sizeof("4.) Update cart\n"));
    write(1, "5.) Exit\n", sizeof("5.) Exit"));
    while (1)
    {
        scanf("%d", &option);
        if (option == 1)
        {
            int val = 1;
            write(sckfd, &val, 4);
            struct Product p1;
            read(sckfd, &p1, sizeof(struct Product));
            if (p1.id != 0)
            {
                write(1, "--------------------------------------------------------------------------\n", sizeof("--------------------------------------------------------------------------\n"));
                write(1, "ProductId        ProductName        ProductQuantity        Price\n", sizeof("ProductId        ProductName        ProductQuantity        Price\n"));
                printf("%3d %20s %4d %5d\n", p1.id, p1.name, p1.quantity, p1.price); // For formatting.
            }
            while (p1.id != 0)
            {
                read(sckfd, &p1, sizeof(struct Product));
                write(1, "ProductId        ProductName        ProductQuantity        Price\n", sizeof("ProductId        ProductName        ProductQuantity        Price\n"));
                printf("%3d %20s %4d %5d\n", p1.id, p1.name, p1.quantity, p1.price);
            }
        }
        else if (option == 2)
        {
            int val = 2;
            write(sckfd, &val, 4);
            int id;
            write(1, "Enter your customerID:\n=> ", sizeof("Enter your customerID:\n=> "));
            read(0, &id, sizeof(id));
            write(sckfd, &id, 4);
            struct Order or ;
            read(sckfd, & or, sizeof(struct Order));
            if (or.order_id != 0 || or.order_id != -1)
            {
                write(1, "--------------------------------------------\n", sizeof("--------------------------------------------\n"));
                write(1, "OrderID     CustomerID\n", sizeof("OrderID     CustomerID\n"));
                printf(" %4d    %4d\n", or.order_id, or.customer_id);
                struct Order ora[10];
                int idx = 0;
                if (ora[0].customer_id != 0)
                {
                    write(1, "ProductId        ProductName        ProductQuantity        Price\n", sizeof("ProductId        ProductName        ProductQuantity        Price\n"));
                }
                while (ora[idx].order_id != 0)
                {
                    printf("%3d %20s %4d\n", ora[idx].cart->id, ora[idx].cart->name, ora[idx].cart->quantity);
                }
            }
        }
        else if (option == 3)
        {
            int val = 3;
            write(sckfd, &val, 4);
            char name[100];
            int c_id;
            write(1, "Enter your customerID:\n=> ", sizeof("Enter your customerID:\n=> "));
            read(0, &c_id, sizeof(c_id));
            write(sckfd, &c_id, 4);
            write(1, "Enter name of product to order\n=> ", sizeof("Enter name of product to order\n=> "));
            int name_length = read(0, name, sizeof(name));
            write(sckfd, name, name_length);

            // ?
        }
        else if (option == 4)
        {
            int val = 4;
            write(sckfd, &val, 4);
            char name[100];
            write(1, "Enter name of product to modify:\n=> ", sizeof("Enter name of product to modify:\n=> "));
            int name_length = read(0, name, sizeof(name));
            int quantity;
            write(1, "Enter quantity to order (0 to delete from cart):\n=> ", sizeof("Enter quantity to order (0 to delete from cart):\n=> "));
            read(0, &quantity, sizeof(quantity));
            write(sckfd, &quantity, sizeof(quantity));

            // Get response from server.
        }
        else if (option == 5)
        {
            break;
        }
    }
    shutdown(sckfd, 2);
    return (0);
}