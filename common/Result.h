#pragma once

#include <optional>
#include <stdexcept>
#include <utility>

template <typename T, typename E = std::runtime_error>
class Result
{
public:
    Result(T v) : val_(std::move(v)) {}

    Result(E e) : err_(std::move(e)) {}

    bool is_err() const { return err_.has_value(); }

    bool is_okay() const { return val_.has_value(); }

    const E &error() const
    {
        if (is_okay())
        {
            throw std::runtime_error("Accessing error from a Success result");
        }
        return *err_;
    }

    const T &value() const
    {
        if (is_err())
        {
            throw std::runtime_error("Attempt to access value from an Error Result");
        }
        return *val_;
    }

private:
    std::optional<T> val_;
    std::optional<E> err_;
};
