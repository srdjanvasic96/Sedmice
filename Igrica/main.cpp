#include <iostream>
#include "Sedmice.h"
#include <regex>

int main()
{
    Sedmice sedmice{};
    std::string userName{};

    std::cout<<"Enter username: ";
    std::getline(std::cin,userName);
    userName = userName.size()==0 ? "User" : userName;
    std::cout<<"Username is set to \""<<userName<<"\".";
        sedmice.addUser(userName);


    std::string userInput;
    sedmice.randomSetup();

    while(!sedmice.isGameOver())
    {
        if(sedmice.isPileEmpty())
        {
            sedmice.dealCards();
            sedmice.printGame();
        }
        if(sedmice.isUsersTurn())
        {
            sedmice.printOptions();
            do
            {
                std::cout<<"\nYour move: ";
                std::getline(std::cin,userInput);
            }
            while(!sedmice.inputHandler(userInput));
            std::cout<<"\n==========================================================";
            sedmice.playUsersChoice(userInput);
        }
        else
        sedmice.playBotsChoice();
    }

    sedmice.printResults();
    return 0;
}
