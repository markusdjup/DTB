#include "DashboardScreen.h"
#include "CreateAccountScreen.h"
#include "Utils.h"
#include "../core/account/SavingsAccount.h"
#include "../core/account/CheckingAccount.h"

const int screenWidth = 1280;
const int screenHeight = 720;

DashboardScreen::DashboardScreen(Ledger& ledger, User& user)
:   AnimationWindow(0, 30, screenWidth, screenHeight - 30, "DTB Dashboard - " + user.name),
    ledger(ledger),
    user(user),
    userAccounts(ledger.getAccountsForUser(user.name)),
    accountLabels(buildAccountLabels()),
    
    accountDropdown{{screenWidth/2 - 265, screenHeight/2 - 200}, 200, 30, accountLabels},
    amountField{{screenWidth/2 - 55, screenHeight/2 - 200}, 120, 30, "Amount (kr)"},
    depositButton{{screenWidth/2 + 75, screenHeight/2 - 200},  90, 30, "Deposit"},
    withdrawButton{{screenWidth/2 + 175, screenHeight/2 - 200},  90, 30, "Withdraw"},
    toAccountIDField{{screenWidth/2 - 55, screenHeight/2 - 150},  200, 30, "Transfer to ID"},
    transferButton{{screenWidth/2 + 175, screenHeight/2 - 150}, 90, 30, "Transfer"},
    newAccountButton{{screenWidth - 260, 10}, 120, 30, "New Account"},
    logoutButton{{screenWidth - 130, 10}, 120, 30, "Log out"},
    statusMessage(""),
    statusColor(TDT4102::Color::black)
{
    add(accountDropdown);
    add(amountField);
    add(toAccountIDField);
    add(depositButton);
    add(withdrawButton);
    add(transferButton);
    add(newAccountButton);
    add(logoutButton); 

    depositButton.setCallback(std::bind(&DashboardScreen::handleDeposit, this));
    withdrawButton.setCallback(std::bind(&DashboardScreen::handleWithdraw, this));
    transferButton.setCallback(std::bind(&DashboardScreen::handleTransfer, this));
    newAccountButton.setCallback(std::bind(&DashboardScreen::handleNewAccount, this));
    logoutButton.setCallback(std::bind(&DashboardScreen::handleLogout, this));

    while (!should_close()) {
        if (nextScreen != "Dashboard") { close(); }
        draw_rectangle({0, 0}, screenWidth, screenHeight, TDT4102::Color::light_gray);
        std::string header = "Logged in as: " + user.name;
        draw_text({screenWidth/2 - static_cast<int>(header.size() * 4.3), 10}, header, TDT4102::Color::green, 20U, TDT4102::Font::arial_bold);
        drawAccountList();
        if (!statusMessage.empty())
            draw_text({screenWidth/2 - static_cast<int>(statusMessage.size() * 4.3), screenHeight/2 - 100} , statusMessage, statusColor);
        next_frame();
    }
    if (nextScreen == "Dashboard")
        nextScreen = std::nullopt;
}

std::vector<std::string> DashboardScreen::buildAccountLabels() const
{
    std::vector<std::string> labels;
    for (const BankAccount* acc : userAccounts) {
        std::string type = dynamic_cast<const SavingsAccount*>(acc) ? "Sav" : "Chk";
        labels.push_back("#" + std::to_string(acc->getID()) + " " + type + " " + formatAmount(acc->getBalance()));
    }
    if (labels.empty())
        labels.push_back("No accounts found");
    return labels;
}

void DashboardScreen::refreshAccounts()
{
    userAccounts = ledger.getAccountsForUser(user.name);
    accountLabels = buildAccountLabels();
    accountDropdown.setOptions(accountLabels);
}

BankAccount* DashboardScreen::getSelectedAccount() const
{
    std::string selected = accountDropdown.getSelectedValue();
    for (BankAccount* acc : userAccounts) {
        if (selected.find("#" + std::to_string(acc->getID())) != std::string::npos)
            return acc;
    }
    return nullptr;
}

