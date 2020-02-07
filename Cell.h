#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QColor>
#include <QLabel>

class Cell : public QWidget
{
    Q_OBJECT
public:
    explicit Cell(QWidget *parent = nullptr);
    void setLabel(QString text);

signals:
    void clicked();

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    QColor m_color;
    QLabel *m_label;
    bool m_cleared;
};

#endif // CELL_H
