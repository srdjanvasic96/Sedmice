#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <vector>
#include "Card.h"


class Player
{
    private:
        std::string m_name{};
        std::vector<Card> m_hand{};
    public:
        Player(std::string="N/A");
        std::string getName();
        std::string getHandValues();
        void printHand(bool=true);
        void takeCard(Card);
        Card giveCard(char);
        Card getCardAt(int);
};

#endif // PLAYER_H_INCLUDED
