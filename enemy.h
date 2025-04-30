#ifndef ENEMY_H
#define ENEMY_H

#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QVector> // 用于存储路径点

class Enemy
{
public:
    enum Type {
        Monster1,
        Monster2,
        Monster3,
        MonsterBoss
    };

    Enemy(Type type);
    ~Enemy();

    void move();
    void draw(QPainter* painter);
    void takeDamage(int damage);
    int getHealth() const;
    QPointF getPosition() const;
    const QVector<QPointF>& getPathPoints() const; // 添加获取路径点列表的方法
    Type getType() const;
    void setSpeed(int newSpeed);//以便便便塔可以降低怪兽的速度
    double getTraveledDistance() const; // 新增方法
private:
    Type type;
    QPointF position;
    double speed;
    int health;
    QPixmap enemyPixmap;
    QVector<QPointF> pathPoints;
    int currentPathIndex;
};

#endif // ENEMY_H
