/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/server.h"

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

				// ! TODO: Refactor as a function, will need to pass socket descriptor as argument.

				// Get cart of customer
				Product *p_arr = malloc(sizeof(Product) * MAX_CART_SIZE);
				Product *p_arr_un = malloc(sizeof(Product) * MAX_CART_SIZE);
				p_arr = getCartByCustomer(q.user_id, p_arr);

				// Check quantity of all products in cart, calculate total.
				int total_cost = 0;
				Product p;
				int cnt_available = 0;
				for (int k = 0; k < MAX_CART_SIZE; k++)
				{
					p = getProductById(p_arr[k].id);
					if (p_arr[k].quantity > 0 && p_arr[k].quantity <= p.quantity)
					{
						// Take cost from PRODUCT_FILE.
						// Product price may have been updated since last adding it to cart.
						p_arr_un[k].id = -1;
						p_arr_un[k].price = -1;
						p_arr_un[k].quantity = -10;
						strcpy(p_arr_un[k].name, "==");

						total_cost += p.price;
						cnt_available++;
					}
					else
					{
						p_arr_un[k] = p_arr[k];
						p_arr[k].quantity = -10;
					}
				}
				write(nsd, p_arr, sizeof(Product) * MAX_CART_SIZE);
				write(nsd, p_arr_un, sizeof(Product) * MAX_CART_SIZE);

				write(nsd, &cnt_available, sizeof(int));

				if (cnt_available > 0)
				{
					write(nsd, &total_cost, sizeof(int));
					int cost_confirm = -1;
					read(nsd, &cost_confirm, sizeof(int));
					int is_success = 0;
					if (cost_confirm == total_cost)
					{
						is_success = 1;
						// Update quantities in PRODUCT_FILE, from p_arr for products having quantity > 0;
						// Empty cart of customer in CUSTOMER_FILE.
						for (int k = 0; k < MAX_CART_SIZE; k++)
						{
							if (p_arr[k].quantity > 0)
							{
								// Get data from PRODUCT_FILE.
								p = getProductById(p_arr[k].id);
								// Decrease quantity.
								p.quantity -= p_arr[k].quantity;
								// Update PRODUCT_FILE.
								p = modifyProduct(p);
								// Remove product from cart.
								p.quantity = -10;
								p = updateProductInCart(p, q.user_id);
							}
						}

						write(nsd, "Payment Successful!\n", sizeof("Payment Successful!\n"));
					}
					else
					{
						write(nsd, "Payment Unsuccessful!\n", sizeof("Payment Unsuccessful!\n"));
					}
				}
				else
				{
					write(nsd, "No item in your cart can be bought! Update cart to continue.\n", sizeof("No item in your cart can be bought! Update cart to continue.\n"));
				}
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

// ! TODO: check flow --> add product(admin), list all(customer)  --> maybe socket buffer ?;

void *connection(void *nsd)
{
	int nsfd = *(int *)nsd;
	server(nsfd);
}

Admin getAdmin(int ID)
{
	int i = ID;
	Admin c;
	int fd = open(ADMIN_FILE, O_RDONLY);

	int l1;
	struct flock lock;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Admin);
	lock.l_len = sizeof(Admin);
	lock.l_pid = getpid();
	l1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, i * sizeof(Admin), SEEK_SET);
	read(fd, &c, sizeof(c));
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return c;
}

Customer getCustomer(int ID)
{
	int i = ID;
	Customer c;
	int fd = open(CUSTOMER_FILE, O_RDONLY);
	int l1;
	struct flock lock;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();
	l1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, i * sizeof(Customer), SEEK_SET);
	read(fd, &c, sizeof(c));
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return c;
}

// TODO
Customer addCustomer(Customer u)
{
	int fd = open(CUSTOMER_FILE, O_RDWR);
	bool res;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_END;
	lock.l_start = (-1) * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	// Read the last customer data entry.
	Customer t;
	lseek(fd, (-1) * sizeof(Customer), SEEK_END);
	read(fd, &t, sizeof(Customer));

	u.id = t.id + 1;
	strcpy(u.details, "ACTIVE");

	int j = write(fd, &u, sizeof(Customer));
	if (j != 0)
		res = true;
	else
		res = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	// return res;
}

