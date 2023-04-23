/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/server.h"

// Server side CRUD operations which works analogous to Client side information.
// Check whether file is present or not and take necessary action (locking and unlocking)
// Prepare a response
// Send the reponse to client

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
			read(nsd, &q, sizeof(Query));
			if (q.query_num == 1)
			{
				Product *p_arr = malloc(sizeof(Product) * MAX_PRODUCTS);
				p_arr = getAllProducts(p_arr);
				write(nsd, p_arr, sizeof(Product) * MAX_PRODUCTS);
			}
			else if (q.query_num == 2)
			{
				Product *p_arr = malloc(sizeof(Product) * MAX_CART_SIZE);
				p_arr = getCartByCustomer(q.user_id);
				write(nsd, p_arr, sizeof(Product) * MAX_CART_SIZE);
			}
		}
		else if (type == 2) // Admin
		{
			read(nsd, &q, sizeof(Query));

			if (q.query_num == 1) // Add
			{
				Product result;
				result = addProduct(q.product);
				write(nsd, &result, sizeof(Product));
			}
			else if (q.query_num == 2) // Delete
			{
				Product p;
				p = deleteProduct(q.product.id);
				write(nsd, &p, sizeof(Product));
			}
			else if (q.query_num == 3) // Update
			{
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
	if(res){
		return(p);
	}
	else{
		p.id = -1;
		return(p);
	}
}

Product deleteProduct(int ID) // Set quantity to negative.
{
	bool result;
	result = false;
	int i = ID - 1;
	printf("i: %d\n", i);
	Product p;
	p.id = -1;
	strcpy(p.name, "==");
	p.price = -1;
	p.quantity = -1;

	int j = -1;

	int fd = open(PRODUCT_FILE, O_RDWR);

	lseek(fd, 0, SEEK_SET);
	int l1;
	struct flock lock;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	l1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, i * sizeof(Product), SEEK_SET);

	Product t;
	read(fd, &t, sizeof(Product));

	// printf("t.id: %d\n", t.id);

	if (t.id == ID)
	{
		p.id = t.id;
		strcpy(p.name, t.name);
		p.price = t.price;
		p.quantity = t.quantity;

		t.quantity = -10;
		lseek(fd, 0, SEEK_SET);
		lseek(fd, i * sizeof(Product), SEEK_SET);
		j = write(fd, &t, sizeof(Product));
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return (p);
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

// TODO
Product addProductToCart(int product_id, int ID, int quantity)
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

	// ! TODO: Update customer here.

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	// return result;
}

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

Product *getCartByCustomer(int ID)
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

	// ! TODO: Finish this.

	close(fd);

	return (c.cart);
}

//  ! TODO
Product updateProductInCart(int ID, Product product)
{
	int i = ID - 1;
	bool result;
	result = false;
	int fl1;
	int fd;

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

	// ! TODO: Update.

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);

	return (product);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
