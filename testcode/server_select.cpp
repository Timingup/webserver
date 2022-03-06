#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>

using namespace std;


int main(int argc, char *argv[]){
	int server_sockfd;
	int client_sockfd;
	int len;

	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	
	char buf[BUFSIZ];

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = INADDR_ANY;


	// create socket
	assert((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) != -1); 
	
	// bind scket to addr
	assert(bind(server_sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) != -1);
	
	// set listen queue len
	assert(listen(server_sockfd, 5) != -1);

	socklen_t client_addr_size = sizeof(sockaddr_in);
	
	// set fd_set
	fd_set reads, cpy_reads;
	timeval timeout;
	int fd_max = server_sockfd, fd_num;

	FD_ZERO(&reads);
	FD_SET(server_sockfd, &reads);

	while(1){
		cpy_reads = reads;

		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		
		assert((fd_num = select(fd_max + 1, &cpy_reads, nullptr, nullptr, &timeout)) != -1);
		
		if(fd_num == 0) continue;
		for(int i = 0; i <= fd_max && fd_num; ++i){
			if(FD_ISSET(i, &cpy_reads)){
				if(i == server_sockfd){
					if((client_sockfd = accept(server_sockfd, (struct sockaddr*)&remote_addr, &client_addr_size)) == -1){
						continue;
					}
					FD_SET(client_sockfd, &reads);
					fd_max = max(fd_max, client_sockfd);
					cout << "Accept client " << inet_ntoa(remote_addr.sin_addr) << " client_sockfd :" <<client_sockfd << endl;
					len = write(client_sockfd, "Welcome to my server\n" ,21);
				}
				else{
					memset(buf, '\0', BUFSIZ);
					if((len = read(i, buf, BUFSIZ)) > 0){
						cout <<"what I read :" << buf << endl;
						assert(write(i, buf, len));
					}
					else {
						close(client_sockfd);
						cout << client_sockfd << "client disconnected" << endl;
					}
				}
				--fd_num;
			}
		}
	}
	close(server_sockfd);

	return 0;
}
