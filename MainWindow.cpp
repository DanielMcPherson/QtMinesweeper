#include "MainWindow.h"
#include "Board.h"
#include "Cell.h"

#include <QLayout>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto mainLayout = new QHBoxLayout();

    // Maintain board state seperately from UI
    m_boardSize = 16;
    int numBombs = 40;
    Board board;
    board.initialize(m_boardSize, m_boardSize);
    board.setBombs(numBombs);

    // Draw a 16x16 grid of cell widgets
    auto boardWidget = new QWidget;
    auto boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);
    boardWidget->setLayout(boardLayout);
    for (int row = 0; row < m_boardSize; row++) {
        for (int col = 0; col < m_boardSize; col++) {
            auto cell = new Cell();
            if (board.hasBomb(row, col)) {
                cell->setLabel("X");
            } else {
                int bombCount = board.bombCount(row, col);
                if (board.bombCount(row, col) > 0) {
                    cell->setLabel(QString::number(bombCount));
                } else {
                    cell->setLabel(" ");
                }
            }
            connect(cell, &Cell::clicked, [=]() { cellClicked(row, col); });
            boardLayout->addWidget(cell, row, col);
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
    qDebug() << Q_FUNC_INFO << row << ", " << col;
}
