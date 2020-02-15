#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "BoardWidget.h"
#include "GameManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void startGame();

private slots:
    void restartClicked(bool checked);
    void winGame();
    void loseGame();

private:
    int m_boardSize;
    int m_numMines;
    GameManager *m_gameManager;
    BoardWidget *m_ui;
    QPushButton *m_restartButton;
    QPushButton *m_button;
};

#endif // MAINWINDOW_H
