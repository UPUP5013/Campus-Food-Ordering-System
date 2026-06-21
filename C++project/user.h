// user.h
#pragma once
#pragma execution_character_set("utf-8")
#include "FileManager.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class UserModule {
private:
    FileManager fm;
    User currentUser;

    void showMainMenu();
    void handleRegister();
    bool handleLogin();
    void handleBrowseMerchants();
    void handleViewOrders();
    void handleViewOrderStatus();
    void handleCancelOrder();
    void handlePlaceOrder(int merchantId);
    void printOrderInfo(const Order& order);

public:
    void run();
};