Product addProduct(Product p)
{
	int fd = open(PRODUCT_FILE, O_RDWR);
	lseek(fd, 0, SEEK_SET);
	bool res;
	res = false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	// Read the last product data entry.
	lseek(fd, (p.id - 1) * sizeof(Product), SEEK_SET);
	// read(fd, &t, sizeof(Product));

	// p.id = t.id + 1;

	int j = write(fd, &p, sizeof(Product));
	printf("j: %d\n", j);
	if (j != 0)
	{
		res = true;
		perror(" ");
	}
	else
	{
		res = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	if (res)
	{
		return (p);
	}
	else
	{
		p.id = -1;
		return (p);
	}
}

Product deleteProduct(int ID) // Set quantity to negative.
{
	int i = ID - 1;
	int fd = open(PRODUCT_FILE, O_RDWR);
	lseek(fd, 0, SEEK_SET);
	bool result = false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Product);
	lock.l_len = sizeof(Product);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	// Read the corresponding product data entry.
	Product t;
	lseek(fd, (i) * sizeof(Product), SEEK_SET);
	read(fd, &t, sizeof(Product));

	Product empty_product;
	empty_product.id = -1;
	empty_product.price = -1;
	empty_product.quantity = -10;

	if (t.id == ID)
	{
		// t.quantity = -10;
		strcpy(empty_product.name, "deleted");

		lseek(fd, (-1) * sizeof(Product), SEEK_CUR);
		int j = write(fd, &empty_product, sizeof(Product));

		if (j != 0)
			result = true;
		else
			result = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	strcpy(empty_product.name, "==");
	if (result)
	{
		return (t);
	}
	else
	{
		return (empty_product);
	}
}

Product modifyProduct(Product n)
{
	int i = n.id - 1;
	int fd = open(PRODUCT_FILE, O_RDWR);
	lseek(fd, 0, SEEK_SET);
	bool result = false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Product);
	lock.l_len = sizeof(Product);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	// Read the corresponding product data entry.
	Product t;
	lseek(fd, (i) * sizeof(Product), SEEK_SET);
	read(fd, &t, sizeof(Product));

	if (t.id == n.id)
	{
		// Set new price, quantity.
		if (n.price >= 0)
		{
			t.price = n.price;
		}
		if (n.quantity >= 0)
		{
			t.quantity = n.quantity;
		}
		lseek(fd, (-1) * sizeof(Product), SEEK_CUR);
		int j = write(fd, &t, sizeof(Product));
		if (j != 0)
			result = true;
		else
			result = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	Product empty_product;
	empty_product.id = -1;
	empty_product.price = -1;
	empty_product.quantity = -10;
	strcpy(empty_product.name, "==");
	if (result)
	{
		return (t);
	}
	else
	{
		return (empty_product);
	}
}

Product addProductToCart(Product product, int ID)
{
	int i = ID;
	bool result;
	result = false;
	int fl1;
	int fd;

	int quant = product.quantity;
	product = getProductById(product.id);
	int cnt = 0;

	fd = open(CUSTOMER_FILE, O_RDWR);
	lseek(fd, 0, SEEK_SET);

	if (quant <= product.quantity)
	{
		product.quantity = quant;

		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i - 1) * sizeof(Customer);
		lock.l_len = sizeof(Customer);
		lock.l_pid = getpid();

		fl1 = fcntl(fd, F_SETLKW, &lock);

		Customer c;
		lseek(fd, (i - 1) * sizeof(Customer), SEEK_SET);
		read(fd, &c, sizeof(Customer));

		// Update customer here
		lseek(fd, (i - 1) * sizeof(Customer), SEEK_SET);

		// ! TODO: check for duplicate here.

		for (int k = 0; k < MAX_CART_SIZE; k++)
		{
			if (c.cart[k].quantity <= 0)
			{
				c.cart[k] = product;
				cnt++;
				break;
			}
		}

		if (cnt > 0)
		{
			write(fd, &c, sizeof(Customer));
		}

		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
	}

	close(fd);
	if (cnt > 0)
	{
		return (product);
	}
	else
	{
		product.id = -1;
		return (product);
	}
}

