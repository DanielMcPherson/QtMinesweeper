#include "MainWindow.h"
#include "BoardSizeDialog.h"

#include <QLayout>
#include <QMenuBar>
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QStack>
#include <QPoint>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto mainLayout = new QVBoxLayout();

    // Default board size
    m_rows = 8;
    m_cols = 8;
    m_numMines = 10;
    // Default custom size
    m_custRows = 12;
    m_custCols = 12;
    m_percentMines = 16;

    // Widget to display Minesweeper UI
    m_ui = new BoardWidget(m_rows, m_cols);
    mainLayout->addWidget(m_ui);

    // Restart button centered at bottom of window
    auto buttonLayout = new QHBoxLayout();
    m_restartButton = new QPushButton(tr("Start Over"));
    m_restartButton->setMinimumWidth(200);
    connect(m_restartButton, &QPushButton::clicked, this, &MainWindow::restartGame);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_restartButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // Set window to main layout created above
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Menus
    // Create Game menu
    auto gameMenu = menuBar()->addMenu(tr("Game"));
    // New game item
    auto newGameAction = new QAction(tr("New Game"));
    connect(newGameAction, &QAction::triggered, this, &MainWindow::restartGame);
    gameMenu->addAction(newGameAction);
    // Difficulty
    gameMenu->addSeparator();
    auto difficultyMenu = new QMenu(tr("Difficulty"));
    auto difficultyGroup = new QActionGroup(this);
    QStringList difficultyLevels = { tr("Easy"), tr("Medium"), tr("Hard"), tr("Custom") };
    for (int i = 0; i < difficultyLevels.size(); i++) {
        auto action = new QAction(difficultyLevels[i]);
        action->setCheckable(true);
        action->setChecked(i == 0);
        connect(action, &QAction::triggered, this,  [=]() {this->setDifficulty(i);});
        difficultyMenu->addAction(action);
        difficultyGroup->addAction(action);
    }
    gameMenu->addMenu(difficultyMenu);
    // Exit menu item
    gameMenu->addSeparator();
    auto exitAction = new QAction(tr("E&xit"));
    exitAction->setShortcuts(QKeySequence::Close);
    gameMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exit);
    // Create About menu
    auto aboutMenu = menuBar()->addMenu(tr("About"));
    auto aboutAction = new QAction("About Minesweeper");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    aboutMenu->addAction(aboutAction);
    gameMenu->addMenu(aboutMenu);

    // Game manager controls the state of the game
    m_gameManager = new GameManager();
    connect(m_gameManager, &GameManager::gameWon, this, &MainWindow::winGame);
    connect(m_gameManager, &GameManager::gameLost, this, &MainWindow::loseGame);
    // Link UI widget to game manager
    // Could create a MinesweeperUI interface class and link different implementations
    // of UIs to game manager
    m_gameManager->setUI(m_ui);

    // Start game
    startGame();
}

MainWindow::~MainWindow()
{

}

void MainWindow::startGame()
{
    m_gameManager->startGame(m_rows, m_cols, m_numMines);
    m_restartButton->setText(tr("Start Over"));

    // Wait for processEvents to redraw widget
    QApplication::processEvents();
    // Resize window to minimum size required to display all widgets
    resize(0, 0);
}

void MainWindow::restartGame(bool checked)
{
    Q_UNUSED(checked);
    startGame();
}

void MainWindow::winGame()
{
    QMessageBox msg;
    msg.setText(tr("You Win!"));
    msg.exec();
    m_restartButton->setText(tr("Play Again"));
}

void MainWindow::loseGame()
{
    m_restartButton->setText(tr("Play Again"));
}

void MainWindow::exit()
{
    QApplication::quit();
}

void MainWindow::setDifficulty(int size)
{
    switch (size) {
    case 0:
    default:
        m_rows = 8;
        m_cols = 8;
        m_numMines = 10;
        break;
    case 1:
        m_rows = 16;
        m_cols = 16;
        m_numMines = 40;
        break;
    case 2:
        m_rows = 16;
        m_cols = 30;
        m_numMines = 99;
        break;
    case 3:
        BoardSizeDialog dlg(m_custRows, m_custCols, m_percentMines);
        dlg.exec();
        if (dlg.result() == QDialog::Rejected) {
            return;
        }
        m_rows = m_custRows;
        m_cols = m_custCols;
        m_numMines = static_cast<int>(m_rows * m_cols * m_percentMines / 100);
        break;
    }

    startGame();
}

void MainWindow::showAboutDialog()
{
    QString str =
            "Qt Minesweeper\n\n"
            "Minesweeper written in C++ with Qt Widgets\n\n"
            "https://github.com/DanielMcPherson/QtMinesweeper";

    QMessageBox msg;
    msg.setText(str);
    msg.exec();

}
