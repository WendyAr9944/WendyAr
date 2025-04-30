#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QPointF>
#include <QPixmap>

class Obstacle
{
public:
    Obstacle(const QPointF& pos);
    ~Obstacle();

    void draw(QPainter* painter) const;
    void takeDamage(int damage);
    int getHealth() const;
    QPointF getPosition() const;
    QPixmap getPixmap() const;
    bool isSelected() const;
    void setSelected(bool selected);
private:
    QPointF position;
    int health;
    QPixmap pixmap;
    bool m_selected;
    QPixmap selectedPixmap; // 选中时的定位小贴图
};

#endif // OBSTACLE_H
