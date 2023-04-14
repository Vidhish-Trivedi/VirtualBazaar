#include "data.h"

int get_customer()
{
	int fd = open("Customer_User", O_RDONLY | 0744);
	customer record;
	lseek(fd, -sizeof(customer), SEEK_END);
	read(fd, &record, sizeof(customer));
	close(fd);
	return record.id + 1;
}

int get_seller()
{
	int fd = open("Seller_User", O_RDONLY | 0744);
	seller record;
	lseek(fd, sizeof(seller), SEEK_END);
	read(fd, &record, sizeof(seller));
	close(fd);
	return record.id + 1;
}

int get_admin()
{
	int fd = open("Admin_User", O_RDONLY | 0744);
	admin record;
	lseek(fd, -sizeof(admin), SEEK_END);
	read(fd, &record, sizeof(admin));
	close(fd);
	return record.id + 1;
}
