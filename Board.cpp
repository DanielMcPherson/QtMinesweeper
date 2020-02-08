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
    srand(time(NULL));
}

int Board::setMine(int row, int col)
{
    if (!isValidCell(row, col)) {
        return -1;
    }
    // Assign value to board position
    m_board[row * m_cols + col] = -1;

    return 0;
}

bool Board::hasMine(int row, int col)
{
    if (!isValidCell(row, col)) {
        return false;
    }
    return m_board[row * m_cols + col] == -1;
}

int Board::mineCount(int row, int col)
{
    if (!isValidCell(row, col)) {
        return false;
    }
    return m_board[row * m_cols + col];
}

void Board::setMines(int numMines)
{
    int minesSet = 0;
    while (minesSet < numMines) {
        int row = rand() % m_rows;
        int col = rand() % m_cols;
        if (!hasMine(row, col)) {
            setMine(row, col);
            minesSet++;
        }
    }
    calcMineCounts();
}

void Board::calcMineCounts()
{
    if (!m_board) {
        return;
    }

    // Compute adjacent mine count for all non-mine cells
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            if (!hasMine(row, col)) {
                int numMines = 0;
                for (int i = row - 1; i <= row + 1; i++) {
                    for (int j = col - 1; j <= col + 1; j++) {
                        if (isValidCell(i, j) && !(i == row && j == col)) {
                            if (hasMine(i, j)) {
                                numMines++;
                            }
                        }
                    }
                }
                m_board[row * m_cols + col] = numMines;
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
    if (col < 0 || col >= m_cols) {
        return false;
    }

    return true;
}
