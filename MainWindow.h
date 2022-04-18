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
    void restartGame(bool checked);
    void winGame();
    void loseGame();
    void exit();
    void setDifficulty(int size);
    void showAboutDialog();

private:
    int m_rows;
    int m_cols;
    int m_numMines;
    int m_custRows;
    int m_custCols;
    int m_percentMines;
    GameManager *m_gameManager;
    BoardWidget *m_ui;
    QPushButton *m_restartButton;
    QPushButton *m_button;
};

#endif // MAINWINDOW_H
