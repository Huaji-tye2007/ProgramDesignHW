#include "ingredient.h"
#include "playgame.h"
#include "customer.h"
#include <iostream>
using namespace std;

Ingredient ingredients = {10, 10, 10, 10, 10, 10, 10, 10};
Wrapper wrappers = {10, 10, 10, 10};
PieState pieState = {false, {0, 0, 0, 0, 0}, false, false};
CState chipsState = {false, false};
CState colaState = {false, false};
MStockState meatstock = {false};
CStockState chipsstock = {false, false};

void StockInterface()
{
    cout << "=============食材=============" << endl;
    cout << "【主料台】卷饼: " << ingredients.Pie << "\n【配料台】肉: " << ingredients.Meat << " 黄瓜: " << ingredients.Cucumber << " 番茄酱: " << ingredients.TomatoJam << " 沙司: " << ingredients.Sauce << endl;
    cout << "【小吃料】薯条: " << ingredients.Chips << " 可乐: " << ingredients.Cola << endl;
    cout << "=============包装=============" << endl;
    cout << "纸: " << wrappers.Paper << " 盒子: " << wrappers.Box << " 杯子: " << wrappers.Bottle << endl;
}

void WorkInterface()
{
    cout << "=============工作台=============" << endl;
    cout << "【制作卷饼】 [1]放置卷饼 [2]放置肉 [3]放置黄瓜 " << endl;
    cout << "            [4]放置番茄酱 [5]放置沙司 [6]放置薯条" << endl;
    cout << "            [7]卷起 [8]装卷饼 [9]给顾客" << endl;
    cout << "【制作薯条】 [A]拿盒子 [B]装薯条 [C]给顾客" << endl;
    cout << "【制作可乐】 [D]拿杯子 [E]装可乐 [F]给顾客" << endl;
    cout << "=============库存台=============" << endl;
    (gamestate.level >= 1) ? (cout << "[H]补肉 " << endl) : (cout << "[G]切肉 [H]补肉" << endl);
    cout << "[I]切土豆 [J]炸薯条 [K]补薯条 " << endl;
    cout << "[L]补充其他食材或包装 " << endl;
}

void StockingInterface()
{
    cout << "=============补货=============" << endl;
    cout << "[1]补卷饼 [2]补黄瓜 [3]补番茄酱 [4]补沙司 [5]补可乐" << endl;
    cout << "[6]补纸 [7]补盒子 [8]补杯子 " << endl;
}

void ProductInterface()
{
    cout << "=============货台=============" << endl;
    cout << "【卷饼台】卷饼: " << (pieState.hasPie ? "有卷饼 " : "无卷饼 ");
    cout << "配料( ";
    const char *dosing_names[] = {"肉 ", "黄瓜 ", "番茄酱 ", "沙司 ", "薯条 "};
    for (int i = 0; i < 5; i++)
    {
        if (pieState.dosing[i])
        {
            cout << dosing_names[pieState.dosing[i] - 1];
        }
    }
    cout << ") ";
    cout << (pieState.folded ? "已卷起 " : "未卷起 ");
    cout << (pieState.wrapped ? "已包装 " : "未包装 ") << endl;
    cout << "【小吃台】薯条: " << (chipsState.haswrapper ? "有包装 " : "无包装 ")
         << (chipsState.filled ? "已装满 " : "未装满 ") << endl;
    cout << "【小吃台】可乐: " << (colaState.haswrapper ? "有包装 " : "无包装 ")
         << (colaState.filled ? "已装满 " : "未装满 ") << endl;
}

// 补货
int iStock(int *s, int max, const char *name)
{
    if (*s < max)
    {
        (*s)++;
        cout << name << ": " << *s << endl;
        return 0;
    }
    else
    {
        cout << name << "已满！" << endl;
        return -1;
    }
}

int iStocking(char choice)
{
    switch (choice)
    {
    case '1':
        return iStock(&ingredients.Pie, ingredients.MAX, "卷饼");
    case '2':
        return iStock(&ingredients.Cucumber, ingredients.MAX, "黄瓜");
    case '3':
        return iStock(&ingredients.TomatoJam, ingredients.MAX, "番茄酱");
    case '4':
        return iStock(&ingredients.Sauce, ingredients.MAX, "沙司");
    case '5':
        return iStock(&ingredients.Cola, ingredients.MAX, "可乐");
    case '6':
        return iStock(&wrappers.Paper, wrappers.MAX, "纸");
    case '7':
        return iStock(&wrappers.Box, wrappers.MAX, "盒子");
    case '8':
        return iStock(&wrappers.Bottle, wrappers.MAX, "杯子");
    default:
        cout << "非法输入！" << endl;
        return -1;
    }
}

int MakeMeat(char choice)
{
    if (ingredients.Meat >= ingredients.MAX)
    {
        cout << "肉库存已满！" << endl;
        return -1;
    }
    if (gamestate.level >= 1)
    {
        if (choice == 'H')
        {
            ingredients.Meat = ingredients.MAX;
            cout << "肉：" << ingredients.Meat << endl;
            return 0;
        }
        else
        {
            cout << "自动切肉机已解锁，请直接补肉！" << endl;
            return -1;
        }
    }
    switch (choice)
    {
    case 'G':
        if (meatstock.cut)
        {
            cout << "肉已经切好了！" << endl;
            return -1;
        }
        meatstock.cut = true;
        cout << "肉切好啦！" << endl;
        return 0;
    case 'H':
        if (!meatstock.cut)
        {
            cout << "请先切肉！" << endl;
            return -1;
        }
        ingredients.Meat++;
        meatstock.cut = false;
        cout << "肉：" << ingredients.Meat << endl;
        return 0;
    default:
        cout << "非法输入！" << endl;
        return -1;
    }
}

