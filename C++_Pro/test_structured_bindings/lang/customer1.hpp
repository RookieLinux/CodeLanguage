//
// Created by rookie on 2025/1/6.
//

#ifndef TEST_STRUCTURED_BINDINGS_CUSTOMER1_HPP
#define TEST_STRUCTURED_BINDINGS_CUSTOMER1_HPP

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
    std::string getFirst() const {
        return first;
    }
    std::string getLast() const {
        return last;
    }
    long getValue() const {
        return val;
    }
};

#endif //TEST_STRUCTURED_BINDINGS_CUSTOMER1_HPP
