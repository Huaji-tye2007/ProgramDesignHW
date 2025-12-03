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

// Helper function to display feedback and pause
void ShowFeedback(const string &message)
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
    cout << "*********沙威玛传奇**********" << endl;
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
        ShowFeedback("无效输入，请输入1、2或3。");
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
    ShowFeedback(feedbackMessage);
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

        // --- 绘制当前界面 ---
        time_t currentTime = time(0);
        double dayElapsedTime = difftime(currentTime, dayStartTime);
        cout << "=========当天剩余时间：" << static_cast<int>(gamestate.countdown - dayElapsedTime) << "s=========" << endl;
        cout << "累计金币：" << gamestate.gold << endl;
        cout << "卷饼价格：" << gamestate.pieprice << " 金币 | 薯条价格：" << gamestate.chipprice << " 金币 | 可乐价格：" << gamestate.colaprice << " 金币" << endl;
        StockInterface();
        ProductInterface();
        WorkInterface();
        CustInterface(currentCustomer);
        cout << "---------------------" << endl;

        // --- 显示上次操作的反馈并暂停 ---
        ShowFeedback(feedbackMessage);
        feedbackMessage = ""; // 清空反馈

        // --- 再次清屏并绘制，准备接受新指令 ---
        system("cls");
        dayElapsedTime = difftime(time(0), dayStartTime); // 重新获取时间
        cout << "=========当天剩余时间：" << static_cast<int>(gamestate.countdown - dayElapsedTime) << "s=========" << endl;
        cout << "累计金币：" << gamestate.gold << endl;
        cout << "卷饼价格：" << gamestate.pieprice << " 金币 | 薯条价格：" << gamestate.chipprice << " 金币 | 可乐价格：" << gamestate.colaprice << " 金币" << endl;
        StockInterface();
        ProductInterface();
        WorkInterface();
        CustInterface(currentCustomer);
        cout << "---------------------" << endl;
        cout << "请输入操作指令 ('Q' 结束当天): ";

        // --- 逻辑更新 ---
        currentTime = time(0); // 重新获取时间
        dayElapsedTime = difftime(currentTime, dayStartTime);
        if (dayElapsedTime >= gamestate.countdown)
        {
            feedbackMessage = "\n时间到！一天结束了！";
            break;
        }
        if (!currentCustomer.isActive)
        {
            if (currentTime >= nextCustomerTime)
            {
                GenCust(currentCustomer);
                feedbackMessage = "新顾客到店！";
                continue; // 立即刷新界面显示新顾客
            }
        }
        if (currentCustomer.isActive)
        {
            double customerElapsedTime = difftime(currentTime, currentCustomer.spawnTime);
            if (customerElapsedTime >= currentCustomer.patience)
            {
                feedbackMessage = "顾客等得不耐烦，已经离开了！";
                currentCustomer.isActive = false;
                nextCustomerTime = currentTime + (rand() % 10 + 2);
                continue; // 立即刷新
            }
        }

        // --- 等待并处理输入 ---
        char choice;
        cin >> choice;
        choice = toupper(choice);

        if (choice == 'Q')
        {
            feedbackMessage = "你提前结束了今天的工作。";
            break;
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
                        nextCustomerTime = currentTime + (rand() % 10 + 5);
                    }
                    else if (currentCustomer.PieServed && currentCustomer.ChipsServed && currentCustomer.ColaServed)
                    {
                        feedbackMessage = "顾客满意离开了！";
                        gamestate.servedCustomers++;
                        currentCustomer.isActive = false;
                        nextCustomerTime = currentTime + (rand() % 16);
                    }
                }
            }
        }
        else if (choice >= '1' && choice <= '8')
        {
            MakePie(choice);
            feedbackMessage = "制作卷饼中...";
        }
        else if ((choice >= 'A' && choice <= 'B') || (choice >= 'D' && choice <= 'E'))
        {
            ChipsNCola(choice);
            feedbackMessage = "制作小吃中...";
        }
        else if (choice >= 'G' && choice <= 'H')
        {
            MakeMeat(choice);
            feedbackMessage = "处理肉类中...";
        }
        else if (choice >= 'I' && choice <= 'K')
        {
            MakeChips(choice);
            feedbackMessage = "处理薯条中...";
        }
        else if (choice == 'L')
        {
            system("cls");
            StockingInterface();
            cout << "请输入补货指令（输入Q返回）：";
            cin >> choice;
            if (toupper(choice) != 'Q')
            {
                iStocking(toupper(choice));
                feedbackMessage = "补货操作完成。";
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

    ShowFeedback(feedbackMessage);
    return Screen::MAIN_MENU;
}
