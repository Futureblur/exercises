#include "Theater.h"

#include <fstream>

#include <json/json.h>

#include "Invoice.h"

PlayType Play::ConvertToPlayType(const std::string& type)
{
    if (type == "tragedy")
        return PlayType::Tragedy;

    if (type == "comedy")
        return PlayType::Comedy;
    
    return PlayType::None;
}

std::string Play::ConvertToString(const PlayType& type)
{
    if(type == PlayType::Tragedy)
        return "tragedy";

    if(type == PlayType::Comedy)
        return "comedy";

    return "";
}

void Transaction::Print()
{
    std::cout << " " << performance->play->name << ": $" << GetDollarAmount() << "(" << performance->audience << " seats)\n";
}

void Transaction::AddCredits(int value)
{
    credits += value;
}

void Transaction::CheckBonuses()
{
    AddCredits(std::max(performance->audience - 30, 0));

    bool isComedy = performance->play->type == PlayType::Comedy;
    if (isComedy)
    {
        AddCredits(std::floor(performance->audience / 5));
    }
}

void Transaction::Charge()
{
    amount += Theater::CalculateCost(performance);
}

Json::Value Theater::m_Prices;

void Theater::LoadPlays()
{
    std::ifstream file("plays.json");
    Json::Value json;
    file >> json;

    for(const auto& playItem : json)
    {
        Play play;
        play.id = playItem["id"].asString();
        play.name = playItem["name"].asString();
        play.type = Play::ConvertToPlayType(playItem["type"].asString());
        m_Plays.push_back(play);
    }
}

void Theater::LoadPrices()
{
    std::ifstream file("prices.json");
    file >> m_Prices;
}

void Theater::LoadInvoices()
{
    std::ifstream file("invoices.json");
    Json::Value json;
    file >> json;

    for(const auto& invoiceItem : json)
    {
        auto invoice = Invoice::Create(invoiceItem);

        invoice->PrintHeader();
        invoice->Checkout();
        invoice->PrintStats();
    }
}

int Theater::CalculateCost(const Performance* performance)
{
    std::string type = Play::ConvertToString(performance->play->type);
    if(type.empty())
    {
        std::cout << "No price entry could be found for " << performance->play->name << "\n";
        return -1;
    }

    auto price = m_Prices[type];
    int amount = price["baseCost"].asInt();

    auto audience = performance->audience;
    int audienceThreshold = price["audienceThreshold"].asInt();
    
    if (audience > audienceThreshold)
        amount += price["audienceThresholdFee"].asInt() + price["audienceCostFactor"].asInt() * (audience - audienceThreshold);

    amount += price["audienceFactor"].asInt() * audience;
    return amount;
}

Play* Theater::FindPlayById(const std::string& id)
{
    auto it = std::find_if(m_Plays.begin(), m_Plays.end(), [&](const Play& in)
    {
        return in.id == id;
    });

    if(it != m_Plays.end())
        return &m_Plays.at(it - m_Plays.begin());

    return nullptr;
}