#include "MainWindow.h"
#include "Board.h"
#include "Cell.h"

#include <QLayout>
#include <QStack>
#include <QPoint>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto mainLayout = new QHBoxLayout();

    // Maintain board state seperately from UI
    m_boardSize = 8;
    m_numMines = 10;
    m_numCleared = 0;
    m_numSqauresToClear = m_boardSize * m_boardSize - m_numMines;
    m_board.initialize(m_boardSize, m_boardSize, m_numMines);

    // UI: Draw a grid of cell widgets
    auto boardWidget = new QWidget;
    auto boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);
    boardWidget->setLayout(boardLayout);
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            auto cell = new Cell();
            connect(cell, &Cell::clicked, [=]() { cellClicked(row, col); });
            connect(cell, &Cell::flagCell, [=]() { flagCell(row, col); });
            boardLayout->addWidget(cell, row, col);
            m_cells.append(cell);
            // Tell the cell if has a mine (for debug/cheat hints)
            if (m_board.hasMine(row, col)) {
                cell->setMine();
            }
            cell->setShowHints(true);
        }
    }
    mainLayout->addWidget(boardWidget);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::cellClicked(int row, int col)
{
    bool clearSurrounding = false;

    // Don't let player accidentally click flagged cells
    if (m_board.isFlagged(row, col)) {
        return;
    }

    // If this cell has not been cleared already
    if (!m_board.isCleared(row, col)) {
        clearCell(row, col);
        // If this cell has no surrounding mines, clear all cells around it
        if (m_board.mineCount(row, col) == 0) {
            clearSurrounding = true;
        }
    } else {
        // If player clicks a cell that has already been cleared AND the player has flagged the correct
        // number of cells around it, clear all surrounding non-flagged cells
        if (m_board.numSurroundingFlags(row, col) == m_board.mineCount(row, col)) {
            clearSurrounding = true;
        }
    }

    if (clearSurrounding) {
        clearNeighboringCells(row, col);
    }
}

void MainWindow::flagCell(int row, int col)
{
    // Toggle flag for this cell
    if (!m_board.isCleared(row, col)) {
        m_board.toggleFlag(row, col);
        Cell *cell = m_cells[row * m_boardSize + col];
        cell->flag(m_board.isFlagged(row, col));
    }
}

void MainWindow::clearCell(int row, int col)
{
    // Clear this cell
    m_board.clearCell(row, col);
    Cell *cell = m_cells[row * m_boardSize + col];
    cell->clear(m_board.mineCount(row, col), m_board.hasMine(row, col));

    // If this cell is a mine, game is over
    if (m_board.hasMine(row, col)) {
        cell->explode();
        doGameLost();
        return;
    }
    m_numCleared++;

    if (m_numCleared >= m_numSqauresToClear) {
        // Player wins
        doGameWon();
    }
}

void MainWindow::clearNeighboringCells(int row, int col)
{
    QStack<QPoint> stack;
    stack.push(QPoint(row, col));

    // Click all cells that touch an empty cell
    while (!stack.isEmpty()) {
        QPoint point = stack.pop();
        // Clear all cells that touch this empty cell
        for (row = point.x() - 1; row <= point.x() + 1; row++) {
            for (col = point.y() - 1; col <= point.y() + 1; col++) {
                if (isValidCell(row, col)) {
                    Cell *cell = m_cells[row * m_boardSize + col];
                    // If cell is not flagged and is not already cleared
                    if (!m_board.isFlagged(row, col) && !m_board.isCleared(row, col)) {
                        // Clear cell
                        cell->clear(m_board.mineCount(row, col), m_board.hasMine(row, col));
                        clearCell(row, col);
                        // If this is another empty cell, add it to the stack and
                        // clear its neighbors as well
                        if (m_board.mineCount(row, col) == 0) {
                            stack.push(QPoint(row, col));
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::clearAllCells()
{
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            Cell *cell = m_cells[row * m_boardSize + col];
            cell->setGameOver();
            if (!m_board.isCleared(row, col)) {
                m_board.clearCell(row, col);
                cell->clear(m_board.mineCount(row, col), m_board.hasMine(row, col));
                // Mark incorrectly flagged cells
                if (m_board.isFlagged(row, col) && !m_board.hasMine(row, col)) {
                    cell->misflag();
                }
            }
        }
    }
}

void MainWindow::flagAllBombs()
{
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            Cell *cell = m_cells[row * m_boardSize + col];
            cell->setGameOver();
            // Flag unflagged mines
            if (!m_board.isFlagged(row, col) && m_board.hasMine(row, col)) {
                cell->flag(true);
            }
        }
    }
}

bool MainWindow::isValidCell(int row, int col)
{
    // Make sure row and col are within bounds
    if (row < 0 || row >= m_boardSize) {
        return false;
    }
    if (col < 0 || col >= m_boardSize) {
        return false;
    }

    return true;
}

void MainWindow::doGameLost()
{
    qDebug() << "You lose!";
    clearAllCells();
}

void MainWindow::doGameWon()
{
    qDebug() << "You win!";
    // Mark any unflagged mines with flags
    flagAllBombs();
}
