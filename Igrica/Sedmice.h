#ifndef SEDMICE_H_INCLUDED
#define SEDMICE_H_INCLUDED

#include "Player.h"
#include <string>
#include <random>

class Sedmice
{
    private:
        std::default_random_engine m_dfe{};
        int m_playercount{};
        Player m_players[4]{};
        std::string m_username{};
        int m_userposition{};
        std::vector<Card> m_deck{};
        int m_forehand{}; // Pozicija igraca koji je na redu da baci kartu
        std::vector<Card> m_pile{};
        int m_teams[2]{};
    public:
        Sedmice();
        void addUser(std::string);
        void randomSetup();
        bool isGameOver();
        bool isPileEmpty();
        void dealCards();
        void printGame();
        void printOptions();
        bool inputHandler(std::string);
        bool isUsersTurn();
        bool isTurnSkippable();
        void playUsersChoice(std::string);
        void playBotsChoice();
        void evaluatePile();
        void printResults();
};

#endif // SEDMICE_H_INCLUDED
