#include "CreateAccountScreen.h"
#include <functional>

const int screenWidth = 1280;
const int screenHeight = 720;

CreateAccountScreen::CreateAccountScreen(Ledger& ledger, User& user)
    : AnimationWindow(0, 30, screenWidth, screenHeight - 30, "New Account"),
      ledger(ledger),
      user(user),
      typeOptions({"Savings account", "Checking account"}),
      typeDropdown{{screenWidth/2 - 205, screenHeight/2 - 100}, 200, 30, typeOptions},
      initialBalanceField{{screenWidth/2 + 5, screenHeight/2 - 100}, 200, 30, "Initial balance (kr)"},
      confirmButton{{screenWidth/2 + 110, screenHeight/2 - 50}, 100, 35, "Create"},
      cancelButton{{screenWidth/2 + 5, screenHeight/2 - 50}, 100, 35, "Cancel"},
      statusMessage(""),
      statusColor(TDT4102::Color::black)
{
    add(typeDropdown);
    add(initialBalanceField);
    add(confirmButton);
    add(cancelButton);

    confirmButton.setCallback(std::bind(&CreateAccountScreen::handleConfirm, this));
    cancelButton.setCallback(std::bind(&CreateAccountScreen::handleCancel, this));

    while (!should_close()) {
        if (nextScreen != "CreateAccount") { close(); }
        draw_rectangle({0, 0}, screenWidth, screenHeight, TDT4102::Color::light_gray);
        std::string header = "Create account";
        draw_text({screenWidth/2 - static_cast<int>(header.size() * 8.6), 200}, header, TDT4102::Color::dark_cyan, 40, TDT4102::Font::arial_bold);
        if (!statusMessage.empty())
            draw_text({screenWidth/2 - static_cast<int>(statusMessage.size() * 4.3), screenHeight/2 + 50}, statusMessage, statusColor);
        next_frame();
    }
    if (nextScreen == "CreateAccount")
        nextScreen = std::nullopt;
}

bool CreateAccountScreen::isSavingsSelected() const
{
    return typeDropdown.getSelectedValue() == "Savings account";
}

void CreateAccountScreen::setStatus(const std::string& message, TDT4102::Color color)
{
    statusMessage = message;
    statusColor   = color;
}

void CreateAccountScreen::handleConfirm()
{
    long long initialBalance = parseKr(initialBalanceField.getText());
    if (initialBalance == -1) {
        setStatus("Invalid balance", TDT4102::Color::red);
        return;
    }
    if (initialBalance < 0) {
        setStatus("Initial balance cannot be negative", TDT4102::Color::red);
        return;
    }
    try {
        if (isSavingsSelected()) {
            double interestRate = 0.05; // 5%
            ledger.createSavingsAccount(user.name, interestRate, initialBalance);
        } else {
            long long overdraftLimit = 1000000; // 1000 kr
            ledger.createCheckingAccount(user.name, overdraftLimit, initialBalance);
        }
        nextScreen = "Dashboard";
    } catch (const std::exception& e) {
        setStatus(e.what(), TDT4102::Color::red);
    }
}

void CreateAccountScreen::handleCancel()
{
    nextScreen = "Dashboard";
}
