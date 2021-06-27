#include <iostream>
#include <mutex>
#include <thread>

class level_mutex {
    std::mutex internal_mutex;
    unsigned long const level_value;
    unsigned long previous_level_value;
    static thread_local unsigned long this_thread_level_value;


    void check_for_level_violation() {
        if (this_thread_level_value <= level_value) {
            throw std::logic_error("mutex level violated");
        }
    }

    void update_level_value() {
        previous_level_value = this_thread_level_value;
        this_thread_level_value = level_value;
    }

public:

    explicit level_mutex(unsigned long value) :level_value(value), previous_level_value(0){

    }

    void lock() {
        check_for_level_violation();
        internal_mutex.lock();
        update_level_value();
    }

    void unlock() {
        this_thread_level_value = previous_level_value;
        internal_mutex.unlock();
    }
    
    bool try_lock() {
        check_for_level_violation();
        if (!internal_mutex.try_lock()) {
            return false;
        }

        update_level_value();
        return true;
    }
}; 

int main()
{
    level_mutex m_h(1000);
    level_mutex m_l(100);
    std::thread t1([&m_h, &m_l] {
            std::lock_guard<level_mutex> lg(m_h);
            std::lock_guard<level_mutex> lh(m_l);
            std::cout << "t1: " << std::this_thread::get_id() << std::endl;
        });

    std::thread t2([&m_h, &m_l] {
            std::lock_guard<level_mutex> ll(m_l);
            std::lock_guard<level_mutex> lh(m_h);
            std::cout << "t2: " << std::this_thread::get_id() << std::endl;
    });

    t1.join();

    t2.join();

    std::cout << "Hello world" << std::endl;
    return 0;
}

