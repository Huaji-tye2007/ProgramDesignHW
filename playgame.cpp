#include "playgame.h"
#include "customer.h"
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include "ingredient.h"
#include <string>
#include <conio.h>

using namespace std;

GameState gamestate = {0, 0, 0, 0, 20, 10, 5, 120}; 

// 暂停以显示反馈信息
void PauseForFeedback(const string &message)
{
    cout << "\n"
         << message << endl;
    this_thread::sleep_for(chrono::seconds(1));
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
    cout << "----------------------------" << endl;
    cout << "自动切肉机(自动补满肉): " << (gamestate.level >= 1 ? "已解锁" : "未解锁") << endl;
    cout << "金盘子服务(卷饼价+10): " << (gamestate.level >= 2 ? "已解锁" : "未解锁") << endl;
    cout << "扩大店面(顾客上限+2): " << (gamestate.level >= 3 ? "已解锁" : "未解锁") << endl;
    cout << "扩大仓库( 食材上限+10): " << (gamestate.level >= 4 ? "已解锁" : "未解锁") << endl;
    cout << "============================" << endl;
    cout << "[1] 新的一天" << endl;
    cout << "[2] 店铺升级" << endl;
    cout << "[3] 退出游戏" << endl;
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
    cout << "当前金币: " << gamestate.gold << endl;
    cout << "----------------------------" << endl;
    cout << "[1] 自动切肉机 (100金币) " << (gamestate.level >= 1 ? "[已购买]" : "") << endl;
    cout << "[2] 金盘子服务 (200金币) " << (gamestate.level >= 2 ? "[已购买]" : "") << endl;
    cout << "[3] 扩大店面 (300金币) " << (gamestate.level >= 3 ? "[已购买]" : "") << endl;
    cout << "[4] 扩大仓库 (400金币) " << (gamestate.level >= 4 ? "[已购买]" : "") << endl;
    cout << "[Q] 返回主菜单" << endl;
    cout << "============================" << endl;
    cout << "请输入升级选项: ";

    char choice;
    cin >> choice;
    choice = toupper(choice);

    switch (choice)
    {
    case '1':
        if (gamestate.level >= 1)
        {
            feedbackMessage = "你已经购买过此升级！";
        }
        else if (gamestate.gold >= 100)
        {
            gamestate.gold -= 100;
            gamestate.level = 1;
            feedbackMessage = "成功购买自动切肉机！";
        }
        else
        {
            feedbackMessage = "金币不足！";
        }
        break;
    case '2':
        if (gamestate.level >= 2)
        {
            feedbackMessage = "你已经购买过此升级！";
        }
        else if (gamestate.level < 1)
        {
            feedbackMessage = "请先购买前置升级！";
        }
        else if (gamestate.gold >= 200)
        {
            gamestate.gold -= 200;
            gamestate.level = 2;
            feedbackMessage = "成功购买金盘子服务！";
        }
        else
        {
            feedbackMessage = "金币不足！";
        }
        break;
    case '3':
        if (gamestate.level >= 3)
        {
            feedbackMessage = "你已经购买过此升级！";
        }
        else if (gamestate.level < 2)
        {
            feedbackMessage = "请先购买前置升级！";
        }
        else if (gamestate.gold >= 300)
        {
            gamestate.gold -= 300;
            gamestate.level = 3;
            feedbackMessage = "成功扩大店面！";
        }
        else
        {
            feedbackMessage = "金币不足！";
        }
        break;
    case 'Q':
        feedbackMessage = "返回主菜单。";
        break;
    case '4':
        if (gamestate.level >= 4)
        {
            feedbackMessage = "你已经购买过此升级！";
        }
        else if (gamestate.level < 3)
        {
            feedbackMessage = "请先购买前置升级！";
        }
        else if (gamestate.gold >= 400)
        {
            gamestate.gold -= 400;
            gamestate.level = 4;
            feedbackMessage = "成功扩大仓库！食材上限增加10！";
            ingredients.MAX += 10;
            wrappers.MAX += 10;
        }
        else
        {
            feedbackMessage = "金币不足！";
        }
        break;
    default:
        feedbackMessage = "无效输入！";
        break;
    }

    PauseForFeedback(feedbackMessage);
    return Screen::MAIN_MENU;
}

