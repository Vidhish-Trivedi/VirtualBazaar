#include "customer.h"

int main(void){
	struct sockaddr_in server ;
	int socket_descriptor;
	socket_descriptor = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8080);
	connect(socket_descriptor,(struct sockaddr*)&server,sizeof(server));
	options(socket_descriptor);
	menu(socket_descriptor);
	close(socket_descriptor);
	return 0 ;
}