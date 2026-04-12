#pragma once
#include <stdexcept>
#include <string>
#include <algorithm>

inline bool isValidUserName(const std::string& name) 
{
    return name.length() >= 5 // user name must be between 5
    && name.length() <= 100   // and 100 in length
    && std::all_of(name.begin(), name.end(), [](char c) {
        // checks if all chars in name are letters, spaces or dashes (might change these rules)
        return std::isalpha(c) || c == ' ' || c == '-';
    });
}
inline bool isValidPassword(const std::string& name)
{
    return name.length() >= 6 && name.length() <= 30; // might change these rules
}

// invalid arguments
class UserNotInLedger : public std::invalid_argument
{
public:
    UserNotInLedger(const std::string& msg) : std::invalid_argument(msg) {}
};
class UserAlreadyExists : public std::invalid_argument
{
public:
    UserAlreadyExists(const std::string& msg) : std::invalid_argument(msg) {}
};
class InvalidUserName : public std::invalid_argument
{
public:
    InvalidUserName(const std::string& msg) : std::invalid_argument(msg) {}
};
class InvalidPassword : public std::invalid_argument
{
public:
    InvalidPassword(const std::string& msg) : std::invalid_argument(msg) {}
};
class AccountNotInLedger : public std::invalid_argument
{
public:
    AccountNotInLedger(const std::string& msg) : std::invalid_argument(msg) {}
};
class InvalidAmount : public std::invalid_argument
{
public:
    InvalidAmount(const std::string& msg) : std::invalid_argument(msg) {}
};

// runtime errors
class InsufficientFunds : public std::runtime_error
{
public:
    InsufficientFunds() : std::runtime_error("Insufficient funds") {}
};
