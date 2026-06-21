#pragma once
#include <iostream>

#include "FileManager.h"
#include "Merchant.h"
#include "DeliveryPerson.h"
#include "user.h"

using namespace std;

int main() {

    FileManager fm;

    vector<MenuItem> menu = fm.loadMenu("menus.txt");
    vector<User> users = fm.loadUsers("users.txt");
    vector<Order> orders = fm.loadOrders("orders.txt");

    cout << "========================================\n";
    cout << "     Welcome to Campus Food Ordering System\n";
    cout << "========================================\n";

    int choice;
    do {
        cout << "\nPlease select your role:\n";
        cout << "1. User\n";
        cout << "2. Merchant\n";
        cout << "3. Delivery Person\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            UserModule um;
            um.run();
            break;
        }
        case 2: {
            int merchantId;
            cout << "(Enter 0 to cancel)\n";
            cout << "Enter your Merchant ID: ";
            cin >> merchantId;

            if (merchantId == 0) {
                cout << "Merchant login cancelled.\n";
                break;
            }

            GeneralMerchant merchant(merchantId, "Merchant " + to_string(merchantId));
            merchant.handleOrders(fm);
            break;
        }
        case 3: {
            int deliveryChoice;
            cout << "\n========== Delivery Person Center ==========" << endl;
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
            cout << "0. Back" << endl;
            cout << "Choice: ";
            cin >> deliveryChoice;

            if (deliveryChoice == 0) {
                break;
            }
            else if (deliveryChoice == 1) {
                // 登录
                int deliveryId;
                string deliveryName;

                cout << "(Enter 0 to cancel)\n";
                cout << "Enter your Delivery Person ID: ";
                cin >> deliveryId;

                if (deliveryId == 0) {
                    cout << "Delivery login cancelled.\n";
                    break;
                }

                cout << "Enter your name: ";
                cin >> deliveryName;

                vector<DeliveryPersonInfo> deliveryList = fm.loadDeliveryPersons("delivery.txt");
                bool valid = false;
                for (const auto& dp : deliveryList) {
                    if (dp.deliveryId == deliveryId && dp.name == deliveryName) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    cout << "Invalid delivery person ID or name. Access denied.\n";
                    break;
                }

                GeneralDeliveryPerson delivery(deliveryId, deliveryName);
                delivery.handleOrders(fm);
            }
            else if (deliveryChoice == 2) {
                // 注册
                string deliveryName;
                cout << "(Enter 0 to cancel)\n";
                cout << "Enter your name: ";
                cin >> deliveryName;

                if (deliveryName == "0") {
                    cout << "Registration cancelled.\n";
                    break;
                }

                fm.saveDeliveryPerson("delivery.txt", deliveryName);
            }
            else {
                cout << "Invalid choice.\n";
            }
            break;
        }
        case 0:
            cout << "Thank you for using the system. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}