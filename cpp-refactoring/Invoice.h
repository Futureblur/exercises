#pragma once

#include <iostream>
#include <memory>
#include <json/json.h>

class Transaction;
class Performance;

class Invoice
{
public:
    Invoice() = default;
    
    // TODO: invoiceItem weird name
    static std::unique_ptr<Invoice> Create(const Json::Value& invoiceItem);
    void PrintHeader();
    void PrintStats();
    void AddTransaction(const Transaction& transaction);
    void Checkout();

private:
    std::string m_Customer;
    std::vector<Performance> m_Performances;
    std::vector<Transaction> m_Transactions;
};