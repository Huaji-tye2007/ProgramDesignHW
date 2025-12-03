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
int iStock(int *s, int max, const char *name);
int iStocking(char choice);
int MakeMeat(char choice);
int MakeChips(char choice);
int AddDose(int dose, int *iStock, const char *iName);
int MakePie(char choice);
int ChipsNCola(char choice);

#endif