// mainwindow.cpp
#include "mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include "ui_mainwindow.h"
#include <QPixmap>

// 主窗口类的构造函数实现
mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent), currentWave(0), totalWaves(5),elapsedTime(0), isGameOver(false), showButtons(false), gameSpeed(1),
      selectedTower(nullptr), selectedObstacle(nullptr), isPaused(false), countdownValue(4),isVictory(false)
{
    ui = new Ui::mainwindow;
    ui->setupUi(this);
    this->setFixedSize(1200, 800); // 设置窗口固定大小为 1200x800
    // 加载初始界面背景图片
    backgroundPixmap.load(":/images/images/background.png");
    // 加载游戏地图图片
    mapPixmap.load(":/images/images/map21.png");
    setFixedSize(1200, 800);
    // 启动定时器，每隔100毫秒触发一次定时器事件
    waveTimerId = -1;//
    // 创建萝卜对象
    radish = new Radish(QPoint(750, 600));
    // 加载游戏失败图片
    gameOverPixmap.load(":/images/images/lose.png");
    // 加载游戏胜利贴图
    winPixmap.load(":/images/images/win.png");
    winPixmap = winPixmap.scaled(1200,800);
    // 定义确定按钮点击区域（居中，覆盖贴图中的按钮区域）
    confirmButtonRect = QRect(400,600,200, 100);
    // 创建金钱管理对象，初始金钱为450
    money = new Money(450);
    // 加载金币图标
    coinPixmap.load(":/images/images/coin.png");
    // 初始化背景音乐
    bgmPlayer = new QMediaPlayer(this);
    bgmPlayer->setMedia(QUrl("qrc:/sounds/sounds/background_music.mp3")); // 设置音频文件
    bgmPlayer->setVolume(50); // 设置音量（0-100）
    bgmPlayer->play();
    // 实现循环播放（Qt5 需要手动连接信号）
    connect(bgmPlayer, &QMediaPlayer::stateChanged, [=]() {
        if (bgmPlayer->state() == QMediaPlayer::StoppedState) {
            bgmPlayer->play(); // 播放结束后重新播放
        }
    });

    // 加载各种按钮的正常状态图片
    QPixmap pauseNormalPixmap(":/images/images/pause_normal.png");
    QPixmap pauseHoverPixmap(":/images/images/pause_hover.png"); // 悬停图片加载
    QPixmap doubleSpeedNormalPixmap(":/images/images/double_speed_normal.png");
    QPixmap doubleSpeedHoverPixmap(":/images/images/double_speed_hover.png"); // 悬停图片加载
    QPixmap cannonButtonPixmap(":/images/images/cannon_button.png");
    QPixmap poopButtonPixmap(":/images/images/poop_button.png");
    QPixmap starButtonPixmap(":/images/images/star_button.png");
    QPixmap upgradeNormalGrayPixmap(":/images/images/upgrade_normal_gray.png");
    QPixmap upgradeNormalBluePixmap(":/images/images/upgrade_normal_blue.png");
    QPixmap upgradeHoverBluePixmap(":/images/images/upgrade_hover_blue.png");
    QPixmap removeNormalPixmap(":/images/images/remove_normal.png");
    QPixmap removeHoverPixmap(":/images/images/remove_hover.png");
    QPixmap crossPixmap(":/images/images/cross.png");

    // 保持纵横比对按键进行缩放
    QPixmap scaledPausePixmap = pauseNormalPixmap.scaled(60, 60, Qt::KeepAspectRatio);
    QPixmap scaledPauseHoverPixmap = pauseHoverPixmap.scaled(60, 60, Qt::KeepAspectRatio); // 缩放悬停图片
    QPixmap scaledDoubleSpeedPixmap = doubleSpeedNormalPixmap.scaled(120, 120, Qt::KeepAspectRatio);
    QPixmap scaledDoubleSpeedHoverPixmap = doubleSpeedHoverPixmap.scaled(120, 120, Qt::KeepAspectRatio); // 缩放悬停图片

    // 创建暂停和二倍速按钮
    buttons.append(new Button(QPoint(1000, 10), scaledPausePixmap, scaledPauseHoverPixmap, Button::PAUSE));
    buttons.append(new Button(QPoint(800, 10), scaledDoubleSpeedPixmap, scaledDoubleSpeedHoverPixmap, Button::DOUBLE_SPEED));

    // 初始化炮塔坑
    towerPits = TowerPit::createTowerPits();
    // 定义要放置障碍物的炮塔坑序号
    QList<int> targetIndices = {9, 15, 17,19,20,22,25,33,34,38};

    // 初始化障碍物
    QString obstaclePaths[10] = {
        ":/images/images/obstacle1.png",
        ":/images/images/obstacle2.png",
        ":/images/images/obstacle3.png",
        ":/images/images/obstacle4.png",
        ":/images/images/obstacle5.png",
        ":/images/images/obstacle6.png",
        ":/images/images/obstacle7.png",
        ":/images/images/obstacle8.png",
        ":/images/images/obstacle9.png",
        ":/images/images/obstacle10.png"
    };
    // 定义不同障碍物的大小
        QList<QSize> obstacleSizes = {
            QSize(180, 180),//obstacle1
            QSize(100, 100),//obstacle2
            QSize(100, 100),//obstacle3
            QSize(100, 100),//obstacle4
            QSize(100, 100),//obstacle5
            QSize(100, 100),//obstacle6
            QSize(180, 180),//obstacle7
            QSize(180, 180),//obstacle8
            QSize(100, 100),//obstacle9
            QSize(180, 180)//obstacle10
        };

        int pathIndex = 0;
        for (int index : targetIndices) {
            for (TowerPit* pit : towerPits) {
                if (pit->getIndex() == index) {
                    QSize size = obstacleSizes[pathIndex];
                    Obstacle* obstacle = new Obstacle(pit->getPosition(), obstaclePaths[pathIndex], size.width(), size.height());
                    obstacles.append(obstacle);
                    pit->setObstacle(obstacle);
                    pathIndex++;
                    break;
                }
            }
        }

    startWindow = new StartWindow(this);  // 创建 StartWindow 对象
    startWindow->show();                  // 显示 StartWindow

    connect(startWindow, &StartWindow::startGame, this, &mainwindow::onStartGame); // 连接开始游戏信号
    connect(startWindow, &StartWindow::showRules, this, &mainwindow::onShowRules); // 连接显示规则信号

    countdownLabel = new QLabel(this);    // 创建倒计时标签
    countdownLabel->setGeometry(500, 300, 200, 200); // 设置标签位置和大小
    countdownLabel->hide();               // 隐藏标签

    countdownTimer = new QTimer(this);    // 创建倒计时定时器
    connect(countdownTimer, &QTimer::timeout, this, &mainwindow::countdown); // 连接定时器超时信号
    // 加载倒计时贴图
    countdownPixmaps[0].load(":/images/images/3.png"); // 替换为实际的 3 的贴图路径
    countdownPixmaps[1].load(":/images/images/2.png"); // 替换为实际的 2 的贴图路径
    countdownPixmaps[2].load(":/images/images/1.png"); // 替换为实际的 1 的贴图路径
    countdownPixmaps[3].load(":/images/images/GO.png"); // 替换为实际的 GO 的贴图路径
    // 定义lose按钮的点击区域（根据实际贴图调整坐标）
    restartButtonRect = QRect(500, 400, 400, 100);  // x, y, width, height
    returnButtonRect = QRect(500, 550, 400, 100);
}