long long DashboardScreen::parseAmount() const
{
    return parseKr(amountField.getText());
}

void DashboardScreen::setStatus(const std::string& newStatusMessage, TDT4102::Color newStatusColor)
{
    statusMessage = newStatusMessage;
    statusColor = newStatusColor;
}

void DashboardScreen::drawAccountList()
{
    std::string line = "Your accounts";
    draw_text({screenWidth/2 - 55, screenHeight/2 - 90}, line, TDT4102::Color::dark_cyan, 30U, TDT4102::Font::arial_bold);
    int y = screenHeight/2 - 50;
    for (const BankAccount* acc : userAccounts) {
        std::string type = dynamic_cast<const SavingsAccount*>(acc) ? "Savings account  " : "Checking account";
        std::string line = "#" + std::to_string(acc->getID()) + " | " + type + " | " + formatAmount(acc->getBalance());
        draw_text({screenWidth/2 - 55, y}, line, TDT4102::Color::black);
        y += 22;
    }
}

void DashboardScreen::handleDeposit()
{
    BankAccount* acc = getSelectedAccount();
    long long amount = parseAmount();
    if (!acc) {
        setStatus("No account selected", TDT4102::Color::red);
        return;
    }
    if (amount == -1) { // parseAmount caught an exception
        setStatus("Invalid amount", TDT4102::Color::red);
        return;
    }
    try {
        ledger.deposit(*acc, amount);
        refreshAccounts();
        setStatus("Deposited " + formatAmount(amount) + " to #" + std::to_string(acc->getID()), TDT4102::Color::green);
    } catch (const std::exception& e) {
        setStatus(e.what(), TDT4102::Color::red);
    }
}

void DashboardScreen::handleWithdraw()
{
    BankAccount* acc = getSelectedAccount();
    long long amount = parseAmount();
    if (!acc) {
        setStatus("No account selected", TDT4102::Color::red);
        return;
    }
    if (amount == -1) { // parseAmount caught an exception
        setStatus("Invalid amount", TDT4102::Color::red);
        return;
    }
    try {
        ledger.withdraw(*acc, amount);
        refreshAccounts();
        setStatus("Withdrew " + formatAmount(amount) + " from #" + std::to_string(acc->getID()), TDT4102::Color::green);
    } catch (const std::exception& e) {
        setStatus(e.what(), TDT4102::Color::red);
    }
}

void DashboardScreen::handleTransfer()
{
    BankAccount* from = getSelectedAccount();
    long long amount = parseAmount();
    if (!from) {
        setStatus("No source account selected", TDT4102::Color::red);
        return;
    }
    if (amount == -1) { // parseAmount caught an exception
        setStatus("Invalid amount", TDT4102::Color::red);
        return;   
    }

    long long toID = -1;
    try {
        toID = std::stoll(toAccountIDField.getText());
    } catch (...) {} // if an exception occurs, toID remains -1
    BankAccount* to = ledger.getAccountByID(toID); // getAccountByID returns nullptr if the ID is invalid
    if (to == nullptr || !(to->getOwner() == user.name)) {
        setStatus("Invalid destination account ID", TDT4102::Color::red);
        return;
    }
    if (from == to) {
        setStatus("Cannot transfer to the same account", TDT4102::Color::red);
        return;
    }
    try {
        ledger.transfer(*from, *to, amount);
        refreshAccounts();
        setStatus("Transferred " + formatAmount(amount) + " from #" + std::to_string(from->getID()) + " to #" + std::to_string(to->getID()), TDT4102::Color::green);
    } catch (const std::exception& e) {
        setStatus(e.what(), TDT4102::Color::red);
    }
}

void DashboardScreen::handleNewAccount()
{
    nextScreen = "CreateAccount";
}

void DashboardScreen::handleLogout()
{
    nextScreen = "Login";
}
