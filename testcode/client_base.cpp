#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <assert.h>

int main(int argc, char *argv[]){
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr;
	char buf[BUFSIZ];


	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(9999);
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	assert((client_sockfd = socket(PF_INET, SOCK_STREAM, 0))!= -1);

	assert(connect(client_sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) != -1);

	printf("connect to server/n");
	len = recv(client_sockfd, buf, BUFSIZ, 0);
	buf[len] = '\0';
	printf("%s",buf);

	while(1){
		printf("Enter string to send:");
		scanf("%s", buf);
		
		if(!strcmp(buf, "quit"))
			break;
		len = send(client_sockfd, buf, strlen(buf), 0);
		len = recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len] = '\0';
		printf("receive:%s/n",buf);
	}

	close(client_sockfd);

	return 0;
}