// 主窗口类的析构函数实现，释放动态分配的资源
mainwindow::~mainwindow()
{
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    for (Tower* tower : towers) {
        delete tower;
    }
    for (Button* button : buttons) {
        delete button;
    }
    for (Obstacle* obstacle : obstacles) {
        delete obstacle;
    }
    for (TowerPit* pit : towerPits) {
        delete pit;
    }
    delete radish;
    delete money;
    delete ui;
    delete startWindow;   // 新增：释放 StartWindow 对象
    delete countdownLabel; // 新增：释放倒计时标签
    delete countdownTimer; // 新增：释放倒计时定时器
    delete bgmPlayer;      // 释放音乐播放器
}

// 处理开始游戏信号的槽函数
void mainwindow::onStartGame()
{
    startWindow->hide();
    countdownValue = 4; // 重置倒计时值
    countdownLabel->show();
    countdownTimer->start(1000); // 每秒触发一次
}

// 新增：处理显示规则信号的槽函数
void mainwindow::onShowRules()
{
    // 显示规则说明文本
    QMessageBox::information(this, "规则说明", "这里是游戏的规则说明文本。");
}

// 新增：倒计时槽函数
void mainwindow::countdown()
{
    if (countdownValue > 0) {
        countdownValue--;
        update(); // 触发 paintEvent 重新绘制界面
    } else {
        countdownTimer->stop();
        countdownLabel->hide();
        elapsedTime = 0; // 重置游戏时间
        currentWave = 0; // 重置波数
        waveTimerId = startTimer(100); // 开始游戏定时器
    }
}

