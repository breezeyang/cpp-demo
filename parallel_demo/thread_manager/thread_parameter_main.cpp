#include <iostream>
#include <string>
#include <sstream>
#include <thread>

void f(int i, std::string const& s) {
    std::stringstream ss;
    ss << "i: " << i << "\t";
    ss << "s: " << s.data();
    std::cout << ss.str() << std::endl; 
}

void f2(int i, std::string & s) {
    std::stringstream ss;
    ss << "i: " << i << "\t";
    ss << "s: " << s.data();
    std::cout << ss.str() << std::endl; 
    s = "456";
}

void way1() {
    // way 1 success
    std::thread t(f, 3, "hello");

    t.join();
    std::cout << "Hello world" << std::endl;
}

void way2() {
    // way 2 error
    // 将局部变量buffer的指针传递给新线程，当函数会在缓冲在新线程上被转换为std::string之前退出，从而导致未定义的行为
    // 解决的办法是在将缓冲传递给std::thread的构造函数之前转化为std::string
    char buffer[1024];
    sprintf(buffer, "%i", 2);
    
    // std::thread t(f, 3, buffer); // error
    std::thread t(f, 3, std::string(buffer)); // 这里依赖std::string的隐式转化

    t.detach();
}

void way3() {
    // way 3
    std::string data = "123"; 
    //std::thread t(f2, 3, data); // compile error
    std::thread t(f2, 3, std::ref(data)); // success
    t.join();
    std::cout << data.data() << std::endl;
}
int main()
{
    way3();
    return 0;
}