// Can remove locking in below function.
Product getProductById(int ID)
{
	int i = ID - 1;
	printf("i: %d\n", i);
	Product p = {-1, "==", -1, -1};
	int fd = open(PRODUCT_FILE, O_RDWR);
	lseek(fd, 0, SEEK_SET);
	int l1;
	struct flock lock;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	printf("1\n");
	l1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, i * sizeof(Product), SEEK_SET);
	printf("2\n");
	int ret;
	Product t;
	ret = read(fd, &t, sizeof(Product));

	p.id = t.id;
	strcpy(p.name, t.name);
	p.price = t.price;
	p.quantity = t.quantity;

	printf("3\n");
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	printf("4\n");
	close(fd);
	return p;
}

// Can do mandatory locking in below function.
Product *getAllProducts(Product p_arr[])
{
	// Product *p_arr = malloc(sizeof(Product) * 30);

	Product p;

	int fd = open(PRODUCT_FILE, O_RDONLY);
	lseek(fd, 0, SEEK_SET);
	// printf("fd %d\n",fd);
	// int l1;
	// struct flock lock;
	// int ndx = 0;
	// lock.l_type = F_RDLCK;
	// lock.l_whence = SEEK_SET;
	// // lock.l_start = (ndx) * sizeof(Product);
	// lock.l_start = 0;
	// lock.l_len = 0;
	// // lock.l_len = sizeof(Product);
	// lock.l_pid = getpid();
	// l1 = fcntl(fd, F_SETLKW, &lock);
	int ndx = 0;
	while (read(fd, &p, sizeof(struct Product)) > 0)
	{
		// lseek(fd, ndx * sizeof(Product), SEEK_SET);
		// int ret = -1;
		// ret = read(fd, &p, sizeof(p));

		// printf("---:%s\n", p.name);

		p_arr[ndx].id = p.id;
		p_arr[ndx].price = p.price;
		p_arr[ndx].quantity = p.quantity;
		strcpy(p_arr[ndx].name, p.name);

		ndx++;
	}

	// lock.l_type = F_UNLCK;
	// fcntl(fd, F_SETLK, &lock);
	close(fd);
	return (p_arr);
}

Product *getCartByCustomer(int ID, Product p_arr[])
{
	int i = ID;
	bool result;
	result = false;
	int fl1;
	int fd;

	fd = open(CUSTOMER_FILE, O_RDWR);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i - 1) * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	Customer c;
	lseek(fd, (i - 1) * sizeof(Customer), SEEK_SET);
	read(fd, &c, sizeof(Customer));

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);

	// For returning cart of customer.
	for (int k = 0; k < MAX_CART_SIZE; k++)
	{
		p_arr[k] = c.cart[k];
	}

	return (p_arr);
}

Product updateProductInCart(Product product, int ID)
{
	int i = ID - 1;
	bool result;
	result = false;
	int fl1;
	int fd;
	int cnt = 0;

	fd = open(CUSTOMER_FILE, O_RDWR, 0777);
	lseek(fd, 0, SEEK_SET);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	Customer c;
	lseek(fd, (i) * sizeof(Customer), SEEK_SET);
	read(fd, &c, sizeof(Customer));

	// Update customer here
	lseek(fd, (i) * sizeof(Customer), SEEK_SET);

	// ! TODO: Update.
	for (int k = 0; k < MAX_CART_SIZE; k++)
	{
		if (c.cart[k].id == product.id && c.cart[k].quantity > 0)
		{
			c.cart[k].quantity = product.quantity;
			cnt++;
		}
	}

	if (cnt > 0)
	{
		write(fd, &c, sizeof(Customer));
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	if (cnt > 0)
	{
		return (product);
	}
	else
	{
		product.id = -1;
		return (product);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
