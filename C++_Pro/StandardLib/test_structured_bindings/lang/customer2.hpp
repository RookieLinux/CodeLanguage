//
// Created by rookie on 2025/1/6.
//

#ifndef TEST_STRUCTURED_BINDINGS_CUSTOMER2_HPP
#define TEST_STRUCTURED_BINDINGS_CUSTOMER2_HPP
//实现 tuple-like API 时可以时候用返回 non-const 引用。这样结构化绑定就变得可写。
#include <string>
#include <utility>
// for std::move()
class Customer {
private:
    std::string first;
    std::string last;
    long val;
public:
    Customer (std::string f, std::string l, long v)
            : first{std::move(f)}, last{std::move(l)}, val{v} {
    }
    const std::string& firstname() const {
        return first;
    }
    std::string& firstname() {
        return first;
    }
    const std::string& lastname() const {
        return last;
    }

    std::string& lastname() {
        return last;
    }

    long value() const {
        return val;
    }
    long& value() {
        return val;
    }
};

#endif //TEST_STRUCTURED_BINDINGS_CUSTOMER2_HPP
