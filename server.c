/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/

#include "server.h"

// Server side CRUD operations which works analogous to Client side information.
// Check whether file is present or not and take necessary action (locking and unlocking)
// Prepare a response
// Send the reponse to client

void server(int nsd)
{
	int msgLength, select, type, option, accType, currUserID;
	bool result;
	Query q;

	while (1)
	{
		read(nsd, &option, sizeof(option));
		printf("Option : %d\n", option);

		if (option == 1)
		{
			Customer currUser1;
			accType = 1;
			msgLength = read(nsd, &currUser1, sizeof(Customer));
			printf("Username : %d\n", currUser1.id);
			printf("Password : %s\n", currUser1.password);
			currUserID = currUser1.id;
			result = checkCustomer(currUser1);
			write(nsd, &result, sizeof(result));
		}
		else if (option == 2)
		{
			Admin currUser3;
			accType = 3;
			msgLength = read(nsd, &currUser3, sizeof(Admin));
			currUserID = currUser3.id;
			printf("Username : %d\n", currUser3.id);
			printf("Password : %s\n", currUser3.password);
			result = checkAdmin(currUser3);
			write(nsd, &result, sizeof(result));
		}
		else
		{
			result = false;
			write(nsd, &result, sizeof(result));
		}
		if (result)
			break;
	}

	// Handling requests.
	while (1)
	{
		read(nsd, &q, sizeof(Query));

		if (option == 1 || option == 2)
		{
			// if (q.query_num == 1)
			// {
			// 	float amt = q.money;

			// 	result = depositMoney(accType, currUserID, amt);
			// 	write(nsd, &result, sizeof(result));
			// }
			// else if (q.query_num == 2)
			// {
			// 	float amt = q.money;

			// 	result = withdrawMoney(accType, currUserID, amt);
			// 	write(nsd, &result, sizeof(result));
			// }
			// else if (q.query_num == 3)
			// {
			// 	float amt;

			// 	amt = getBalance(accType, currUserID);
			// 	write(nsd, &amt, sizeof(float));
			// }
			if (q.query_num == 4)
			{
				char pwd[10];
				read(nsd, pwd, sizeof(pwd));
				result = alterPassword(accType, currUserID, pwd);
				write(nsd, &result, sizeof(result));
			}
			else if (q.query_num == 5)
				break;
		}
		else if (option == 3)
		{

			if (q.query_num == 1)
			{
				read(nsd, &type, sizeof(int));
				if (type == 1)
				{
					Customer newUser1;
					read(nsd, &newUser1, sizeof(Customer));
					result = addCustomer(newUser1);
					write(nsd, &result, sizeof(result));
				}
			}
			else if (q.query_num == 2)
			{
				read(nsd, &type, sizeof(int));
				if (type == 1)
				{
					int delUserID1;
					read(nsd, &delUserID1, sizeof(int));
					result = deleteCustomer(delUserID1);
					write(nsd, &result, sizeof(result));
				}
			}
			else if (q.query_num == 3)
			{
				read(nsd, &type, sizeof(int));
				if (type == 1)
				{
					Customer modUser1;
					read(nsd, &modUser1, sizeof(Customer));
					result = modifyCustomer(modUser1);
					write(nsd, &result, sizeof(result));
				}
			}
			else if (q.query_num == 4)
			{
				read(nsd, &type, sizeof(int));
				if (type == 1)
				{
					Customer searchUser1;
					int userID1;
					read(nsd, &userID1, sizeof(int));
					searchUser1 = getCustomer(userID1);
					write(nsd, &searchUser1, sizeof(Customer));
				}
			}
			else if (q.query_num == 5)
				break;
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

Admin getAdmin(int ID)
{
	int i = ID - 1000;
	Admin c;
	int fd = open("Admin_User", O_RDONLY, 0744);

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

bool checkAdmin(Admin c)
{
	int i = c.id - 1000;
	int fd = open("Admin_User", O_RDONLY, 0744);
	bool res;
	Admin t;

	int l1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = i * sizeof(Admin);
	lock.l_len = sizeof(Admin);
	lock.l_pid = getpid();
	l1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(Admin), SEEK_SET);
	read(fd, &t, sizeof(Admin));
	if (!strcmp(t.password, c.password))
		res = true;
	else
		res = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return res;
}

bool alterPassword(int user_type, int ID, char newpass[10])
{
	int i = ID - 1000;
	int fd;
	int fl1;
	bool result;
	struct flock lock;
	switch (user_type)
	{
	case 1: // customer
		fd = open("Customer_User", O_RDWR, 0744);

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(Customer);
		lock.l_len = sizeof(Customer);
		lock.l_pid = getpid();

		fl1 = fcntl(fd, F_SETLKW, &lock);

		Customer n;
		lseek(fd, (i) * sizeof(Customer), SEEK_SET);
		read(fd, &n, sizeof(Customer));

		if (!strcmp(n.details, "ACTIVE"))
		{
			strcpy(n.password, newpass);
			lseek(fd, sizeof(Customer) * (-1), SEEK_CUR);
			write(fd, &n, sizeof(Customer));
			result = true;
		}
		else
			result = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);

		close(fd);
		return result;
		break;
	default:
		break;
	}

	return false;
}

Customer getCustomer(int ID)
{
	int i = ID - 1000;
	Customer c;
	int fd = open("Customer_User", O_RDONLY, 0744);
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

bool checkCustomer(Customer c)
{
	int i = c.id - 1000;
	int fd = open("Customer_User", O_RDONLY, 0744);
	bool res;
	Customer t;

	int l1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = i * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();
	l1 = fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(Customer), SEEK_SET);
	read(fd, &t, sizeof(Customer));
	printf("%s %s\n", t.password, c.password);
	if (!strcmp(t.password, c.password) && !strcmp(t.details, "ACTIVE"))
		res = true;
	else
		res = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return res;
}

bool addCustomer(Customer u)
{
	int fd = open("Customer_User", O_RDWR, 0744);
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
	return res;
}

bool deleteCustomer(int ID)
{
	int i = ID - 1000;
	int fd = open("Customer_User", O_RDWR, 0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);
	// getchar();

	Customer n;
	lseek(fd, (i) * sizeof(Customer), SEEK_SET);
	read(fd, &n, sizeof(Customer));

	if (!strcmp(n.details, "ACTIVE"))
	{
		strcpy(n.details, "CLOSED");

		lseek(fd, (-1) * sizeof(Customer), SEEK_CUR);
		int j = write(fd, &n, sizeof(Customer));
		if (j != 0)
			result = true;
		else
			result = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return result;
}

// ! TODO: Check this!
bool modifyCustomer(Customer n)
{
	int i = n.id - 1000;
	int fd = open("Customer_User", O_RDWR, 0744);
	bool result = false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Customer);
	lock.l_len = sizeof(Customer);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	// Read the corresponding customer data entry.
	Customer t;
	lseek(fd, (i) * sizeof(Customer), SEEK_SET);
	read(fd, &t, sizeof(Customer));

	if (!strcmp(t.details, "ACTIVE") && (n.id == t.id))
	{
		strcpy(n.details, "ACTIVE");
		lseek(fd, (-1) * sizeof(Customer), SEEK_CUR);
		int j = write(fd, &n, sizeof(Customer));
		if (j != 0)
			result = true;
		else
			result = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// bool depositMoney(int account_type, int ID, float amnt)
// {
// 	int i = ID - 1000;
// 	bool result;
// 	int fl1;
// 	int fd;
// 	switch (account_type)
// 	{
// 	case 1:
// 		fd = open("Normal_User", O_RDWR, 0744);

// 		struct flock lock;
// 		lock.l_type = F_WRLCK;
// 		lock.l_whence = SEEK_SET;
// 		lock.l_start = (i) * sizeof(normal);
// 		lock.l_len = sizeof(normal);
// 		lock.l_pid = getpid();

// 		fl1 = fcntl(fd, F_SETLKW, &lock);

// 		normal c;
// 		lseek(fd, (i) * sizeof(normal), SEEK_SET);
// 		read(fd, &c, sizeof(normal));

// 		if (!strcmp(c.details, "ACTIVE"))
// 		{
// 			c.balance += amnt;
// 			lseek(fd, sizeof(normal) * (-1), SEEK_CUR);
// 			write(fd, &c, sizeof(normal));
// 			result = true;
// 		}
// 		else
// 			result = false;
// 		lock.l_type = F_UNLCK;
// 		fcntl(fd, F_SETLK, &lock);

// 		close(fd);
// 		return result;

// 		break;
// 	case 2:
// 		fd = open("Joint_User", O_RDWR, 0744);

// 		lock.l_type = F_WRLCK;
// 		lock.l_whence = SEEK_SET;
// 		lock.l_start = (i) * sizeof(joint);
// 		lock.l_len = sizeof(joint);
// 		lock.l_pid = getpid();

// 		fl1 = fcntl(fd, F_SETLKW, &lock);

// 		joint currUser;
// 		lseek(fd, (i) * sizeof(joint), SEEK_SET);
// 		read(fd, &currUser, sizeof(joint));

// 		if (!strcmp(currUser.details, "ACTIVE"))
// 		{
// 			currUser.balance += amnt;
// 			lseek(fd, sizeof(joint) * (-1), SEEK_CUR);
// 			write(fd, &currUser, sizeof(joint));
// 			result = true;
// 		}
// 		else
// 			result = false;
// 		lock.l_type = F_UNLCK;
// 		fcntl(fd, F_SETLK, &lock);

// 		close(fd);
// 		return result;

// 	default:

// 		break;
// 	}

// 	return false;
// }
// bool withdrawMoney(int account_type, int ID, float amnt)
// {
// 	int i = ID - 1000;
// 	bool res;
// 	int fl1;
// 	struct flock lock;
// 	int fd;
// 	switch (account_type)
// 	{
// 	case 1:
// 		fd = open("Normal_User", O_RDWR, 0744);

// 		lock.l_type = F_WRLCK;
// 		lock.l_whence = SEEK_SET;
// 		lock.l_start = (i) * sizeof(normal);
// 		lock.l_len = sizeof(normal);
// 		lock.l_pid = getpid();

// 		fl1 = fcntl(fd, F_SETLKW, &lock);

// 		normal currUser;
// 		lseek(fd, (i) * sizeof(normal), SEEK_SET);
// 		read(fd, &currUser, sizeof(normal));

// 		if (!strcmp(currUser.details, "ACTIVE") && currUser.balance >= amnt)
// 		{
// 			currUser.balance -= amnt;
// 			lseek(fd, sizeof(normal) * (-1), SEEK_CUR);
// 			write(fd, &currUser, sizeof(normal));
// 			res = true;
// 		}
// 		else
// 			res = false;
// 		lock.l_type = F_UNLCK;
// 		fcntl(fd, F_SETLK, &lock);

// 		close(fd);
// 		return res;

// 		break;
// 	case 2:
// 		fd = open("Joint_User", O_RDWR, 0744);

// 		lock.l_type = F_WRLCK;
// 		lock.l_whence = SEEK_SET;
// 		lock.l_start = (i) * sizeof(joint);
// 		lock.l_len = sizeof(joint);
// 		lock.l_pid = getpid();

// 		fl1 = fcntl(fd, F_SETLKW, &lock);

// 		joint c;
// 		lseek(fd, (i) * sizeof(joint), SEEK_SET);
// 		read(fd, &c, sizeof(joint));

// 		if (!strcmp(c.details, "ACTIVE") && c.balance >= amnt)
// 		{
// 			c.balance -= amnt;
// 			lseek(fd, sizeof(joint) * (-1), SEEK_CUR);
// 			write(fd, &c, sizeof(joint));
// 			res = true;
// 		}
// 		else
// 			res = false;
// 		lock.l_type = F_UNLCK;
// 		fcntl(fd, F_SETLK, &lock);

// 		close(fd);
// 		return res;

// 	default:
// 		break;
// 	}
// 	return false;
// }
// float getBalance(int account_type, int ID)
// {
// 	int i = ID - 1000;
// 	float result;
// 	int fl1;
// 	struct flock lock;
// 	int fd;
// 	switch (account_type)
// 	{
// 	case 1:

// 		fd = open("Normal_User", O_RDONLY, 0744);
// 		normal temp;

// 		lock.l_type = F_RDLCK;
// 		lock.l_whence = SEEK_SET;
// 		lock.l_start = (i) * sizeof(normal);
// 		lock.l_len = sizeof(normal);
// 		lock.l_pid = getpid();

// 		fl1 = fcntl(fd, F_SETLKW, &lock);

// 		lseek(fd, (i) * sizeof(normal), SEEK_SET);
// 		read(fd, &temp, sizeof(normal));

// 		if (!strcmp(temp.details, "ACTIVE"))
// 			result = temp.balance;
// 		else
// 			result = 0;

// 		lock.l_type = F_UNLCK;
// 		fcntl(fd, F_SETLK, &lock);

// 		close(fd);
// 		return result;

// 		break;
// 	case 2:

// 		fd = open("Joint_User", O_RDONLY, 0744);
// 		joint j;
// 		lock.l_type = F_RDLCK;
// 		lock.l_whence = SEEK_SET;
// 		lock.l_start = (i) * sizeof(joint);
// 		lock.l_len = sizeof(joint);
// 		lock.l_pid = getpid();

// 		fl1 = fcntl(fd, F_SETLKW, &lock);

// 		lseek(fd, (i) * sizeof(joint), SEEK_SET);
// 		read(fd, &j, sizeof(j));
// 		if (!strcmp(j.details, "ACTIVE"))
// 			result = j.balance;
// 		else
// 			result = 0;

// 		lock.l_type = F_UNLCK;
// 		fcntl(fd, F_SETLK, &lock);

// 		close(fd);
// 		return result;
// 		break;

// 	default:
// 		break;
// 	}
// 	return 0;
// }