// 绘制事件处理函数的实现，用于绘制游戏界面元素
void mainwindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // 绘制背景图
    painter.drawPixmap(0, 0, backgroundPixmap.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    // 绘制游戏地图
    painter.drawPixmap(0, 0, mapPixmap.scaled(width(), height()));
    if (!startWindow->isVisible()) {
        // 只有在开始窗口隐藏后才绘制游戏元素
        // 绘制游戏地图
        painter.drawPixmap(0, 0, mapPixmap.scaled(width(), height()));
    }
    // 绘制所有敌人
    for (Enemy* enemy : enemies) {
        enemy->draw(&painter);
    }
//    //绘制炮塔坑
//    QPixmap pitPixmap(":/images/images/tower_pit.png"); // 假设的炮塔坑贴图路径
//    for (TowerPit* pit : towerPits) {
//        painter.drawPixmap(pit->getPosition(), pitPixmap.scaled(80,80,Qt::KeepAspectRatio));
//    }
    // 绘制所有炮塔
    for (Tower* tower : towers) {
        tower->draw(&painter, enemies);
        // 绘制炮塔发射的子弹
        for (Bullet* bullet : tower->getBullets()) {
            bullet->draw(&painter);
        }
    }

    // 绘制萝卜
    radish->draw(&painter);

    // 绘制所有障碍物
    for (Obstacle* obstacle : obstacles) {
        obstacle->draw(&painter);
    }
    // 绘制倒计时贴图
    if (countdownValue > 0 && countdownValue <= 4) {
        QPixmap countdownPixmap = countdownPixmaps[4 - countdownValue];
        painter.drawPixmap((width() - countdownPixmap.width()) / 2, (height() - countdownPixmap.height()) / 2, countdownPixmap);
    }
    // 如果需要显示创建炮塔的按钮，则绘制它们
        if (showButtons) {
            QPixmap cannonButtonPixmap(":/images/images/cannon_button.png");
            QPixmap poopButtonPixmap(":/images/images/poop_button.png");
            QPixmap starButtonPixmap(":/images/images/star_button.png");
            QPixmap fanButtonPixmap(":/images/images/fan_button.png");

            int buttonSize = 100;
            int gap = 10;

            // 绘制加农炮炮塔选择按钮
            painter.drawPixmap(clickPos.x(), clickPos.y(), cannonButtonPixmap.scaled(buttonSize, buttonSize));
            // 绘制便便炮塔选择按钮
            painter.drawPixmap(clickPos.x() + buttonSize + gap, clickPos.y(), poopButtonPixmap.scaled(buttonSize, buttonSize));
            // 绘制星星炮塔选择按钮
            painter.drawPixmap(clickPos.x() + 2 * (buttonSize + gap), clickPos.y(), starButtonPixmap.scaled(buttonSize, buttonSize));
            // 绘制风扇炮塔选择按钮
            painter.drawPixmap(clickPos.x() + 3 * (buttonSize + gap), clickPos.y(), fanButtonPixmap.scaled(buttonSize, buttonSize));
        }

        // 如果有选中的炮塔，则绘制该炮塔的升级和移除按钮
        if (selectedTower) {
            QPixmap upgradePixmap;
            // 根据金币数量选择升级按钮的图片
            if (money->canAfford(selectedTower->getUpgradeCost())) {
                upgradePixmap = QPixmap(":/images/images/upgrade_normal_blue.png").scaled(50,50,Qt::KeepAspectRatio);
            }
            else {
                upgradePixmap = QPixmap(":/images/images/upgrade_normal_gray.png").scaled(50,50,Qt::KeepAspectRatio);
            }
            // 计算升级按钮的位置并绘制
            QPoint upgradePos(selectedTower->getPosition().x(), selectedTower->getPosition().y() - upgradePixmap.height()-20);
            painter.drawPixmap(upgradePos, upgradePixmap);

            // 加载移除按钮的图片
            QPixmap removePixmap;
            removePixmap=QPixmap(":/images/images/remove_normal.png").scaled(50,50,Qt::KeepAspectRatio);
            // 计算移除按钮的位置并绘制
            QPoint removePos(selectedTower->getPosition().x(), selectedTower->getPosition().y() + 100);
            painter.drawPixmap(removePos, removePixmap);
        }
    // 绘制当前金钱数量
    QFont font;
    font.setFamily("Times New Roman"); // 设置字体家族为Times New Roman
    font.setPointSize(16);   // 设置字体大小为 16 磅
    font.setBold(true);      // 设置字体为粗体
    painter.setFont(font);   // 将字体应用到 QPainter
    painter.setPen(Qt::white);//设置字体颜色
    painter.drawText(130, 50, QString("Money: %1").arg(money->getAmount()));

    // 绘制所有按钮
    for (Button* button : buttons) {
        button->draw(&painter);
    }
    // 绘制波数信息
        QString waveText;
        if (isPaused) {
            waveText = "暂停中";
        } else {//%1 和 %2 是占位符，用于后续填充具体的值
            waveText = QString("%1/%2").arg(currentWave + 1).arg(totalWaves);
        }
        QFont waveFont;
        waveFont.setFamily("Ubuntu");
        waveFont.setPointSize(20);
        waveFont.setBold(true);
        painter.setFont(waveFont);
        QRect waveRect(0, 15, width(), 50);
        painter.drawText(waveRect, Qt::AlignHCenter | Qt::AlignVCenter, waveText);
        // 如果游戏胜利，绘制胜利贴图
        if (isVictory) {
            painter.drawPixmap(0, 0, width(), height(), winPixmap);
        }
        // 否则，如果游戏失败，绘制失败贴图
        else if (isGameOver) {
            painter.drawPixmap(0, 0, width(), height(), gameOverPixmap);
        }
}

