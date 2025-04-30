#ifndef TOWERPIT_H
#define TOWERPIT_H

#include <QPoint>
#include <QRect>
#include <QList>
#include "obstacle.h"

class TowerPit {
public:
    // 构造函数，初始化炮塔坑的位置，默认没有炮塔
    TowerPit(const QPoint& pos, int width = 50, int height = 50);

    // 获取炮塔坑的位置
    QPoint getPosition() const;

    // 检查炮塔坑是否已有炮塔
    bool hasTower() const;

    // 设置炮塔坑是否有炮塔
    void setHasTower(bool hasTower);

    // 获取炮塔坑的矩形区域
    QRect getRect() const;

    // 静态函数，用于创建多个炮塔坑
    static QList<TowerPit*> createTowerPits();
    // 检查炮塔坑中是否有障碍物
        bool hasObstacle() const;

    // 设置炮塔坑中的障碍物
    void setObstacle(Obstacle* obstacle);

    // 获取炮塔坑中的障碍物
    Obstacle* getObstacle() const;

private:
    QPoint position;  // 炮塔坑的位置
    bool m_hasTower;  // 标记是否已有炮塔
    int m_width;      // 炮塔坑的宽度
    int m_height;     // 炮塔坑的高度
    Obstacle* m_obstacle; // 指向障碍物的指针
};

#endif // TOWERPIT_H
