#include "playgame.h"
#include "customer.h"
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include "ingredient.h"
#include <string>

using namespace std;

GameState gamestate = {0, 0, 0, 0, 20, 10, 5, 120};

// 暂停以显示反馈信息
void PauseForFeedback(const string &message)
{
    cout << "\n"
         << message << endl;
    this_thread::sleep_for(chrono::seconds(2));
}

Screen Interface()
{
    system("cls");
    gamestate.pieprice = (gamestate.level >= 2 ? 30 : 20);
    cout << "============================" << endl;
    cout << "*********沙威玛传奇*********" << endl;
    cout << "*****作者：Huaji_tye2007****" << endl;
    cout << "============================" << endl;
    cout << "金币数量：" << gamestate.gold << endl;
    cout << "店铺等级：" << gamestate.level << endl;
    cout << "营业天数：" << gamestate.days << endl;
    cout << "已经接待顾客数：" << gamestate.servedCustomers << endl;
    cout << "自动切肉机(支持自动补满)：" << (gamestate.level >= 1 ? "已解锁" : "未解锁") << endl;
    cout << "金盘子服务(卷饼价格升至30金币)：" << (gamestate.level >= 2 ? "已解锁" : "未解锁") << endl;
    cout << "============================" << endl;
    cout << "[1]新的一天" << endl;
    cout << "[2]店铺升级" << endl;
    cout << "[3]退出游戏" << endl;
    cout << "============================" << endl;
    cout << "请输入操作指令：";
    char choice;
    cin >> choice;
    switch (choice)
    {
    case '1':
        return Screen::IN_GAME;
    case '2':
        return Screen::UPGRADE_MENU;
    case '3':
        cout << "退出游戏，感谢您的游玩！" << endl;
        return Screen::EXIT;
    default:
        PauseForFeedback("无效输入，请输入1、2或3。");
        return Screen::MAIN_MENU;
    }
}

Screen Upgrade()
{
    system("cls");
    string feedbackMessage = "";
    cout << "===========店铺升级==========" << endl;
    cout << "当前等级：" << gamestate.level << endl;
    cout << "升级所需金币：" << (gamestate.level + 1) * 100 << endl;
    cout << "确定升级吗？（Y/N）" << endl;
    char choice;
    cin >> choice;
    choice = toupper(choice);
    if (choice == 'Y')
    {
        int cost = (gamestate.level + 1) * 100;
        if (gamestate.gold >= cost)
        {
            gamestate.gold -= cost;
            gamestate.level++;
            feedbackMessage = "升级成功！当前等级：" + to_string(gamestate.level);
        }
        else
            feedbackMessage = "金币不足，无法升级！";
    }
    else if (choice == 'N')
    {
        feedbackMessage = "取消升级。";
    }
    else
    {
        feedbackMessage = "无效输入，请输入Y或N。";
    }
    PauseForFeedback(feedbackMessage);
    return Screen::MAIN_MENU;
}

