#include <unistd.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <deque>
#include <future>


std::mutex m;
std::deque<std::packaged_task<void()>> tasks;
volatile bool stop = false;

bool gui_showdown_message_received() {
    std::lock_guard<std::mutex> lk(m);
    return stop && tasks.empty();
}

void get_and_process_gui_message() {
    //std::cout << "get_and_process_gui_message" << std::endl;

}

void gui_thread() {
    while (!gui_showdown_message_received()) {

        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if (tasks.empty()) {
                continue;
            }

            task = std::move(tasks.front());
            tasks.pop_front();
        }
        task();
    }
    std::cout << "gui_thread stop" << std::endl;
}

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f) {
    std::packaged_task<void()> task(f);
    std::future<void> res = task.get_future();
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
}

struct print {
    void operator() () {
        std::cout << "print " << std::endl;
    }
};

int main()
{
    std::thread t1(gui_thread);
    std::vector<std::future<void>> results;
    for (int i = 0; i < 10; ++i) {
        results.emplace_back(post_task_for_gui_thread(print()));
    }
    //sleep(1);
    stop = true;
    t1.join();
    std::cout << "Hello world" << std::endl;
    return 0;
}

