#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QLabel>
#include <QMap>

class Cell : public QWidget
{
    Q_OBJECT
public:
    explicit Cell(QWidget *parent = nullptr);
    void clear(int count, bool mine);
    void flag(bool flagged);
    void explode();

signals:
    void clicked();
    void flagCell();

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
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
