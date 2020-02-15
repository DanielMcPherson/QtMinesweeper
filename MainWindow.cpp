#include "MainWindow.h"

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

    // Maintain board state seperately from UI
    m_boardSize = 8;
    m_numMines = 10;

    // Widget to display Minesweeper UI
    m_ui = new BoardWidget(m_boardSize, m_boardSize);
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
    // Create Mines menu
    auto minesMenu = menuBar()->addMenu(tr("Mines"));
    // New game item
    auto newGameAction = new QAction(tr("New Game"));
    connect(newGameAction, &QAction::triggered, this, &MainWindow::restartGame);
    minesMenu->addAction(newGameAction);
    // Difficulty
    minesMenu->addSeparator()->setText(tr("Difficulty"));
    auto easyAction = new QAction(tr("Easy"));
    easyAction->setCheckable(true);
    easyAction->setChecked(true);
    auto mediumAction = new QAction(tr("Medium"));
    mediumAction->setCheckable(true);
    auto hardAction = new QAction(tr("Hard"));
    hardAction->setChecked(true);
    auto customAction = new QAction(tr("Custom"));
    customAction->setCheckable(true);
    minesMenu->addAction(easyAction);
    minesMenu->addAction(mediumAction);
    minesMenu->addAction(hardAction);
    minesMenu->addAction(customAction);
    auto difficultyGroup = new QActionGroup(this);
    difficultyGroup->addAction(easyAction);
    difficultyGroup->addAction(mediumAction);
    difficultyGroup->addAction(hardAction);
    difficultyGroup->addAction(customAction);
    // Exit menu item
    minesMenu->addSeparator();
    auto exitAction = new QAction(tr("E&xit"));
    exitAction->setShortcuts(QKeySequence::Close);
    minesMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exit);


    // Game manager controls the state of the game
    m_gameManager = new GameManager();
    connect(m_gameManager, &GameManager::gameWon, this, &MainWindow::winGame);
    connect(m_gameManager, &GameManager::gameLost, this, &MainWindow::loseGame);
    // Link UI widget to game manager
    // Could create a MinesweeperUI interface class and link different implementations
    // of UIs to game manager
    m_gameManager->setUI(m_ui);

    startGame();
}

MainWindow::~MainWindow()
{

}

void MainWindow::startGame()
{
    m_gameManager->startGame(m_boardSize, m_boardSize, m_numMines);
    m_restartButton->setText(tr("Start Over"));
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
    qDebug() << Q_FUNC_INFO << size;
}
