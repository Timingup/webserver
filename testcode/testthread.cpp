#include <iostream>

#include <pthread.h>

using namespace std;

int num = 0;

pthread_mutex_t mutex;

void* add(void * args){
	int tid = *((int*)args);
	pthread_mutex_lock(&mutex);
	++num;
	cout << tid << " add num, num is :" << num << endl;
	pthread_mutex_unlock(&mutex);
	return 0;
}

void* sub(void* args){
	int tid = *((int*)args);	
	pthread_mutex_lock(&mutex);
	--num;
	cout << tid << " sub num, num is :" << num << endl;
	pthread_mutex_unlock(&mutex);
	return 0;
}


int main(){
	pthread_t tids[10];
	pthread_mutex_init(&mutex, nullptr);
	for(int i = 0; i < 10; ++i){
		if(i%2)
			pthread_create(&tids[i], nullptr, add, (void*)&(tids[i]));
		else 
			pthread_create(&tids[i], nullptr, sub, (void*)&(tids[i]));
	}
	pthread_exit(nullptr);
}
