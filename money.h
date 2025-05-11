#ifndef MONEY_H
#define MONEY_H

class Money {
public:
    // 构造函数，初始化金钱数量
    Money(int initialAmount);

    // 获取当前金钱数量
    int getAmount() const;

    // 检查是否有足够的金钱来支付给定的成本
    bool canAfford(int cost) const;

    // 花费指定数量的金钱
    void spend(int cost);

    // 增加指定数量的金钱
    void earn(int amount);

    //重置金钱
    void reset(int amount) { this->amount = amount; }

private:
    int amount;
};

#endif // MONEY_H