Screen NewDay()
{
    gamestate.days++;
    time_t dayStartTime = time(0);
    Customer currentCustomer;
    currentCustomer.isActive = false;
    time_t nextCustomerTime = time(0) + (rand() % 5 + 1);
    string feedbackMessage = "新的一天开始了！今天是第 " + to_string(gamestate.days) + " 天。";

    while (true)
    {
        system("cls");

        time_t currentTime = time(0);
        double dayElapsedTime = difftime(currentTime, dayStartTime);

        if (dayElapsedTime >= gamestate.countdown)
        {
            feedbackMessage = "\n时间到！一天结束了！";
            PauseForFeedback(feedbackMessage);
            return Screen::MAIN_MENU;
        }

        if (!currentCustomer.isActive && currentTime >= nextCustomerTime)
        {
            GenCust(currentCustomer);
            feedbackMessage = "新顾客到店！";
        }
        else if (currentCustomer.isActive)
        {
            double customerElapsedTime = difftime(currentTime, currentCustomer.spawnTime);
            if (customerElapsedTime >= currentCustomer.patience)
            {
                feedbackMessage = "顾客等得不耐烦，已经离开了！";
                currentCustomer.isActive = false;
                nextCustomerTime = currentTime + (rand() % 10 + 2);
            }
        }

        cout << "=========当天剩余时间：" << static_cast<int>(gamestate.countdown - dayElapsedTime) << "s=========" << endl;
        cout << "累计金币：" << gamestate.gold << endl;
        cout << "卷饼价格：" << gamestate.pieprice << " 金币 | 薯条价格：" << gamestate.chipprice << " 金币 | 可乐价格：" << gamestate.colaprice << " 金币" << endl;
        StockInterface();
        ProductInterface();
        WorkInterface();
        CustInterface(currentCustomer);
        cout << "---------------------" << endl;
        cout << "【反馈提示】" << feedbackMessage << endl; // 显示上一轮操作的反馈
        cout << "---------------------" << endl;
        cout << "请输入操作指令 ('Q' 结束当天): ";

        char choice;
        cin >> choice;
        choice = toupper(choice);

        if (choice == 'Q')
        {
            feedbackMessage = "你提前结束了今天的工作。";
            PauseForFeedback(feedbackMessage);
            return Screen::MAIN_MENU;
        }
        else if (choice == '9' || choice == 'C' || choice == 'F')
        {
            if (!currentCustomer.isActive)
            {
                feedbackMessage = "当前没有顾客！";
            }
            else
            {
                if (ServeCust(currentCustomer, choice))
                {
                    int earned = (choice == '9') ? gamestate.pieprice : ((choice == 'C') ? gamestate.chipprice : gamestate.colaprice);
                    feedbackMessage = "交货成功！赚到了" + to_string(earned) + "金币！";
                    gamestate.gold += earned;
                }
                else
                {
                    feedbackMessage = "交货失败！和顾客需求不符！";
                    currentCustomer.angerLevel++;
                    if (choice == '9')
                        pieState = {false, {0, 0, 0, 0, 0}, false, false};
                    else if (choice == 'C')
                        chipsState = {false, false};
                    else if (choice == 'F')
                        colaState = {false, false};
                }

                if (currentCustomer.isActive)
                {
                    if (currentCustomer.angerLevel >= 2)
                    {
                        feedbackMessage = "顾客愤怒地离开了！";
                        currentCustomer.isActive = false;
                        nextCustomerTime = time(0) + (rand() % 10 + 5);
                    }
                    else if (currentCustomer.PieServed && currentCustomer.ChipsServed && currentCustomer.ColaServed)
                    {
                        feedbackMessage = "顾客满意离开了！";
                        gamestate.servedCustomers++;
                        currentCustomer.isActive = false;
                        nextCustomerTime = time(0) + (rand() % 16);
                    }
                }
            }
        }
        else if (choice >= '1' && choice <= '8')
        {
            feedbackMessage = MakePie(choice);
        }
        else if ((choice >= 'A' && choice <= 'B') || (choice >= 'D' && choice <= 'E'))
        {
            feedbackMessage = ChipsNCola(choice);
        }
        else if (choice >= 'G' && choice <= 'H')
        {
            feedbackMessage = MakeMeat(choice);
        }
        else if (choice >= 'I' && choice <= 'K')
        {
            feedbackMessage = MakeChips(choice);
        }
        else if (choice == 'L')
        {
            system("cls");
            StockingInterface();
            cout << "请输入补货指令（输入Q返回）：";
            char stockChoice;
            cin >> stockChoice;
            if (toupper(stockChoice) != 'Q')
            {
                feedbackMessage = iStocking(toupper(stockChoice));
            }
            else
            {
                feedbackMessage = "取消补货。";
            }
        }
        else
        {
            feedbackMessage = "无效指令，请重新输入！";
        }
    }
}
