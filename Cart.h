/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/
# include <Item.h>

// Struct for cart.
typedef struct Cart{
    int id;
    int customer_id;
    Product cart[25];
} Cart;
