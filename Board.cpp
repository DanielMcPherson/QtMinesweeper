#include "Board.h"

Board::Board(QObject *parent) : QObject(parent)
{
    m_rows = 0;
    m_cols = 0;
    m_board = nullptr;
}

void Board::initialize(int rows, int cols)
{
    delete [] m_board;
    m_rows = rows;
    m_cols = cols;
    m_board = new int[m_rows * m_cols];
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            m_board[row * m_cols + col] = 0;
        }
    }
}

int Board::setBomb(int row, int col)
{
    if (!isValidCell(row, col)) {
        return -1;
    }
    // Assign value to board position
    m_board[row * m_cols + col] = -1;

    return 0;
}

bool Board::hasBomb(int row, int col)
{
    if (!isValidCell(row, col)) {
        return false;
    }
    return m_board[row * m_cols + col] == -1;
}

int Board::bombCount(int row, int col)
{
    if (!isValidCell(row, col)) {
        return false;
    }
    return m_board[row * m_cols + col];
}

void Board::setBombs(int numBombs)
{
    int bombsSet = 0;
    while (bombsSet < numBombs) {
        int row = rand() % m_rows;
        int col = rand() % m_cols;
        if (!hasBomb(row, col)) {
            setBomb(row, col);
            bombsSet++;
        }
    }
    calcBombs();
}

void Board::calcBombs()
{
    if (!m_board) {
        return;
    }

    // Compute adjacent bomb count for all non-bomb cells
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            if (!hasBomb(row, col)) {
                int bombCount = 0;
                for (int i = row - 1; i <= row + 1; i++) {
                    for (int j = col - 1; j <= col + 1; j++) {
                        if (isValidCell(i, j) && !(i == row && j == col)) {
                            if (hasBomb(i, j)) {
                                bombCount++;
                            }
                        }
                    }
                }
                m_board[row * m_cols + col] = bombCount;
            }
        }
    }
}


bool Board::isValidCell(int row, int col)
{
    // Make sure board has been allocated
    if (!m_board) {
        return false;
    }
    // Make sure row and col are within bounds
    if (row < 0 || row >= m_rows) {
        return false;
    }
    if (col < 0 || col >> m_cols) {
        return false;
    }

    return true;
}
