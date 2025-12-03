#include "playgame.h"
#include <iostream>
#include <cstdlib>
#include <ctime> 
using namespace std;

int main()
{
    system("chcp 65001");
    srand(time(0)); 

    Screen currentScreen = Screen::MAIN_MENU;

    while (true)
    {
        switch (currentScreen)
        {
        case Screen::MAIN_MENU:
            currentScreen = Interface();
            break;
        case Screen::UPGRADE_MENU:
            currentScreen = Upgrade();
            break;
        case Screen::IN_GAME:
            currentScreen = NewDay();
            break;
        case Screen::EXIT:
            return 0;
        }
    }

    return 0;
}
