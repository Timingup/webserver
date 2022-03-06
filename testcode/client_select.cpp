#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <assert.h>

using namespace std;

int main(int argc, char *argv[]){
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr;
	char buf[BUFSIZ];


	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(atoi(argv[1]));
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	assert((client_sockfd = socket(PF_INET, SOCK_STREAM, 0))!= -1);

	assert(connect(client_sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) != -1);

	cout << "connect to server" << endl;
	len = recv(client_sockfd, buf, BUFSIZ, 0);
	buf[len] = '\0';
	cout << buf << endl;

	while(1){
		cout << "Enter string to send:" << endl;
		len = read(0, buf, BUFSIZ);	
		buf[len] = '\0';
		if(!strcmp(buf, "quit"))
			break;
		len = write(client_sockfd, buf, strlen(buf));
		len = read(client_sockfd, buf, BUFSIZ );
		cout << "receive: " << buf << endl;
	}
	close(client_sockfd);
	return 0;
}
