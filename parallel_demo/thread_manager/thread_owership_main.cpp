#include <iostream>
#include <thread>

void func() {
    std::cout << "func" << std::endl;
}

void func2() {
    std::cout << "func2" << std::endl;
}

int main()
{
    std::thread t1(func);
    std::thread t2 = std::move(t1);
    t1 = std::thread(func2);
    std::thread t3;
    t3 = std::move(t2);
    // t1 = std::move(t3); // t1 已经存在一个关联线程,所以会挂掉
    if (t1.joinable()) {
        t1.join();

    }
    if (t2.joinable()) {
        t2.join();
    }
    if (t3.joinable()) {
        t3.join();
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

