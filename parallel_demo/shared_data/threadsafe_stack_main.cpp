#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack : std::exception {
    const char* what() const throw ();
};


template<typename T>
class threadsafe_stack {
private:
    std::stack<T> data;
    // mutable 作用是const 方法中，可以对变量进行修改
    mutable std::mutex m;

public:
    threadsafe_stack() {}
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator = (const threadsafe_stack& ) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) {
            throw empty_stack();
        }
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }

};
int main()
{
    threadsafe_stack<int> stack;
    stack.push(static_cast<int>(1));
    stack.empty();

    std::cout << "Hello world" << std::endl;
    return 0;
}

