#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <assert.h>

/*
void Assert(char * filename, unsigned int lineno){
	fflush(stdout);
	fprintf(stderr, "\nAssert failed: %s, line %u\n", filename, lineno);
	fflush(stderr);
	abort();
}

#define ASSERT(condition)\
if(condition)\
	NULL;\
else\
	Assert(__FILE__, __LINE__)

*/




int main(int argc, char *argv[]){
	int server_sockfd;
	int client_sockfd;
	int len;

	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	
	char buf[BUFSIZ];

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(9999);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
	// create socket
	assert((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) != -1); 
	
	// bind scket to addr
	
	printf("%d", server_sockfd);	
	// int a = bind(server_sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr));

	// printf("%d/n",a);

	assert(bind(server_sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) != -1);
	
	// set listen queue len
	assert(listen(server_sockfd, 5) != -1);
	
	socklen_t sin_size = sizeof(struct sockaddr_in);
	
	// get remote_addr
	assert((client_sockfd = accept(server_sockfd, (struct sockaddr*)&remote_addr, &sin_size)) != -1);

	printf("accept client %s/n", inet_ntoa(remote_addr.sin_addr));
	len = send(client_sockfd, "Welcome to my server/n", 21, 0);
	
	while((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0){
		buf[len] = '\0';
		printf("%s\n", buf);
		assert(send(client_sockfd, buf, len, 0));

	}	
	
	close(client_sockfd);
	close(server_sockfd);

	return 0;
}
