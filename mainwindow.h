#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QUdpSocket>
#include "DrawWidget/drawwidget.h"
#include "InfoWidget/infowidget.h"
#include "api_generated.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showDrawWidget();

    void deleteWidget(int type);

    void changeOpacityWidget(int type);

    void showInfoWidget();

    void clearDrawWidget();

    void handleFigureReceived(const Figure* figure);

private:
    DrawWidget* drawWidget;
    InfoWidget* infoWidget;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
