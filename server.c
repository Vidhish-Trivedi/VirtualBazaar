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
			result = checkNormalUser(currUser1);
			write(nsd, &result, sizeof(result));
		}
		else if (option == 2)
		{
			Seller currUser2;
			accType = 2;
			msgLength = read(nsd, &currUser2, sizeof(Seller));
			currUserID = currUser2.id;
			printf("Username : %d\n", currUser2.id);
			printf("Password : %s\n", currUser2.password);
			result = checkJointUser(currUser2);
			write(nsd, &result, sizeof(result));
		}
		else if (option == 3)
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

	while (1)
	{
		read(nsd, &q, sizeof(Query));

		if (option == 1 || option == 2)
		{
			if (q.query_num == 1)
			{
				float amt = q.money;

				result = depositMoney(accType, currUserID, amt);
				write(nsd, &result, sizeof(result));
			}
			else if (q.query_num == 2)
			{
				float amt = q.money;

				result = withdrawMoney(accType, currUserID, amt);
				write(nsd, &result, sizeof(result));
			}
			else if (q.query_num == 3)
			{
				float amt;

				amt = getBalance(accType, currUserID);
				write(nsd, &amt, sizeof(float));
			}
			else if (q.query_num == 4)
			{
				char pwd[10];
				read(nsd, pwd, sizeof(pwd));
				result = alterPassword(accType, currUserID, pwd);
				write(nsd, &result, sizeof(result));
			}
			else if (q.query_num == 5)
			{
				if (option == 1)
				{
					Customer user1 = getNormalUser(currUserID);
					write(nsd, &user1, sizeof(Customer));
				}
				else if (option == 2)
				{
					Seller user2 = getJointUser(currUserID);
					write(nsd, &user2, sizeof(Seller));
				}
			}
			else if (q.query_num == 6)
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
					result = addNormalUser(newUser1);
					write(nsd, &result, sizeof(result));
				}
				else if (type == 2)
				{
					Seller newUser2;
					read(nsd, &newUser2, sizeof(Seller));
					result = addJointUser(newUser2);
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
					result = deleteNormalUser(delUserID1);
					write(nsd, &result, sizeof(result));
				}
				else if (type == 2)
				{
					int delUserID2;
					read(nsd, &delUserID2, sizeof(int));
					result = deleteJointUser(delUserID2);
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
					result = modifyNormalUser(modUser1);
					write(nsd, &result, sizeof(result));
				}
				else if (type == 2)
				{
					Seller modUser2;
					read(nsd, &modUser2, sizeof(Seller));
					result = modifyJointUser(modUser2);
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
					searchUser1 = getNormalUser(userID1);
					write(nsd, &searchUser1, sizeof(Customer));
				}
				else if (type == 2)
				{
					Seller searchUser2;
					int userID2;
					read(nsd, &userID2, sizeof(int));
					searchUser2 = getJointUser(userID2);
					write(nsd, &searchUser2, sizeof(Seller));
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
	case 1:		// customer
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
	case 2: 	// seller
		fd = open("Seller_User", O_RDWR, 0744);

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(Seller);
		lock.l_len = sizeof(Seller);
		lock.l_pid = getpid();

		fl1 = fcntl(fd, F_SETLKW, &lock);

		Seller j;
		lseek(fd, (i) * sizeof(Seller), SEEK_SET);

		if (!strcmp(j.details, "ACTIVE"))
		{
			strcpy(j.password, newpass);
			lseek(fd, sizeof(Seller) * (-1), SEEK_CUR);
			write(fd, &j, sizeof(Seller));
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


// normal getNormalUser(int ID)
// {
// 	int i = ID - 1000;
// 	normal c;
// 	int fd = open("Normal_User", O_RDONLY, 0744);
// 	int l1;
// 	struct flock lock;

// 	lock.l_type = F_RDLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = (i) * sizeof(normal);
// 	lock.l_len = sizeof(normal);
// 	lock.l_pid = getpid();
// 	l1 = fcntl(fd, F_SETLKW, &lock);
// 	lseek(fd, i * sizeof(normal), SEEK_SET);
// 	read(fd, &c, sizeof(c));
// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);
// 	close(fd);
// 	return c;
// }
// joint getJointUser(int ID)
// {
// 	int i = ID - 1000;
// 	joint c;
// 	int fd = open("Joint_User", O_RDONLY, 0744);

// 	int l1;
// 	struct flock lock;

// 	lock.l_type = F_RDLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = (i) * sizeof(joint);
// 	lock.l_len = sizeof(joint);
// 	lock.l_pid = getpid();
// 	l1 = fcntl(fd, F_SETLKW, &lock);
// 	lseek(fd, i * sizeof(joint), SEEK_SET);
// 	read(fd, &c, sizeof(c));
// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);
// 	close(fd);
// 	return c;
// }

// bool checkNormalUser(normal c)
// {
// 	int i = c.id - 1000;
// 	int fd = open("Normal_User", O_RDONLY, 0744);
// 	bool res;
// 	normal t;

// 	int l1;
// 	struct flock lock;
// 	lock.l_type = F_RDLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = i * sizeof(normal);
// 	lock.l_len = sizeof(normal);
// 	lock.l_pid = getpid();
// 	l1 = fcntl(fd, F_SETLKW, &lock);
// 	lseek(fd, (i) * sizeof(normal), SEEK_SET);
// 	read(fd, &t, sizeof(normal));
// 	printf("%s %s\n", t.password, c.password);
// 	if (!strcmp(t.password, c.password) && !strcmp(t.details, "ACTIVE"))
// 		res = true;
// 	else
// 		res = false;

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return res;
// }
// bool checkJointUser(joint c)
// {
// 	int i = c.id - 1000;
// 	int fd = open("Joint_User", O_RDONLY, 0744);
// 	bool res;
// 	joint t;

// 	int l1;
// 	struct flock lock;
// 	lock.l_type = F_RDLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = i * sizeof(joint);
// 	lock.l_len = sizeof(joint);
// 	lock.l_pid = getpid();
// 	l1 = fcntl(fd, F_SETLKW, &lock);
// 	lseek(fd, (i) * sizeof(joint), SEEK_SET);
// 	read(fd, &t, sizeof(joint));
// 	if (!strcmp(t.password, c.password) && !strcmp(t.details, "ACTIVE"))
// 		res = true;
// 	else
// 		res = false;

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return res;
// }

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
// bool addNormalUser(normal u)
// {
// 	int fd = open("Normal_User", O_RDWR, 0744);
// 	bool res;

// 	int fl1;
// 	struct flock lock;
// 	lock.l_type = F_WRLCK;
// 	lock.l_whence = SEEK_END;
// 	lock.l_start = (-1) * sizeof(normal);
// 	lock.l_len = sizeof(normal);
// 	lock.l_pid = getpid();

// 	fl1 = fcntl(fd, F_SETLKW, &lock);

// 	normal t;
// 	lseek(fd, (-1) * sizeof(normal), SEEK_END);
// 	read(fd, &t, sizeof(normal));

// 	u.id = t.id + 1;
// 	u.account_number = t.account_number + 1;
// 	strcpy(u.details, "ACTIVE");

// 	int j = write(fd, &u, sizeof(normal));
// 	if (j != 0)
// 		res = true;
// 	else
// 		res = false;

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return res;
// }
// bool addJointUser(joint j)
// {
// 	int fd = open("Joint_User", O_RDWR, 0744);
// 	bool result;

// 	int fl1;
// 	struct flock lock;
// 	lock.l_type = F_WRLCK;
// 	lock.l_whence = SEEK_END;
// 	lock.l_start = (-1) * sizeof(joint);
// 	lock.l_len = sizeof(joint);
// 	lock.l_pid = getpid();

// 	fl1 = fcntl(fd, F_SETLKW, &lock);

// 	joint endUser;
// 	lseek(fd, (-1) * sizeof(joint), SEEK_END);
// 	read(fd, &endUser, sizeof(joint));

// 	j.id = endUser.id + 1;
// 	j.account_number = endUser.account_number + 1;
// 	strcpy(j.details, "ACTIVE");

// 	int k = write(fd, &j, sizeof(joint));
// 	if (k != 0)
// 		result = true;
// 	else
// 		result = false;

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return result;
// }
// bool deleteNormalUser(int ID)
// {
// 	int i = ID - 1000;
// 	int fd = open("Normal_User", O_RDWR, 0744);
// 	bool result;

// 	int fl1;
// 	struct flock lock;
// 	lock.l_type = F_WRLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = (i) * sizeof(normal);
// 	lock.l_len = sizeof(normal);
// 	lock.l_pid = getpid();

// 	fl1 = fcntl(fd, F_SETLKW, &lock);
// 	// getchar();

// 	normal n;
// 	lseek(fd, (i) * sizeof(normal), SEEK_SET);
// 	read(fd, &n, sizeof(normal));

// 	if (!strcmp(n.details, "ACTIVE"))
// 	{
// 		strcpy(n.details, "CLOSED");
// 		n.balance = 0;

// 		lseek(fd, (-1) * sizeof(normal), SEEK_CUR);
// 		int j = write(fd, &n, sizeof(normal));
// 		if (j != 0)
// 			result = true;
// 		else
// 			result = false;
// 	}

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return result;
// }

// bool deleteJointUser(int ID)
// {
// 	int i = ID - 1000;
// 	int fd = open("Joint_User", O_RDWR, 0744);
// 	bool result;

// 	int fl1;
// 	struct flock lock;
// 	lock.l_type = F_WRLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = (i) * sizeof(joint);
// 	lock.l_len = sizeof(joint);
// 	lock.l_pid = getpid();

// 	fl1 = fcntl(fd, F_SETLKW, &lock);

// 	joint j;
// 	lseek(fd, (i) * sizeof(joint), SEEK_SET);
// 	read(fd, &j, sizeof(joint));

// 	if (!strcmp(j.details, "ACTIVE"))
// 	{
// 		strcpy(j.details, "CLOSED");
// 		j.balance = 0;

// 		lseek(fd, (-1) * sizeof(joint), SEEK_CUR);
// 		int k = write(fd, &j, sizeof(joint));
// 		if (k != 0)
// 			result = true;
// 		else
// 			result = false;
// 	}

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return result;
// }
// bool modifyNormalUser(normal n)
// {
// 	int i = n.id - 1000;
// 	int fd = open("Normal_User", O_RDWR, 0744);
// 	bool result = false;

// 	int fl1;
// 	struct flock lock;
// 	lock.l_type = F_WRLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = (i) * sizeof(normal);
// 	lock.l_len = sizeof(normal);
// 	lock.l_pid = getpid();

// 	fl1 = fcntl(fd, F_SETLKW, &lock);

// 	normal t;
// 	lseek(fd, (i) * sizeof(normal), SEEK_SET);
// 	read(fd, &t, sizeof(normal));

// 	if (!strcmp(t.details, "ACTIVE") && (n.account_number == t.account_number))
// 	{
// 		strcpy(n.details, "ACTIVE");
// 		lseek(fd, (-1) * sizeof(normal), SEEK_CUR);
// 		int j = write(fd, &n, sizeof(normal));
// 		if (j != 0)
// 			result = true;
// 		else
// 			result = false;
// 	}

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return result;
// }

// bool modifyJointUser(joint j)
// {
// 	int i = j.id - 1000;
// 	int fd = open("Joint_User", O_RDWR, 0744);
// 	bool result = false;

// 	int fl1;
// 	struct flock lock;
// 	lock.l_type = F_WRLCK;
// 	lock.l_whence = SEEK_SET;
// 	lock.l_start = (i) * sizeof(joint);
// 	lock.l_len = sizeof(joint);
// 	lock.l_pid = getpid();

// 	fl1 = fcntl(fd, F_SETLKW, &lock);

// 	joint t;
// 	lseek(fd, (i) * sizeof(joint), SEEK_SET);
// 	read(fd, &t, sizeof(joint));

// 	if (!strcmp(t.details, "ACTIVE") && (j.account_number == t.account_number))
// 	{
// 		strcpy(j.details, "ACTIVE");
// 		lseek(fd, (-1) * sizeof(joint), SEEK_CUR);
// 		int k = write(fd, &j, sizeof(joint));
// 		if (k != 0)
// 			result = true;
// 		else
// 			result = false;
// 	}

// 	lock.l_type = F_UNLCK;
// 	fcntl(fd, F_SETLK, &lock);

// 	close(fd);
// 	return result;
// }
