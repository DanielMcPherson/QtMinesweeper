#include "GameManager.h"
#include <QPoint>
#include <QStack>
#include <QDebug>

GameManager::GameManager(QObject *parent) : QObject(parent)
{
    // Internal representation of board
    m_board = new Board();

    // Initialize settings
    m_showHints = false;

    // Connect to Game Signals
    m_gameSignals = GameSignals::getInstance();
    connect(m_gameSignals, &GameSignals::startGame, this, &GameManager::startGame);
    connect(m_gameSignals, &GameSignals::playerClickedCell, this, &GameManager::cellClicked);
    connect(m_gameSignals, &GameSignals::playerFlaggedCell, this, &GameManager::cellFlagged);
}

// Set whether to show hints or not
void GameManager::showHints(bool hints)
{
    m_showHints = hints;
}

// Start a game
void GameManager::startGame(int rows, int cols, int mines)
{
    m_rows = rows;
    m_cols = cols;
    m_mines = mines;

    // Initialize board
    m_board->initialize(m_rows, m_cols, m_mines);

    // Tell the UI the mines are (for debug/cheat hints)
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            if (m_board->hasMine(row, col)) {
                emit m_gameSignals->setMine(row, col);
            }
        }
    }
}

// Do row and col designate a valid cell?
bool GameManager::isValidCell(int row, int col)
{
    // Make sure row and col are within bounds
    if (row < 0 || row >= m_rows) {
        return false;
    }
    if (col < 0 || col >= m_cols) {
        return false;
    }

    return true;
}

// Called when cell is clicked in the UI
void GameManager::cellClicked(int row, int col)
{
    bool clearSurrounding = false;

    // Don't let player accidentally click flagged cells
    if (m_board->isFlagged(row, col)) {
        return;
    }

    // If this cell has not been cleared already
    if (!m_board->isCleared(row, col)) {
        clearCell(row, col);
        // Check if the player cleared a mine
        if (m_board->mineTriggered()) {
            doGameLost();
            return;
        }
        // If this cell has no surrounding mines, clear all cells around it
        if (m_board->mineCount(row, col) == 0) {
            clearSurrounding = true;
        }
    } else {
        // If player clicks a cell that has already been cleared AND the player has flagged the correct
        // number of cells around it, clear all surrounding non-flagged cells
        if (m_board->numSurroundingFlags(row, col) == m_board->mineCount(row, col)) {
            clearSurrounding = true;
        }
    }

    if (clearSurrounding) {
        clearNeighboringCells(row, col);
    }
}

// Called when cell is flagged or unflagged in the UI
void GameManager::cellFlagged(int row, int col)
{
    // Toggle flag for this cell
    if (!m_board->isCleared(row, col)) {
        m_board->toggleFlag(row, col);
        emit m_gameSignals->setCellFlagged(row, col, m_board->isFlagged(row, col));
    }
}

// Clear a cell, revealing its contents
void GameManager::clearCell(int row, int col)
{
    // Clear this cell
    m_board->clearCell(row, col);
    emit m_gameSignals->clearCell(
                row, col,
                m_board->mineCount(row, col), m_board->hasMine(row, col));

    // If this cell is a mine, game is over
    if (m_board->hasMine(row, col)) {
        emit m_gameSignals->explode(row, col);
    }

    if (m_board->allCellsCleared()) {
        // Player wins
        doGameWon();
    }
}

// Clear all the neighbors around a cell, either because the player has cleared
// a cell with no surrounding mines, or because player has flagged the correct
// number of surrounding cells and wants to clear all of the non-flagged cells
void GameManager::clearNeighboringCells(int row, int col)
{
    QStack<QPoint> stack;
    stack.push(QPoint(row, col));

    while (!stack.isEmpty()) {
        QPoint point = stack.pop();
        // Clear all cells that touch this cell
        for (row = point.x() - 1; row <= point.x() + 1; row++) {
            for (col = point.y() - 1; col <= point.y() + 1; col++) {
                if (isValidCell(row, col)) {
                    // If cell is not flagged and is not already cleared
                    if (!m_board->isFlagged(row, col) && !m_board->isCleared(row, col)) {
                        // Clear cell
                        clearCell(row, col);
                        // If this is another empty cell, add it to the stack and
                        // clear its neighbors as well
                        if (m_board->mineCount(row, col) == 0) {
                            stack.push(QPoint(row, col));
                        }
                    }
                }
            }
        }
        // Check for triggered mines after clearing all surrounding cells
        if (m_board->mineTriggered()) {
            doGameLost();
            return;
        }
    }
}

// Clear all cells after the game has been lost
void GameManager::clearAllCells()
{
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            if (!m_board->isCleared(row, col)) {
                // Clear non-flagged cells
                if (!m_board->isFlagged(row, col)) {
                    m_board->clearCell(row, col);
                    emit m_gameSignals->clearCell(
                                row, col, m_board->mineCount(row, col), m_board->hasMine(row, col));
                }
                // Mark incorrectly flagged cells
                if (m_board->isFlagged(row, col) && !m_board->hasMine(row, col)) {
                    emit m_gameSignals->markIncorrectlyFlaggedCell(row, col);
                }
            }
        }
    }
}

// Called after winning the game to flag mines that were not flagged by the player
void GameManager::flagAllBombs()
{
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            // Flag unflagged mines
            if (!m_board->isFlagged(row, col) && m_board->hasMine(row, col)) {
                emit m_gameSignals->setCellFlagged(row, col, true);
            }
        }
    }
}

// Player loses
void GameManager::doGameLost()
{
    emit m_gameSignals->gameLost();
    clearAllCells();
}

// Player wins
void GameManager::doGameWon()
{
    emit m_gameSignals->gameWon();
    flagAllBombs();
}
