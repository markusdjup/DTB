#pragma once
#include <stdexcept>
#include <string>
#include <algorithm>

bool isValidOwnerName(const std::string& name) 
{
    return name.length() >= 3 
    && name.length() <= 100 
    && std::all_of(name.begin(), name.end(), [](char c) {
        return std::isalpha(c) || c == ' ' || c == '-';
    });
}

// invalid arguments
class AccountNotInLedger : public std::invalid_argument
{
public:
    AccountNotInLedger(const std::string& msg) : std::invalid_argument(msg) {}
};
class InvalidOwnerName : public std::invalid_argument
{
public:
    InvalidOwnerName() : std::invalid_argument("Invalid owner name") {}
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
