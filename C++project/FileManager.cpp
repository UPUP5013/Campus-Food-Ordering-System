// FileManager.cpp
#pragma execution_character_set("utf-8")
#include "FileManager.h"

// ========== 读取菜单 ==========
vector<MenuItem> FileManager::loadMenu(const string& filename) {
    vector<MenuItem> menuList;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open menu file " << filename << endl;
        return menuList;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 3) {
            MenuItem item;
            item.merchantId = stoi(tokens[0]);
            item.name = tokens[1];
            item.price = stod(tokens[2]);
            menuList.push_back(item);
        }
    }

    file.close();
    return menuList;
}

// ========== 读取所有用户 ==========
vector<User> FileManager::loadUsers(const string& filename) {
    vector<User> userList;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open user file " << filename << endl;
        return userList;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 3) {
            User user;
            user.userId = stoi(tokens[0]);
            user.userName = tokens[1];
            user.dormitory = tokens[2];
            userList.push_back(user);
        }
    }

    file.close();
    return userList;
}

// ========== 保存新用户（自动分配ID） ==========
User FileManager::saveUser(const string& filename, const string& userName, const string& dormitory) {
    vector<User> userList = loadUsers(filename);
    int maxId = 0;
    for (const auto& u : userList) {
        if (u.userId > maxId) {
            maxId = u.userId;
        }
    }
    int newId = maxId + 1;

    User newUser;
    newUser.userId = newId;
    newUser.userName = userName;
    newUser.dormitory = dormitory;

    // 确保文件末尾有换行符
    fstream checkFile(filename, ios::in | ios::out | ios::ate);
    if (checkFile.is_open()) {
        checkFile.seekg(0, ios::end);
        int length = checkFile.tellg();
        if (length > 0) {
            checkFile.seekg(-1, ios::end);
            char lastChar;
            checkFile.get(lastChar);
            if (lastChar != '\n') {
                checkFile.close();
                ofstream addNewline(filename, ios::app);
                addNewline << endl;
                addNewline.close();
            }
        }
        checkFile.close();
    }

    // 追加写入新用户
    ofstream fileOut(filename, ios::app);
    if (!fileOut.is_open()) {
        cerr << "Error: Cannot write to user file " << filename << endl;
        return User{ -1, "", "" };
    }

    fileOut << newUser.userId << "," << newUser.userName << "," << newUser.dormitory << endl;
    fileOut.close();

    cout << "Registration successful! Your user ID is " << newId << endl;
    return newUser;
}

// ========== 读取所有订单 ==========
vector<Order> FileManager::loadOrders(const string& filename) {
    vector<Order> orderList;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open order file " << filename << endl;
        return orderList;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 5) {
            Order order;
            order.orderId = stoi(tokens[0]);
            order.userId = stoi(tokens[1]);
            order.merchantId = stoi(tokens[2]);
            order.itemName = tokens[3];
            order.status = tokens[4];
            orderList.push_back(order);
        }
    }

    file.close();
    return orderList;
}

// ========== 更新订单状态 ==========
bool FileManager::updateOrderStatus(const string& filename, int orderId, const string& newStatus) {
    vector<Order> orders = loadOrders(filename);
    bool found = false;

    for (auto& order : orders) {
        if (order.orderId == orderId) {
            order.status = newStatus;
            found = true;
            break;
        }
    }

    if (!found) {
        cerr << "Error: Order ID " << orderId << " not found." << endl;
        return false;
    }

    ofstream fileOut(filename);
    if (!fileOut.is_open()) {
        cerr << "Error: Cannot write to order file " << filename << endl;
        return false;
    }

    for (const auto& order : orders) {
        fileOut << order.orderId << ","
            << order.userId << ","
            << order.merchantId << ","
            << order.itemName << ","
            << order.status << endl;
    }

    fileOut.close();
    return true;
}

// ========== 保存新订单 ==========
bool FileManager::saveOrder(const string& filename, const Order& newOrder) {
    ofstream fileOut(filename, ios::app);
    if (!fileOut.is_open()) {
        cerr << "Error: Cannot open order file " << filename << endl;
        return false;
    }

    fileOut << newOrder.orderId << ","
        << newOrder.userId << ","
        << newOrder.merchantId << ","
        << newOrder.itemName << ","
        << newOrder.status << endl;
    fileOut.close();
    return true;
}
// ========== 读取外送员名单 ==========
vector<DeliveryPersonInfo> FileManager::loadDeliveryPersons(const string& filename) {
    vector<DeliveryPersonInfo> deliveryList;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open delivery file " << filename << endl;
        return deliveryList;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 2) {
            DeliveryPersonInfo dp;
            dp.deliveryId = stoi(tokens[0]);
            dp.name = tokens[1];
            deliveryList.push_back(dp);
        }
    }

    file.close();
    return deliveryList;
}
void FileManager::saveDeliveryPerson(const string& filename, const string& name) {
    vector<DeliveryPersonInfo> deliveryList = loadDeliveryPersons(filename);
    int maxId = 0;
    for (const auto& dp : deliveryList) {
        if (dp.deliveryId > maxId) {
            maxId = dp.deliveryId;
        }
    }
    int newId = maxId + 1;

    ofstream fileOut(filename, ios::app);
    if (!fileOut.is_open()) {
        cerr << "Error: Cannot open delivery file " << filename << endl;
        return;
    }

    fileOut << newId << "," << name << endl;
    fileOut.close();

    cout << "Registration successful! Your delivery person ID is " << newId << endl;
    cout << "Please remember your ID for future login.\n";
}