#pragma once
#include <string>

class Ledger;

class LedgerSerializer 
{
private:
    static const std::string usersPath;
    static const std::string accountsPath;
    static const std::string transactionLogPath;

public:
    static void save(const Ledger& ledger);
    static void load(Ledger& ledger);
};
