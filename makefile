test: test.cpp ThreadPool.h SynQueue.h
	g++ test.cpp ThreadPool.h SynQueue.h -o test -pthread
