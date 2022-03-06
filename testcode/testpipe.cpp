#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[]){
	int fds[2];
	char s1[] = "who is my father?";
	char s2[] = "You are my son.";
	char buf[BUFSIZ];

	pid_t pid;
	pipe(fds);

	if((pid = fork()) == -1){
		cout << "error" << endl;
	}
	else if(pid == 0){
		write(fds[1], s1, sizeof(s1));	
		sleep(2);
		read(fds[0], buf, BUFSIZ);
		cout << "what I read:" << buf << endl;
	}
	else {
		read(fds[0], buf, BUFSIZ);
		cout << "what I read:" << buf << endl;
		write(fds[1], s2, sizeof(s2));
		sleep(3);
	}

	return 0;
}
