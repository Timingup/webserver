#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <assert.h>

using namespace std;
pthread_mutex_t mutex;
vector<int> client_fds;


void * handler_clnt(void * args){
	int len;
	char buf[BUFSIZ];
	int fd = *((int*) args);
	memset(buf, '\0', BUFSIZ);
	while((len = read(fd, buf, BUFSIZ)) > 0){
		cout <<"what I read :" << buf << endl;
		assert(write(fd, buf, len));
		memset(buf, '\0', BUFSIZ);
	}
	pthread_mutex_lock(&mutex);	
	for(vector<int>::iterator i = client_fds.begin(); i != client_fds.end(); ++i){
		if(*i == fd){
			client_fds.erase(i);
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
	close(fd);
	cout << fd << "client disconnected" << fd << endl;

	return 0;
}

int main(int argc, char *argv[]){
	int server_sockfd;
	int client_sockfd;
	int len;

	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	

	pthread_t tid;

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = INADDR_ANY;


	assert((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) != -1); 
	assert(bind(server_sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) != -1);
	assert(listen(server_sockfd, 5) != -1);

	socklen_t client_addr_size = sizeof(sockaddr_in);
	
	pthread_mutex_init(&mutex, nullptr);
	

	while(1){
		assert((client_sockfd = accept(server_sockfd, (struct sockaddr*)&remote_addr, &client_addr_size)) != -1);

		pthread_mutex_lock(&mutex);
		client_fds.push_back(client_sockfd);			
		pthread_mutex_unlock(&mutex);

		cout << "Accept client " << inet_ntoa(remote_addr.sin_addr) << " client_sockfd :" <<client_sockfd << endl;
		len = write(client_sockfd, "Welcome to my server\n" ,21);

		pthread_create(&tid, nullptr, handler_clnt, (void *) &client_sockfd);
		pthread_detach(tid);

	}
	close(server_sockfd);
	return 0;
}

