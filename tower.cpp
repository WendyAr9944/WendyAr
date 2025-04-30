#include "tower.h"
#include "bullet.h"
#include <QPainter>
#include <QtMath>
#include <QTime>

// 炮塔类构造函数实现
Tower::Tower(Type type, const QPoint& pos)
    : towerType(type), position(pos), level(1), rotationAngle(0)
{
    // 根据不同的炮塔类型，初始化炮塔的属性和贴图
    switch (type) {
    case CANNON:
        pixmap.load(":/images/images/cannon.png");
        range = 100; // 射程
        damage = 20;
        upgradeCost = 50;
        sellPrice = 30;
        break;
    case POOP:
        pixmap.load(":/images/images/poop.png");
        range = 80;
        damage = 15;
        upgradeCost = 40;
        sellPrice = 25;
        break;
    case STAR:
        pixmap.load(":/images/images/star.png");
        range = 120;
        damage = 25;
        upgradeCost = 60;
        sellPrice = 35;
        break;
    }
    // 初始化上次发射子弹的时间
    lastShotTime = QTime::currentTime();
}

// 炮塔类析构函数实现，释放子弹资源
Tower::~Tower()
{
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
}

// 绘制炮塔的方法实现
void Tower::draw(QPainter* painter, const QList<Enemy *>& enemies)
{
    if (towerType == CANNON) {
        QPointF targetPos;
        // 寻找射程范围内已移动路径最远的敌人
        Enemy* targetEnemy = nullptr;
        double maxTraveledDistance = 0;
        for (Enemy* enemy : enemies) {
            if ((enemy->getPosition() - position).manhattanLength() <= range) {
                double traveledDistance = enemy->getTraveledDistance();
                if (traveledDistance > maxTraveledDistance) {
                    maxTraveledDistance = traveledDistance;
                    targetEnemy = enemy;
                }
            }
        }
        if (targetEnemy) {
            targetPos = targetEnemy->getPosition();
            // 计算炮塔需要旋转的角度
            QPointF diff = targetPos - position;
            rotationAngle = qAtan2(diff.y(), diff.x()) * 180 / M_PI; // 更新旋转角度
            painter->save();
            // 平移到图片中心，以便围绕中心旋转
            painter->translate(position.x() + pixmap.width() / 2, position.y() + pixmap.height() / 2);
            painter->rotate(rotationAngle);
            // 绘制旋转后的炮塔图片
            painter->drawPixmap(-pixmap.width() / 2, -pixmap.height() / 2, pixmap.scaled(80, 80, Qt::KeepAspectRatio));
            painter->restore();
        } else {
            // 若没有目标敌人，正常绘制炮塔图片
            painter->drawPixmap(position, pixmap.scaled(80, 80, Qt::KeepAspectRatio));
        }
    } else {
        // 非加农炮炮塔正常绘制
        painter->drawPixmap(position, pixmap.scaled(80, 80, Qt::KeepAspectRatio));
    }
}

// 炮塔攻击敌人的方法实现
void Tower::attack(const QList<Enemy *>& enemies)
{
    int interval = 0;
    // 根据不同的炮塔类型，设置子弹发射间隔
    switch (towerType) {
    case CANNON:
        interval = 2000;  // 2s
        break;
    case POOP:
        interval = 2500;  // 2.5s
        break;
    case STAR:
        interval = 3000;  // 3s
        break;
    }

    // 检查是否达到发射间隔
    if (lastShotTime.msecsTo(QTime::currentTime()) >= interval) {
        Enemy* targetEnemy = nullptr;
        double maxTraveledDistance = 0;
        // 寻找射程范围内已移动路径最远的敌人
        for (Enemy* enemy : enemies) {
            if ((enemy->getPosition() - position).manhattanLength() <= range) {
                double traveledDistance = enemy->getTraveledDistance();
                if (traveledDistance > maxTraveledDistance) {
                    maxTraveledDistance = traveledDistance;
                    targetEnemy = enemy;
                }
            }
        }
        if (targetEnemy) {
            QString bulletImagePath;
            // 根据炮塔类型选择子弹贴图
            switch (towerType) {
            case CANNON:
                bulletImagePath = ":/images/images/cannon_bullet.png";
                break;
            case POOP:
                bulletImagePath = ":/images/images/poop_bullet.png";
                break;
            case STAR:
                bulletImagePath = ":/images/images/star_bullet.png";
                break;
            }
            // 创建新的子弹并添加到子弹列表
            bullets.push_back(new Bullet(position, targetEnemy, damage, bulletImagePath));
            // 更新上次发射子弹的时间
            lastShotTime = QTime::currentTime();
        }
    }
}

// 炮塔攻击障碍物的方法实现
void Tower::attackObstacle(Obstacle* obstacle)
{
    int interval = 0;
    // 根据不同的炮塔类型，设置子弹发射间隔
    switch (towerType) {
    case CANNON:
        interval = 2000;  // 2s
        break;
    case POOP:
        interval = 2500;  // 2.5s
        break;
    case STAR:
        interval = 3000;  // 3s
        break;
    }

    // 检查是否达到发射间隔
    if (lastShotTime.msecsTo(QTime::currentTime()) >= interval) {
        if (obstacle && isInRange(obstacle->getPosition())) {
            QString bulletImagePath;
            // 根据炮塔类型选择子弹贴图
            switch (towerType) {
            case CANNON:
                bulletImagePath = ":/images/images/cannon_bullet.png";
                break;
            case POOP:
                bulletImagePath = ":/images/images/poop_bullet.png";
                break;
            case STAR:
                bulletImagePath = ":/images/images/star_bullet.png";
                break;
            }
            // 创建新的子弹并添加到子弹列表
            bullets.push_back(new Bullet(position, obstacle, damage, bulletImagePath));
            // 更新上次发射子弹的时间
            lastShotTime = QTime::currentTime();
        }
    }
}

// 更新炮塔发射的子弹状态的方法实现
void Tower::updateBullets()
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        Bullet* bullet = *it;
        bullet->move();
        if (bullet->isHit()) {
            // 若子弹击中目标，释放子弹资源并从列表中移除
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

// 升级炮塔的方法实现
void Tower::upgrade()
{
    level++;
    damage += 10;
    range += 20;
    upgradeCost += 20;
}

// 获取炮塔升级所需成本的方法实现
int Tower::getUpgradeCost() const
{
    return upgradeCost;
}

// 获取出售炮塔所得金额的方法实现
int Tower::getSellPrice() const
{
    return sellPrice;
}

// 获取炮塔位置的方法实现
QPoint Tower::getPosition() const
{
    return position;
}

// 获取炮塔贴图的方法实现
QPixmap Tower::getPixmap() const
{
    return pixmap;
}

// 获取炮塔攻击范围的方法实现
int Tower::getRange() const
{
    return range;
}

// 静态方法，获取指定类型炮塔的创建成本
int Tower::getCost(Type type)
{
    switch (type) {
    case CANNON:
        return 100;
    case POOP:
        return 80;
    case STAR:
        return 120;
    default:
        return 0;
    }
}

// 获取炮塔发射的子弹列表的方法实现
const std::vector<Bullet*>& Tower::getBullets() const
{
    return bullets;
}

// 判断目标位置是否在射程范围内
bool Tower::isInRange(const QPointF& targetPos) const
{
    qreal dx = targetPos.x() - position.x();
    qreal dy = targetPos.y() - position.y();
    qreal distance = qSqrt(dx * dx + dy * dy);
    return distance <= range;
}
