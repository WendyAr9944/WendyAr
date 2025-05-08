// startwindow.cpp
#include "startwindow.h"
#include <QPixmap>

StartWindow::StartWindow(QWidget *parent) : QWidget(parent)
{
    // 设置窗口固定大小（与主窗口一致）
    setFixedSize(1200, 800); // 新增：固定窗口大小

    // 设置背景图
    backgroundLabel = new QLabel(this);
    updateBackground(); // 新增：封装背景更新逻辑

    // 创建开始游戏按钮
    startButton = new QPushButton(this);
    startButton->setGeometry(450, 250, 200, 100); // 调整按钮位置和大小
    startButton->setStyleSheet("background: transparent; border: none;"); // 设置透明和无边框
    connect(startButton, &QPushButton::clicked, this, &StartWindow::onStartGameClicked);

    // 创建规则说明按钮
    rulesButton = new QPushButton(this);
    rulesButton->setGeometry(1000, 700, 200, 100); // 调整按钮位置和大小
    rulesButton->setStyleSheet("background: transparent; border: none;"); // 设置透明和无边框
    connect(rulesButton, &QPushButton::clicked, this, &StartWindow::onRulesClicked);
}

// 新增：窗口大小变化时自动调整背景
void StartWindow::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    updateBackground();
}

// 新增：更新背景图片尺寸
void StartWindow::updateBackground() {
    QPixmap backgroundPixmap(":/images/images/background.png");
    backgroundLabel->setPixmap(backgroundPixmap.scaled(
        this->width(),
        this->height(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
    ));
    backgroundLabel->setGeometry(0, 0, this->width(), this->height());
}
StartWindow::~StartWindow()
{
    // 析构函数
}

void StartWindow::onStartGameClicked()
{
    emit startGame();
}

void StartWindow::onRulesClicked()
{
    emit showRules();
}
