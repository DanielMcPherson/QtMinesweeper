#ifndef GAMESIGNALS_H
#define GAMESIGNALS_H

#include <QObject>

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
    // Player actions
    void playerClickedCell(int row, int col);
    void playerFlaggedCell(int row, int col);
    // Cell changes
    void setCellFlagged(int row, int col, bool flagged);
    void clearCell(int row, int col, int count, bool hasMine);
    void explode(int row, int col);
    void markIncorrectlyFlaggedCell(int row, int col);
};

#endif // GAMESIGNALS_H
