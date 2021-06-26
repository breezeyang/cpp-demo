#include <iostream>
#include <thread>

void print() 
{
    auto tid = std::this_thread::get_id();
    std::cout << "tid: " << tid << " something" << std::endl;
}

/*
 * 注意所提供的函数对象被复制到属于新创建的执行线程的存储器中，
 * 并从那里开始调用，副本与原版有着等效的行为,否则结果可能会与预期不符
 */
void print() 

class background_task 
{
public:
    void operator() () const {
        print();
    }
};

int main()
{
    auto main_tid = std::this_thread::get_id();
    std::cout << "main tid: " << main_tid << std::endl;
    // way 1 success
    /*
    background_task f;
    std::thread my_thread(f);
    */

    // way 2 error 
    // std::thread my_thread(background_task());

    // way 3 success 
    // std::thread my_thread((background_task()));

    // way 4 success 
    // std::thread my_thread{background_task()};

    // way 5 success 
    std::thread my_thread([] {
            print();
        });
    my_thread.join();
    return 0;
}

