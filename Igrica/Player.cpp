#include "Player.h"
#include <algorithm>

Player::Player(std::string name) : m_name{name}
{
}

std::string Player::getName()
{
    return m_name;
}

std::string Player::getHandValues()
{
    std::string handValues{};
    for(auto c : m_hand)
        handValues.push_back(c.m_value);
    return handValues;
}

void Player::printHand(bool reveal)
{
    for(auto c : m_hand)
    {
        if(reveal)
            std::cout << " " << c;
        else
            std::cout << " " << Card{};
    }
}



void Player::takeCard(Card c)
{
    m_hand.push_back(c);
}

Card Player::giveCard(char value)
{
    auto toGive = std::find_if(m_hand.begin(), m_hand.end(), [&value = value](const Card& c) { return c.m_value == value; });
    if(toGive != m_hand.end())
    {
        Card givenCard = *toGive;
        m_hand.erase(toGive);
        return givenCard;
    }
    else
    {
        return Card{};
    }
}

Card Player::getCardAt(int position)
{
    return m_hand.at(static_cast<long long unsigned int>(position));
}
