#include "ingredient.h"
#include "playgame.h"
#include "customer.h"
#include <iostream>
#include <string> 
using namespace std;

Ingredient ingredients = {10, 10, 10, 10, 10, 10, 10, 10};
Wrapper wrappers = {10, 10, 10, 10};
PieState pieState = {false, {0, 0, 0, 0, 0}, false, false};
CState chipsState = {false, false};
CState colaState = {false, false};
MStockState meatstock = {false};
CStockState chipsstock = {false, false};

string iStock(int *s, int max, const char *name)
{
    if (*s < max)
    {
        (*s)++;
        return string(name) + "补充成功！当前数量: " + to_string(*s);
    }
    else
    {
        return string(name) + "已满！";
    }
}

string AddDose(int dose, int *iStock, const char *iName)
{
    if (!pieState.hasPie)
    {
        return "请先放置卷饼！";
    }
    int index = 0;
    for (index = 0; index < 5; index++)
    {
        if (pieState.dosing[index] == 0)
            break;
    }
    if (index == 5)
    {
        return "配料已满！";
    }
    if (*iStock <= 0)
    {
        return string(iName) + "库存不足！";
    }
    pieState.dosing[index] = dose;
    (*iStock)--;
    return string(iName) + "已添加。";
}

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

string iStocking(char choice)
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
        return "非法输入！";
    }
}

string MakeMeat(char choice)
{
    if (ingredients.Meat >= ingredients.MAX)
    {
        return "肉库存已满！";
    }
    if (gamestate.level >= 1)
    {
        if (choice == 'H')
        {
            ingredients.Meat = ingredients.MAX;
            return "肉已自动补满！";
        }
        else
        {
            return "自动切肉机已解锁，请直接补肉！";
        }
    }
    switch (choice)
    {
    case 'G':
        if (meatstock.cut)
        {
            return "肉已经切好了！";
        }
        meatstock.cut = true;
        return "肉切好啦！";
    case 'H':
        if (!meatstock.cut)
        {
            return "请先切肉！";
        }
        ingredients.Meat++;
        meatstock.cut = false;
        return "肉补充成功！当前数量: " + to_string(ingredients.Meat);
    default:
        return "非法输入！";
    }
}

string MakeChips(char choice)
{
    if (ingredients.Chips >= ingredients.MAX)
    {
        return "薯条库存已满！";
    }
    switch (choice)
    {
    case 'I':
        if (chipsstock.cut)
        {
            return "土豆已经切好了！";
        }
        chipsstock.cut = true;
        return "土豆切好啦！";
    case 'J':
        if (!chipsstock.cut)
        {
            return "请先切土豆！";
        }
        chipsstock.fried = true;
        return "薯条炸好啦！";
    case 'K':
        if (!chipsstock.cut)
        {
            return "请先切土豆！";
        }
        if (!chipsstock.fried)
        {
            return "请先炸薯条！";
        }
        ingredients.Chips++;
        chipsstock.cut = false;
        chipsstock.fried = false;
        return "薯条补充成功！当前数量: " + to_string(ingredients.Chips);
    default:
        return "非法输入！";
    }
}

string MakePie(char choice)
{
    switch (choice)
    {
    case '1':
        if (pieState.hasPie)
        {
            return "已有卷饼！";
        }
        if (ingredients.Pie == 0)
        {
            return "卷饼库存不足！";
        }
        pieState.hasPie = true;
        ingredients.Pie--;
        return "已放置卷饼。";
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
            return "请先放置卷饼！";
        }
        if (pieState.folded)
        {
            return "卷饼已卷好！";
        }
        pieState.folded = true;
        return "卷饼已卷好。";
    case '8':
        if (!pieState.hasPie)
        {
            return "请先放置卷饼！";
        }
        if (!pieState.folded)
        {
            return "请先卷起卷饼！";
        }
        if (pieState.wrapped)
        {
            return "卷饼已包装！";
        }
        if (wrappers.Paper <= 0)
        {
            return "包装纸库存不足！";
        }
        pieState.wrapped = true;
        wrappers.Paper--;
        return "卷饼已包装。";
    default:
        return "非法输入！";
    }
}

string ChipsNCola(char choice)
{
    CState *obj = NULL;
    int *stock = NULL;
    const char *item_name = "";
    const char *wrapper_name = "";
    int *wrapper_stock = NULL;

    if (choice >= 'A' && choice <= 'B')
    {
        obj = &chipsState;
        stock = &ingredients.Chips;
        item_name = "薯条";
        wrapper_name = "盒子";
        wrapper_stock = &wrappers.Box;
    }
    else if (choice >= 'D' && choice <= 'E')
    {
        obj = &colaState;
        stock = &ingredients.Cola;
        item_name = "可乐";
        wrapper_name = "杯子";
        wrapper_stock = &wrappers.Bottle;
    }
    else
    {
        return "非法输入！";
    }

    int act = (choice - ((choice >= 'A' && choice <= 'B') ? 'A' : 'D'));
    switch (act)
    {
    case 0:
        if (obj->haswrapper)
        {
            return "已有" + string(wrapper_name) + "！";
        }
        if (*wrapper_stock <= 0)
        {
            return string(wrapper_name) + "库存不足！";
        }
        obj->haswrapper = true;
        (*wrapper_stock)--;
        return "已拿出" + string(wrapper_name) + "。";
    case 1:
        if (!obj->haswrapper)
        {
            return "请先拿" + string(wrapper_name) + "！";
        }
        if (obj->filled)
        {
            return string(item_name) + "已装满！";
        }
        if (*stock <= 0)
        {
            return string(item_name) + "库存不足！";
        }
        obj->filled = true;
        (*stock)--;
        return string(item_name) + "已装满。";
    default:
        return "非法输入！";
    }
}