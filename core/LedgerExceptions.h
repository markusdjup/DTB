#pragma once
#include <stdexcept>
#include <string>

// invalid arguments
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
class InsufficientFunds : public std::runtime_error{
public:
    InsufficientFunds() : std::runtime_error("Insufficient funds") {}
};
