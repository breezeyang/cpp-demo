#include <iostream>
#include <future>
#include <unistd.h>
#include <sstream>

int find_the_answer() {
    std::stringstream ss;
    ss << "thread_id: " << std::this_thread::get_id();
    std::cout << ss.str() << std::endl;
    sleep(1);
    return 100;
}

int main()
{
    std::stringstream ss;
    ss << "main thread_id:" << std::this_thread::get_id();
    std::cout << ss.str() << std::endl;

    std::future<int> the_answer(std::async(find_the_answer));
    auto answer = the_answer.get();
    std::stringstream ss1;
    ss1 << "answer: " << answer;
    std::cout << ss1.str() << std::endl;
    return 0;
}

