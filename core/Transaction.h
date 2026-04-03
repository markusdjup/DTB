#pragma once

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
    const long long fromID;
    const long long toID;
};
