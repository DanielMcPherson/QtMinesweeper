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
    void setShowHints(bool showHints);
    void setMine();
    void clear(int count, bool mine);
    void flag(bool flagged);
    void misflag();
    void explode();
    void setGameOver();

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
    void showImage(QString imagePath);
    void showCount(int count);

private:
    QColor m_color;
    QColor m_normalColor;
    QColor m_highlightColor;
    QColor m_clearedColor;
    QColor m_explodeColor;
    QLabel *m_label;
    QMap<QString, QString> m_labelColor;
    bool m_cleared;
    bool m_hasMine;
    bool m_gameOver;
    bool m_showHints;
};

#endif // CELL_H
