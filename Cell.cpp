#include "Cell.h"
#include "GameSignals.h"
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
    m_playingAnimation = false;
    m_cleared = false;
    m_hasMine = false;
    m_flagged = false;
    m_showHints = false;
    m_gameOver = false;

    // Connect to Game Signals
    auto gameSignals = GameSignals::getInstance();
    connect(gameSignals, &GameSignals::gameWon, this, &Cell::gameWon);
    connect(gameSignals, &GameSignals::gameLost, this, &Cell::gameLost);
    connect(gameSignals, &GameSignals::showHints, this, &Cell::showHints);
}

// Redraw the cell, using the current color
void Cell::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_color);
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

// Show mine count
void Cell::showCount(int count)
{
    // Clear any image being displayed
    showImage(":/Images/blank.png");
    m_playingAnimation = false;

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

// Show an image
void Cell::showImage(QString imagePath)
{
    QPixmap img;
    if (!imagePath.isEmpty()) {
        img.load(imagePath);
    }
    m_label->setPixmap(img.scaled(30, 30, Qt::KeepAspectRatio));
}

//
// Functions to display an animated image
//
void Cell::playAnimation()
{
    m_playingAnimation = true;
    m_animationCount = 0;
    showImage(m_animationImages[m_animationCount]);
    m_timerID = startTimer(m_animationDelay);
}

void Cell::stopAnimation()
{
    m_playingAnimation = false;
    killTimer(m_timerID);
}

void Cell::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (m_playingAnimation) {
        m_animationCount++;
        if (m_animationCount >= m_animationImages.size()) {
            if (m_loopingAnimation) {
                m_animationCount = 0;
            } else {
                stopAnimation();
                // Redraw final animation frame
                m_animationCount = m_animationImages.size() - 1;
            }
        }
        showImage(m_animationImages[m_animationCount]);
    }
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
    } else if (event->button() == Qt::RightButton && !m_cleared) {
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

// Called when player clears a cell that contains a mine
void Cell::explode()
{
    m_cleared = true;
    m_animationImages.clear();
    m_animationImages.append(":/Images/explosionSmoke1.png");
    m_animationImages.append(":/Images/explosionSmoke2.png");
    m_animationImages.append(":/Images/explosionSmoke3.png");
    m_animationImages.append(":/Images/explosionSmoke4.png");
    m_animationImages.append(":/Images/explosionSmoke5.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/blank.png");
    m_animationImages.append(":/Images/explosionSmoke3.png");
    m_animationDelay = 100;
    m_loopingAnimation = false;
    playAnimation();

    m_color = m_explodeColor;
    repaint();
}

// Flag or unflag a cell
void Cell::flag(bool flagged)
{
    m_flagged = flagged;

    if (flagged) {
        m_animationImages.clear();
        m_animationImages.append(":/Images/flagRed1.png");
        m_animationImages.append(":/Images/flagRed2.png");
        m_animationDelay = 180;
        m_loopingAnimation = true;
        playAnimation();
    } else {
        stopAnimation();
        showImage(":/Images/blank.png");
    }
}

// Called at game end to show that player incorrectly flagged a
// cell that does not contain a mine
void Cell::misflag()
{
    // Draw flag on a red background
    stopAnimation();
    showImage(":/Images/flagRed1.png");
    m_color = m_explodeColor;
    repaint();
}

// Player won
void Cell::gameWon()
{
    // Flags wave faster after winning
    if (m_flagged) {
        stopAnimation();
        m_animationImages.clear();
        m_animationImages.append(":/Images/flagRed1.png");
        m_animationImages.append(":/Images/flagRed2.png");
        m_animationDelay = 100;
        m_loopingAnimation = true;
        playAnimation();
    }

    // Don't respond to move events
    m_gameOver = true;
}

// Player lost
void Cell::gameLost()
{
    // Stop flag waving
    if (m_flagged) {
        stopAnimation();
        showImage(":/Images/flagRed1.png");
    }

    // Don't respond to move events
    m_gameOver = true;
}

// Show debug/cheat hints
void Cell::showHints(bool showHints)
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
