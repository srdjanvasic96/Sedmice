#include "Card.h"

Card::Card(char value, std::string suit) : m_value{value}, m_suit{suit}
{
}

std::ostream& operator<<(std::ostream& stream, const Card& card)
{
    stream << "[" << card.m_value << "," << card.m_suit << "]";
    return stream;
}
