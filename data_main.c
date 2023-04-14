// Code to create flat files for data storage and populate it with some data automatically based
// on user input
#include "data.h"

int main()
{
	int fd = open("Admin_User", O_RDWR | O_CREAT, 0744);
	int choice = 0;
	admin a;
	printf("Enter the name of the admin: ");
	scanf(" %s", a.uname);
	printf("Enter the password(max 10 char): ");
	scanf(" %s", a.password);
	a.id = 1;
	printf("Your userID is : %d\n", a.id);
	write(fd, &a, sizeof(a));
	printf("Do you want to continue\n(0 to No, 1 to Yes)? ");
	scanf("%d", &choice);
	close(fd);
	while (choice)
	{
		printf("Enter the name of the admin: ");
		scanf(" %[^\n]", a.uname);
		printf("Enter the password(max 10 char): ");
		scanf(" %[^\n]", a.password);
		a.id = get_admin();
		printf("Your userID is : %d\n", a.id);
		fd = open("Admin_User", O_RDWR | O_CREAT | O_EXCL, 0744);
		write(fd, &a, sizeof(admin));
		close(fd);
		printf("Do you want to continue\n(0 to No, 1 to Yes)? ");
		scanf("%d", &choice);
	}

	fd = open("Customer_User", O_RDWR | O_CREAT, 0744);
	choice = 1;
	customer n;
	printf("Enter the name of the customer user: ");
	scanf(" %[^\n]", n.name);
	printf("Enter the password(max 10 char): ");
	scanf(" %[^\n]", n.password);
	n.id = 1;
	printf("Your userID is : %d\n", n.id);
	strcpy(n.details, "ACTIVE");
	write(fd, &n, sizeof(customer));
	printf("Do you want to continue\n(0 to No, 1 to Yes)? ");
	scanf("%d", &choice);
	close(fd);
	while (choice)
	{
		printf("Enter the name of the customer user: ");
		scanf(" %[^\n]", n.name);
		printf("Enter the password(max 10 char): ");
		scanf(" %[^\n]", n.password);
		n.id = get_customer();
		printf("Your userID is : %d\n", n.id);
		strcpy(n.details, "ACTIVE");
		fd = open("Customer_User", O_RDWR | O_CREAT | O_EXCL, 0744);
		write(fd, &n, sizeof(customer));
		close(fd);
		printf("Do you want to continue\n(0 to No, 1 to Yes)? ");
		scanf("%d", &choice);
	}

	fd = open("Seller_User", O_RDWR | O_CREAT, 0744);
	choice = 1;
	seller sel;
	printf("Enter the name of the seller user: ");
	scanf(" %[^\n]", sel.name);
	printf("Enter the password(max 10 char): ");
	scanf(" %[^\n]", sel.password);
	sel.id = 1;
	printf("Your userID is : %d\n", sel.id);
	strcpy(sel.details, "ACTIVE");
	write(fd, &sel, sizeof(seller));
	printf("Do you want to continue\n(0 to No, 1 to Yes)? ");
	scanf("%d", &choice);
	close(fd);
	while (choice)
	{
		printf("Enter the name of the seller user: ");
		scanf(" %[^\n]", sel.name);
		printf("Enter the password(max 10 char): ");
		scanf(" %[^\n]", sel.password);
		sel.id = get_seller();
		printf("Your userID is : %d\n", sel.id);
		strcpy(sel.details, "ACTIVE");
		fd = open("Seller_User", O_RDWR | O_CREAT | O_EXCL, 0744);
		write(fd, &sel, sizeof(seller));
		close(fd);
		printf("Do you want to continue\n(0 to No, 1 to Yes)? ");
		scanf("%d", &choice);
	}
	return 0;
}
