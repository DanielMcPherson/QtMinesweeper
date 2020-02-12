#include "MainWindow.h"
#include "Board.h"
#include "BoardWidget.h"
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

    m_ui = new BoardWidget(m_boardSize, m_boardSize);
    connect(m_ui, &BoardWidget::cellClicked, this, &MainWindow::cellClicked);
    connect(m_ui, &BoardWidget::cellFlagged, this, &MainWindow::flagCell);
    mainLayout->addWidget(m_ui);

    // Tell the UI the mines are (for debug/cheat hints)
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            if (m_board.hasMine(row, col)) {
                m_ui->setMine(row, col);
            }
        }
    }
    m_ui->showHints(false);

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
        // Check if the player cleared a mine
        if (m_board.mineTriggered()) {
            doGameLost();
            return;
        }
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
        m_ui->flagCell(row, col, m_board.isFlagged(row, col));
    }
}

void MainWindow::clearCell(int row, int col)
{
    // Clear this cell
    m_board.clearCell(row, col);
    m_ui->clearCell(row, col, m_board.mineCount(row, col), m_board.hasMine(row, col));

    // If this cell is a mine, game is over
    if (m_board.hasMine(row, col)) {
        m_ui->explode(row, col);
    } else {
        // Increment number of cleared non-mine cells
        m_numCleared++;
    }

    if (m_numCleared >= m_numSqauresToClear) {
        // Player wins
        doGameWon();
    }
}

// Clear all the neighbors around a cell, either because the player has cleared
// a cell with no surrounding mines, or because player has flagged the correct
// number of surrounding cells and wants to clear all of the non-flagged cells
void MainWindow::clearNeighboringCells(int row, int col)
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
                    if (!m_board.isFlagged(row, col) && !m_board.isCleared(row, col)) {
                        // Clear cell
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
        // Check for triggered mines after clearing all surrounding cells
        if (m_board.mineTriggered()) {
            doGameLost();
            return;
        }
    }
}

// Clear all cells after the game has been lost
void MainWindow::clearAllCells()
{
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            if (!m_board.isCleared(row, col)) {
                m_board.clearCell(row, col);
                m_ui->clearCell(row, col, m_board.mineCount(row, col), m_board.hasMine(row, col));
                // Mark incorrectly flagged cells
                if (m_board.isFlagged(row, col) && !m_board.hasMine(row, col)) {
                    m_ui->misflagCell(row, col);
                }
            }
        }
    }
}

// Called after winning the game to mark flags on any mines that were not flagged
// by the player
void MainWindow::flagAllBombs()
{
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            // Flag unflagged mines
            if (!m_board.isFlagged(row, col) && m_board.hasMine(row, col)) {
                m_ui->flagCell(row, col, true);
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
    m_ui->setGameOver();
    clearAllCells();
}

void MainWindow::doGameWon()
{
    qDebug() << "You win!";
    m_ui->setGameOver();
    // Mark any unflagged mines with flags
    flagAllBombs();
}
