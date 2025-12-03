#include <iostream>
#ifndef INGREDIENT_H
#define INGREDIENT_H

struct Ingredient
{
    int Pie;
    int Meat;
    int Cucumber;
    int TomatoJam;
    int Sauce;
    int Chips;
    int Cola;
    int MAX;
};

struct Wrapper
{
    int Paper;
    int Box;
    int Bottle;
    int MAX;
};

struct PieState
{
    bool hasPie;
    int dosing[5];
    bool folded;
    bool wrapped;
};

struct CState
{
    bool haswrapper;
    bool filled;
};

struct MStockState
{
    bool cut;
};

struct CStockState
{
    bool cut;
    bool fried;
};

extern Ingredient ingredients;
extern Wrapper wrappers;
extern PieState pieState;
extern CState chipsState, colaState;
extern MStockState meatstock;
extern CStockState chipsstock;

void StockInterface();
void WorkInterface();
void StockingInterface();
void ProductInterface();
std::string iStocking(char choice);
std::string MakeMeat(char choice);
std::string MakeChips(char choice);
std::string MakePie(char choice);
std::string ChipsNCola(char choice);

#endif