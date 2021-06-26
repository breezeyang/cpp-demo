#include <iostream>
#include <thread>

/*
 * my_thread线程调用了detach，决定不等待该线程的执行了
 * 当main线程退出的时候，my_thread可能还没有结束，所以会存在访问一个已被销毁的变量
 * */
struct func {

    int& i;
    
    func(int& i_): i(i_) {}

    void operator() () {
        for (unsigned j = 0; j < 1000000; ++j) {
            // do something
            i += 1;
        }
    }

};

int main()
{
    // local var used detach thread error !!!
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach();
    std::cout << "Hello world" << std::endl;
    return 0;
}

