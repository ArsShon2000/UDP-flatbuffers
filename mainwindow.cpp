#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UdpReceiver/udpreceiver.h"
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    drawWidget = new DrawWidget(ui->pageForFugire);
    infoWidget = new InfoWidget(ui->pageForTable);
    ui->layoutForFugire->addWidget(drawWidget);
    ui->layoutForTable->addWidget(infoWidget);
    ui->stackedWidget->setCurrentIndex(0);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* viewMenu = new QMenu("Меню", this);
    menuBar->addMenu(viewMenu);
    setMenuBar(menuBar);

    QAction* drawAction = viewMenu->addAction("Отрисовка");
    QAction* infoAction = viewMenu->addAction("Таблица");
    QAction* cleanAction = viewMenu->addAction("Очистить");

    connect(infoWidget, &InfoWidget::deleteFigure, this, &MainWindow::deleteWidget);
    connect(infoWidget, &InfoWidget::hideFigure, this, &MainWindow::changeOpacityWidget);

    connect(drawAction, &QAction::triggered, this, &MainWindow::showDrawWidget);
    connect(infoAction, &QAction::triggered, this, &MainWindow::showInfoWidget);
    connect(cleanAction, &QAction::triggered, this, &MainWindow::clearDrawWidget);

    QShortcut *shortcutDW = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Left), this);
    connect(shortcutDW, &QShortcut::activated, this, &MainWindow::showDrawWidget);
    QShortcut *shortcutIF = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Right), this);
    connect(shortcutIF, &QShortcut::activated, this, &MainWindow::showInfoWidget);
    QShortcut *shortcutCL = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(shortcutCL, &QShortcut::activated, this, &MainWindow::clearDrawWidget);

    UdpReceiver* receiver = new UdpReceiver(this);

    connect(receiver, &UdpReceiver::figureReceived, this, &MainWindow::handleFigureReceived);
    receiver->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showDrawWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::deleteWidget(int type)
{
    drawWidget->deleteFigure(type);
}

void MainWindow::changeOpacityWidget(int type)
{
    drawWidget->changeOpacityFigure(type);
}

void MainWindow::showInfoWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::clearDrawWidget()
{
    infoWidget->clear();
    drawWidget->clear();
}

void MainWindow::handleFigureReceived(const Figure *figure)
{
    int figureType = figure->type();
    drawWidget->drawFigure(figure , figureType);
    infoWidget->addFigureInfo(figure , figureType);
}
