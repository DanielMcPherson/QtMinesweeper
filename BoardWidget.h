#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include "Cell.h"
#include <QWidget>
#include <QLayout>
#include <QVector>

// Widget to provide the minesweeper UI
// Contains no game logic other than detecting user actions
// and emitting a signal when an action has occurred

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);

private slots:
    // Slots to handle Game Signals
    void startGame(int rows, int cols, int mines);
    void setMine(int row, int col);
    void flagCell(int row, int col, bool flagged);
    void clearCell(int row, int col, int count, bool mine);
    void explode(int row, int col);
    void misflagCell(int row, int col);
    // Slots to handle user actions
    void click(int row, int col);
    void rightClick(int row, int col);

private:
    Cell *getCell(int row, int col);

private:
    QGridLayout *m_layout;
    QVector<Cell *> m_cells;
    int m_numRows;
    int m_numCols;
    bool m_gameOver;
};

#endif // BOARDWIDGET_H
