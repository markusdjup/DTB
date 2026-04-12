#include "CreateAccountScreen.h"
#include <functional>

const int screenWidth = 1280;
const int screenHeight = 720;

CreateAccountScreen::CreateAccountScreen(Ledger& ledger, const std::string& username)
    : AnimationWindow(0, 30, screenWidth, screenHeight - 30, "New Account"),
      ledger(ledger),
      username(username),
      typeOptions({"Savings", "Checking"}),
      typeDropdown{{screenWidth/2 - 205, screenHeight/2 - 100},  200, 30, typeOptions},
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
    cancelButton.setCallback([this]() { close(); });

    while (!should_close()) {
        draw_rectangle({0, 0}, screenWidth, screenHeight, TDT4102::Color::light_gray);
        std::string header = "Create account";
        draw_text({screenWidth/2 - static_cast<int>(header.size() * 8.6), 200}, header, TDT4102::Color::dark_cyan, 40, TDT4102::Font::arial_bold);
        
        if (!statusMessage.empty())
            draw_text({screenWidth/2 - static_cast<int>(statusMessage.size() * 4.3), screenHeight/2 + 50}, statusMessage, statusColor);
        next_frame();
    }
}

bool CreateAccountScreen::isSavingsSelected() const
{
    return typeDropdown.getSelectedValue() == "Savings";
}

void CreateAccountScreen::setStatus(const std::string& message, TDT4102::Color color)
{
    statusMessage = message;
    statusColor   = color;
}

void CreateAccountScreen::handleConfirm()
{
    long long initialBalance = 0;
    try {
        size_t pos;
        initialBalance = std::stoll(initialBalanceField.getText(), &pos);
        if (pos != initialBalanceField.getText().size()) {
            setStatus("Invalid balance.", TDT4102::Color::red);
            return;
        }
    } catch (...) {
        setStatus("Invalid balance.", TDT4102::Color::red);
        return;
    }

    try {
        if (isSavingsSelected()) {
            double interestRate = 0.05; // 15%
            ledger.createSavingsAccount(username, interestRate, initialBalance);
        } else {
            long long overdraftLimit = 1000000; // 1000 kr
            ledger.createCheckingAccount(username, overdraftLimit, initialBalance);
        }
        close();
    } catch (const std::exception& e) {
        setStatus(e.what(), TDT4102::Color::red);
    }
}
