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

// Initialize board with given dimensions and number of mines
void Board::initialize(int rows, int cols, int numMines)
{
    // Remember number of rows and columns
    m_rows = rows;
    m_cols = cols;

    // Initialize board with empty cells
    CellStruct cell;
    cell.cleared = false;
    cell.hasMine = false;
    cell.numNeighboringMines = 0;
    m_cells.clear();
    m_cells.fill(cell, m_rows * m_cols);

    // Add mines
    setMines(numMines);
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
void Board::setMine(int row, int col)
{
    if (isValidCell(row, col)) {
        m_cells[row * m_cols + col].hasMine = true;
    }
}

// Compute the number of surrounding mines for each cell
void Board::calcMineCounts()
{
    if (m_cells.isEmpty()) {
        return;
    }

    // Loop over all cells
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_cols; col++) {
            // Count number of mines surrounding this cell
            m_cells[row * m_cols + col].numNeighboringMines = numSurroundingMines(row, col);
        }
    }
}

// Return the number of mines surrounding this cell
int Board::numSurroundingMines(int row, int col)
{
    int numMines = 0;

    // Loop over all surrounding cells
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            // If this is a valid cell and is not the cell we're counting around
            if (isValidCell(i, j) && !(i == row && j == col)) {
                // See if this cell has a mine
                if (hasMine(i, j)) {
                    numMines++;
                }
            }
        }
    }

    return numMines;
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
