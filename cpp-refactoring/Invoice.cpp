#include "Invoice.h"

#include <iostream>
#include <math.h>

#include "Theater.h"

std::unique_ptr<Invoice> Invoice::Create(const Json::Value& invoiceItem)
{
    auto invoice = std::make_unique<Invoice>();
    invoice->m_Customer = invoiceItem["customer"].asString();
    
    for(const auto& performanceItem : invoiceItem["performances"])
    {
        Performance performance;
        performance.audience = performanceItem["audience"].asInt();

        std::string playId = performanceItem["playID"].asString();
        auto play = Theater::FindPlayById(playId);

        if(!play)
        {
            std::cout << "No performance with id " << playId << " was found.\n";
            continue;
        }

        performance.play = play;

        invoice->m_Performances.push_back(performance);
    }

    return invoice;
}

void Invoice::PrintHeader()
{
    std::cout << "Statement for " << m_Customer << " \n";
}

void Invoice::PrintStats()
{
    int totalAmount = 0;
    int totalCredits = 0;

    for(const auto& transaction : m_Transactions)
    {
        totalAmount += transaction.GetDollarAmount();
        totalCredits += transaction.credits;
    }

    std::cout << "Amount owed: $" << totalAmount << "\n";
    std::cout << "Credits earned: " << totalCredits << "\n";
}

void Invoice::AddTransaction(const Transaction& transaction)
{
    m_Transactions.push_back(transaction);
}

void Invoice::Checkout()
{
    for (const auto& performance : m_Performances)
    {
        Transaction transaction;
        transaction.performance = &performance;

        transaction.Charge();
        transaction.CheckBonuses();
        transaction.Print();

        AddTransaction(transaction);
    }
}