// 定时器事件处理函数的实现，处理游戏的定时逻辑
void mainwindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == waveTimerId) {
        if (isGameOver || isPaused) {
            return;
        }

        for (int i = 0; i < gameSpeed; ++i) {
            // 增加游戏已过去的时间
            elapsedTime += 100;

            // 第一波怪物生成逻辑
            if (currentWave == 0 && elapsedTime <= 20000) {
                if (elapsedTime % 3000 == 0) {
                    generateMonster(Enemy::Monster1);
                }
                if (elapsedTime % 5000 == 0) {
                    generateMonster(Enemy::Monster2);
                }
            } else if (elapsedTime == 20000) {
                // 进入第二波
                currentWave = 1;
            }

            // 第二波怪物生成逻辑
            if (currentWave == 1 && elapsedTime - 20000 <= 20000) {
                if ((elapsedTime - 20000) % 3000 == 0) {
                    generateMonster(Enemy::Monster3);
                }
                if ((elapsedTime - 20000) % 5000 == 0) {
                    generateMonster(Enemy::Monster2);
                }
            } else if (elapsedTime == 40000) {
                // 进入第三波
                currentWave = 2;
            }

            // 第三波怪物生成逻辑
            if (currentWave == 2 && elapsedTime - 40000 <= 20000) {
                if ((elapsedTime - 40000) % 3000 == 0) {
                    generateMonster(Enemy::Monster4);
                }
                if ((elapsedTime - 40000) % 5000 == 0) {
                    generateMonster(Enemy::Monster5);
                }
            } else if (elapsedTime == 60000) {
                // 进入第四波
                currentWave = 3;
            }

            // 第四波怪物生成逻辑
            if (currentWave == 3 && elapsedTime - 60000 <= 20000) {
                if ((elapsedTime - 60000) % 3000 == 0) {
                    generateMonster(Enemy::Monster1);
                }
                if ((elapsedTime - 60000) % 5000 == 0) {
                    generateMonster(Enemy::Monster5);
                }
            } else if (elapsedTime == 80000) {
                // 进入第五波
                currentWave = 4;
            }

            // 第五波怪物生成逻辑
            if (currentWave == 4 && elapsedTime - 80000 <= 20000) {
                if ((elapsedTime - 80000) % 2000 == 0) {
                    generateMonster(Enemy::Monster1);
                }
                if ((elapsedTime - 80000) % 4000 == 0) {
                    generateMonster(Enemy::Monster2);
                }
                if ((elapsedTime - 80000) % 5000 == 0) {
                    generateMonster(Enemy::Monster3);
                }
            } else if (elapsedTime - 80000 == 30000) {
                generateMonster(Enemy::MonsterBoss);
            }

            // 移动所有敌人
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                Enemy* enemy = *it;
                enemy->move();
                if (enemy->getHealth() <= 0 || isEnemyAtEnd(enemy)) {
                    // 检测是否消灭了Boss且萝卜存活
                    if (enemy->getType() == Enemy::MonsterBoss && enemy->getHealth() <= 0 && radish->getHealth() > 0) {
                        isVictory = true;//游戏胜利
                        killTimer(waveTimerId);
                        waveTimerId = -1;
                    }
                    if (enemy->getHealth() <= 0) {
                        // 根据不同类型的敌人增加不同数量的金钱
                        switch (enemy->getType()) {
                        case Enemy::Monster1:
                            money->earn(50);
                            break;
                        case Enemy::Monster2:
                            money->earn(80);
                            break;
                        case Enemy::Monster3:
                            money->earn(80);
                            break;
                        case Enemy::Monster4:
                            money->earn(80);
                            break;
                        case Enemy::Monster5:
                            money->earn(80);
                            break;
                        case Enemy::MonsterBoss:
                            money->earn(200);
                            break;
                        }
                    }
                    if (isEnemyAtEnd(enemy)) {
                        // 检查敌人是否与萝卜碰撞
                        checkEnemyCollideWithRadish();
                    }
                    delete enemy;
                    it = enemies.erase(it); // 移除死亡或到达终点的敌人
                } else {
                    ++it;
                }
            }

            // 炮塔攻击敌人
            for (Tower* tower : towers) {
                if (selectedObstacle && tower->isInRange(selectedObstacle->getPosition())) {
                    tower->attackObstacle(selectedObstacle);
                } else {
                    tower->attack(enemies);
                }
                tower->updateBullets();
            }

            // 障碍物被攻击处理
            for (auto it = obstacles.begin(); it != obstacles.end(); ) {
                Obstacle* obstacle = *it;
                if (obstacle->getHealth() <= 0) {
                    // 障碍物被摧毁处理
                    if (selectedObstacle == obstacle) {
                        selectedObstacle = nullptr;
                    }
                    for (TowerPit* pit : towerPits) {
                        if (pit->getObstacle() == obstacle) {
                            pit->setObstacle(nullptr);
                            break;
                        }
                    }
                    delete obstacle;
                    it = obstacles.erase(it);
                } else {
                    ++it;
                }
            }

            // 如果萝卜生命值为0，游戏失败
            if (radish->getHealth() <= 0) {
                isGameOver = true;
                killTimer(waveTimerId);  // 立即停止定时器
                waveTimerId = -1;
            }
        }
        // 更新界面
        update();
    }
}

