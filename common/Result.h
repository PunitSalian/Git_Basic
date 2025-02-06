#pragma once 

#include <optional>
#include <exception>
#include <stdexcept> 

template <typename T, typename E>
class Result {
public:

Result(const T& v) : is_ok(true), val_(v) {}
Result(const E& e) : is_ok(false), err_(e) {}

bool is_err() const { return !is_ok;}
bool is_okay() const { return is_ok;} 

E error() {
    if (is_okay()) {
        throw std::runtime_error("Accessing error from a Success result");
    }

    return err_;
}

T value() {
    if (is_err()) {
        throw std::runtime_error(" Attempt to access Result from Error Result");
    }

    return val_;
}

private:
bool is_ok;
E err_;
T val_;

};