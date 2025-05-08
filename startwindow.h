// startwindow.h
#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

signals:
    void startGame();
    void showRules();

private slots:
    void onStartGameClicked();
    void onRulesClicked();

private:
    QPushButton *startButton;
    QPushButton *rulesButton;
    QLabel *backgroundLabel;
protected:
    void resizeEvent(QResizeEvent *event) override; // 新增：处理窗口大小变化
private:
    void updateBackground(); // 新增：更新背景
};

#endif // STARTWINDOW_H
