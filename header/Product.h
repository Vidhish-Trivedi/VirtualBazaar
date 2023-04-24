/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

/* Header file for product and order */

# include <stdio.h>

#ifndef PRODUCT_H
#define PRODUCT_H

// Struct for product.
typedef struct Product{
    int id;
    char name[100];
    int price;
    int quantity;
} Product;

#endif
