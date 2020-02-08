#ifndef BOARD_H
#define BOARD_H

#include <QObject>

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    void initialize(int rows, int cols);
    bool hasMine(int row, int col);
    int mineCount(int row, int col);
    void setMines(int numMines);

signals:


private:
    bool isValidCell(int row, int col);
    int setMine(int row, int col);
    void calcMineCounts();

public slots:

private:
    int *m_board;
    int m_rows;
    int m_cols;
};

#endif // BOARD_H
