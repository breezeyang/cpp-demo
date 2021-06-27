#include <iostream>
#include <mutex>

class X {
private:
    int a;
    std::mutex m;
public:
    X(int a_): a(a_) {

    }

    friend void swap(X& lhs, X& rhs) {
        if (&rhs == &lhs) {
            return ;
        }
        /*
        std::lock(lhs.m, rhs.m);
        // std::adopt告知std::lock_guard对象该互斥元已被锁定，并且他们只沿用互斥元上已有的所有权
        // 而不是试图在构造函数中锁定互斥元
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(lhs.m, std::adopt_lock);
        */
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
        std::unique_lock<std::mutex> lock_b(lhs.m, std::defer_lock);
        std::lock(lhs.m, rhs.m);
        int tmp = rhs.a;
        rhs.a = lhs.a;
        lhs.a = tmp;
    }

    int value() {
        return this->a;
    }

};
int main()
{
    X a(1);
    X b(2);
    swap(a, b);
    std::cout << "a: " << a.value() << std::endl;
    std::cout << "b: " << b.value() << std::endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

