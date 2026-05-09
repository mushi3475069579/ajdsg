#pragma once
#include <exception>
#include <string>

class NotEnoughHpException : public std::exception {
private:
    std::string message_;
public:
    explicit NotEnoughHpException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};

class InvalidMoveException : public std::exception {
private:
    std::string message_;
public:
    explicit InvalidMoveException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};
