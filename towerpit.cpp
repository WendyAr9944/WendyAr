#include "towerpit.h"

// 构造函数实现
TowerPit::TowerPit(const QPoint& pos, int index, int width, int height)
    : position(pos), m_hasTower(false), m_width(width), m_height(height), m_obstacle(nullptr), m_index(index) {}

// 获取炮塔坑位置的函数实现
QPoint TowerPit::getPosition() const {
    return position;
}

// 检查是否有炮塔的函数实现
bool TowerPit::hasTower() const {
    return m_hasTower;
}

// 设置是否有炮塔的函数实现
void TowerPit::setHasTower(bool hasTower) {
    m_hasTower = hasTower;
}

// 获取炮塔坑的矩形区域的函数实现
QRect TowerPit::getRect() const {
    return QRect(position, QSize(m_width, m_height));
}

// 静态函数，用于创建多个炮塔坑
QList<TowerPit*> TowerPit::createTowerPits() {
    QList<TowerPit*> pits;
    int index = 0;

    // 横向循环
    for (int i = 0; i < 8; ++i) {
        pits.append(new TowerPit(QPoint(300 + i * 90, 120), index++));
    }
    for (int i = 0; i < 4; ++i) {
        pits.append(new TowerPit(QPoint(50 + i * 90, 220), index++));
    }
    for (int i = 0; i < 8; ++i) {
        pits.append(new TowerPit(QPoint(170 + i * 90, 310), index++));
    }
    for (int i = 0; i < 7; ++i) {
        pits.append(new TowerPit(QPoint(330 + i * 90, 510), index++));
    }
    for (int i = 0; i < 4; ++i) {
        pits.append(new TowerPit(QPoint(350 + i * 90, 700), index++));
    }

    // 竖向循环
    for (int i = 0; i < 3; ++i) {
        pits.append(new TowerPit(QPoint(1000,200 + i * 90), index++));
    }
    for (int i = 0; i < 3; ++i) {
        pits.append(new TowerPit(QPoint(1100,200 + i * 90), index++));
    }
    for (int i = 0; i < 3; ++i) {
        pits.append(new TowerPit(QPoint(20, 400 + i * 90), index++));
    }
    for (int i = 0; i < 3; ++i) {
        pits.append(new TowerPit(QPoint(120, 400 + i * 90), index++));
    }

    return pits;
}

// 检查炮塔坑中是否有障碍物
bool TowerPit::hasObstacle() const {
    return m_obstacle != nullptr;
}

// 设置炮塔坑中的障碍物
void TowerPit::setObstacle(Obstacle* obstacle) {
    m_obstacle = obstacle;
}

// 获取炮塔坑中的障碍物
Obstacle* TowerPit::getObstacle() const {
    return m_obstacle;
}

// 获取炮塔坑的序号
int TowerPit::getIndex() const {
    return m_index;
}
