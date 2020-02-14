#include "Cell.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPixmap>
#include <QDebug>

Cell::Cell(QWidget *parent) : QWidget(parent)
{
    // Draw a box with a label inside indicating the cell state
    setFixedSize(45, 45);
    auto layout = new QHBoxLayout();
    // Label will either display text (mine count) or an image
    m_label = new QLabel();
    QFont font = m_label->font();
    font.setBold(true);
    m_label->setFont(font);
    layout->addWidget(m_label);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    // Background colors
    m_normalColor = QColor(192, 192, 192, 255);
    m_highlightColor = QColor(210, 210, 210, 255);
    m_clearedColor = QColor(220, 220, 220, 255);
    m_explodeColor = Qt::darkRed;

    // Initialize state
    m_color = m_normalColor;
    m_cleared = false;
    m_hasMine = false;
    m_showHints = false;
    m_gameOver = false;
}

// Redraw the cell, using the current color
void Cell::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_color);
}

// Show an image
void Cell::showImage(QString imagePath)
{
    QPixmap img;
    if (!imagePath.isEmpty()) {
        img.load(imagePath);
    }
    m_label->setPixmap(img.scaled(30, 30, Qt::KeepAspectRatio));
}

// Show mine count
void Cell::showCount(int count)
{
    // Clear any image being displayed
    showImage(":/Images/blank.png");

    // Set text color based on label text
    QString text = QString::number(count);
    static QString labelColor[] = { "Black", "Blue", "Green", "Maroon", "DarkBlue",
                                    "Purple", "LightBlue", "Yellow", "White" };
    if (count > 0 && count < 9) {
        QString colorStr = labelColor[count];
        text = QString("<font color='%1'>%2</font>").arg(colorStr).arg(text);
    }

    // Redraw label
    m_label->setText(text);
}

// Handle mouse event
void Cell::mousePressEvent(QMouseEvent *event)
{
    if (m_gameOver) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        // Left click to clear a cell
        emit clicked();
    } else if (event->button() == Qt::RightButton and !m_cleared) {
        // Right click to flag a cell
        emit flagCell();
    }
}

// Redraw the cell with a highlight color on mouse hover
void Cell::enterEvent(QEvent *)
{
    if (m_cleared) {
        return;
    }
    m_color = m_highlightColor;
    // Use a different color for mines if we are showing hints
    if (m_showHints && m_hasMine) {
        m_color = Qt::white;
    }
    repaint();
}

// Redraw the cell in the original color when the mouse leaves
void Cell::leaveEvent(QEvent *)
{
    if (m_cleared) {
        return;
    }
    m_color = m_normalColor;
    repaint();
}

// Show the contents of a cell
void Cell::clear(int count, bool mine)
{
    m_cleared = true;
    if (mine) {
        showImage(":/Images/mine.png");
    } else if (count > 0) {
        showCount(count);
    }
    m_color = m_clearedColor;
    repaint();
}

// Called when player clears a cell that contains a mine
void Cell::explode()
{
    m_cleared = true;
    showImage(":/Images/explosion3.png");
    m_color = m_explodeColor;
    repaint();
}

// Flag or unflag a cell
void Cell::flag(bool flagged)
{
    if (flagged) {
        showImage(":/Images/flagRed1.png");
    } else {
        showImage(":/Images/blank.png");
    }
}

// Called at game end to show that player incorrectly flagged a
// cell that does not contain a mine
void Cell::misflag()
{
    // Draw flag on a red background
    showImage(":/Images/flagRed1.png");
    m_color = m_explodeColor;
    repaint();
}

// Called to show debug/cheat hints
void Cell::setShowHints(bool showHints)
{
    m_showHints = showHints;
}

// Tell the cell it has a mine.
// Only used for debug/cheat hints. All other mine logic is handled
// in the game manager.
void Cell::setMine()
{
    m_hasMine = true;
}

// Set game over. Don't respond to move events.
void Cell::setGameOver()
{
    m_gameOver = true;
}

