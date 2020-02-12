#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QVector>

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    void initialize(int rows, int cols, int numMines);
    bool hasMine(int row, int col);
    int mineCount(int row, int col);
    void toggleFlag(int row, int col);
    void clearCell(int row, int col);
    bool isFlagged(int row, int col);
    bool isCleared(int row, int col);
    bool mineTriggered();
    int numSurroundingFlags(int row, int col);

signals:


private:
    bool isValidCell(int row, int col);
    void setMines(int numMines);
    void setMine(int row, int col);
    void calcMineCounts();
    int numSurroundingMines(int row, int col);

public slots:

private:
    struct CellStruct {
        bool flagged;
        bool cleared;
        bool hasMine;
        int numNeighboringMines;
    };
    QVector<CellStruct> m_cells;
    int m_rows;
    int m_cols;
    bool m_mineTriggered;

};

#endif // BOARD_H
