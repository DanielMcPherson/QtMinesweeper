#include "BoardWidget.h"
#include "GameSignals.h"
#include <QLayout>
#include <QDebug>

BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent)
{
    m_layout = new QGridLayout();
    setLayout(m_layout);
    m_layout->setSpacing(2);

    // Connect to Game Signals
    auto gameSignals = GameSignals::getInstance();
    connect(gameSignals, &GameSignals::startGame, this, &BoardWidget::startGame);
    connect(gameSignals, &GameSignals::setCellFlagged, this, &BoardWidget::flagCell);
    connect(gameSignals, &GameSignals::clearCell, this, &BoardWidget::clearCell);
    connect(gameSignals, &GameSignals::explode, this, &BoardWidget::explode);
    connect(gameSignals, &GameSignals::setMine, this, &BoardWidget::setMine);
    connect(gameSignals, &GameSignals::markIncorrectlyFlaggedCell, this, &BoardWidget::misflagCell);
}

// Start game and create Cell widgets
void BoardWidget::startGame(int rows, int cols, int mines)
{
    Q_UNUSED(mines)

    // Remove old cells from layout
    QLayoutItem* item;
    while ((item = m_layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Remember board dimensions
    m_numRows = rows;
    m_numCols = cols;

    // Add new cells
    m_cells.clear();
    for (int row = 0; row < m_numRows; row++) {
        for (int col = 0; col < m_numCols; col++) {
            auto cell = new Cell();
            connect(cell, &Cell::clicked, [=]() { click(row, col); });
            connect(cell, &Cell::flagCell, [=]() { rightClick(row, col); });
            m_layout->addWidget(cell, row, col);
            m_cells.append(cell);
        }
    }
    m_gameOver = false;
}

//
// User actions
//
void BoardWidget::click(int row, int col)
{
    emit GameSignals::getInstance()->playerClickedCell(row, col);
}

void BoardWidget::rightClick(int row, int col)
{
    emit GameSignals::getInstance()->playerFlaggedCell(row, col);
}

//
// Handle game state updates trigger by game action signals
//
void BoardWidget::clearCell(int row, int col, int count, bool mine)
{
    Cell *cell = getCell(row, col);
    if (!cell) {
        return;
    }

    cell->clear(count, mine);
}

void BoardWidget::flagCell(int row, int col, bool flagged)
{
    Cell *cell = getCell(row, col);
    if (!cell) {
        return;
    }

    cell->flag(flagged);
}

void BoardWidget::misflagCell(int row, int col)
{
    Cell *cell = getCell(row, col);
    if (!cell) {
        return;
    }

    cell->misflag();
}

void BoardWidget::explode(int row, int col)
{
    Cell *cell = getCell(row, col);
    if (!cell) {
        return;
    }

    cell->explode();
}

void BoardWidget::setMine(int row, int col)
{
    Cell *cell = getCell(row, col);
    if (!cell) {
        return;
    }

    cell->setMine();
}

Cell *BoardWidget::getCell(int row, int col)
{
    Cell *cell = nullptr;
    if (row >= 0 && row < m_numRows && col >= 0 && col < m_numCols) {
        cell = m_cells[row * m_numCols + col];
    }

    return cell;
}


