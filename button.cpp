#include "button.h"
#include <QPainter>

Button::Button(const QPoint& pos, const QPixmap& normalPixmap, const QPixmap& hoverPixmap, ButtonType type)
    : position(pos), normalPixmap(normalPixmap), hoverPixmap(hoverPixmap), buttonType(type), isActive(false)
{
}

bool Button::isClicked(const QPoint& pos)
{
    QRect buttonRect(position, normalPixmap.size());
    if (buttonRect.contains(pos)) {
        isActive =!isActive; // 切换按钮状态
        return true;
    }
    return false;
}

void Button::draw(QPainter* painter) const
{
    if (isActive) {
        painter->drawPixmap(position, hoverPixmap);
    } else {
        painter->drawPixmap(position, normalPixmap);
    }
}

Button::ButtonType Button::getType() const
{
    return buttonType;
}
