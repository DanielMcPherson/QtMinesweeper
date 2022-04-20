#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QLabel>
#include <QVector>

// Displays a single cell on the Minesweeper board

class Cell : public QWidget
{
    Q_OBJECT
public:
    explicit Cell(QWidget *parent = nullptr);
    void setMine();
    void clear(int count, bool mine);
    void flag(bool flagged);
    void misflag();
    void explode();

signals:
    void clicked();
    void flagCell();

private slots:
    void gameWon();
    void gameLost();
    void showHints(bool showHints);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    void showImage(QString imagePath);
    void showCount(int count);
    void playAnimation();
    void stopAnimation();

private:
    QLabel *m_label;
    // Colors
    QColor m_color;
    QColor m_normalColor;
    QColor m_highlightColor;
    QColor m_clearedColor;
    QColor m_explodeColor;
    // State
    bool m_cleared;
    bool m_hasMine;
    bool m_flagged;
    bool m_gameOver;
    bool m_showHints;
    // Animations
    QVector<QString> m_animationImages;
    int m_animationDelay;
    int m_animationCount;
    bool m_playingAnimation;
    bool m_loopingAnimation;
    int m_timerID;
};

#endif // CELL_H
