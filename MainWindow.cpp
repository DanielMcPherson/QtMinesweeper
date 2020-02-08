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
    m_numBombs = 10;
    m_numSqauresToClear = m_boardSize * m_boardSize - m_numBombs;
    m_numCleared = 0;
    m_board.initialize(m_boardSize, m_boardSize);
    m_board.setBombs(m_numBombs);

    // Draw a grid of cell widgets
    auto boardWidget = new QWidget;
    auto boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);
    boardWidget->setLayout(boardLayout);
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            auto cell = new Cell();
            if (m_board.hasBomb(row, col)) {
                cell->setLabel("X");
            } else {
                int bombCount = m_board.bombCount(row, col);
                if (m_board.bombCount(row, col) > 0) {
                    cell->setLabel(QString::number(bombCount));
                } else {
                    cell->setLabel(" ");
                }
            }
            connect(cell, &Cell::clicked, [=]() { cellClicked(row, col); });
            connect(cell, &Cell::clearNeighbors, [=]() { clearNeighboringCells(row, col); });
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
    clearCell(row, col);

    // If this cell has no surrounding bombs, clear all surrounding cells
    if (m_board.bombCount(row, col) == 0) {
        clearNeighboringCells(row, col);
    }
}

void MainWindow::clearCell(int row, int col)
{
    // If this cell is a bomb, game is over
    if (m_board.hasBomb(row, col)) {
        clearAllCells();
        return;
    }

    m_numCleared++;

    qDebug() << "Cells cleared: " << m_numCleared << ", cells remaining: " << m_numSqauresToClear - m_numCleared;
    if (m_numCleared >= m_numSqauresToClear) {
        // Player wins
        qDebug() << "You win!";
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
                    // If cell is not flagged as a bomb and is not already cleared
                    if (!cell->isFlagged() && !cell->isCleared()) {
                        // Clear cell
                        cell->click();
                        clearCell(row, col);
                        // If this is another empty cell, add it to the stack and
                        // clear its neighbors as well
                        if (m_board.bombCount(row, col) == 0) {
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
    foreach (Cell *cell, m_cells) {
        if (!cell->isCleared()) {
            cell->click();
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
