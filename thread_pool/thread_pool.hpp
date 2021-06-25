#pragma once
#include <future> // std::future
#include <vector> // std::vector
#include <queue>  // std::queue
#include <thread> // std::thread
#include <memory> // std::make_shared 
#include <mutex>  // std::mutex
#include <functional> // std::function
#include <type_traits> // std::result_of
#include <condition_variable> // std::condition_variable
#include <iostream>
class thread_pool
{
public:
    thread_pool(int th_nums, int tk_nums)
        :_th_nums(th_nums), _tk_nums(tk_nums) {}

    int start_consume();
    
    template<class F, class... Args>
    auto add_task(F&& f, Args&& ...args) -> std::future<typename std::result_of<F(Args...)>::type>;

    int stop_consume();

    virtual ~thread_pool() {}

private:
    int _th_nums;
    int _tk_nums;
    std::condition_variable _condition;
    std::mutex _mutex;
    std::queue<std::function<void()>> _tasks;
    std::vector<std::thread> _workers;
    bool _stop;
};


int thread_pool::start_consume() {
    for (int i = 0; i < _th_nums; ++i) {
        _workers.emplace_back([this] {
            while (1) {
                std::function<void()> task;
                {

                    std::unique_lock<std::mutex> lock(this->_mutex);
                    this->_condition.wait(lock, [this] {return this->_stop || !this->_tasks.empty(); });
                    if (this->_stop && this->_tasks.empty()) {
                        return ;
                    }
                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();
                    this->_condition.notify_all();
                }
                task();
            }

        });

    }
    return 0;
}

template<class F, class... Args>
auto thread_pool::add_task(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>> (
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                );
    std::future<return_type> fut = task->get_future();
    while (1) {
        std::unique_lock<std::mutex> lock(this->_mutex);
        if (this->_stop) {
            break;
        }
        this->_condition.wait(lock, [this] {return this->_tasks.size() < _tk_nums; } );
        this->_tasks.emplace([task]() { (*task)();});
        std::cout << "tasks: " << this->_tasks.size() << std::endl;
        break;
    }
    this->_condition.notify_all();
    return fut;
}

int thread_pool::stop_consume() {
    {
        std::unique_lock<std::mutex> lock(this->_mutex);
        if (this->_stop) {
            return 0;
        }
        this->_stop = true;
    }
    this->_condition.notify_all();
    for (std::thread& woker : this->_workers) {
        if (woker.joinable()) {
            woker.join();
        }
    }
    return 0;
}
