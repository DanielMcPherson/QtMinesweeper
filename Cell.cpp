#include "Cell.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QDebug>

Cell::Cell(QWidget *parent) : QWidget(parent)
{
    setFixedSize(35, 35);
    auto layout = new QHBoxLayout();
    m_label = new QLabel();
    QFont font = m_label->font();
    font.setBold(true);
    m_label->setFont(font);
    m_labelText = "";
//    m_label->setVisible(false);
    layout->addWidget(m_label);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    m_color = Qt::darkGray;
    m_cleared = false;
    m_flagged = false;

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

void Cell::drawLabel(QString text)
{
    QString labelText = text;

    // Set text color based on label text
    QString colorStr = m_labelColor[text];
    if (!colorStr.isEmpty()) {
        labelText = QString("<font color='%1'>%2</font>").arg(colorStr).arg(text);
    }

    // Redraw label
    m_label->setText(labelText);
}

void Cell::setLabel(QString text)
{
    m_labelText = text;
    drawLabel(m_labelText);
}

bool Cell::isRevealed()
{
    return m_cleared;
}

bool Cell::isFlagged()
{
    return m_flagged;
}

void Cell::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_color);
}

void Cell::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (m_cleared) {
        return;
    }
    m_color = Qt::lightGray;
    repaint();
}

void Cell::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (m_cleared) {
        return;
    }
    m_color = Qt::darkGray;
    repaint();
}

void Cell::click()
{
    m_label->setVisible(true);
    m_cleared = true;
    m_color = Qt::lightGray;
    repaint();
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    // If player clicks a cell that's already been revealed,
    // click all neighboring cells
    if (m_cleared) {
        emit clearNeighbors();
        return;
    }

    // Left click to clear a cell
    if (event->button() == Qt::LeftButton) {
        click();
        emit clicked();
    } else if (event->button() == Qt::RightButton) {
        // Right click to flag a cell
        m_flagged = !m_flagged;
        if (m_flagged) {
            drawLabel("F");
        } else {
            drawLabel(m_labelText);
        }
        emit rightClicked();
    }
}

void Cell::mouseDoubleClickEvent(QMouseEvent *)
{
    if (m_cleared) {
        return;
    }
}
