#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Board.h"
#include "BoardWidget.h"
#include <QObject>

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);
    void setUI(BoardWidget *ui);
    void showHints(bool hints);

public slots:

private slots:
    // Slots to handle GameSignals
    void startGame(int rows, int cols, int mines);
    // Internal slots
    void cellClicked(int row, int col);
    void cellFlagged(int row, int col);

private:
    bool isValidCell(int row, int col);
    void clearCell(int row, int col);
    void clearNeighboringCells(int row, int col);
    void clearAllCells();
    void flagAllBombs();
    void doGameLost();
    void doGameWon();

private:
    Board *m_board;
    BoardWidget *m_ui;
    int m_rows;
    int m_cols;
    int m_mines;
    bool m_showHints;
};

#endif // GAMEMANAGER_H
