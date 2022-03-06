base: server_base.cpp client_base.cpp
	gcc -o server server_base.cpp
	gcc -o client client_base.cpp

process: server_process.cpp client_base.cpp
	g++ -o server server_process.cpp
	g++ -o client client_process.cpp

pipe: server_pipe.cpp client_pipe.cpp
	g++ -o server server_pipe.cpp
	g++ -o client client_process.cpp

select: server_select.cpp client_select.cpp
	g++ -o server server_select.cpp
	g++ -o client client_select.cpp
epoll: server_epoll.cpp client_epoll.cpp
	g++ -o server server_epoll.cpp
	g++ -o client client_epoll.cpp

thread: server_thread.cpp client_thread.cpp
	g++ -o server server_thread.cpp -pthread
	g++ -o client client_thread.cpp -pthread

clean :
	rm  server client
