#include "obstacle.h"
#include <QPainter>

Obstacle::Obstacle(const QPointF& pos, const QString& imagePath, int width, int height)
    : position(pos), health(300), m_selected(false)
{
    QPixmap originalPixmap(imagePath);
    pixmap = originalPixmap.scaled(width, height, Qt::KeepAspectRatio);//根据指定1大小缩放
    selectedPixmap.load(":/images/images/selected_marker.png"); // 假设的选中标记贴图路径
}

Obstacle::~Obstacle()
{
}

void Obstacle::draw(QPainter* painter) const
{
    painter->drawPixmap(position.toPoint(), pixmap);
    if (m_selected) {
        QPointF markerPos(position.x() + pixmap.width() / 2 - selectedPixmap.width() / 2, position.y() - selectedPixmap.height());
        painter->drawPixmap(markerPos.toPoint(), selectedPixmap);
    }
}

void Obstacle::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

int Obstacle::getHealth() const
{
    return health;
}

QPointF Obstacle::getPosition() const
{
    return position;
}

QPixmap Obstacle::getPixmap() const
{
    return pixmap;
}

bool Obstacle::isSelected() const
{
    return m_selected;
}

void Obstacle::setSelected(bool selected)
{
    m_selected = selected;
}
