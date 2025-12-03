#include "customer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ingredient.h" // For pieState, chipsState, colaState
using namespace std;

static int next_customer_id = 1;

// 在顾客数组中找到一个空位并生成一个新顾客
void GenCust(Customer *customers, int maxCustomers)
{
    for (int i = 0; i < maxCustomers; ++i)
    {
        if (!customers[i].isActive)
        {
            Customer &cust = customers[i];
            cust.id = next_customer_id++;
            cust.patience = (rand() % 21 + 20); // 初始耐心值 20-40s
            cust.spawnTime = time(0);

            for (int j = 0; j < 5; j++)
            {
                cust.pieOrder[j] = 0;
            }
            cust.wantsPie = false;
            for (int j = 0; j < 5; j++)
            {
                int dose = rand() % 6;
                if (dose != 0)
                {
                    cust.wantsPie = true;
                    cust.pieOrder[j] = dose;
                }
            }
            cust.wantsChips = rand() % 2;
            cust.wantsCola = rand() % 2;

            if (!cust.wantsPie && !cust.wantsChips && !cust.wantsCola)
            {
                i--; // 重新为这个顾客生成订单
                continue;
            }

            cust.isActive = true;
            cust.PieServed = !cust.wantsPie;
            cust.ChipsServed = !cust.wantsChips;
            cust.ColaServed = !cust.wantsCola;
            cust.angerLevel = 0;
            return; // 成功生成一个顾客后就退出
        }
    }
}

// 显示所有活跃顾客的信息，并标记目标顾客
void CustInterface(const Customer *customers, int maxCustomers, int targetIndex)
{
    bool hasActiveCustomer = false;
    for (int i = 0; i < maxCustomers; ++i)
    {
        if (customers[i].isActive)
        {
            hasActiveCustomer = true;
            const Customer &cust = customers[i];

            // 如果当前顾客是目标顾客，则显示标记
            if (i == targetIndex)
            {
                cout << "-> ";
            }
            else
            {
                cout << "   ";
            }

            cout << "=============顾客 " << cust.id << " 需求=============" << endl;
            cout << "   "; // 为了对齐
            if (cust.wantsPie)
            {
                cout << "卷饼配料( ";
                const char *dosing_names[] = {"肉", "黄瓜", "番茄酱", "沙司", "薯条"};
                for (int j = 0; j < 5; j++)
                {
                    if (cust.pieOrder[j])
                    {
                        cout << dosing_names[cust.pieOrder[j] - 1] << " ";
                    }
                }
                cout << ") ";
            }

            if (cust.wantsChips)
                cout << "薯条 ";
            if (cust.wantsCola)
                cout << "可乐 ";
            cout << endl;

            double elapsedTime = difftime(time(0), cust.spawnTime);
            double remainingPatience = cust.patience - elapsedTime;
            if (remainingPatience < 0)
                remainingPatience = 0;

            cout << "   "; // 为了对齐
            cout << "耐心值：" << static_cast<int>(remainingPatience) << endl;
        }
    }

    if (!hasActiveCustomer)
    {
        cout << "===========暂无顾客===========" << endl;
        cout << "可以准备一下食材和半成品！" << endl;
    }
}

// 为指定的顾客提供服务 (函数逻辑保持不变)
bool ServeCust(Customer &cust, char choice, std::string &feedback)
{
    if (!cust.isActive)
    {
        feedback = "当前没有顾客！";
        return false;
    }

    switch (choice)
    {
    // 检查卷饼
    case '9':
    {
        if (!cust.wantsPie)
        {
            feedback = "顾客不需要卷饼！";
            return false;
        }
        if (cust.PieServed)
        {
            feedback = "已经给过卷饼了！";
            return false;
        }
        if (!pieState.hasPie)
        {
            feedback = "货台上没有卷饼！";
            return false;
        }
        if (!pieState.folded)
        {
            feedback = "请先卷起卷饼！";
            return false;
        }
        if (!pieState.wrapped)
        {
            feedback = "请先包装卷饼！";
            return false;
        }
        // 检查配料是否符合顾客需求
        int order_counts[6] = {0};
        int pie_counts[6] = {0};

        for (int i = 0; i < 5; i++)
        {
            if (cust.pieOrder[i] > 0)
            {
                order_counts[cust.pieOrder[i]]++;
            }
        }

        for (int i = 0; i < 5; i++)
        {
            if (pieState.dosing[i] > 0)
            {
                pie_counts[pieState.dosing[i]]++;
            }
        }

        for (int i = 1; i <= 5; i++)
        {
            if (order_counts[i] != pie_counts[i])
            {
                feedback = "卷饼配料和顾客需求不符！";
                return false;
            }
        }
        cust.PieServed = true;
        cust.wantsPie = false;
        pieState = {false, {0, 0, 0, 0, 0}, false, false};
        break;
    }
    // 检查小吃
    case 'C':
    {
        if (!cust.wantsChips)
        {
            feedback = "顾客不需要薯条！";
            return false;
        }
        if (cust.ChipsServed)
        {
            feedback = "已经给过薯条了！";
            return false;
        }
        if (!chipsState.haswrapper || !chipsState.filled)
        {
            feedback = "薯条未准备好！";
            return false;
        }
        cust.ChipsServed = true;
        cust.wantsChips = false;
        chipsState = {false, false};
        break;
    }
    case 'F':
    {
        if (!cust.wantsCola)
        {
            feedback = "顾客不需要可乐！";
            return false;
        }
        if (cust.ColaServed)
        {
            feedback = "已经给过可乐了！";
            return false;
        }
        if (!colaState.haswrapper || !colaState.filled)
        {
            feedback = "可乐未准备好！";
            return false;
        }
        cust.ColaServed = true;
        cust.wantsCola = false;
        colaState = {false, false};
        break;
    }
    default:
    {
        feedback = "非法的服务指令！";
        return false;
    }
    }
    return true;
}
