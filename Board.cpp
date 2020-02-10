#include "Board.h"

Board::Board(QObject *parent) : QObject(parent)
{
    // Initialize member variables
    m_rows = 0;
    m_cols = 0;
    m_cells.clear();

    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Initialize empty board with given dimensions
void Board::initialize(int rows, int cols)
{
    // Remember number of rows and columns
    m_rows = rows;
    m_cols = cols;

    // Initialize board
    CellStruct cell;
    cell.cleared = false;
    cell.hasMine = false;
    cell.numNeighboringMines = 0;
    m_cells.clear();
    m_cells.fill(cell, m_rows * m_cols);
}

// Does a given cell contain a mine?
bool Board::hasMine(int row, int col)
{
    if (!isValidCell(row, col)) {
        return false;
    }
    return m_cells[row * m_cols + col].hasMine;
}

// Return the number of neighboring cells that contain mines
int Board::mineCount(int row, int col)
{
    if (!isValidCell(row, col)) {
        return false;
    }
    return m_cells[row * m_cols + col].numNeighboringMines;
}

// Set a specified number of mines randomly on the board
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

// Set a mine on a given cell
int Board::setMine(int row, int col)
{
    if (!isValidCell(row, col)) {
        return -1;
    }
    m_cells[row * m_cols + col].hasMine = true;

    return 0;
}

// Compute the number of neighboring mines for each cell
void Board::calcMineCounts()
{
    if (m_cells.isEmpty()) {
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
                m_cells[row * m_cols + col].numNeighboringMines = numMines;
            }
        }
    }
}

// Are the given cell coordinates valid?
bool Board::isValidCell(int row, int col)
{
    // Make sure board has been allocated
    if (m_cells.isEmpty()) {
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
