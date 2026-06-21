// user.cpp
#include "FileManager.h"
#include "user.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

void UserModule::run() {
    while (true) {
        cout << "\n========== User Center ==========" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            return;
        }
        else if (choice == 1) {
            handleRegister();
        }
        else if (choice == 2) {
            if (handleLogin()) {
                showMainMenu();
            }
        }
        else {
            cout << "Invalid input.\n";
        }
    }
}

void UserModule::handleRegister() {
    cout << "\n========== Register ==========" << endl;
    cout << "(Enter 0 at any prompt to cancel)\n";

    string userName, dormitory;
    cout << "Enter username: ";
    getline(cin, userName);

    if (userName == "0") {
        cout << "Registration cancelled.\n";
        return;
    }

    vector<User> users = fm.loadUsers("users.txt");
    for (const User& u : users) {
        if (u.userName == userName) {
            cout << "Username already exists.\n";
            return;
        }
    }

    cout << "Enter dormitory: ";
    getline(cin, dormitory);

    if (dormitory == "0") {
        cout << "Registration cancelled.\n";
        return;
    }

    fm.saveUser("users.txt", userName, dormitory);
    cout << "Registration successful! Please login.\n";
}

bool UserModule::handleLogin() {
    cout << "\n========== Login ==========" << endl;
    cout << "(Enter 0 to cancel)\n";

    string userName;
    cout << "Enter username: ";
    getline(cin, userName);

    if (userName == "0") {
        cout << "Login cancelled.\n";
        return false;
    }

    vector<User> users = fm.loadUsers("users.txt");
    for (const User& u : users) {
        if (u.userName == userName) {
            currentUser = u;
            cout << "Login successful! Welcome, " << currentUser.userName << "!\n";
            return true;
        }
    }

    cout << "User not found.\n";
    return false;
}

void UserModule::showMainMenu() {
    while (true) {
        cout << "\n========== User Menu ==========" << endl;
        cout << "Current user: " << currentUser.userName << endl;
        cout << "1. Browse merchants & order\n";
        cout << "2. View order history\n";
        cout << "3. Check order status\n";
        cout << "4. Cancel order\n";
        cout << "0. Logout\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: handleBrowseMerchants(); break;
        case 2: handleViewOrders(); break;
        case 3: handleViewOrderStatus(); break;
        case 4: handleCancelOrder(); break;
        case 0: cout << "Logged out.\n"; return;
        default: cout << "Invalid.\n";
        }
    }
}

void UserModule::handleBrowseMerchants() {
    cout << "\n========== Merchant List ==========" << endl;

    vector<MenuItem> menu = fm.loadMenu("menus.txt");

    if (menu.empty()) {
        cout << "No merchants available.\n";
        return;
    }

    vector<int> merchantIds;
    for (const MenuItem& item : menu) {
        if (find(merchantIds.begin(), merchantIds.end(), item.merchantId) == merchantIds.end()) {
            merchantIds.push_back(item.merchantId);
        }
    }

    for (int i = 0; i < (int)merchantIds.size(); i++) {
        cout << i + 1 << ". Merchant " << merchantIds[i] << endl;
    }
    cout << "0. Back\n";
    cout << "Select merchant: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) return;

    if (choice < 1 || choice >(int)merchantIds.size()) {
        cout << "Invalid.\n";
        return;
    }

    int selectedMerchantId = merchantIds[choice - 1];
    handlePlaceOrder(selectedMerchantId);
}

