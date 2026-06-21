#pragma once
// Merchant.h
#pragma execution_character_set("utf-8")
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Merchant {
protected:
    int merchantId;
    string merchantName;

public:
    Merchant(int id, const string& name);
    virtual ~Merchant();
    virtual void handleOrders(FileManager& fm) = 0;
    int getId();
    string getName();
};

class GeneralMerchant : public Merchant {
public:
    GeneralMerchant(int id, const string& name);
    void handleOrders(FileManager& fm) override;

private:
    void showPendingOrders(FileManager& fm);
    void showPreparingOrders(FileManager& fm);
    void acceptOrder(FileManager& fm);
    void markAsPreparing(FileManager& fm);
    void markAsReady(FileManager& fm);
    void manageMenu(FileManager& fm);
    void addMenuItem(FileManager& fm);
    void viewMenu(FileManager& fm);
};