#ifndef RADISH_H
#define RADISH_H

#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QVector>

class Radish
{
public:
    Radish(const QPoint& position);
    ~Radish();
    void draw(QPainter* painter);
    void takeDamage(double damage);
    int getHealth() const;
    void resetHealth();//重置生命值
private:
    QPoint position;
    int health;
    QVector<QPixmap> radishPixmaps;
    QVector<QPixmap> healthPixmaps;
};

#endif // RADISH_H
