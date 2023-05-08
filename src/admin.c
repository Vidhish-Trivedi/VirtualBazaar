/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "./../header/Admin.h"

// TODO
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

// No locking required.
Product addProduct(Product p)
{
	int fd = open(PRODUCT_FILE, O_RDWR);
	lseek(fd, 0, SEEK_SET);
	bool res;
	res = false;

	// int fl1;
	// struct flock lock;
	// lock.l_type = F_WRLCK;
	// lock.l_whence = SEEK_SET;
	// lock.l_start = (p.id - 1) * sizeof(Product);
	// lock.l_len = sizeof(Product);
	// lock.l_pid = getpid();

	// fl1 = fcntl(fd, F_SETLKW, &lock);

	Product t;
	lseek(fd, (p.id - 1) * sizeof(Product), SEEK_SET);
	read(fd, &t, sizeof(Product));

	if (p.id == t.id && t.quantity >= 0)
	{
		// Product already exists.
		// lock.l_type = F_UNLCK;
		// fcntl(fd, F_SETLK, &lock);

		close(fd);
		p.id = -1;
		return (p);
	}

	lseek(fd, (p.id - 1) * sizeof(Product), SEEK_SET);
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

	// lock.l_type = F_UNLCK;
	// fcntl(fd, F_SETLK, &lock);

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

// Advisory locking (Write lock).
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

// Advisory locking (Write lock).
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

// Generate Log.
int generateLog()
{
	// strcat(ADMIN_LOG_FILE, ".txt");
	int fd = open(ADMIN_LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);
	lseek(fd, 0, SEEK_END);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("Log generated at: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	char buf[200];
	char buf1[500];
	int k = sprintf(buf, "Log generated at: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	Product *product_array = malloc(sizeof(Product) * MAX_PRODUCTS);
	product_array = getAllProducts(product_array);

	write(fd, buf, k);
	k = sprintf(buf, "------------------------------------------------------\n");
	write(fd, buf, k);
    k = sprintf(buf, "| ProductId\t\tProductName\t\t Cost\t\tQuantityAvailable |\n");
	write(fd, buf, k);
	for (int idx = 0; idx < MAX_PRODUCTS; idx++)
    {
        if (product_array[idx].id > 0 && product_array[idx].id < MAX_PRODUCTS + 1 && product_array[idx].quantity > 0)
        {
            k = sprintf(buf1, "| %9d\t\t%11s\t\t%5d\t\t%17d |\n", product_array[idx].id, product_array[idx].name, product_array[idx].price, product_array[idx].quantity); // For formatting.
			write(fd, buf1, k);
		}
    }
	k = sprintf(buf, "------------------------------------------------------\n");
	write(fd, buf, k);
	close(fd);
	return (0);
}
