#include "BoardWidget.h"
#include <QLayout>
#include <QDebug>

BoardWidget::BoardWidget(int numRows, int numCols, QWidget *parent) : QWidget(parent)
{
    auto layout = new QGridLayout();
    setLayout(layout);
    layout->setSpacing(2);

    m_cells.clear();
    m_numRows = numRows;
    m_numCols = numCols;

    for (int row = 0; row < m_numRows; row++) {
        for (int col = 0; col < m_numCols; col++) {
            auto cell = new Cell();
            connect(cell, &Cell::clicked, [=]() { click(row, col); });
            connect(cell, &Cell::flagCell, [=]() { rightClick(row, col); });
            layout->addWidget(cell, row, col);
            m_cells.append(cell);
        }
    }

    m_gameOver = false;
}

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

void BoardWidget::click(int row, int col)
{
    emit cellClicked(row, col);
}

void BoardWidget::rightClick(int row, int col)
{
    emit cellFlagged(row, col);
}

Cell *BoardWidget::getCell(int row, int col)
{
    Cell *cell = nullptr;
    if (row >= 0 && row < m_numRows && col >= 0 && col < m_numCols) {
        cell = m_cells[row * m_numCols + col];
    }

    return cell;
}

void BoardWidget::setGameOver()
{
    foreach (Cell *cell, m_cells) {
        cell->setGameOver();
    }
}

void BoardWidget::showHints(bool showHints)
{
    foreach (Cell *cell, m_cells) {
        cell->setShowHints(showHints);
    }
}

