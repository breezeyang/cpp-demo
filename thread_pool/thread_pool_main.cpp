#include <iostream>
#include <algorithm>
#include "thread_pool.hpp"
int main()
{
    int ret = 0;
    int N = 10;
    thread_pool pool(1, 3);
    std::vector<std::future<int>> result_futs;
    std::cout << "pool.start_consume" << std::endl;
    ret = pool.start_consume();
    std::cout << "add task start" << std::endl;
    for (int i = 0; i < N; ++i) {
        result_futs.emplace_back(
            pool.add_task([i] () {
                return i + i;
        }));
    }
    std::cout << "add task end" << std::endl;
    int result = 0;
    for (int i = 0; i < N; ++i) {
        int tmp  = result_futs[i].get();
        std::cout << "i : " << tmp << std::endl; 
        result += tmp;
    }
    std::cout << "result: " << result << std::endl;
    std::cout << "pool.stop_consum" << std::endl;
    pool.stop_consume();
    return 0;
}

