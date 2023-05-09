/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/User.h"
#include "./../header/Admin.h"

Customer getCustomer(int ID)
{
	int i = ID - 1;
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

	printf("==> %d and %d\n", quant, product.quantity);
	if (quant <= product.quantity)
	{
		printf("fi\n");
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

		int is_dup = 0;
		for (int k = 0; k < MAX_CART_SIZE; k++)
		{
			if (c.cart[k].quantity > 0 && c.cart[k].id == product.id)
			{
				is_dup++;
				printf("same: %d\n", product.id);
				break;
			}
		}

		if (is_dup == 0)
		{
			printf("si\n");
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
				printf("ti\n");
				write(fd, &c, sizeof(Customer));
			}
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

// Advisory locking (Read lock).
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
	lock.l_start = i * sizeof(Product);
	lock.l_len = sizeof(Product);
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

// Mandatory locking (Read lock). // TODO
Product *getAllProducts(Product p_arr[])
{
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

int generateReciept(Query q, int total, Product p_arr[])
{
	// strcat(ADMIN_LOG_FILE, ".txt");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char file_name[100];
	int k = sprintf(file_name, "/home/vidhish/Desktop/os-project/Online-Retail-Store/logs/receipts/%d-%02d-%02d_%02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	int fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0777);
	lseek(fd, 0, SEEK_END);
	
	printf("Reciept generated at: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	char buf[200];
	char buf1[500];
	k = sprintf(buf, "Receipt generated at: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	write(fd, buf, k);
	k = sprintf(buf, "------------------------------------------------------\n");
	write(fd, buf, k);
    k = sprintf(buf, "| ProductId\t\tProductName\t\t Cost\t\tQuantity |\n");
	write(fd, buf, k);
    for (int idx = 0; idx < MAX_PRODUCTS; idx++)
    {
        if (p_arr[idx].id > 0 && p_arr[idx].id < MAX_PRODUCTS + 1 && p_arr[idx].quantity > 0)
        {
            k = sprintf(buf1, "| %9d\t\t%11s\t\t%5d\t\t%8d |\n", p_arr[idx].id, p_arr[idx].name, p_arr[idx].price, p_arr[idx].quantity); // For formatting.
			write(fd, buf1, k);
		}
    }
	k = sprintf(buf, "------------------------------------------------------\n");
	write(fd, buf, k);
	k = sprintf(buf, "Total Cost: %d\n", total);
	write(fd, buf, k);

	close(fd);
	return (0);
}

void payment(int nsd, Query q)
{
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

			total_cost += p.price * p_arr[k].quantity;
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
			write(nsd, "Payment Successful!\n", sizeof("Payment Successful!\n"));
			generateReciept(q, total_cost, p_arr);
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
