#include "Cell.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

Cell::Cell(QWidget *parent) : QWidget(parent)
{
    setFixedSize(35, 35);
    auto layout = new QHBoxLayout();
    m_label = new QLabel();
    m_labelText = "";
//    m_label->setVisible(false);
    layout->addWidget(m_label);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);
    m_color = Qt::darkGray;
    m_cleared = false;
    m_flagged = false;
}

void Cell::setLabel(QString text)
{
    m_labelText = text;
    m_label->setText(m_labelText);
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
            m_label->setText("F");
        } else {
            m_label->setText(m_labelText);
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
