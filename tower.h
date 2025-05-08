#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QPixmap>
#include <QList>
#include <QPainter>
#include <QTime>  // 引入QTime类，用于控制子弹发射的时间间隔
#include "enemy.h"
#include "bullet.h"
#include "obstacle.h"

// 炮塔类定义
class Tower
{
public:
    // 炮塔类型枚举，包含加农炮、便便、星星、风扇四种类型
    enum Type {
        CANNON,
        POOP,
        STAR,
        FAN
    };

    // 构造函数，根据炮塔类型和位置创建炮塔
    Tower(Type type, const QPoint& pos);
    // 析构函数，负责释放炮塔的子弹资源
    ~Tower();

    // 绘制炮塔的方法，需要传入painter对象和敌人列表以及障碍物指针
    void draw(QPainter* painter, const QList<Enemy *>& enemies, Obstacle* obstacle = nullptr);
    // 炮塔攻击敌人的方法，传入敌人列表
    void attack(const QList<Enemy *>& enemies);
    // 攻击障碍物的方法，传入障碍物对象
    void attackObstacle(Obstacle* obstacle);
    // 更新炮塔发射的子弹状态的方法
    void updateBullets();
    // 升级炮塔的方法
    void upgrade();
    // 获取炮塔升级所需成本的方法
    int getUpgradeCost() const;
    // 获取出售炮塔所得金额的方法
    int getSellPrice() const;
    // 获取炮塔位置的方法
    QPoint getPosition() const;
    // 获取炮塔贴图的方法
    QPixmap getPixmap() const;
    // 获取炮塔攻击范围的方法
    int getRange() const;
    // 静态方法，获取指定类型炮塔的创建成本
    static int getCost(Type type);
    // 获取炮塔发射的子弹列表的方法
    const std::vector<Bullet*>& getBullets() const;
    // 判断障碍物是否在射程范围内
    bool isInRange(const QPointF& targetPos) const;

private:
    Type towerType;    // 炮塔类型
    QPoint position;   // 炮塔位置
    QPointF rotationCenter; //炮塔旋转中心
    QPixmap pixmap;    // 炮塔贴图
    QPixmap upgradedPixmap; // 升级后的炮塔贴图
    int range;         // 炮塔攻击范围
    int damage;        // 炮塔攻击力
    int level;         // 炮塔等级
    int upgradeCost;   // 炮塔升级成本
    int sellPrice;     // 炮塔出售价格
    std::vector<Bullet*> bullets;  // 炮塔发射的子弹列表
    QTime lastShotTime;  // 记录上次发射子弹的时间，用于控制发射间隔
    double rotationAngle; // 旋转角度
    QPointF upgradedPosition; // 升级后的位置
    bool isUpgraded; // 标记是否已升级
};

#endif // TOWER_H
