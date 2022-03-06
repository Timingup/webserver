#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <assert.h>

using namespace std;

int main(int argc, char *argv[]){
	
	int listenfd;
	sockaddr_in client_addr;
	
	// init socket	
	int connfd;
	assert(connfd = socket(PF_INET, SOCK_STREAM, 0) != -1);

	// bind port
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr = INADDR_ANY;	

	assert(bind(connfd, (sockaddr*) &server_addr, sizeof(server_addr)) != -1);
	
	// linsten
	assert(listen(connfd, 5) != -1);


	// accept
	socklen_t client_addr_size = sizeof(sockaddr_in);

	while(1){
		assert((listenfd = accept(connfd,(sockaddr* )&client_addr, &client_addr_size)) != -1);

	}


	return 0;
}