Screen NewDay()
{
    gamestate.days++;
    time_t dayStartTime = time(0);

    const int MAX_CUSTOMERS = 3;
    Customer customers[MAX_CUSTOMERS] = {};
    for (int i = 0; i < MAX_CUSTOMERS; ++i)
        customers[i].isActive = false;

    int maxAllowedCustomers = (gamestate.level >= 3) ? 3 : 1;
    int targetIndex = 0;

    time_t nextCustomerTime = time(0) + (rand() % 5 + 1);

    vector<string> feedbackMessages; // 使用vector来存储多条反馈信息
    feedbackMessages.push_back("新的一天开始了！今天是第 " + to_string(gamestate.days) + " 天。");

    while (true)
    {
        // 1. 渲染UI
        system("cls");
        time_t currentTime = time(0);
        cout << "=========当天剩余时间：" << static_cast<int>(gamestate.countdown - difftime(currentTime, dayStartTime)) << "s=========" << endl;
        cout << "累计金币：" << gamestate.gold << endl;
        cout << "卷饼价格：" << gamestate.pieprice << " 金币 | 薯条价格：" << gamestate.chipprice << " 金币 | 可乐价格：" << gamestate.colaprice << " 金币" << endl;
        StockInterface();
        ProductInterface();
        WorkInterface();
        CustInterface(customers, maxAllowedCustomers, targetIndex);
        cout << "---------------------" << endl;
        cout << "【反馈提示】" << endl;
        if (feedbackMessages.empty())
        {
            cout << "等待操作中..." << endl;
        }
        else
        {
            for (const string &msg : feedbackMessages)
            {
                cout << "- " << msg << endl;
            }
        }
        cout << "---------------------" << endl;
        cout << "切换顾客: [<]左 [>]右 | 制作: [1-8, A-K] | 服务: [9,C,F] | 补货: [L] | 结束当天: [Q]" << endl;
        cout << "请输入操作指令: ";

        // 2. 处理输入 (阻塞式)
        char choice;
        cin >> choice;
        choice = toupper(choice);

        // 准备处理新一轮的反馈，先清空旧信息
        feedbackMessages.clear();
        currentTime = time(0);

        // 3. 更新游戏状态 (顾客生成、离开)
        if (difftime(currentTime, dayStartTime) >= gamestate.countdown)
        {
            PauseForFeedback("\n时间到！一天结束了！");
            return Screen::MAIN_MENU;
        }

        if (currentTime >= nextCustomerTime)
        {
            GenCust(customers, maxAllowedCustomers);
            nextCustomerTime = currentTime + (rand() % 10 + 5);
        }

        for (int i = 0; i < maxAllowedCustomers; ++i)
        {
            if (customers[i].isActive)
            {
                if (customers[i].PieServed && customers[i].ChipsServed && customers[i].ColaServed)
                {
                    feedbackMessages.push_back("顾客 " + to_string(customers[i].id) + " 满意离开了！");
                    gamestate.servedCustomers++;
                    customers[i].isActive = false;
                    continue;
                }
                if (difftime(currentTime, customers[i].spawnTime) >= customers[i].patience)
                {
                    feedbackMessages.push_back("顾客 " + to_string(customers[i].id) + " 等得不耐烦，已经离开了！");
                    customers[i].isActive = false;
                }
            }
        }

        // 4. 根据输入更新状态
        if (choice == 'Q')
        {
            PauseForFeedback("你提前结束了今天的工作。");
            return Screen::MAIN_MENU;
        }
        else if (choice == '<' || choice == '>')
        {
            int direction = (choice == '<') ? -1 : 1;
            int initialIndex = targetIndex;
            int activeCustomers = 0;
            for (int i = 0; i < maxAllowedCustomers; ++i)
                if (customers[i].isActive)
                    activeCustomers++;

            if (activeCustomers > 0)
            {
                do
                {
                    targetIndex = (targetIndex + direction + maxAllowedCustomers) % maxAllowedCustomers;
                } while (!customers[targetIndex].isActive && targetIndex != initialIndex);
            }
            feedbackMessages.push_back("已切换目标顾客。");
        }
        else if (choice == '9' || choice == 'C' || choice == 'F')
        {
            if (!customers[targetIndex].isActive)
            {
                feedbackMessages.push_back("当前没有顾客或目标顾客已经走了！");
            }
            else
            {
                string serveFeedback;
                if (ServeCust(customers[targetIndex], choice, serveFeedback))
                {
                    int earned = 0;
                    if (choice == '9')
                        earned = gamestate.pieprice;
                    else if (choice == 'C')
                        earned = gamestate.chipprice;
                    else if (choice == 'F')
                        earned = gamestate.colaprice;
                    gamestate.gold += earned;
                    feedbackMessages.push_back("交货成功！赚到了 " + to_string(earned) + " 金币！");

                    if (customers[targetIndex].PieServed && customers[targetIndex].ChipsServed && customers[targetIndex].ColaServed)
                    {
                        feedbackMessages.push_back("顾客 " + to_string(customers[targetIndex].id) + " 满意地离开了！");
                        gamestate.servedCustomers++;
                        customers[targetIndex].isActive = false;
                    }
                }
                else
                {
                    feedbackMessages.push_back(serveFeedback);
                    customers[targetIndex].angerLevel++;
                    if (choice == '9')
                        pieState = {false, {0, 0, 0, 0, 0}, false, false};
                    else if (choice == 'C')
                        chipsState = {false, false};
                    else if (choice == 'F')
                        colaState = {false, false};

                    if (customers[targetIndex].angerLevel >= 2)
                    {
                        feedbackMessages.push_back("顾客 " + to_string(customers[targetIndex].id) + " 愤怒地离开了！");
                        customers[targetIndex].isActive = false;
                    }
                }
            }
        }
        else if (choice >= '1' && choice <= '8')
        {
            feedbackMessages.push_back(MakePie(choice));
        }
        else if ((choice >= 'A' && choice <= 'B') || (choice >= 'D' && choice <= 'E'))
        {
            feedbackMessages.push_back(ChipsNCola(choice));
        }
        else if (choice >= 'G' && choice <= 'H')
        {
            if (gamestate.level >= 1)
            {
                iStocking('R');
                feedbackMessages.push_back("自动切肉机工作完成！");
            }
            else
            {
                feedbackMessages.push_back(MakeMeat(choice));
            }
        }
        else if (choice >= 'I' && choice <= 'K')
        {
            feedbackMessages.push_back(MakeChips(choice));
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
                feedbackMessages.push_back(iStocking(toupper(stockChoice)));
            }
            else
            {
                feedbackMessages.push_back("取消补货。");
            }
        }
        else
        {
            feedbackMessages.push_back("无效指令，请重新输入！");
        }
    }
}
