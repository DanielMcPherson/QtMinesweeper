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
    connect(m_restartButton, &QPushButton::clicked, this, &MainWindow::restartClicked);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_restartButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // Set window to main layout created above
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // File menu
    // Create file menu
    auto fileMenu = menuBar()->addMenu(tr("File"));
    // Exit menu item
    QAction *exitAction = new QAction(tr("E&xit"));
    exitAction->setShortcuts(QKeySequence::Close);
    fileMenu->addAction(exitAction);
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

void MainWindow::restartClicked(bool checked)
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
    qDebug() << Q_FUNC_INFO;
    QApplication::quit();
}