// 鼠标按下事件处理函数的实现，处理鼠标点击操作
void mainwindow::mousePressEvent(QMouseEvent *event)
{
    if (isVictory) {
        QPoint clickPos = event->pos();
        if (confirmButtonRect.contains(clickPos)) {
            returnToStart(); // 返回开始界面
            return;
        }
    }
    if (isGameOver) {
            QPoint clickPos = event->pos();
            // 检查是否点击了重新开始按钮区域
            if (restartButtonRect.contains(clickPos)) {
                restartGame();
                return;
            }
            // 检查是否点击了返回按钮区域
            if (returnButtonRect.contains(clickPos)) {
                returnToStart();
                return;
            }
        }

    QPoint clickPos = event->pos();

    // 检查是否点击了全局按钮（暂停、二倍速）
    for (Button* button : buttons) {
        if (button->isClicked(clickPos)) {
            handleButtonClick(button->getType());
            update(); // 强制更新界面，确保贴图刷新
            return;
        }
    }
    // 检查是否点击了障碍物
        for (Obstacle* obstacle : obstacles) {
            QRect obstacleRect(obstacle->getPosition().toPoint(), obstacle->getPixmap().size());
            if (obstacleRect.contains(clickPos)) {
                if (selectedObstacle == obstacle) {
                    // 再次点击已选中的障碍物，取消选中
                    selectedObstacle->setSelected(false);
                    selectedObstacle = nullptr;
                } else {
                    if (selectedObstacle) {
                        selectedObstacle->setSelected(false);
                    }
                    selectedObstacle = obstacle;
                    selectedObstacle->setSelected(true);
                }
                update();
                return;
            }
        }
    // 检查是否点击了炮塔坑
    for (TowerPit* pit : towerPits) {
        if (pit->getRect().contains(clickPos) && !pit->hasTower() && !pit->hasObstacle()) {
            this->clickPos = pit->getPosition();
            showButtons = true;
            update();
            return;
        }
    }

    // 检查是否点击了炮塔
        for (Tower* tower : towers) {
            QRect towerRect(tower->getPosition(), tower->getPixmap().size());
            if (towerRect.contains(clickPos)) {
                if (selectedTower == tower) {
                    // 再次点击已选中的炮塔，取消选中
                    selectedTower = nullptr;
                } else {
                    selectedTower = tower;
                }
                update();
                return;
            }
        }

        // 如果有选中的炮塔，检查是否点击了升级或移除按钮
        if (selectedTower) {
            QPixmap upgradePixmap;
            // 根据金币数量选择升级按钮的图片
            if (money->canAfford(selectedTower->getUpgradeCost())) {
                upgradePixmap = QPixmap(":/images/images/upgrade_normal_blue.png").scaled(50, 50, Qt::KeepAspectRatio);
            } else {
                upgradePixmap = QPixmap(":/images/images/upgrade_normal_gray.png").scaled(50, 50, Qt::KeepAspectRatio);
            }
            // 计算升级按钮的矩形区域
            QPoint upgradePos(selectedTower->getPosition().x(), selectedTower->getPosition().y() - upgradePixmap.height());
            QRect upgradeRect(upgradePos, upgradePixmap.size());

            // 加载移除按钮的图片
            QPixmap removePixmap=QPixmap(":/images/images/remove_normal.png").scaled(50, 50, Qt::KeepAspectRatio);
            // 计算移除按钮的矩形区域
            QPoint removePos(selectedTower->getPosition().x(), selectedTower->getPosition().y() + selectedTower->getPixmap().height()-35);
            QRect removeRect(removePos, removePixmap.size());

            // 如果点击了升级按钮且金币足够，进行升级操作并取消选中
            if (upgradeRect.contains(clickPos) && money->canAfford(selectedTower->getUpgradeCost())) {
                selectedTower->upgrade();
                money->spend(selectedTower->getUpgradeCost());
                selectedTower = nullptr; // 取消选中
                update();
                return;
            }

            // 如果点击了移除按钮，移除炮塔并取消选中
            if (removeRect.contains(clickPos)) {
                money->earn(selectedTower->getSellPrice());
                auto it = std::find(towers.begin(), towers.end(), selectedTower);
                if (it != towers.end()) {
                    delete *it;
                    towers.erase(it);
                }
                selectedTower = nullptr; // 取消选中
                update();
                return;
            }
        }

        // 如果显示了选项按钮，检查是否点击了某个选项按钮
        if (showButtons) {
            QPixmap cannonButtonPixmap(":/images/images/cannon_button.png");
            QPixmap poopButtonPixmap(":/images/images/poop_button.png");
            QPixmap starButtonPixmap(":/images/images/star_button.png");

            int buttonSize = 100;
            int gap = 10;

            // 定义三个选项按钮的矩形区域
            QRect cannonButtonRect(this->clickPos.x(), this->clickPos.y(), buttonSize, buttonSize);
            QRect poopButtonRect(this->clickPos.x() + buttonSize + gap, this->clickPos.y(), buttonSize, buttonSize);
            QRect starButtonRect(this->clickPos.x() + 2 * (buttonSize + gap), this->clickPos.y(), buttonSize, buttonSize);
            QRect fanButtonRect(this->clickPos.x() + 3 * (buttonSize + gap), this->clickPos.y(), buttonSize, buttonSize);
            // 标记是否点击了有效按钮
            bool validButtonClicked = false;

            // 如果点击了加农炮选项按钮
            if (cannonButtonRect.contains(clickPos)) {
                if (money->canAfford(Tower::getCost(Tower::CANNON))) {
                    towers.push_back(new Tower(Tower::CANNON, this->clickPos));
                    money->spend(Tower::getCost(Tower::CANNON));
                    validButtonClicked = true;
                }
            }
            // 如果点击了便便炮塔选项按钮
            else if (poopButtonRect.contains(clickPos)) {
                if (money->canAfford(Tower::getCost(Tower::POOP))) {
                    towers.push_back(new Tower(Tower::POOP, this->clickPos));
                    money->spend(Tower::getCost(Tower::POOP));
                    validButtonClicked = true;
                }
            }
            // 如果点击了星星炮塔选项按钮
            else if (starButtonRect.contains(clickPos)) {
                if (money->canAfford(Tower::getCost(Tower::STAR))) {
                    towers.push_back(new Tower(Tower::STAR, this->clickPos));
                    money->spend(Tower::getCost(Tower::STAR));
                    validButtonClicked = true;
                }
            }
            // 如果点击了风扇炮塔选项按钮
            else if (fanButtonRect.contains(clickPos)) {
                if (money->canAfford(Tower::getCost(Tower::FAN))) {
                    towers.push_back(new Tower(Tower::FAN, this->clickPos));
                    money->spend(Tower::getCost(Tower::FAN));
                    validButtonClicked = true;
                }
            }
            // 无论是否成功放置炮塔，点击按钮后都隐藏选项按钮
            showButtons = false;
            update(); // 刷新界面

            // 如果点击了有效按钮，直接返回，避免其他逻辑处理
            if (validButtonClicked) {
                return;
            }
        }
}

