/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/
#include <Cart.h>

// Struct for order.
typedef struct Order{
    int order_id;
    int customer_id;
    Cart cart;
} Order;
