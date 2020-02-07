#ifndef BOARD_H
#define BOARD_H

#include <QObject>

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    void initialize(int rows, int cols);
    bool hasBomb(int row, int col);
    int bombCount(int row, int col);
    void setBombs(int numBombs);

signals:


private:
    bool isValidCell(int row, int col);
    int setBomb(int row, int col);
    void calcBombs();

public slots:

private:
    int *m_board;
    int m_rows;
    int m_cols;
};

#endif // BOARD_H
