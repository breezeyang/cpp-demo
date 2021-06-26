#include <iostream>
#include <thread>

class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_) : t(t_){
    }

    ~thread_guard() {
        if (t.joinable()) {
            t.join();
        }
    }
    thread_guard(thread_guard const &) = delete;
    thread_guard& operator = (thread_guard const &) = delete;
};
int main()
{
    int some_local_state = 0;
    std::thread t([&some_local_state] {
            some_local_state += 1;
            std::cout << "thread t some_local_state: " << some_local_state << std::endl;
        });
    {
        thread_guard guard(t);
    }
    std::cout << "thread main some_local_state: " << some_local_state << std::endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

