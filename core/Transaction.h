#pragma once
#include <optional>

struct Transaction
{
    enum class Type {
        Deposit,
        Withdrawal,
        Transfer,
        Interest
    };
    // member variables are const to make transactions immutable
    const Type type; 
    const long long amount; // amount in øre/cents to avoid float-errors
    const std::optional<long long> fromID;
    const std::optional<long long> toID;
};
