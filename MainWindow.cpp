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

    m_boardUI = new BoardWidget(m_boardSize, m_boardSize);
    connect(m_boardUI, &BoardWidget::cellClicked, this, &MainWindow::cellClicked);
    connect(m_boardUI, &BoardWidget::cellFlagged, this, &MainWindow::flagCell);
    mainLayout->addWidget(m_boardUI);

    // Tell the UI the mines are (for debug/cheat hints)
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            if (m_board.hasMine(row, col)) {
                m_boardUI->setMine(row, col);
            }
        }
    }
    m_boardUI->showHints(false);

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
        m_boardUI->flagCell(row, col, m_board.isFlagged(row, col));
    }
}

void MainWindow::clearCell(int row, int col)
{
    // Clear this cell
    m_board.clearCell(row, col);
    m_boardUI->clearCell(row, col, m_board.mineCount(row, col), m_board.hasMine(row, col));

    // If this cell is a mine, game is over
    if (m_board.hasMine(row, col)) {
        m_boardUI->explode(row, col);
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
                        m_boardUI->clearCell(row, col, m_board.mineCount(row, col), m_board.hasMine(row, col));
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
            if (!m_board.isCleared(row, col)) {
                m_board.clearCell(row, col);
                m_boardUI->clearCell(row, col, m_board.mineCount(row, col), m_board.hasMine(row, col));
                // Mark incorrectly flagged cells
                if (m_board.isFlagged(row, col) && !m_board.hasMine(row, col)) {
                    m_boardUI->misflagCell(row, col);
                }
            }
        }
    }
}

void MainWindow::flagAllBombs()
{
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            // Flag unflagged mines
            if (!m_board.isFlagged(row, col) && m_board.hasMine(row, col)) {
                m_boardUI->flagCell(row, col, true);
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
    m_boardUI->setGameOver();
    clearAllCells();
}

void MainWindow::doGameWon()
{
    qDebug() << "You win!";
    m_boardUI->setGameOver();
    // Mark any unflagged mines with flags
    flagAllBombs();
}
