#include "Sedmice.h"
#include <chrono>
#include <algorithm>
#include <iomanip>

Sedmice::Sedmice()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    m_dfe = std::default_random_engine{seed};

    m_players[0] = Player{"Alice"};
    m_players[1] = Player{"Bob"};
    m_players[2] = Player{"Carol"};
    m_playercount = 3;

    m_deck.insert(m_deck.end(),{{'A',"clbs"},{'7',"clbs"},{'8',"clbs"},{'9',"clbs"},{'T',"clbs"},{'J',"clbs"},{'Q',"clbs"},{'K',"clbs"}});
    m_deck.insert(m_deck.end(),{{'A',"dmds"},{'7',"dmds"},{'8',"dmds"},{'9',"dmds"},{'T',"dmds"},{'J',"dmds"},{'Q',"dmds"},{'K',"dmds"}});
    m_deck.insert(m_deck.end(),{{'A',"hrts"},{'7',"hrts"},{'8',"hrts"},{'9',"hrts"},{'T',"hrts"},{'J',"hrts"},{'Q',"hrts"},{'K',"hrts"}});
    m_deck.insert(m_deck.end(),{{'A',"spds"},{'7',"spds"},{'8',"spds"},{'9',"spds"},{'T',"spds"},{'J',"spds"},{'Q',"spds"},{'K',"spds"}});
}

void Sedmice::addUser(std::string name)
{
    m_username = name;
    m_players[3] = Player{name};
    m_playercount++;
    m_userposition = 3;
}

void Sedmice::randomSetup()
{
    //Promijesaj karte
    std::shuffle(m_deck.begin(),m_deck.end(),m_dfe);

    //Pomijesaj igrace
    std::shuffle(std::begin(m_players),std::end(m_players),m_dfe);

    //Nadji poziciju korisnika nakon mijesanja
    for(int i=0;i<m_playercount;i++)
        if(m_players[i].getName()==m_username)
        {
            m_userposition = i;
            break;
        }
}

bool Sedmice::isGameOver()
{

    return m_deck.empty() && m_players[m_forehand].getHandValues().size()==0 && m_pile.empty();
}

bool Sedmice::isPileEmpty()
{
    return m_pile.empty();
}

void Sedmice::dealCards()
{
    int handSize = static_cast<int> (m_players[m_forehand].getHandValues().size());
    int cardCount, dealCount; // Koliko karata iz koliko dijeljenja
    if(m_deck.size()==32) // Prvo dijeljenje
    {
        cardCount = 2;
        dealCount = 8;
    }
    else // Svako dijeljenje koje nije prvo
    {
        int cardsNeeded = 4 - handSize;
        int cardsLeft = static_cast<int>(m_deck.size())/4;

        cardCount = cardsNeeded <= cardsLeft ? cardsNeeded : cardsLeft ;
        dealCount = 4;
    }
    for(int i=0;i<dealCount;i++)
    {
        for(int j=0;j<cardCount;j++)
        {
            m_players[m_forehand].takeCard(m_deck.back());
            m_deck.pop_back();
        }
        m_forehand++;
        m_forehand%=4;
    }
}

void Sedmice::printGame()
{
    char teamSign;
    std::cout<<"\n==========================================================";
    for(int i=0;i<m_playercount;i++)
    {
        std::cout<<"\n";
        teamSign = i%2==0 ? 'O' : 'E';
        if(i==m_forehand)
            std::cout << teamSign << std::setw(15) << ">> " + m_players[i].getName() << " has: ";
        else
            std::cout << teamSign << std::setw(15) << m_players[i].getName() << " has: ";
        if(i==m_userposition)
            m_players[i].printHand();
        else
            m_players[i].printHand(false);
    }
    std::cout<<"\n==========================================================";
    std::cout<<"\nTeam Evens: "<<m_teams[0]<<"    | Team Odds: "<<m_teams[1];
    std::cout<<"\n==========================================================";
}

void Sedmice::printOptions()
{
    std::cout<<"\n==========================================================";
    std::cout<<"\nYour hand : ";
    m_players[m_forehand].printHand();
    if( isTurnSkippable() )
        std::cout<<"\n (You can skip!)";
}

