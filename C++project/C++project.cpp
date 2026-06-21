// main.cpp
#include "FileManager.h"

int main() {
    FileManager fm;

    // ---------- 1. 测试菜单 ----------
    cout << "===== Menu =====" << endl;
    vector<MenuItem> menu = fm.loadMenu("menus.txt");
    for (const auto& item : menu) {
        cout << "Merchant ID: " << item.merchantId
            << ", Dish: " << item.name
            << ", Price: " << item.price << endl;
    }

    // ---------- 2. 测试已有用户 ----------
    cout << "\n===== Existing Users =====" << endl;
    vector<User> users = fm.loadUsers("users.txt");
    for (const auto& u : users) {
        cout << "User ID: " << u.userId
            << ", Name: " << u.userName
            << ", Dorm: " << u.dormitory << endl;
    }

    // ---------- 3. 注册新用户（自动分配ID） ----------
    cout << "\n===== Register New User =====" << endl;
    string name, dorm;
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your dormitory: ";
    cin >> dorm;

    fm.saveUser("users.txt", name, dorm);

    // ---------- 4. 再次显示所有用户，验证是否保存成功 ----------
    cout << "\n===== All Users After Registration =====" << endl;
    users = fm.loadUsers("users.txt");
    for (const auto& u : users) {
        cout << "User ID: " << u.userId
            << ", Name: " << u.userName
            << ", Dorm: " << u.dormitory << endl;
    }
    // 放在 main() 里面，比如在注册用户测试之后
    cout << "\n===== Order Test =====" << endl;

    // 读取并显示所有订单
    vector<Order> orders = fm.loadOrders("orders.txt");
    for (const auto& o : orders) {
        cout << "Order ID: " << o.orderId
            << ", User: " << o.userId
            << ", Merchant: " << o.merchantId
            << ", Item: " << o.itemName
            << ", Status: " << o.status << endl;
    }

    // 尝试更新一个订单的状态（假设有个订单ID为1）
    cout << "\nUpdating order 1 to COOKING..." << endl;
    if (fm.updateOrderStatus("orders.txt", 1, "COOKING")) {
        cout << "Update successful!" << endl;
    }

    // 再次读取并显示，验证状态是否已改变
    cout << "\n===== Orders After Update =====" << endl;
    orders = fm.loadOrders("orders.txt");
    for (const auto& o : orders) {
        cout << "Order ID: " << o.orderId
            << ", Status: " << o.status << endl;
    }

    return 0;
}