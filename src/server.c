/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/server.h"
#include "./../header/Query.h"
#include "./../header/Product.h"
#include "./../header/User.h"

/*
	Server side CRUD operations which works analogous to Client side information.
	Check whether file is present or not and take necessary action (locking and unlocking).
	Prepare a response.
	Send the reponse to clien.t
*/

void server(int nsd)
{
	int msgLength, select, type, option, currUserID;
	Query q;

	// Handling requests.
	while (1)
	{
		read(nsd, &type, sizeof(type));

		if (type == 1)
		{
			printf("...customer...\n");
			read(nsd, &q, sizeof(Query));
			if (q.query_num == 1)
			{
				printf("in 1,1\n");
				Product *p_arr = malloc(sizeof(Product) * MAX_PRODUCTS);
				p_arr = getAllProducts(p_arr);
				write(nsd, p_arr, sizeof(Product) * MAX_PRODUCTS);
			}
			else if (q.query_num == 2)
			{
				printf("in 1,2\n");
				Product *p_arr = malloc(sizeof(Product) * MAX_CART_SIZE);
				p_arr = getCartByCustomer(q.user_id, p_arr);
				write(nsd, p_arr, sizeof(Product) * MAX_CART_SIZE);
			}
			else if (q.query_num == 3)
			{
				printf("in 1,3\n");
				Product p;
				p = addProductToCart(q.product, q.user_id);
				write(nsd, &p, sizeof(Product));
			}
			else if (q.query_num == 4)
			{
				printf("in 1,4\n");
				Product p;
				p = updateProductInCart(q.product, q.user_id);
				write(nsd, &p, sizeof(Product));
			}
			else if (q.query_num == 5)
			{
				printf("in 1,5\n");
				payment(nsd, q);
			}
			else if (q.query_num == 6)
			{
			}
		}
		else if (type == 2) // Admin
		{
			printf("...admin...\n");
			read(nsd, &q, sizeof(Query));

			if (q.query_num == 1) // Add
			{
				printf("in 2,1\n");
				Product result;
				result = addProduct(q.product);
				write(nsd, &result, sizeof(Product));
			}
			else if (q.query_num == 2) // Delete
			{
				printf("in 2,2\n");
				Product p;
				p = deleteProduct(q.product.id);
				write(nsd, &p, sizeof(Product));
			}
			else if (q.query_num == 3) // Update
			{
				printf("in 2,3\n");
				Product p;
				p = modifyProduct(q.product);
				write(nsd, &p, sizeof(Product));
			}
			else if (q.query_num == 4)
			{
			}
		}
	}
	close(nsd);
	write(1, "Ended client session.....\n", sizeof("Ended client session.....\n"));
	return;
}

void *connection(void *nsd)
{
	int nsfd = *(int *)nsd;
	server(nsfd);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