bool Sedmice::inputHandler(std::string input)
{
    std::transform(input.begin(), input.end(), input.begin(),[](unsigned char c){ return std::tolower(c); });

    std::string usersHandValues = m_players[m_forehand].getHandValues();
    std::transform(usersHandValues.begin(), usersHandValues.end(), usersHandValues.begin(),[](unsigned char c){ return std::tolower(c); });

    if(input == "skip")
    {
        if(!isTurnSkippable())
        {
            std::cout<<"\nYou can not skip right now!";
            return false;
        }
        else
            return true;
    }

    if(usersHandValues.find(input[0]) != std::string::npos)
        return true;
    else
    {
        std::cout<<"\nEhmmm, what?! Try again.";
        return false;
    }
}

bool Sedmice::isUsersTurn()
{
    return m_forehand == m_userposition;
}

bool Sedmice::isTurnSkippable()
{
    int pilesize = static_cast<int>(m_pile.size());
    return ( pilesize > 0 && pilesize%4==0 );
}

void Sedmice::playUsersChoice(std::string input)
{
    std::transform(input.begin(), input.end(), input.begin(),[](unsigned char c){ return std::tolower(c); });
    if(input == "skip")
    {
        evaluatePile();
        return;
    }
    std::transform(input.begin(), input.end(), input.begin(),[](unsigned char c){ return std::toupper(c); });
    Card playedCard = m_players[m_forehand].giveCard(input[0]);
    char teamSign = m_forehand%2==0 ? 'O' : 'E';
    std::cout << "\n" << teamSign << std::setw(15) << m_players[m_forehand].getName() << " has played " << playedCard;
    m_pile.push_back(playedCard);
    m_forehand++;
    m_forehand%=4;
}

void Sedmice::playBotsChoice() // Pokusati napraviti optimalan odabir u buducnosti!
{
    std::string botsHandValues = m_players[m_forehand].getHandValues();
    int botsHandSize = static_cast<int> (botsHandValues.size());

    std::uniform_int_distribution<int> dist {0, isTurnSkippable() ? botsHandSize : botsHandSize - 1 };
    int botsChoice = dist(m_dfe);
    auto index = static_cast<long long unsigned int> (botsChoice);

    if(isTurnSkippable() && botsChoice != botsHandSize)                                                 // Pokusaj "razigravanja"
    {
        if( botsHandValues[index] == '7' || botsHandValues[index] == m_pile.front().m_value ) // Validan pokusaj
            botsChoice = botsChoice;
        else                                                                                            // Nevalidan pokusaj
            botsChoice = botsHandSize;
    }
    if(botsChoice == botsHandSize)                                                                      // Skip
    {
        evaluatePile();
        return;
    }

    Card playedCard = m_players[m_forehand].giveCard(botsHandValues[index]);
    char teamSign = m_forehand%2==0 ? 'O' : 'E';
    std::cout << "\n" << teamSign << std::setw(15) << m_players[m_forehand].getName() << " has played " << playedCard;
    m_pile.push_back(playedCard);
    m_forehand++;
    m_forehand%=4;
}


void Sedmice::evaluatePile()
{
    Card rank{m_pile.front()};
    std::string pilestring{};
    for(auto c : m_pile)
    {
        if(c.m_value == rank.m_value || c.m_value == '7')
            pilestring.push_back('R');
        else if(c.m_value == 'A' || c.m_value == 'T')
            pilestring.push_back('1');
        else
            pilestring.push_back('0');
    }

    auto points = std::count(pilestring.begin(), pilestring.end(), '1'); // Ukupno poena koje nosi ruka
    std::string lastFour = pilestring.substr(pilestring.length() - 4); // Posljednje cetiri karte odredjuju koji tim nosi

    auto nextForehandOffset = pilestring.find_last_of('R'); // Ko je posljednji bacio stih

    while(nextForehandOffset--) // Pomjeramo se sa prvog koji je bacio stih na onog koji je posljednji bacio stih
    {
        m_forehand++;
        m_forehand %=4;
    }

    m_teams[m_forehand%2]+=static_cast<int> (points); // Dodjela poena timu kojem pripada posljednji koji je bacio stih

    m_pile.clear();
}

void Sedmice::printResults()
{
    std::cout<<"\n==========================================================";
    if(m_teams[0]>m_teams[1])
        std::cout<<"\n"<<m_players[0].getName()<<" and "<<m_players[2].getName()<<" have won! "<<m_teams[0]<<" - "<<m_teams[1];
    else if(m_teams[0]<m_teams[1])
        std::cout<<"\n"<<m_players[1].getName()<<" and "<<m_players[3].getName()<<" have won! "<<m_teams[0]<<" - "<<m_teams[1];
    else
        std::cout<<"\n"<<"It's a draw!";
    std::cout<<"\n==========================================================";
}


