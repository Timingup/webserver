#include <thread>    
#include <iostream> 

#include "ThreadPool.h"

using namespace std;

int main(int argc, char *argv[])
{
    ThreadPool pool;

    //pool.Start(2);
    std::thread thd1([&pool]
    {
        for (int i = 0; i < 10; i++)
        {
            auto thdId = this_thread::get_id();
            cout << "线程 1 添加任务..." << endl;
            pool.AddTask([thdId] {cout << "线程1任务，线程ID:" << thdId << "\t执行线程：" << this_thread::get_id() << endl; });
        }
    });

    std::thread thd2([&pool]
    {
        for (int i = 0; i < 10; i++)
        {
            auto thdId = this_thread::get_id();
            cout << "线程 2 添加任务..." << endl;
            pool.AddTask([thdId]    { cout << "线程2任务，线程ID:" << thdId << "\t执行线程：" << this_thread::get_id() << endl;  });
        }
    });

    thd1.join();
    thd2.join();

    //等待线程池执行30s
    cout << "主线程休眠等待..." << endl;
    this_thread::sleep_for(std::chrono::seconds(30));
    pool.Stop();

    return EXIT_SUCCESS;
}
