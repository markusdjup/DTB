#pragma once
#include <string>

class BankAccount
{
    friend class Ledger;
private:
    void setBalance(long long newBalance) { balance = newBalance; }
protected:
    const long long id;
    long long balance;  // balance in øre/cents to avoid float errors
    std::string owner;
public:
    BankAccount(long long id, const std::string& initialOwner, long long initialBalance = 0)
    : id(id), balance(initialBalance), owner(initialOwner) {}
    virtual ~BankAccount() {}
    
    long long getID() const;
    std::string getOwner() const;
    void setOwner(const std::string& newOwner);
    long long getBalance() const;
    virtual bool canWithdraw(long long amount) const;
};