// 生成指定类型的怪物
void mainwindow::generateMonster(int type)
{
    enemies.append(new Enemy(static_cast<Enemy::Type>(type)));
}

// 检查敌人是否到达终点
bool mainwindow::isEnemyAtEnd(Enemy* enemy)
{
    QPointF endPoint = enemy->getPathPoints().last();
    return enemy->getPosition() == endPoint;
}

// 检查敌人是否与萝卜碰撞，并处理萝卜的生命值减少
void mainwindow::checkEnemyCollideWithRadish()
{
    for (Enemy* enemy : enemies) {
        if (isEnemyAtEnd(enemy)) {
            switch (enemy->getType()) {
            case Enemy::Monster1:
                radish->takeDamage(0.5);
                break;
            case Enemy::Monster2:
            case Enemy::Monster3:
            case Enemy::Monster4:
            case Enemy::Monster5:
                radish->takeDamage(1);
                break;
            case Enemy::MonsterBoss:
                radish->takeDamage(10);
                break;
            }
        }
    }
}

// 创建指定类型的炮塔
void mainwindow::createTower(Tower::Type type)
{
    if (money->canAfford(Tower::getCost(type))) {
        towers.append(new Tower(type, clickPos));
        money->spend(Tower::getCost(type));
    }
}

// 处理按钮点击事件，根据按钮类型执行相应的操作
void mainwindow::handleButtonClick(Button::ButtonType type)
{
    switch (type) {
    case Button::PAUSE:
        if (waveTimerId != -1) {
            killTimer(waveTimerId);
            waveTimerId = -1;
            isPaused = true;
        } else {
            if (gameSpeed == 1) {
                waveTimerId = startTimer(100);
            } else {
                waveTimerId = startTimer(50);
            }
             isPaused = false;
        }
        update();
        break;
    case Button::DOUBLE_SPEED:
        gameSpeed = (gameSpeed == 1) ? 2 : 1;
        if (waveTimerId != -1) {
            killTimer(waveTimerId);
            if (gameSpeed == 1) {
                waveTimerId = startTimer(100);
            } else {
                waveTimerId = startTimer(50);
            }
        }
        update();
        break;
    case Button::CLICK_OBSTACLE:
        break;
    case Button::TOWER_UPGRADE:
        break;
    case Button::TOWER_REMOVE:
        break;
    case Button::RESTART:
        break;
    case Button:: RETURN:
        break;
    }
}
// 重新开始游戏逻辑
void mainwindow::restartGame() {
    // 重置游戏状态
    isGameOver = false;
    isVictory = false;
    currentWave = 0;
    elapsedTime = 0;
    radish->resetHealth();
    money->reset(450);

    // 清空敌人和炮塔
    for (Enemy* enemy : enemies) delete enemy;
    enemies.clear();
    for (Tower* tower : towers) delete tower;
    towers.clear();

    // 重启定时器
    if (waveTimerId != -1) killTimer(waveTimerId);
    waveTimerId = startTimer(100);
    update();
}

// 返回开始界面逻辑
void mainwindow::returnToStart()
{
    // 重置所有游戏状态
    isGameOver = false;
    isVictory = false;
    currentWave = 0;
    elapsedTime = 0;
    radish->resetHealth();
    money->reset(450);

    // 清空敌人和炮塔
    qDeleteAll(enemies);
    enemies.clear();
    qDeleteAll(towers);
    towers.clear();

    // 显示开始界面
    startWindow->show();
    this->hide();

    // 停止定时器
    if (waveTimerId != -1) killTimer(waveTimerId);
}
