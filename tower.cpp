#include "tower.h"
#include "bullet.h"
#include <QPainter>
#include <QtMath>
#include <QTime>

// 炮塔类构造函数实现
Tower::Tower(Type type, const QPoint& pos)
    : towerType(type), position(pos), level(1), rotationAngle(0)
{
    // 计算新的位置
        QPoint newPos = pos;
        if (type == CANNON) {
            // 炮塔坑的宽度为 80
            int pitWidth = 80;
            newPos.setX(pos.x() + pitWidth / 4);
        }
        position = newPos;
    // 根据不同的炮塔类型，初始化炮塔的属性和贴图
    switch (type) {
    case CANNON:
        pixmap.load(":/images/images/cannon.png");
        upgradedPixmap.load(":/images/images/cannon_upgraded.png");
        range = 200; // 射程
        damage = 50;
        upgradeCost = 180;
        sellPrice = 50;
        // 设置旋转中心，这里假设旋转中心
        rotationCenter = QPointF(position.x() + pixmap.width() / 2, position.y() + pixmap.height()/2);
        break;
    case POOP:
        pixmap.load(":/images/images/poop.png");
        upgradedPixmap.load(":/images/images/poop_upgraded.png");
        range = 200;
        damage = 50;
        upgradeCost = 180;
        sellPrice = 50;
        break;
    case STAR:
        pixmap.load(":/images/images/star.png");
        upgradedPixmap.load(":/images/images/star_upgraded.png");
        range = 240;
        damage = 80;
        upgradeCost = 180;
        sellPrice = 50;
        break;
    case FAN:
        pixmap.load(":/images/images/fan.png");
        upgradedPixmap.load(":/images/images/fan_upgraded.png");
        range = 240;
        damage = 80;
        upgradeCost = 180;
        sellPrice = 50;
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
void Tower::draw(QPainter* painter, const QList<Enemy *>& enemies, Obstacle* obstacle)
{
    QPointF targetPos; // 目标位置（敌人或障碍物）
    Enemy* targetEnemy = nullptr; // 目标敌人
    double maxTraveledDistance = 0; // 敌人已移动的最大距离

    // 第一阶段：寻找射程范围内的目标
    // 寻找射程范围内已移动路径最远的敌人（最接近终点的敌人）
    for (Enemy* enemy : enemies) {
        if ((enemy->getPosition() - position).manhattanLength() <= range) {
            double traveledDistance = enemy->getTraveledDistance();
            if (traveledDistance > maxTraveledDistance) {
                maxTraveledDistance = traveledDistance;
                targetEnemy = enemy;
            }
        }
    }
    // 如果找到敌人，记录其位置
    if (targetEnemy) {
        targetPos = targetEnemy->getPosition();
    }
    // 否则检查是否有障碍物且在射程内
    else if (obstacle && isInRange(obstacle->getPosition())) {
        targetPos = obstacle->getPosition();
    }
    // 第二阶段：计算炮塔需要旋转的角度
    if (!targetPos.isNull()) {
        // 计算从炮塔位置到目标的向量
        QPointF diff = targetPos - position;
        // 使用atan2计算角度（弧度），然后转换为度
        // 加90度是为了调整坐标系（Qt默认0度指向右侧，而游戏需要0度指向上方）
        rotationAngle = qAtan2(diff.y(), diff.x()) * 180 / M_PI + 90; // 加90度以调整初始方向
    }
    // 第三阶段：绘制炮塔（带旋转效果）
    painter->save();// 保存当前绘制状态（防止影响其他元素）
    if (towerType == CANNON) {
        // 加农炮需要特殊处理旋转（围绕中心旋转）
        painter->translate(rotationCenter.x(), rotationCenter.y()); // 平移到旋转中心
        painter->rotate(rotationAngle); // 旋转指定角度

        // 绘制旋转后的炮塔图片（根据等级选择普通或升级后的贴图）
        if (level > 1) {
            painter->drawPixmap(-upgradedPixmap.width() / 2, -upgradedPixmap.height() / 2, upgradedPixmap.scaled(80, 80, Qt::KeepAspectRatio));
        }
        else {
            painter->drawPixmap(-pixmap.width() / 2, -pixmap.height() / 2, pixmap.scaled(80, 80, Qt::KeepAspectRatio));
        }
    }
    else {
        // 对于非CANNON炮塔，直接绘制
        if (level > 1) {
            painter->drawPixmap(position.x() , position.y(), upgradedPixmap.scaled(80, 80, Qt::KeepAspectRatio));
        } else {
            painter->drawPixmap(position.x(), position.y(), pixmap.scaled(80, 80, Qt::KeepAspectRatio));
        }
    }
    painter->restore();// 恢复绘制状态
}

// 升级炮塔的方法实现
void Tower::upgrade()
{
    if (towerType == CANNON) {
        // 单独控制升级后cannon炮塔的位置
        //int pitWidth = 80;
        upgradedPosition.setX(position.x());
        upgradedPosition.setY(position.y());
        rotationCenter = QPointF(upgradedPosition.x() + upgradedPixmap.width() / 2, upgradedPosition.y() + upgradedPixmap.height()/2);
    }
    level++;
    damage += 20;
    range += 30;
    upgradeCost += 30;
}

// 炮塔攻击敌人的方法实现
void Tower::attack(const QList<Enemy *>& enemies)
{
    int interval = 0;
    // 根据不同的炮塔类型，设置子弹发射间隔（毫秒）
    switch (towerType) {
    case CANNON:  // 加农炮射速最快
        interval = 500;  // 0.5秒发射一次
        break;
    case POOP:    // 便便炮塔次之
        interval = 1000;  // 1秒发射一次
        break;
    case STAR:    // 星星炮塔较慢
        interval = 1500;  // 1.5秒发射一次
        break;
    case FAN:     // 风扇炮塔射速最慢
        interval = 2000;  // 2秒发射一次
        break;
    }
    // 检查是否达到发射间隔（防止炮塔无限连发）
    if (lastShotTime.msecsTo(QTime::currentTime()) >= interval) {
        Enemy* targetEnemy = nullptr;  // 目标敌人
        double maxTraveledDistance = 0;  // 敌人已移动的最大距离
        // 遍历所有敌人，寻找射程范围内最移动距离最远的敌人
        for (Enemy* enemy : enemies) {
            // 计算敌人与炮塔的距离（横纵距离之和）
            if ((enemy->getPosition() - position).manhattanLength() <= range) {
                // 如果敌人在射程内，比较它已移动的距离
                double traveledDistance = enemy->getTraveledDistance();
                if (traveledDistance > maxTraveledDistance) {
                    maxTraveledDistance = traveledDistance;
                    targetEnemy = enemy;  // 记录距离最远的敌人为目标
                }
            }
        }
        // 如果找到目标敌人，就发射子弹
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
            case FAN:
                bulletImagePath = ":/images/images/fan_bullet.png";
                break;
            }
            // 计算子弹发射位置（从炮塔中心还是炮口）
            QPointF bulletStartPos;
            if (towerType == CANNON) {
                // 加农炮需要计算炮口位置（考虑旋转角度）
                double angle = qDegreesToRadians(rotationAngle - 90);
                double offsetX = 50 * qCos(angle);  // 炮口偏移量（X方向）
                double offsetY = 50 * qSin(angle);  // 炮口偏移量（Y方向）
                bulletStartPos = rotationCenter + QPointF(offsetX, offsetY);
            }
            else {
                // 其他炮塔从中心发射
                bulletStartPos = position;
            }
            // 创建新的子弹并添加到子弹列表
            bullets.push_back(new Bullet(position, targetEnemy, damage, bulletImagePath));
            // 更新上次发射时间，用于控制射速
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
        interval = 1500;
        break;
    case POOP:
        interval = 2000;
        break;
    case STAR:
        interval = 2000;
        break;
    case FAN:
        interval = 2000;
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
            case FAN:
                bulletImagePath = ":/images/images/fan_bullet.png";
                break;
            }

            // 计算炮塔需要旋转的角度
            QPointF targetPos = obstacle->getPosition();
            QPointF diff = targetPos - position;
            rotationAngle = qAtan2(diff.y(), diff.x()) * 180 / M_PI + 90; // 加90度以调整初始方向

            // 计算子弹发射位置
            QPointF bulletStartPos;
            if (towerType == CANNON) {
                double angle = qDegreesToRadians(rotationAngle - 90);
                double offsetX = 35 * qCos(angle); // 假设炮塔口距离中心40像素
                double offsetY = 35 * qSin(angle);
                bulletStartPos = rotationCenter + QPointF(offsetX, offsetY);
            }
            else {
                bulletStartPos = position;
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

// 获取炮塔升级所需成本的方法实现
int Tower::getUpgradeCost() const
{
    return upgradeCost;
}

// 获取出售炮塔所得金额的方法实现
int Tower::getSellPrice() const
{
    return sellPrice * 0.5;
}

// 获取炮塔位置的方法实现
QPoint Tower::getPosition() const
{
    return position;
}

// 获取炮塔贴图的方法实现
QPixmap Tower::getPixmap() const
{
    if (level > 1) {
        return upgradedPixmap;
    }
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
        return 120;
    case STAR:
        return 160;
    case FAN:
        return 160;
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
