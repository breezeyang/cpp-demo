#include <iostream>
#include<string>
#include <future>
#include <sstream>

struct X {
    void foo(int num, std::string const& data) {
        std::stringstream ss;
        ss << "foo num: " << num;
        ss << " data: " << data.data();
        std::cout << ss.str() << std::endl;
    } 
    std::string bar(std::string const& data) {
        std::cout << "bar data: " << data.data() << std::endl;
        return data;
    }
};

struct Y {

    double operator() (double data) {
        std::cout << "Y operator: " << data << std::endl;
        return static_cast<double>(data + 1);
    }
};


class move_only {
public:
    move_only() {
        std::cout << "move_only" << std::endl;
    }
    move_only(move_only&&) {
        std::cout << "move_only&&" << std::endl;
    }
    move_only(move_only const&) = delete;
    move_only& operator=(move_only&&) = delete;
    move_only& operator=(move_only const&) = delete;
    void operator() () {
        std::cout << "move_only operator" << std::endl;
    }
};

X baz(X& x) {
    std::cout << "func baz " << std::endl; 
    return x;
}

int main()
{
    X x;
    auto f1 = std::async(&X::foo, &x, 42, "hello");
    auto f2 = std::async(&X::bar, x, "goodbye");
    std::cout << "f2: " << f2.get() << std::endl;
    Y y;
    auto f3 = std::async(Y(), 3.1415);
    auto f4 = std::async(std::ref(y), 2.718);
    std::cout << "f3: " << f3.get() << std::endl;
    std::cout << "f4: " << f4.get() << std::endl;

    //X baz(X&);
    std::async(baz, std::ref(x));

    auto f5 = std::async(move_only());

    auto f6 = std::async(std::launch::async, Y(), 1.2); // 在新线程中执行
    auto f7 = std::async(std::launch::deferred, baz, std::ref(x)); // 在wait/get时运行

    auto f8 = std::async(std::launch::async | std::launch::deferred, baz, std::ref(x)); // 具体实现来选择
    auto f9 = std::async(baz, std::ref(x)); 
    f7.wait(); //调用延迟函数

    std::cout << "Hello world" << std::endl;
    return 0;
}

