#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QObject>
#include <QTableWidget>
#include <QWidget>
#include <QPushButton>
#include "api_generated.h"


class InfoWidget : public QTableWidget
{
    Q_OBJECT
public:
    InfoWidget(QWidget* parent = nullptr);

    void addFigureInfo(const Figure* figure, int figureType);

    void clear();


signals:
    void hideFigure(int row);
    void deleteFigure(int row);

private:
    void hideBtnFunc(QPushButton* hideBtn);
    void deleteBtnFunc(QPushButton* deleteBtn);
};

#endif // INFOWIDGET_H
