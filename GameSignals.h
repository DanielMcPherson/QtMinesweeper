#ifndef GAMESIGNALS_H
#define GAMESIGNALS_H

#include <QObject>

// GameSignals contains signals for all game events.
//
// Defining these signals here allows the UI and backend game logic
// to be completely decoupled.
// The altnerative would be to have signals defined in BoardWidget
// and/or GameManager and to require at least one of those classes
// to be aware of each other in order to connect the signals to
// the taget slots.

class GameSignals : public QObject
{
    Q_OBJECT
    // Private constructor so that no objects can be created
    explicit GameSignals(QObject *parent = nullptr);
    static GameSignals *instance;

public:
    // Public function to get single instance of object
    static GameSignals *getInstance();

signals:
    // Start/win/lose
    void startGame(int rows, int cols, int mines);
    void gameWon();
    void gameLost();
    // Game initialization
    void setMine(int row, int col);
    // Player actions (from UI)
    void playerClickedCell(int row, int col);
    void playerFlaggedCell(int row, int col);
    // Game state actions (from backend)
    void setCellFlagged(int row, int col, bool flagged);
    void clearCell(int row, int col, int count, bool hasMine);
    void explode(int row, int col);
    void markIncorrectlyFlaggedCell(int row, int col);
};

#endif // GAMESIGNALS_H
