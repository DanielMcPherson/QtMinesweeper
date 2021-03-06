#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include "Cell.h"
#include <QWidget>
#include <QLayout>
#include <QVector>

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(int numRows, int numCols, QWidget *parent = nullptr);
    void init(int numRows, int numCols);
    void clearCell(int row, int col, int count, bool mine);
    void flagCell(int row, int col, bool flagged);
    void misflagCell(int row, int col);
    void explode(int row, int col);
    void gameWon();
    void gameLost();
    void setMine(int row, int col);
    void showHints(bool showHints);

signals:
    void cellClicked(int row, int col);
    void cellFlagged(int row, int col);

private slots:
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
