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
    void setLabel(QString text);
    void explode();
    void clear(int count, bool mine);

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
    void drawLabel();

private:
    QColor m_color;
    QLabel *m_label;
    QString m_labelText;
    QMap<QString, QString> m_labelColor;
    bool m_cleared;
    bool m_flagged;
};

#endif // CELL_H
