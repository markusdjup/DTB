#pragma once
#include <string>

class BankAccount
{
    friend class LedgerSystem;
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
    
    long long getID() const { return id; }
    std::string getOwner() const { return owner; }
    void setOwner(const std::string& newOwner) { owner = newOwner; }
    long long getBalance() const { return balance; }
    virtual bool canWithdraw(long long amount) const { return amount <= balance; }
};
