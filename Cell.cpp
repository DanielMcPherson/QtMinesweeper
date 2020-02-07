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
//    m_label->setVisible(false);
    layout->addWidget(m_label);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);
    m_color = Qt::darkGray;
    m_cleared = false;
}

void Cell::setLabel(QString text)
{
    m_label->setText(text);
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

void Cell::mousePressEvent(QMouseEvent *)
{
    if (m_cleared) {
        return;
    }
    m_label->setVisible(true);
    m_cleared = true;
    emit clicked();
}

void Cell::mouseDoubleClickEvent(QMouseEvent *)
{
    if (m_cleared) {
        return;
    }
}
