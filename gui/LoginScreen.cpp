#include "LoginScreen.h"

const int screenWidth = 1280;
const int screenHeight = 720;

LoginScreen::LoginScreen(Ledger& ledger) 
:   AnimationWindow(0, 30, screenWidth, screenHeight - 30, "DTB Login"),
    ledger(ledger),
    usernameField{{screenWidth/2 - 100, screenHeight/2 - 50}, 200, 30, "Username"},
    passwordField{{screenWidth/2- 100, screenHeight/2}, 200, 30, "Password"},
    loginButton{{screenWidth/2 - 100, screenHeight/2 + 50}, 90, 35, "Log in"},
    registerButton{{screenWidth/2 + 10, screenHeight/2 + 50}, 90, 35, "Register"},
    statusMessage("")
{
    add(usernameField);
    add(passwordField);
    add(loginButton);
    add(registerButton);

    loginButton.setCallback(std::bind(&LoginScreen::handleLogin, this));
    registerButton.setCallback(std::bind(&LoginScreen::handleRegister, this));
    TDT4102::Image dtb_icon("./images/dtb_icon.png");

    while (!should_close()) {
        draw_rectangle({0, 0}, screenWidth, screenHeight, TDT4102::Color::light_gray);
        draw_image({screenWidth/2 - 429*1/3, 100}, dtb_icon, 429*2/3, 211*2/3);
        draw_text({screenWidth/2 - 150, screenHeight/2 - 110}, "Welcome to Den Trønderske Bank", TDT4102::Color::dark_cyan, 20U, TDT4102::Font::arial_bold);
        if (!statusMessage.empty())
            draw_text({screenWidth/2 - static_cast<int>(statusMessage.size() * 4.3), screenHeight/2 + 100}, statusMessage, statusColor);
        next_frame();
    }
}

void LoginScreen::setStatus(std::string newStatusMessage, TDT4102::Color newStatusColor)
{
    statusMessage = newStatusMessage;
    statusColor = newStatusColor;
}

void LoginScreen::handleLogin()
{
    std::string name = usernameField.getText();
    std::string password = passwordField.getText();

    if (ledger.loginUser(name, password)) {
        setStatus("Successfully logged in", TDT4102::Color::green);
        // send user to DashboardScreen
    }
    else {
        setStatus("Wrong username or password", TDT4102::Color::red);
    }
}

void LoginScreen::handleRegister()
{
    std::string name = usernameField.getText();
    std::string password = passwordField.getText();

    try {
        ledger.registerUser(name, password);
        setStatus("Successfully registered user " + name, TDT4102::Color::green);
    } catch (const std::exception& e) {
        setStatus(e.what(), TDT4102::Color::red);
    }
}
