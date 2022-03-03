#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[]){
	int keyboard;
	int ret, i;
	fd_set readfd;
	timeval timeout;

	assert((keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK)));

	while(1){
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		
		FD_ZERO(&readfd);
		FD_SET(keyboard, &readfd);

		assert((ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout)) != -1);

		if(ret){
			if(FD_ISSET(keyboard, &readfd)){
				char c;
				i = read(keyboard, &c, 1);
				if('\n' == c) continue;
				cout << "the word your put is " << c << endl;
				if('q' == c) break;
			}
		}
		else {
			cout << "time out" << endl;
			continue;
		}

	}

	return 0;
}
