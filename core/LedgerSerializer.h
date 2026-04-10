#pragma once
#include <string>

class Ledger;

namespace LedgerSerializer 
{
    const std::string accountsPath = "data/accounts.csv";
    const std::string transactionLogPath = "data/transactionLog.csv";

    void save(const Ledger& ledger);
    void load(Ledger& ledger);
}
