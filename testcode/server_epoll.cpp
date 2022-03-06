#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <assert.h>

#define EPOLL_SIZE 50

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


	assert((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) != -1); 
	assert(bind(server_sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) != -1);
	assert(listen(server_sockfd, 5) != -1);

	socklen_t client_addr_size = sizeof(sockaddr_in);
	
	epoll_event *eptr, event;
	int epfd, event_num;

	epfd = epoll_create(EPOLL_SIZE);
	eptr = (epoll_event *)malloc(sizeof(epoll_event) *EPOLL_SIZE);

	event.data.fd = server_sockfd;
	event.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, server_sockfd, &event);


	while(1){
		assert((event_num = epoll_wait(epfd, eptr, EPOLL_SIZE, -1)) != -1);
		
		for(int i = 0; i < event_num; ++i){
			if(eptr[i].data.fd == server_sockfd){
				if((client_sockfd = accept(server_sockfd, (struct sockaddr*)&remote_addr, &client_addr_size)) == -1){
					continue;
				}
				event.events = EPOLLIN;
				event.data.fd = client_sockfd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &event);
				cout << "Accept client " << inet_ntoa(remote_addr.sin_addr) << " client_sockfd :" <<client_sockfd << endl;
				len = write(client_sockfd, "Welcome to my server\n" ,21);
			}
			else{
				memset(buf, '\0', BUFSIZ);
				if((len = read(eptr[i].data.fd, buf, BUFSIZ)) > 0){
					cout <<"what I read :" << buf << endl;
					assert(write(eptr[i].data.fd, buf, len));
				}
				else {
					epoll_ctl(epfd, EPOLL_CTL_DEL, eptr[i].data.fd, nullptr);
					close(client_sockfd);
					cout << client_sockfd << "client disconnected" << eptr[i].data.fd << endl;
				}
			}
		}
	}
	close(server_sockfd);
	close(epfd);
	return 0;
}
