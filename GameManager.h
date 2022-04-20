#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Board.h"
#include "GameSignals.h"
#include <QObject>

// Game logic
// Maintains the state of the board and determines the new game
// state given user actions.
// Connects to GameSignals signals to know when user actions have
// occurred, and emits signals to communicate updated game state
// to the UI.

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);
    void showHints(bool hints);

private slots:
    void startGame(int rows, int cols, int mines);
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
    GameSignals *m_gameSignals;
    int m_rows;
    int m_cols;
    int m_mines;
    bool m_showHints;
};

#endif // GAMEMANAGER_H
