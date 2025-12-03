#ifndef PLAYGAME_H
#define PLAYGAME_H

enum class Screen
{
    MAIN_MENU,
    IN_GAME,
    UPGRADE_MENU,
    EXIT
};

struct GameState
{
    int gold;
    int level;
    int days;
    int servedCustomers;
    int pieprice;
    int chipprice;
    int colaprice;
    int countdown;
};

extern GameState gamestate;

// 更新函数原型以返回新的屏幕状态
Screen Interface();
Screen Upgrade();
Screen NewDay();

#endif
