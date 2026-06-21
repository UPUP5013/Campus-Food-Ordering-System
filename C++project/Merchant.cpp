// Merchant.cpp
#pragma execution_character_set("utf-8")
#include "Merchant.h"

// ========== 构造函数 ==========
Merchant::Merchant(int id, const string& name) : merchantId(id), merchantName(name) {}

Merchant::~Merchant() {}

int Merchant::getId() { return merchantId; }

string Merchant::getName() { return merchantName; }

GeneralMerchant::GeneralMerchant(int id, const string& name) : Merchant(id, name) {}

// ========== 商家主菜单 ==========
void GeneralMerchant::handleOrders(FileManager& fm) {
    int choice;
    while (true) {
        cout << "\n========== Merchant Menu ==========" << endl;
        cout << "1. Show pending orders" << endl;
        cout << "2. Show cooking orders" << endl;
        cout << "3. Accept order" << endl;
        cout << "4. Mark as cooking" << endl;
        cout << "5. Mark as ready for pickup" << endl;
        cout << "6. Menu management" << endl;
        cout << "7. Return to main menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: showPendingOrders(fm); break;
        case 2: showPreparingOrders(fm); break;
        case 3: acceptOrder(fm); break;
        case 4: markAsPreparing(fm); break;
        case 5: markAsReady(fm); break;
        case 6: manageMenu(fm); break;
        case 7: return;
        default: cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// ========== 显示待接单订单 ==========
void GeneralMerchant::showPendingOrders(FileManager& fm) {
    vector<Order> orders = fm.loadOrders("orders.txt");
    cout << "\n========== Pending Orders ==========" << endl;
    bool found = false;
    for (const auto& order : orders) {
        if (order.merchantId == merchantId && order.status == "pending") {
            cout << "Order ID: " << order.orderId
                << " | User ID: " << order.userId
                << " | Item: " << order.itemName << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No pending orders." << endl;
    }
}

// ========== 显示制作中订单 ==========
void GeneralMerchant::showPreparingOrders(FileManager& fm) {
    vector<Order> orders = fm.loadOrders("orders.txt");
    cout << "\n========== Cooking Orders ==========" << endl;
    bool found = false;
    for (const auto& order : orders) {
        if (order.merchantId == merchantId && order.status == "cooking") {
            cout << "Order ID: " << order.orderId
                << " | User ID: " << order.userId
                << " | Item: " << order.itemName << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No cooking orders." << endl;
    }
}

// ========== 接单（pending → cooking） ==========
void GeneralMerchant::acceptOrder(FileManager& fm) {
    vector<Order> orders = fm.loadOrders("orders.txt");

    // 显示属于本商家的待接单订单
    cout << "\n========== Pending Orders ==========" << endl;
    bool found = false;
    for (const auto& order : orders) {
        if (order.merchantId == merchantId && order.status == "pending") {
            cout << "Order ID: " << order.orderId
                << " | User ID: " << order.userId
                << " | Item: " << order.itemName << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No pending orders." << endl;
        return;
    }

    cout << "(Enter 0 to cancel)\n";
    cout << "Enter the order ID to accept: ";
    int orderId;
    cin >> orderId;

    if (orderId == 0) {
        cout << "Accept cancelled.\n";
        return;
    }
    // 【修正】验证订单必须属于本商家，且状态为 pending
    bool valid = false;
    for (const auto& order : orders) {
        if (order.orderId == orderId && order.merchantId == merchantId && order.status == "pending") {
            valid = true;
            break;
        }
    }

    if (valid && fm.updateOrderStatus("orders.txt", orderId, "cooking")) {
        cout << "Order accepted! Status updated to cooking." << endl;
    }
    else {
        cout << "Failed to accept order. It may not belong to you or status is not pending." << endl;
    }
}

// ========== 标记为制作中（pending → cooking） ==========
void GeneralMerchant::markAsPreparing(FileManager& fm) {
    vector<Order> orders = fm.loadOrders("orders.txt");

    cout << "\n========== Pending Orders ==========" << endl;
    bool found = false;
    for (const auto& order : orders) {
        if (order.merchantId == merchantId && order.status == "pending") {
            cout << "Order ID: " << order.orderId
                << " | User ID: " << order.userId
                << " | Item: " << order.itemName << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No pending orders." << endl;
        return;
    }
    cout << "(Enter 0 to cancel)\n";
    cout << "Enter the order ID to mark as cooking: ";
    int orderId;
    cin >> orderId;
    if (orderId == 0) {
        cout << "Operation cancelled.\n";
        return;
    }

    // 【修正】验证订单必须属于本商家，且状态为 pending
    bool valid = false;
    for (const auto& order : orders) {
        if (order.orderId == orderId && order.merchantId == merchantId && order.status == "pending") {
            valid = true;
            break;
        }
    }

    if (valid && fm.updateOrderStatus("orders.txt", orderId, "cooking")) {
        cout << "Order status updated to cooking." << endl;
    }
    else {
        cout << "Failed to update. The order may not belong to you or status is not pending." << endl;
    }
}

// ========== 标记为待取餐（cooking → ready） ==========
void GeneralMerchant::markAsReady(FileManager& fm) {
    vector<Order> orders = fm.loadOrders("orders.txt");

    cout << "\n========== Cooking Orders ==========" << endl;
    bool found = false;
    for (const auto& order : orders) {
        if (order.merchantId == merchantId && order.status == "cooking") {
            cout << "Order ID: " << order.orderId
                << " | User ID: " << order.userId
                << " | Item: " << order.itemName << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No cooking orders." << endl;
        return;
    }

    cout << "(Enter 0 to cancel)\n";
    cout << "Enter the order ID to mark as ready for pickup: ";
    int orderId;
    cin >> orderId;
    if (orderId == 0) {
        cout << "Operation cancelled.\n";
        return;
    }

    // 【修正】验证订单必须属于本商家，且状态为 cooking
    bool valid = false;
    for (const auto& order : orders) {
        if (order.orderId == orderId && order.merchantId == merchantId && order.status == "cooking") {
            valid = true;
            break;
        }
    }

    if (valid && fm.updateOrderStatus("orders.txt", orderId, "ready")) {
        cout << "Order status updated to ready for pickup." << endl;
    }
    else {
        cout << "Failed to update. The order may not belong to you or status is not cooking." << endl;
    }
}

// ========== 菜单管理 ==========
void GeneralMerchant::manageMenu(FileManager& fm) {
    int choice;
    while (true) {
        cout << "\n========== Menu Management ==========" << endl;
        cout << "1. View menu" << endl;
        cout << "2. Add item" << endl;
        cout << "3. Return" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: viewMenu(fm); break;
        case 2: addMenuItem(fm); break;
        case 3: return;
        default: cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// ========== 查看菜单 ==========
void GeneralMerchant::viewMenu(FileManager& fm) {
    vector<MenuItem> menu = fm.loadMenu("menus.txt");
    cout << "\n========== My Menu ==========" << endl;
    bool found = false;
    for (const auto& item : menu) {
        if (item.merchantId == merchantId) {
            cout << "Item: " << item.name << " | Price: " << item.price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No items in menu." << endl;
    }
}

// ========== 添加菜品 ==========
void GeneralMerchant::addMenuItem(FileManager& fm) {
    string itemName;
    double price;

    cout << "\n(Enter 0 at any prompt to cancel)\n";
    cout << "\nEnter item name: ";
    cin >> itemName;

    if (itemName == "0") {
        cout << "Add item cancelled.\n";
        return;
    }
    cout << "Enter price: ";
    cin >> price;

    ofstream file("menus.txt", ios::app);
    if (file.is_open()) {
        file << merchantId << "," << itemName << "," << price << endl;
        file.close();
        cout << "Item added successfully!" << endl;
    }
    else {
        cout << "Failed to open menu file." << endl;
    }
}