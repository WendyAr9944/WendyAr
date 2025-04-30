#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QPixmap>
#include "enemy.h"
#include "obstacle.h"

// 子弹类定义
class Bullet
{
public:
    // 构造函数，初始化子弹的起始位置、目标、攻击力和贴图，目标为敌人
    Bullet(const QPoint& startPos, Enemy* target, int damage, const QString& imagePath);
    // 构造函数，初始化子弹的起始位置、目标、攻击力和贴图，目标为障碍物
    Bullet(const QPoint& startPos, Obstacle* target, int damage, const QString& imagePath);
    // 析构函数
    ~Bullet();

    // 绘制子弹的方法
    void draw(QPainter* painter) const;
    // 移动子弹的方法
    void move();
    // 判断子弹是否击中目标的方法
    bool isHit() const;

private:
    QPoint m_startPos; // 子弹起始位置
    QPoint m_currentPos; // 子弹当前位置
    Enemy* m_enemyTarget; // 子弹的敌人目标
    Obstacle* m_obstacleTarget; // 子弹的障碍物目标
    int m_damage; // 子弹攻击力
    QPixmap m_pixmap; // 子弹贴图
    bool m_isReachedTarget; // 子弹是否到达目标的标志
    int m_speed; // 子弹移动速度
    int m_rotation; // 子弹旋转角度
    bool m_shouldRotate;  // 标记子弹是否需要旋转

    // 计算子弹的移动方向
    QPoint calculateMoveDirection() const;
    // 获取目标位置
    QPointF getTargetPosition() const;
    // 检查是否到达目标并造成伤害
    void checkAndApplyDamage();
};

#endif // BULLET_H
