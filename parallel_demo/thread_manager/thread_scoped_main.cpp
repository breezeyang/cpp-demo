#include <iostream>
#include <thread>

class scoped_thread {
    std::thread t;
public:
    explicit scoped_thread(std::thread t_) : t(std::move(t_)) {

    }

    ~scoped_thread() {
        if (t.joinable()) {
            t.join();
        }
    }
    scoped_thread(scoped_thread const& ) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;
};

int main()
{
    int some_local_state = 0;
    {
        scoped_thread t(std::thread([] { std::cout << "temp thread" << std::endl;}));
    }

    std::cout << "Hello world" << std::endl;
    return 0;
}

