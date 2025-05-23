#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QList>
#include <QPixmap>
#include <QTimer>
#include <QLabel>
#include "startwindow.h"
#include "enemy.h"
#include "tower.h"
#include "radish.h"
#include "money.h"
#include "button.h"
#include "obstacle.h"
#include "towerpit.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    // 构造函数，初始化窗口和游戏相关属性
    mainwindow(QWidget *parent = nullptr);
    // 析构函数，释放动态分配的资源
    ~mainwindow();

protected:
    // 重写绘制事件处理函数，用于绘制游戏界面元素
    void paintEvent(QPaintEvent *event) override;
    // 重写定时器事件处理函数，处理游戏的定时逻辑，如怪物生成、移动等
    void timerEvent(QTimerEvent *event) override;
    // 重写鼠标按下事件处理函数，处理鼠标点击操作，如按钮点击、炮塔创建等
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onStartGame();    // 处理开始游戏信号的槽函数
    void onShowRules();    // 处理显示规则信号的槽函数
    void countdown();      // 倒计时槽函数
    void restartGame();       // 重新开始游戏
    void returnToStart();     // 返回开始界面

private:
    Ui::mainwindow* ui; // UI界面指针
    QMediaPlayer* bgmPlayer;//音乐
    QList<Enemy*> enemies; // 存储所有敌人对象的列表
    QList<Tower*> towers; // 存储所有炮塔对象的列表
    Radish* radish; // 萝卜对象指针
    Money* money; // 金钱管理对象指针
    int currentWave; // 当前游戏波数
    int totalWaves; // 总游戏波数
    int waveTimerId; // 定时器ID，用于控制游戏定时逻辑
    QPixmap mapPixmap; // 游戏地图的图片
    int elapsedTime; // 游戏已过去的时间
    QPixmap gameOverPixmap; // 游戏结束时显示的图片
    bool isGameOver; // 游戏是否结束的标志
    QPoint clickPos; // 鼠标点击的位置
    bool showButtons; // 是否显示创建炮塔的按钮
    QList<Button*> buttons; // 存储所有按钮对象的列表
    int gameSpeed; // 游戏速度
    QPixmap coinPixmap; // 金币图标
    Tower* selectedTower; // 记录当前选中的炮塔，用于显示升级和移除按钮
    QList<TowerPit*> towerPits; // 炮塔坑列表
    QList<Obstacle*> obstacles; // 障碍物列表
    Obstacle* selectedObstacle;  // 定义成员变量
    bool isPaused; // 判断游戏是否暂停

    StartWindow *startWindow; // StartWindow 对象指针
    QLabel *countdownLabel;   // 倒计时标签指针
    QTimer *countdownTimer;   // 倒计时定时器指针
    int countdownValue;       // 倒计时数值
    QPixmap countdownPixmaps[4]; // 用于存储倒计时 3，2，1 的贴图
    QPixmap backgroundPixmap;
    QRect restartButtonRect;  // 重新开始按钮的点击区域
    QRect returnButtonRect;   // 返回按钮的点击区域
    bool isVictory;// 是否胜利
    QPixmap winPixmap;// 胜利贴图
    QRect confirmButtonRect;// 确定按钮点击区域

    // 生成指定波数的怪物
    void generateWave(int wave);
    // 生成指定类型的怪物
    void generateMonster(int type);
    // 检查敌人是否到达终点
    bool isEnemyAtEnd(Enemy* enemy);
    // 检查敌人是否与萝卜碰撞，并处理萝卜的生命值减少
    void checkEnemyCollideWithRadish();
    // 创建指定类型的炮塔
    void createTower(Tower::Type type);
    // 处理按钮点击事件，根据按钮类型执行相应的操作
    void handleButtonClick(Button::ButtonType type);
    // 检查点击位置是否在路径上
    bool isClickOnPath(const QPoint& pos) const;
    // 检查点击位置是否在障碍物上
    Obstacle* isClickOnObstacle(const QPoint& pos) const;
    // 让周围一定范围内的炮塔攻击障碍物
    void attackObstacle(Obstacle* obstacle);
    // 检查点击位置是否在炮塔上
    bool isClickOnTower(const QPoint& pos) const;
};

#endif // MAINWINDOW_H
