#include "money.h"

Money::Money(int initialAmount) : amount(initialAmount) {}

int Money::getAmount() const {
    return amount;
}

bool Money::canAfford(int cost) const {
    return amount >= cost;
}

void Money::spend(int cost) {
    if (canAfford(cost)) {
        amount -= cost;
    }
}

void Money::earn(int amountToEarn) {
    amount += amountToEarn;
}
