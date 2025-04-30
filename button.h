#ifndef BUTTON_H
#define BUTTON_H

#include <QPoint>
#include <QPixmap>

class Button
{
public:
    enum ButtonType {
        PAUSE,
        DOUBLE_SPEED,
        CLICK_OBSTACLE,
        TOWER_UPGRADE,
        TOWER_REMOVE
    };

    Button(const QPoint& pos, const QPixmap& normalPixmap, const QPixmap& hoverPixmap, ButtonType type);
    bool isClicked(const QPoint& pos);
    void draw(QPainter* painter) const;
    ButtonType getType() const;

private:
    QPoint position;
    QPixmap normalPixmap;
    QPixmap hoverPixmap;
    ButtonType buttonType;
    bool isActive; // 记录按钮是否处于激活（点击）状态
};

#endif // BUTTON_H
