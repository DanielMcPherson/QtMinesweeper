#include "BoardSizeDialog.h"
#include <QLayout>
#include <QFormLayout>
#include <QPushButton>

BoardSizeDialog::BoardSizeDialog(int &rows, int &cols, int &mines, QWidget *parent) :
    QDialog(parent), m_rows(rows), m_cols(cols), m_mines(mines)
{
    auto mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    setWindowTitle(tr("Custom Difficulty"));

    auto formLayout = new QFormLayout;
    m_rowsSpinBox = new QSpinBox;
    m_colsSpinBox = new QSpinBox;
    m_minesSpinBox = new QSpinBox;
    m_rowsSpinBox->setRange(4, 60);
    m_rowsSpinBox->setValue(rows);
    m_colsSpinBox->setRange(4, 40);
    m_colsSpinBox->setValue(cols);
    m_minesSpinBox->setRange(1, 99);
    m_minesSpinBox->setValue(mines);
    formLayout->addRow(tr("Rows"), m_rowsSpinBox);
    formLayout->addRow(tr("Columns"), m_colsSpinBox);
    formLayout->addRow(tr("% Mines"), m_minesSpinBox);
    mainLayout->addLayout(formLayout);

    auto okButton = new QPushButton(tr("OK"));
    connect(okButton, &QPushButton::clicked, this, &BoardSizeDialog::ok);
    auto cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, &QPushButton::clicked, this, &BoardSizeDialog::cancel);
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignRight);
    buttonLayout->addWidget(okButton);
    buttonLayout->addSpacing(1);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

}

void BoardSizeDialog::ok()
{
    m_rows = m_rowsSpinBox->value();
    m_cols = m_colsSpinBox->value();
    m_mines = m_minesSpinBox->value();
    accept();
}

void BoardSizeDialog::cancel()
{
    reject();
}
