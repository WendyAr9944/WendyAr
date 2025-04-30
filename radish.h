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
    void takeDamage(int damage);
    int getHealth() const;

private:
    QPoint position;
    int health;
    QVector<QPixmap> radishPixmaps;
    QVector<QPixmap> healthPixmaps;
};

#endif // RADISH_H
