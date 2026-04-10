#include "LedgerSerializer.h"
#include "Ledger.h"
#include "./exceptions/FileExceptions.h"
#include "account/CheckingAccount.h"
#include "account/SavingsAccount.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

void LedgerSerializer::save(const Ledger& ledger)
{
    std::filesystem::create_directories("data"); // creates folder named data in root, does nothing if it already exists

    // accounts
    std::ofstream accountFile(accountsPath);
    if (!accountFile) {
        throw FileError(accountsPath);
    }
    accountFile << "type,id,owner,balance,extra\n";
    for (const auto& acc : ledger.getAccounts()) {
        if (const SavingsAccount* sa = dynamic_cast<const SavingsAccount*>(acc.get())) {
            accountFile << "savings,"
                        << sa->getID() << ","
                        << sa->getOwner() << ","
                        << sa->getBalance() << ","
                        << sa->getInterestRate() << "\n";
        }
        else if (const CheckingAccount* ca = dynamic_cast<const CheckingAccount*>(acc.get())) {
            accountFile << "checking,"
                        << ca->getID() << ","
                        << ca->getOwner() << ","
                        << ca->getBalance() << ","
                        << ca->getOverdraftLimit() << "\n";
        }
    }

    // transactions
    std::ofstream transactionLogFile(transactionLogPath);
    if (!transactionLogFile) {
        throw FileError(transactionLogPath);
    }
    transactionLogFile << "type,amount,fromID,toID\n";
    for (const auto& t : ledger.getTransactionLog()) {
        switch (t.type) {
            case Transaction::Type::Deposit: transactionLogFile     << "deposit,";      break;
            case Transaction::Type::Withdrawal: transactionLogFile  << "withdrawal,";   break;
            case Transaction::Type::Transfer: transactionLogFile    << "transfer,";     break;
            case Transaction::Type::Interest: transactionLogFile    << "interest,";     break;
        }
        transactionLogFile  << t.amount << ","
                            << (t.fromID.has_value() ? std::to_string(t.fromID.value()) : "") << ","
                            << (t.toID.has_value() ? std::to_string(t.toID.value()): "") << "\n";
    }
}

void LedgerSerializer::load(Ledger& ledger)
{
    // accounts
    std::ifstream accountFile(accountsPath);
    if (accountFile.is_open()) {    // silently does nothing if accountFile doesn't exist yet
        std::string line;
        std::getline(accountFile, line); // skips header
        while (getline(accountFile, line)) {
            std::stringstream ss(line);
            std::string type, idStr, owner, balanceStr, extraStr;
            std::getline(ss, type, ',');
            std::getline(ss, idStr, ',');
            std::getline(ss, owner, ',');
            std::getline(ss, balanceStr, ',');
            std::getline(ss, extraStr, ',');

            long long id = std::stoll(idStr);
            long long balance = std::stoll(balanceStr);

            if (type == "savings") {
                double interestRate = std::stod(extraStr);
                ledger.accounts.push_back(std::make_unique<SavingsAccount>(id, owner, interestRate, balance));
            }
            else if (type == "checking") {
                long long overdraftLimit = std::stoll(extraStr);
                ledger.accounts.push_back(std::make_unique<CheckingAccount>(id, owner, overdraftLimit, balance));
            }
            else throw CorruptedFileError(accountsPath, type + " is not a valid account type");

            ledger.nextAccountID = std::max(ledger.nextAccountID, id + 1); // eventually sets nextAccountID to the right value
        }
    }

    // transactions
    std::ifstream transactionLogFile(transactionLogPath);
    if (transactionLogFile.is_open()) {     // silently does nothing if transactionLogFile doesn't exist yet
        std::string line;
        std::getline(transactionLogFile, line); // skip header
        while (getline(transactionLogFile, line)) {
            std::stringstream ss(line);
            std::string typeStr, amountStr, fromIDStr, toIDStr;
            std::getline(ss, typeStr, ',');
            std::getline(ss, amountStr, ',');
            std::getline(ss, fromIDStr, ',');
            std::getline(ss, toIDStr, ',');

            Transaction::Type type;
            if (typeStr == "deposit")           type = Transaction::Type::Deposit;
            else if (typeStr == "withdrawal")   type = Transaction::Type::Withdrawal;
            else if (typeStr == "transfer")     type = Transaction::Type::Transfer;
            else if (typeStr == "interest")     type = Transaction::Type::Interest;
            else throw CorruptedFileError(transactionLogPath, typeStr + " is not a valid transaction type");

            long long amount = std::stoll(amountStr);
            std::optional<long long> fromID = fromIDStr.empty() ? std::nullopt : std::optional(stoll(fromIDStr));
            std::optional<long long> toID = toIDStr.empty() ? std::nullopt : std::optional(std::stoll(toIDStr));

            ledger.transactionLog.push_back({type, amount, fromID, toID});
        }
    }
}
