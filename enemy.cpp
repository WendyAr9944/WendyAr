#include "enemy.h"
#include"QtMath"
Enemy::Enemy(Type type)
    : type(type), isFirstPixmap(true), currentPathIndex(0)
{
    switch (type) {
    case Monster1:
        speed = 4;
        health = 100;
        enemyPixmap.load(":/images/images/monster1.png");
        enemyPixmap2.load(":/images/images/monster1_2.png"); // 加载第二张贴图
        pathPoints << QPointF(400, 200)   // 起点
                   << QPointF(930, 200)  // 右移
                   << QPointF(930, 400)  // 下移
                   << QPointF(210, 400)  // 左移
                   << QPointF(210, 600)  // 下移
                   << QPointF(700, 600); // 终点
        position = pathPoints[0];
        break;
    case Monster2:
        speed = 6;
        health = 100;
        enemyPixmap.load(":/images/images/monster2.png");
        enemyPixmap2.load(":/images/images/monster2_2.png"); // 加载第二张贴图
        pathPoints << QPointF(400, 240)   // 起点
                   << QPointF(900, 240)  // 右移
                   << QPointF(900, 410)  // 下移
                   << QPointF(210, 410)  // 左移
                   << QPointF(210, 650)  // 下移
                   << QPointF(700, 650); // 终点
        position = pathPoints[0];
        break;
    case Monster3:
        speed = 4;
        health = 120;
        enemyPixmap.load(":/images/images/monster3.png");
        enemyPixmap2.load(":/images/images/monster3_2.png"); // 加载第二张贴图
        pathPoints << QPointF(400, 200)   // 起点
                   << QPointF(930, 200)  // 右移
                   << QPointF(930, 400)  // 下移
                   << QPointF(210, 400)  // 左移
                   << QPointF(210, 650)  // 下移
                   << QPointF(700, 650); // 终点
        position = pathPoints[0];
        break;
    case Monster4:
            speed = 3.5;
            health = 150;
            enemyPixmap.load(":/images/images/monster4.png"); // 新怪兽第一张贴图
            enemyPixmap2.load(":/images/images/monster4_2.png"); // 新怪兽第二张贴图
            pathPoints << QPointF(400, 200)   // 起点
                       << QPointF(920, 200)  // 右移
                       << QPointF(920, 420)  // 下移
                       << QPointF(220, 420)  // 左移
                       << QPointF(220, 620)  // 下移
                       << QPointF(700, 620); // 终点
            position = pathPoints[0];
            break;
        case Monster5:
            speed = 4;
            health = 2000;
            enemyPixmap.load(":/images/images/monster5.png"); // 新怪兽第一张贴图
            enemyPixmap2.load(":/images/images/monster5_2.png"); // 新怪兽第二张贴图
            pathPoints << QPointF(400, 200)   // 起点
                       << QPointF(910, 200)  // 右移
                       << QPointF(910, 400)  // 下移
                       << QPointF(230, 400)  // 左移
                       << QPointF(230, 600)  // 下移
                       << QPointF(700, 600); // 终点
            position = pathPoints[0];
            break;
    case MonsterBoss:
        speed = 2;
        health = 180;
        enemyPixmap.load(":/images/images/monsterboss.png");
        enemyPixmap2.load(":/images/images/monsterboss.png"); // 加载第二张贴图
        pathPoints << QPointF(400, 200)   // 起点
                   << QPointF(930, 200)  // 右移
                   << QPointF(930, 400)  // 下移
                   << QPointF(210, 400)  // 左移
                   << QPointF(210, 650)  // 下移
                   << QPointF(700, 650); // 终点
        position = pathPoints[0];
        break;
    }
    enemyPixmap = enemyPixmap.scaled(80, 80, Qt::KeepAspectRatio);
    enemyPixmap2 = enemyPixmap2.scaled(80, 80, Qt::KeepAspectRatio);
}

Enemy::~Enemy()
{
}

void Enemy::move()
{
    if (currentPathIndex < pathPoints.size()) {
        QPointF target = pathPoints[currentPathIndex];
        // 处理敌人在 X 轴上的移动
        if (position.x() < target.x()) {
            position.setX(std::min(position.x() + speed, target.x()));
        }
        else if (position.x() > target.x()) {
            position.setX(std::max(position.x() - speed, target.x()));
        }
        // 处理敌人在 Y 轴上的移动
        if (position.y() < target.y()) {
            position.setY(std::min(position.y() + speed, target.y()));
        }
        else if (position.y() > target.y()) {
            position.setY(std::max(position.y() - speed, target.y()));
        }
        if (position == target) {
            currentPathIndex++;
        }
    }
    switchPixmap(); // 移动时切换贴图
}

void Enemy::draw(QPainter* painter)
{
    QPixmap currentPixmap = isFirstPixmap ? enemyPixmap : enemyPixmap2;//贴图状态
    if (type == MonsterBoss) {
        painter->drawPixmap(position.x(), position.y(), currentPixmap.scaled(120,120,Qt::KeepAspectRatio));
    }
    else
       painter->drawPixmap(position.x(), position.y(), currentPixmap);
}

void Enemy::takeDamage(int damage)
{
    health -= damage;
}

int Enemy::getHealth() const
{
    return health;
}

QPointF Enemy::getPosition() const
{
    return position;
}
const QVector<QPointF>& Enemy::getPathPoints() const
{
    return pathPoints;
}
Enemy::Type Enemy::getType() const
{
    return type;
}
void Enemy::setSpeed(int newSpeed)
{
    speed = newSpeed;
}
double Enemy::getTraveledDistance() const
{
    // 初始化一个变量 distance 用于存储敌人已经走过的总距离，初始值为 0
    double distance = 0;

    // 遍历敌人已经走过的路径点，currentPathIndex 表示敌人当前所在路径点的索引
    for (int i = 0; i < currentPathIndex; ++i) {
        // 获取当前路径点 p1
        QPointF p1 = pathPoints[i];
        // 获取下一个路径点 p2
        QPointF p2 = pathPoints[i + 1];
        // 计算两点之间的欧几里得距离，并累加到总距离 distance 中
        // 欧几里得距离公式为：sqrt((x2 - x1)^2 + (y2 - y1)^2)
        distance += qSqrt(qPow(p2.x() - p1.x(), 2) + qPow(p2.y() - p1.y(), 2));
    }

    // 检查敌人是否还未到达路径的最后一个点
    if (currentPathIndex < pathPoints.size() - 1) {
        // 获取敌人当前所在的路径点 p1
        QPointF p1 = pathPoints[currentPathIndex];
        // 获取敌人的当前位置 p2
        QPointF p2 = position;
        // 计算从当前路径点到敌人当前位置的欧几里得距离，并累加到总距离 distance 中
        distance += qSqrt(qPow(p2.x() - p1.x(), 2) + qPow(p2.y() - p1.y(), 2));
    }

    // 返回敌人已经走过的总距离
    return distance;
}
void Enemy::switchPixmap()
{
    isFirstPixmap = !isFirstPixmap; // 切换贴图标记
}
