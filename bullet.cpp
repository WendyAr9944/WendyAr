#include "bullet.h"
#include <QPainter>
#include <qmath.h>
#include <QDebug>

// 子弹类构造函数实现，目标为敌人
Bullet::Bullet(const QPoint& startPos, Enemy* target, int damage, const QString& imagePath)
    : m_startPos(startPos), m_currentPos(startPos), m_enemyTarget(target), m_obstacleTarget(nullptr), m_damage(damage), m_isReachedTarget(false), m_speed(20), m_rotation(0)
{
    if (!m_pixmap.load(imagePath)) {
        // 处理图片加载失败的情况
        qDebug() << "Failed to load bullet image: " << imagePath;
    }
    // 判断是否为星星子弹或风扇子弹，若是则需要旋转
    if (imagePath.contains("star_bullet") || imagePath.contains("fan_bullet")) {
        m_shouldRotate = true;
    } else {
        m_shouldRotate = false;
    }
}

// 子弹类构造函数实现，目标为障碍物
Bullet::Bullet(const QPoint& startPos, Obstacle* target, int damage, const QString& imagePath)
    : m_startPos(startPos), m_currentPos(startPos), m_enemyTarget(nullptr), m_obstacleTarget(target), m_damage(damage), m_isReachedTarget(false), m_speed(20), m_rotation(0)
{
    if (!m_pixmap.load(imagePath)) {
        // 处理图片加载失败的情况
        qDebug() << "Failed to load bullet image: " << imagePath;
    }
    // 判断是否为星星子弹或风扇子弹，若是则需要旋转
    if (imagePath.contains("star_bullet") || imagePath.contains("fan_bullet")) {
        m_shouldRotate = true;
    }
    else {
        m_shouldRotate = false;
    }
}

// 子弹类析构函数实现
Bullet::~Bullet()
{
    // 这里可以添加释放资源的代码，如果有的话
}

// 绘制子弹的方法实现
void Bullet::draw(QPainter* painter) const
{
    if (!m_pixmap.isNull()) {
        painter->save();
        // 平移到图片中心，以便围绕中心绘制
        painter->translate(m_currentPos.x() + m_pixmap.width() / 2, m_currentPos.y() + m_pixmap.height() / 2);
        if (m_shouldRotate) {
            // 若需要旋转，进行旋转操作
            painter->rotate(m_rotation);
        }
        // 绘制子弹图片
        painter->drawPixmap(-m_pixmap.width() / 2, -m_pixmap.height() / 2, m_pixmap);
        painter->restore();
    }
}

// 移动子弹的方法实现
void Bullet::move()
{
    if (m_enemyTarget && m_enemyTarget->getHealth() <= 0) {
        // 若目标敌人已死亡，标记子弹已到达目标
        m_isReachedTarget = true;
        return;
    }

    QPoint direction = calculateMoveDirection();
    m_currentPos += direction;
    if (m_shouldRotate) {
        // 若需要旋转，更新旋转角度
        m_rotation += 5;  // 每次旋转 5 度
    }

    checkAndApplyDamage();
}

// 判断子弹是否击中目标的方法实现
bool Bullet::isHit() const
{
    return m_isReachedTarget;
}

// 计算子弹的移动方向的方法实现
QPoint Bullet::calculateMoveDirection() const
{
    QPointF targetPos = getTargetPosition();
    QPointF diff = targetPos - m_currentPos;
    double length = qSqrt(diff.x() * diff.x() + diff.y() * diff.y());
    if (length > 0) {
        return QPoint(static_cast<int>(diff.x() * m_speed / length), static_cast<int>(diff.y() * m_speed / length));
    }
    return QPoint(0, 0);
}

// 获取目标位置的方法实现
QPointF Bullet::getTargetPosition() const
{
    if (m_enemyTarget) {
        return m_enemyTarget->getPosition();
    } else if (m_obstacleTarget) {
        return m_obstacleTarget->getPosition();
    }
    return QPointF(0, 0);
}

// 检查是否到达目标并造成伤害的方法实现
void Bullet::checkAndApplyDamage()
{
    // 获取子弹的目标位置
    QPointF targetPos = getTargetPosition();
    // 计算子弹当前位置和目标位置的曼哈顿距离（横纵距离之和），并判断是否小于等于子弹的移动速度，是的话就认为子弹击中了目标
    if ((m_currentPos - targetPos).manhattanLength() <= m_speed) {
        if (m_enemyTarget) {
            // 若目标为敌人，敌人受到伤害
            m_enemyTarget->takeDamage(m_damage);
        } else if (m_obstacleTarget) {
            // 若目标为障碍物，障碍物受到伤害
            m_obstacleTarget->takeDamage(m_damage);
        }
        // 标记子弹已到达目标
        m_isReachedTarget = true;
    }
}
