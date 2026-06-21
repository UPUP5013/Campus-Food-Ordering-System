// FileManager.h
#pragma execution_character_set("utf-8")
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// 菜单项结构体
struct MenuItem {
    int merchantId;
    string name;
    double price;
};

// 用户结构体
struct User {
    int userId;
    string userName;
    string dormitory;
};

// 订单结构体
struct Order {
    int orderId;
    int userId;
    int merchantId;
    string itemName;
    string status;
};
// 外送员结构体
struct DeliveryPersonInfo {
    int deliveryId;
    string name;
};

class FileManager {
public:
    void saveDeliveryPerson(const string& filename, const string& name);
    vector<DeliveryPersonInfo> loadDeliveryPersons(const string& filename);
    // 读取菜单文件
    vector<MenuItem> loadMenu(const string& filename);

    // 读取所有用户
    vector<User> loadUsers(const string& filename);

    // 保存新用户（自动分配ID），返回新创建的User对象
    User saveUser(const string& filename, const string& userName, const string& dormitory);

    // 订单相关的函数
    vector<Order> loadOrders(const string& filename);
    bool updateOrderStatus(const string& filename, int orderId, const string& newStatus);
    bool saveOrder(const string& filename, const Order& newOrder);
};