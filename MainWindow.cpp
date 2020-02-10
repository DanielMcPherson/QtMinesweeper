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
    // Q: Does this make sense? Board object knows what cells have mines,
    // but UI board also knows this. UI is the only one who knows if cells
    // have been cleared or flagged.
    // If there is separation, then UI should just report button presses
    // and backend board should maintain state of flagged/cleared and then
    // tell UI what to display.
    // Should Cell retain any logic about whether or not it is flagged or cleared?
    // Ex: When the player loses, any mis-flagged cells should show a red background.
    // Should the cell remember that it was flagged and handle coloring itself when
    // revealed, or should GameManager (using Board) handle all of that logic and tell
    // BoardWidget exactly what to display?
    // Ex 2: When player clicks a cell, should Cell or BoardWidget have enough logic to
    // reveal that cell, or just report the player input to GameManager and wait for GameManager
    // to tell BoardWidget/Cell what to display?
    // Some things (ex: debugging cheat hints) are easier to implement if we tell the UI the
    // mine count and mine locations at setup time instead of waiting for a user interaction
    // to prompt GameManager to tell UI what to display.

    m_boardSize = 8;
    m_numMines = 10;
    m_numCleared = 0;
    m_numSqauresToClear = m_boardSize * m_boardSize - m_numMines;
    m_board.initialize(m_boardSize, m_boardSize, m_numMines);

    // Draw a grid of cell widgets
    auto boardWidget = new QWidget;
    auto boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);
    boardWidget->setLayout(boardLayout);
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            auto cell = new Cell();
            if (m_board.hasMine(row, col)) {
                cell->setLabel("X");
            } else {
                int mineCount = m_board.mineCount(row, col);
                if (m_board.mineCount(row, col) > 0) {
                    cell->setLabel(QString::number(mineCount));
                } else {
                    cell->setLabel(" ");
                }
            }
            connect(cell, &Cell::clicked, [=]() { cellClicked(row, col); });
            connect(cell, &Cell::flagCell, [=]() { flagCell(row, col); });
            boardLayout->addWidget(cell, row, col);
            m_cells.append(cell);
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
        qDebug() << "You win!";
        // ToDo: Mark any unflagged mines with flags
        // ToDo: Should flagging the last mine be a win condition, or do we only have to check for win when
        // clearing a square?
        // ToDo: Don't let UI update any more (no flagging or clicking) once game is done
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
            if (!m_board.isCleared(row, col)) {
                m_board.clearCell(row, col);
                Cell *cell = m_cells[row * m_boardSize + col];
                cell->clear(m_board.mineCount(row, col), m_board.hasMine(row, col));
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
    // ToDo: Show exploded mines. Mark any mis-flagged cells.
}
