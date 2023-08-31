#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <string>
#include <iostream>

class Card
{
    public:
        char m_value{};
        std::string m_suit{};

        Card(char='x',std::string="xxxx");
        friend std::ostream& operator<<(std::ostream&,const Card&);
};

#endif // CARD_H_INCLUDED
