#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>

using namespace std;

void read_childproc(int sig){
	int status;
	pid_t id = waitpid(0, &status, WNOHANG);
	if(WIFEXITED(status)){
		cout << "remove proc id: " << id << endl;
		cout << "child send :" << WEXITSTATUS(status) << endl;
	}
}


int main(int argc, char *argv[]){
	int server_sockfd;
	int client_sockfd;
	int len;

	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	
	char buf[BUFSIZ];

	// use signal
	pid_t pid;
	struct sigaction act;
	int state;


	// set signal
	
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, 0);

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
	
	while(1){

		// get remote_addr
		if((client_sockfd = accept(server_sockfd, (struct sockaddr*)&remote_addr, &client_addr_size)) == -1){
			continue;
		}

		if((pid = fork()) == -1){
			close(client_sockfd);
			continue;
		}
		else if(pid == 0){
			close(server_sockfd);
			
			cout << "Accept client " << inet_ntoa(remote_addr.sin_addr) << endl;
			len = send(client_sockfd, "Welcome to my server\n", 21, 0);

			while((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0){
				buf[len] = '\0';
				cout << buf << endl;
				// printf("%s\n", buf);
				assert(send(client_sockfd, buf, len, 0));
			}	
			close(client_sockfd);
			cout << "client disconnected...";
			return 0;
		}
		else{
			close(client_sockfd);
		}

	}
	
	
	close(server_sockfd);

	return 0;
}
