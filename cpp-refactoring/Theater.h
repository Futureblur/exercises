#pragma once

#include <iostream>
#include <vector>

namespace Json
{
    class Value;
}

enum class PlayType
{
    Tragedy,
    Comedy,
    None
};

struct Play
{
    std::string id;
    std::string name;
    PlayType type;

    static PlayType ConvertToPlayType(const std::string& type);
    static std::string ConvertToString(const PlayType& type);
};

struct Performance 
{
    Play* play = nullptr;
    int audience = 0;
};

struct Transaction
{
    uint32_t credits = 0;
    uint32_t amount = 0;
    const Performance* performance;

    [[nodiscard]] inline int GetDollarAmount() const
    {
        return amount / 100;
    }

    void Print();
    void AddCredits(int value);
    void CheckBonuses();
    void Charge();
};

class Theater 
{
public:
    static void LoadPlays();
    static void LoadPrices();
    static void LoadInvoices();
    static int CalculateCost(const Performance* performance);

    static Play* FindPlayById(const std::string& id);

private:
    static inline std::vector<Play> m_Plays;
    static Json::Value m_Prices;
};