int MakeChips(char choice)
{
    if (ingredients.Chips >= ingredients.MAX)
    {
        cout << "薯条库存已满！" << endl;
        return -1;
    }
    switch (choice)
    {
    case 'I':
        if (chipsstock.cut)
        {
            cout << "土豆已经切好了！" << endl;
            return -1;
        }
        chipsstock.cut = true;
        cout << "土豆切好啦！" << endl;
        return 0;
    case 'J':
        if (!chipsstock.cut)
        {
            cout << "请先切土豆！" << endl;
            return -1;
        }
        chipsstock.fried = true;
        cout << "薯条炸好啦！" << endl;
        return 0;
    case 'K':
        if (!chipsstock.cut)
        {
            cout << "请先切土豆！" << endl;
            return -1;
        }
        if (!chipsstock.fried)
        {
            cout << "请先炸薯条！" << endl;
            return -1;
        }
        ingredients.Chips++;
        chipsstock.cut = false;
        chipsstock.fried = false;
        cout << "薯条：" << ingredients.Chips << endl;
        return 0;
    default:
        cout << "非法输入！" << endl;
        return -1;
    }
}

// 制作卷饼
int AddDose(int dose, int *iStock, const char *iName)
{
    if (!pieState.hasPie)
    {
        cout << "请先放置卷饼！" << endl;
        return -1;
    }
    int index = 0;
    for (index; index < 5; index++)
    {
        if (pieState.dosing[index] == 0)
            break;
    }
    if (index == 5)
    {
        cout << "配料已满！" << endl;
        return -1;
    }
    if (*iStock <= 0)
    {
        cout << iName << "库存不足！" << endl;
        return -1;
    }
    pieState.dosing[index] = dose;
    (*iStock)--;
    return 0;
}

int MakePie(char choice)
{
    switch (choice)
    {
    case '1':
        if (pieState.hasPie)
        {
            cout << "已有卷饼！" << endl;
            return -1;
        }
        if (ingredients.Pie == 0)
        {
            cout << "卷饼库存不足！" << endl;
            return -1;
        }
        pieState.hasPie = true;
        ingredients.Pie--;
        return 0;
    case '2':
        return AddDose(1, &ingredients.Meat, "肉");
    case '3':
        return AddDose(2, &ingredients.Cucumber, "黄瓜");
    case '4':
        return AddDose(3, &ingredients.TomatoJam, "番茄酱");
    case '5':
        return AddDose(4, &ingredients.Sauce, "沙司");
    case '6':
        return AddDose(5, &ingredients.Chips, "薯条");
    case '7':
        if (!pieState.hasPie)
        {
            cout << "请先放置卷饼！" << endl;
            return -1;
        }
        if (pieState.folded)
        {
            cout << "卷饼已卷好！" << endl;
            return -1;
        }
        pieState.folded = true;
        return 0;
    case '8':
        if (!pieState.hasPie)
        {
            cout << "请先放置卷饼！" << endl;
            return -1;
        }
        if (!pieState.folded)
        {
            cout << "请先卷起卷饼！" << endl;
            return -1;
        }
        if (pieState.wrapped)
        {
            cout << "卷饼已包装！" << endl;
            return -1;
        }
        if (wrappers.Paper <= 0)
        {
            cout << "包装纸库存不足！" << endl;
            return -1;
        }
        pieState.wrapped = true;
        wrappers.Paper--;
        return 0;
    default:
        cout << "非法输入！" << endl;
        return -1;
    }
}

// 制作小吃
int ChipsNCola(char choice)
{
    CState *obj = NULL;
    int *stock = NULL;
    const char *item_name = "";
    const char *wrapper_name = "";

    if (choice >= 'A' && choice <= 'C')
    {
        obj = &chipsState;
        stock = &ingredients.Chips;
        item_name = "薯条";
        wrapper_name = "盒子";
    }
    else if (choice >= 'D' && choice <= 'F')
    {
        obj = &colaState;
        stock = &ingredients.Cola;
        item_name = "可乐";
        wrapper_name = "杯子";
    }

    int act = (choice - ((choice >= 'A' && choice <= 'C') ? 'A' : 'D'));
    switch (act)
    {
    case 0:
        if (obj->haswrapper)
        {
            cout << "已有" << wrapper_name << "！" << endl;
            return -1;
        }
        if ((choice >= 'A' && choice <= 'C' ? wrappers.Box : wrappers.Bottle) == 0)
        {
            cout << wrapper_name << "库存不足！" << endl;
            return -1;
        }
        obj->haswrapper = true;
        (choice >= 'A' && choice <= 'C' ? wrappers.Box-- : wrappers.Bottle--);
        return 0;
    case 1:
        if (!obj->haswrapper)
        {
            cout << "请先拿" << wrapper_name << "！" << endl;
            return -1;
        }
        if (obj->filled)
        {
            cout << item_name << "已装满！" << endl;
            return -1;
        }
        if (*stock <= 0)
        {
            cout << item_name << "库存不足！" << endl;
            return -1;
        }
        obj->filled = true;
        (*stock)--;
        return 0;
    default:
        cout << "非法输入！" << endl;
        return -1;
    }
}