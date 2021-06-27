#include <iostream>
#include <list>
#include <mutex>
#include <algorithm>
#include <thread>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
} 

bool list_contain(int value_to_find) {
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}


int main()
{
    std::thread t1([] {
            for (int i = 0; i < 10; ++i) {
                add_to_list(i);
            }
        });


    for (int i = 9; i >= 0; --i){
        std::cout <<"i: " << i << " flag: " << list_contain(i) << std::endl;
    }
    t1.join();

    std::cout << "Hello world" << std::endl;
    return 0;
}