void UserModule::handlePlaceOrder(int merchantId) {
    cout << "\n========== Merchant " << merchantId << " Menu ==========" << endl;

    vector<MenuItem> menu = fm.loadMenu("menus.txt");

    vector<MenuItem> merchantMenu;
    for (const MenuItem& item : menu) {
        if (item.merchantId == merchantId) {
            merchantMenu.push_back(item);
        }
    }

    if (merchantMenu.empty()) {
        cout << "This merchant has no items.\n";
        return;
    }

    for (int i = 0; i < (int)merchantMenu.size(); i++) {
        cout << i + 1 << ". " << merchantMenu[i].name
            << "  $" << merchantMenu[i].price << endl;
    }
    cout << "0. Back\n";
    cout << "Select item: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) return;

    if (choice < 1 || choice >(int)merchantMenu.size()) {
        cout << "Invalid.\n";
        return;
    }

    MenuItem selectedItem = merchantMenu[choice - 1];

    vector<Order> orders = fm.loadOrders("orders.txt");
    int maxId = 0;
    for (const Order& o : orders) {
        if (o.orderId > maxId) maxId = o.orderId;
    }
    int newOrderId = maxId + 1;

    Order newOrder;
    newOrder.orderId = newOrderId;
    newOrder.userId = currentUser.userId;
    newOrder.merchantId = merchantId;
    newOrder.itemName = selectedItem.name;
    newOrder.status = "pending";

    if (fm.saveOrder("orders.txt", newOrder)) {
        cout << "\nOrder placed successfully!\n";
        printOrderInfo(newOrder);
    }
    else {
        cout << "Order failed.\n";
    }
}

void UserModule::handleViewOrders() {
    cout << "\n========== Order History ==========" << endl;

    vector<Order> orders = fm.loadOrders("orders.txt");

    vector<Order> myOrders;
    for (const Order& o : orders) {
        if (o.userId == currentUser.userId) {
            myOrders.push_back(o);
        }
    }

    if (myOrders.empty()) {
        cout << "No orders.\n";
        return;
    }

    for (const Order& o : myOrders) {
        printOrderInfo(o);
        cout << "-----------------------------\n";
    }
}

void UserModule::handleViewOrderStatus() {
    cout << "\n========== Check Order Status ==========" << endl;

    vector<Order> orders = fm.loadOrders("orders.txt");

    vector<Order> myOrders;
    for (const Order& o : orders) {
        if (o.userId == currentUser.userId) {
            myOrders.push_back(o);
        }
    }

    if (myOrders.empty()) {
        cout << "No orders.\n";
        return;
    }

    for (int i = 0; i < (int)myOrders.size(); i++) {
        cout << i + 1 << ". Order #" << myOrders[i].orderId
            << " | " << myOrders[i].itemName
            << " | Status: " << myOrders[i].status << endl;
    }
    cout << "0. Back\n";
    cout << "Select order for details: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) return;

    if (choice < 1 || choice >(int)myOrders.size()) {
        cout << "Invalid.\n";
        return;
    }

    cout << "\n========== Order Details ==========" << endl;
    printOrderInfo(myOrders[choice - 1]);
}

void UserModule::handleCancelOrder() {
    cout << "\n========== Cancel Order ==========" << endl;

    vector<Order> orders = fm.loadOrders("orders.txt");

    vector<Order> cancelableOrders;
    for (const Order& o : orders) {
        if (o.userId == currentUser.userId && o.status == "pending") {
            cancelableOrders.push_back(o);
        }
    }

    if (cancelableOrders.empty()) {
        cout << "No orders can be cancelled (only 'pending' orders).\n";
        return;
    }

    for (int i = 0; i < (int)cancelableOrders.size(); i++) {
        cout << i + 1 << ". Order #" << cancelableOrders[i].orderId
            << " | " << cancelableOrders[i].itemName << endl;
    }
    cout << "0. Back\n";
    cout << "Select order to cancel: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) return;

    if (choice < 1 || choice >(int)cancelableOrders.size()) {
        cout << "Invalid.\n";
        return;
    }

    int orderId = cancelableOrders[choice - 1].orderId;

    if (fm.updateOrderStatus("orders.txt", orderId, "cancelled")) {
        cout << "Order #" << orderId << " cancelled.\n";
    }
    else {
        cout << "Cancellation failed.\n";
    }
}

void UserModule::printOrderInfo(const Order& order) {
    cout << "Order ID: " << order.orderId << endl;
    cout << "Item: " << order.itemName << endl;
    cout << "Status: " << order.status << endl;
}