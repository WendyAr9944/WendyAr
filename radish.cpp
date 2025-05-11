#include "radish.h"

Radish::Radish(const QPoint& position)
    : position(position), health(10)
{
    // 加载萝卜不同生命状态的贴图
    for (int i = 1; i <= 10; ++i) {
        QString filename = QString(":/images/images/radish_%1.png").arg(i);
        //:/images/images/radish_10.png
        QPixmap pixmap;
        pixmap.load(filename);
        radishPixmaps.append(pixmap);
    }

     //加载生命值数字的贴图
    for (int i = 1; i <= 10; ++i) {
        QString filename = QString(":/images/images/health_%1.png").arg(i);
        QPixmap pixmap;
        pixmap.load(filename);
        healthPixmaps.append(pixmap);
    }
}

Radish::~Radish()
{
}

void Radish::draw(QPainter* painter)
{
    if (health > 0) {
        // 绘制萝卜贴图
        QPixmap radishPixmap = radishPixmaps[health - 1].scaled(100, 100,Qt::KeepAspectRatio);
        painter->drawPixmap(position.x(), position.y(), radishPixmap);

        // 绘制生命值贴图
        QPixmap healthPixmap = healthPixmaps[health - 1].scaled(40, 25,Qt::KeepAspectRatio);
        int healthX = position.x() + radishPixmap.width(); // 萝卜右侧
        int healthY = position.y(); // 萝卜顶部
        painter->drawPixmap(healthX, healthY, healthPixmap);
    }
}

void Radish::takeDamage(double damage)
{
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

int Radish::getHealth() const
{
    return health;
}
//重置生命值
void Radish::resetHealth() {
    health = 10;
}
