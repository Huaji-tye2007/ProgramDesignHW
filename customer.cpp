#include "customer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ingredient.h"
using namespace std;

int id = 0; // 顾客ID，用以区分
void GenCust(Customer &cust)
{
    id += 1;
    cust.patience = (rand() % 21 + 20); // 初始耐心值 20-40s
    cust.spawnTime = time(0);

    for (int i = 0; i < 5; i++)
    {
        cust.pieOrder[i] = 0;
    }
    cust.wantsPie = false;
    for (int i = 0; i < 5; i++)
    {
        int dose = rand() % 6;
        if (dose != 0)
        {
            cust.wantsPie = true;
            cust.pieOrder[i] = dose;
        }
    }
    cust.wantsChips = rand() % 2;
    cust.wantsCola = rand() % 2;
    cust.isActive = true;
    if (!cust.wantsPie && !cust.wantsChips && !cust.wantsCola)
    {
        GenCust(cust); // 确保至少有一个需求
        return;
    }
    cust.PieServed = !cust.wantsPie;
    cust.ChipsServed = !cust.wantsChips;
    cust.ColaServed = !cust.wantsCola;
    cust.angerLevel = 0;
}

void CustInterface(const Customer &cust)
{
    if (!cust.isActive)
    {
        cout << "===========暂无顾客===========" << endl;
        cout << "可以准备一下食材和半成品！" << endl;
        return;
    }

    cout << "=============顾客" << id << "需求=============" << endl;
    if (cust.wantsPie)
    {
        cout << "卷饼配料( ";
        const char *dosing_names[] = {"肉 ", "黄瓜 ", "番茄酱 ", "沙司 ", "薯条 "};
        bool wantsPie = false;
        for (int i = 0; i < 5; i++)
        {
            if (cust.pieOrder[i])
            {
                cout << dosing_names[cust.pieOrder[i] - 1];
            }
        }
        cout << ") " << endl;
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

    cout << endl;
    cout << "耐心值：" << static_cast<int>(remainingPatience) << endl;
}

bool ServeCust(Customer &cust, char choice)
{
    if (!cust.isActive)
        return false;

    switch (choice)
    {
    // 检查卷饼
    case '9':
    {
        if (!cust.wantsPie)
        {
            cout << "顾客不需要卷饼！" << endl;
            return false;
        }
        if (cust.PieServed)
        {
            cout << "已经给过卷饼了！" << endl;
            return false;
        }
        if (!pieState.hasPie)
        {
            cout << "货台上没有卷饼！" << endl;
            return false;
        }
        if (!pieState.folded)
        {
            cout << "请先卷起卷饼！" << endl;
            return false;
        }
        if (!pieState.wrapped)
        {
            cout << "请先包装卷饼！" << endl;
            return false;
        }
        // 检查配料是否符合顾客需求
        int order_counts[6] = {0};
        int pie_counts[6] = {0};

        // 统计顾客需求的每种配料数量
        for (int i = 0; i < 5; i++)
        {
            if (cust.pieOrder[i] > 0)
            {
                order_counts[cust.pieOrder[i]]++;
            }
        }

        // 统计当前卷饼的每种配料数量
        for (int i = 0; i < 5; i++)
        {
            if (pieState.dosing[i] > 0)
            {
                pie_counts[pieState.dosing[i]]++;
            }
        }

        // 比较两种配料数量是否完全一致
        for (int i = 1; i <= 5; i++)
        {
            if (order_counts[i] != pie_counts[i])
            {
                return false;
            }
        }
        cust.PieServed = true;
        cust.wantsPie = false;
        pieState.hasPie = false;
        pieState.folded = false;
        pieState.wrapped = false;
        for (int i = 0; i < 5; i++)
        {
            pieState.dosing[i] = 0;
        }
        break;
    }
    // 检查小吃
    case 'C':
    {
        if (cust.wantsChips)
        {
            if (!chipsState.haswrapper || !chipsState.filled)
            {
                cout << "薯条未准备好！" << endl;
                return false;
            }
        }
        else
        {
            cout << "顾客不需要薯条！" << endl;
            return false;
        }
        if (cust.ChipsServed)
        {
            cout << "已经给过薯条了！" << endl;
            return false;
        }
        cust.ChipsServed = true;
        cust.wantsChips = false;
        chipsState.haswrapper = false;
        chipsState.filled = false;
        break;
    }
    case 'F':
    {
        if (cust.wantsCola)
        {
            if (!colaState.haswrapper || !colaState.filled)
            {
                cout << "可乐未准备好！" << endl;
                return false;
            }
        }
        else
        {
            cout << "顾客不需要可乐！" << endl;
            return false;
        }
        if (cust.ColaServed)
        {
            cout << "已经给过可乐了！" << endl;
            return false;
        }
        cust.ColaServed = true;
        cust.wantsCola = false;
        colaState.haswrapper = false;
        colaState.filled = false;
        break;
    }
    default:
    {
        cout << "非法输入！" << endl;
        return false;
    }
    }
    return true;
}
