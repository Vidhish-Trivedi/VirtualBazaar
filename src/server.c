/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/server.h"
#include "./../header/User.h"
#include "./../header/Admin.h"

/*
	Server side CRUD operations which works analogous to Client side information.
	Check whether file is present or not and take necessary action (locking and unlocking).
	Prepare a response.
	Send the reponse to clien.t
*/

void server(int nsd)
{
	int msgLength, select, type, option, currUserID;
	char psswd[15];
	Query q;

	// Handling requests.
	while (1)
	{
		int is_auth = -1;
		read(nsd, &type, sizeof(type));

		if (type == 1)
		{
			read(nsd, &currUserID, sizeof(currUserID));
			read(nsd, psswd, sizeof(psswd));

			Customer tmp_customer = getCustomer(currUserID);
			printf("f: %s\ne: %s\n", tmp_customer.password, psswd);
			if (strcmp(tmp_customer.password, psswd) == 0)
			{
				is_auth = 0;
			}
			write(nsd, &is_auth, sizeof(is_auth));

			while (type == 1 && !is_auth)
			{
				read(nsd, &type, sizeof(type));
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
					union semun arg;
					key_t key = ftok(".", 'a');
					int semid = semget(key, 1, 0);
					struct sembuf buf = {0, -1, SEM_UNDO};
					int retval = semop(semid, &buf, 1);
					if (retval == -1)
					{
						printf("Error in getting the semaphore\n");
					}
					else
					{
						printf("Have the semaphore\n");
					}
					payment(nsd, q);
					buf.sem_op = 1;
					semop(semid, &buf, 1);
					printf("Semaphore released\n");
				}
				else if (q.query_num == 6)
				{
					write(1, "Logout...\n", sizeof("Logout...\n"));
					is_auth = 0;
					break;
				}
			}
		}

		if (type == 2)
		{
			read(nsd, &currUserID, sizeof(currUserID));
			read(nsd, psswd, sizeof(psswd));

			Admin tmp_admin = getAdmin(currUserID);
			if (strcmp(tmp_admin.password, psswd) == 0)
			{
				is_auth = 0;
			}
			write(nsd, &is_auth, sizeof(is_auth));

			while (type == 2 && !is_auth) // Admin
			{
				read(nsd, &type, sizeof(type));
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
					printf("in 2,4\n");
					generateLog();
					write(nsd, "Generated Log\n", sizeof("Generated Log\n"));
					break;
				}
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
