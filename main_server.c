/* Code written by: IMT2021055 - Vidhish Trivedi
   Project made as part of Operating Systems course at IIIT-Bangalore.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "server.h"

int main()
{
	struct sockaddr_in server, client;
	int sd, nsd, clientLen;
	pthread_t threads;
	bool result;
	sd = socket(AF_INET, SOCK_STREAM, 0);

	// Keeping localhost as the server address , same as client for demo purpose
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5000);

	// server setup steps followed as shown in class
	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd, 5);

	write(1, "Waiting for the client.....\n", 29);

	while (1)
	{
		clientLen = sizeof(client);
		nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

		write(1, "Connected to the client.....\n", 30);
		if (pthread_create(&threads, NULL, (void *)connection, (void *)&nsd) < 0)
		{
			perror("could not create thread");
			return 1;
		}
	}

	pthread_exit(NULL);
	close(sd);
	return (0);
}