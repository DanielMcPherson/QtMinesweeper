#include "Cell.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QDebug>

Cell::Cell(QWidget *parent) : QWidget(parent)
{
    // Draw a box with a label inside indicating the cell state
    setFixedSize(35, 35);
    auto layout = new QHBoxLayout();
    m_label = new QLabel();
    QFont font = m_label->font();
    font.setBold(true);
    m_label->setFont(font);
    layout->addWidget(m_label);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    m_color = Qt::gray;
    m_cleared = false;

    // Use different colors for different surround mine counts
    m_labelColor["1"] = "Blue";
    m_labelColor["2"] = "Green";
    m_labelColor["3"] = "Maroon";
    m_labelColor["4"] = "DarkBlue";
    m_labelColor["5"] = "Purple";
    m_labelColor["6"] = "LightBlue";
    m_labelColor["7"] = "Yellow";
    m_labelColor["8"] = "White";
    m_labelColor["F"] = "DarkRed";
    m_labelColor["X"] = "Black";
}

// Update the label with a mine count, mine marker, or flag marker
void Cell::drawLabel(QString text)
{
    // Decorated text to assign to label
    QString labelText = text;

    // Set text color based on label text
    QString colorStr = m_labelColor[text];
    if (!colorStr.isEmpty()) {
        labelText = QString("<font color='%1'>%2</font>").arg(colorStr).arg(text);
    }

    // Redraw label
    m_label->setText(labelText);
}

// Redraw the cell, using the current color
void Cell::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_color);
}

// Redraw the cell with a highlight color on mouse hover
void Cell::enterEvent(QEvent *)
{
    if (m_cleared) {
        return;
    }
    m_color = Qt::lightGray;
    repaint();
}

// Redraw the cell in the original color when the mouse leaves
void Cell::leaveEvent(QEvent *)
{
    if (m_cleared) {
        return;
    }
    m_color = Qt::gray;
    repaint();
}

// Called when player clears a cell that contains a mine
void Cell::explode()
{
    m_cleared = true;
    m_color = Qt::red;
    QString labelText = "<font color=yellow>X</font>";
    m_label->setText(labelText);
    repaint();
}

// Show the contents of a cell
void Cell::clear(int count, bool mine)
{
    m_cleared = true;
    m_color = Qt::lightGray;
    QString labelText = "";
    if (mine) {
        labelText = "X";
    } else if (count > 0) {
        labelText = QString::number(count);
    }
    drawLabel(labelText);
    repaint();
}

// Flag or unflag a cell
void Cell::flag(bool flagged)
{
    if (flagged) {
        drawLabel("F");
    } else {
        drawLabel("");
    }
}

// Called at game end to show that player incorrectly flagged a
// cell that does not contain a mine
void Cell::misflag()
{
    // Draw flag on a red background
    m_color = Qt::red;
    drawLabel("F");
    repaint();
}

// Handle mouse event
void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Left click to clear a cell
        emit clicked();
    } else if (event->button() == Qt::RightButton and !m_cleared) {
        // Right click to flag a cell
        emit flagCell();
    }
}
