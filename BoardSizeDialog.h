#ifndef BOARDSIZEDIALOG_H
#define BOARDSIZEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QSpinBox>

class BoardSizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BoardSizeDialog(int &rows, int &cols, int &mines, QWidget *parent = nullptr);

signals:

private slots:
    void ok();
    void cancel();

public slots:

private:
    int &m_rows, &m_cols, &m_mines;
    QSpinBox *m_rowsSpinBox;
    QSpinBox *m_colsSpinBox;
    QSpinBox *m_minesSpinBox;
};

#endif // BOARDSIZEDIALOG_H
