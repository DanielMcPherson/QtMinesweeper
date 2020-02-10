#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QLabel>
#include <QMap>

// Displays a single cell on the Minesweeper board

class Cell : public QWidget
{
    Q_OBJECT
public:
    explicit Cell(QWidget *parent = nullptr);
    void clear(int count, bool mine);
    void flag(bool flagged);
    void misflag();
    void explode();

signals:
    void clicked();
    void flagCell();

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);

private:
    void drawLabel(QString text);

private:
    QColor m_color;
    QLabel *m_label;
    QMap<QString, QString> m_labelColor;
    bool m_cleared;
};

#endif // CELL_H
