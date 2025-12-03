#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <ctime>
#include <string>

struct Customer
{
    int id;
    double patience; // 初始耐心值
    int pieOrder[5];
    bool wantsPie;
    bool wantsChips;
    bool wantsCola;
    bool PieServed;
    bool ChipsServed;
    bool ColaServed;
    bool isActive;    // 当前是否有顾客
    int angerLevel;   // 愤怒等级
    time_t spawnTime; // 顾客生成的时间点
};

void GenCust(Customer *customers, int maxCustomers);
void CustInterface(const Customer *customers, int maxCustomers, int targetIndex);
bool ServeCust(Customer &cust, char choice, std::string &feedback);

#endif
