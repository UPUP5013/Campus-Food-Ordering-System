// DeliveryPerson.h
// Delivery person module
// Function: Implements order processing logic for delivery personnel
// Dependency: FileManager.h
#pragma execution_character_set("utf-8")
#pragma once
#include <string>
#include <vector>
#include "FileManager.h"

using namespace std;

// Base class for delivery person
class DeliveryPerson {
protected:
    int deliveryPersonId;
    string name;

public:
    DeliveryPerson(int id, const string& n) : deliveryPersonId(id), name(n) {}
    virtual ~DeliveryPerson() {}
    virtual void handleOrders(FileManager& fm) = 0;
    int getId() { return deliveryPersonId; }
    string getName() { return name; }
};

// General delivery person derived class
class GeneralDeliveryPerson : public DeliveryPerson {
public:
    GeneralDeliveryPerson(int id, const string& n) : DeliveryPerson(id, n) {}

    void handleOrders(FileManager& fm) override {
        int choice;
        string orderFilename = "orders.txt";

        while (true) {
            cout << "\n========== Delivery Menu ==========\n";
            cout << "1. Show orders ready for pickup\n";
            cout << "2. Show orders being delivered\n";
            cout << "3. Accept order\n";
            cout << "4. Confirm delivery\n";
            cout << "5. Return to main menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            vector<Order> orders = fm.loadOrders(orderFilename);

            switch (choice) {
            case 1:
                showReadyOrders(orders);
                break;

            case 2:
                showDeliveringOrders(orders);
                break;

            case 3: {
                showReadyOrders(orders);
                cout << "(Enter 0 to cancel)\n";
                cout << "Enter the order ID to accept: ";
                int orderId;
                cin >> orderId;

                if (orderId == 0) {
                    cout << "Accept cancelled.\n";
                    break;
                }

                if (validateOrder(orders, orderId, "ready")) {
                    fm.updateOrderStatus(orderFilename, orderId, "delivering");
                    cout << "Order accepted! Status updated to delivering.\n";
                }
                else {
                    cout << "Order not found or status is not ready.\n";
                }
                break;
            }

            case 4: {
                showDeliveringOrders(orders);
                cout << "(Enter 0 to cancel)\n";
                cout << "Enter the order ID to confirm delivery: ";
                int orderId;
                cin >> orderId;

                if (orderId == 0) {
                    cout << "Confirmation cancelled.\n";
                    break;
                }

                if (validateOrder(orders, orderId, "delivering")) {
                    fm.updateOrderStatus(orderFilename, orderId, "completed");
                    cout << "Order delivered!\n";
                }
                else {
                    cout << "Order not found or status is not delivering.\n";
                }
                break;
            }

            case 5:
                cout << "Returning to main menu...\n";
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

private:
    void showReadyOrders(const vector<Order>& orders) {
        cout << "\n========== Orders Ready for Pickup ==========\n";
        cout << "Order ID | User ID | Merchant ID | Item Name\n";
        cout << "--------------------------------------------\n";

        bool hasOrders = false;
        for (const auto& order : orders) {
            if (order.status == "ready") {
                cout << order.orderId << " | " << order.userId
                    << " | " << order.merchantId << " | " << order.itemName << "\n";
                hasOrders = true;
            }
        }

        if (!hasOrders) {
            cout << "No orders ready for pickup.\n";
        }
        cout << "============================================\n";
    }

    void showDeliveringOrders(const vector<Order>& orders) {
        cout << "\n========== Orders Being Delivered ==========\n";
        cout << "Order ID | User ID | Merchant ID | Item Name\n";
        cout << "--------------------------------------------\n";

        bool hasOrders = false;
        for (const auto& order : orders) {
            if (order.status == "delivering") {
                cout << order.orderId << " | " << order.userId
                    << " | " << order.merchantId << " | " << order.itemName << "\n";
                hasOrders = true;
            }
        }

        if (!hasOrders) {
            cout << "No orders being delivered.\n";
        }
        cout << "============================================\n";
    }

    bool validateOrder(const vector<Order>& orders, int orderId, const string& expectedStatus) {
        for (const auto& order : orders) {
            if (order.orderId == orderId && order.status == expectedStatus) {
                return true;
            }
        }
        return false;
    }
};