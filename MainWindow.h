#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "Board.h"
#include "BoardWidget.h"
#include "Cell.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool isValidCell(int row, int col);
    void doGameLost();
    void doGameWon();
    void clearCell(int row, int col);
    void clearNeighboringCells(int row, int col);
    void clearAllCells();
    void flagAllBombs();

private slots:
    void cellClicked(int row, int col);
    void flagCell(int row, int col);

private:
    int m_boardSize;
    int m_numMines;
    int m_numSqauresToClear;
    int m_numCleared;
    Board m_board;
    BoardWidget *m_ui;
};

#endif // MAINWINDOW_H
