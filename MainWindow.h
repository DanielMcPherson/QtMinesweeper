#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "Board.h"
#include "Cell.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool isValidCell(int row, int col);

private slots:
    void cellClicked(int row, int col);
    void clickNeighboringCells(int row, int col);
    void clickAllCells();

private:
    int m_boardSize;
    Board m_board;
    QVector<Cell *> m_cells;
};

#endif // MAINWINDOW_H
