#include "infowidget.h"
#include <QPainter>

InfoWidget::InfoWidget(QWidget *parent) : QTableWidget(0, 6, parent)
{
    setHorizontalHeaderLabels({"ID", "Type", "Position", "Region", "HideBtn", "Delete"});
}

void InfoWidget::addFigureInfo(const Figure *figure, int figureType)
{
    int row = rowCount();

    QString idItem;
    QString type;
    QString position;
    QString region;

    if (figureType < FigureType_MIN || figureType > FigureType_MAX) {
        qDebug() << "Ошибка: Неправильный тип фигуры";
        return;
    }

    if (!figure->payload()) {
        qDebug() << "Ошибка: Нет данных payload";
        return;
    }

    idItem = QString::number(figureType);

    switch (figureType)
    {
    case FigureType_Rect:
    {
        auto rect = flatbuffers::GetRoot<RectData>(figure->payload()->c_str());
        if (!rect) return;
        type = "Rect";
        position = QString("(%1, %2)").arg(rect->x()).arg(rect->y());
        region = QString("(%1 x %2)").arg(rect->width()).arg(rect->height());
        break;
    }
    case FigureType_Ellipse:
    {
        auto ellipse = flatbuffers::GetRoot<EllipseData>(figure->payload()->c_str());
        if (!ellipse) return;
        type = "Ellipse";
        position = QString("(%1, %2)").arg(ellipse->x()).arg(ellipse->y());
        region = QString("(%1 x %2)").arg(2 * ellipse->r1()).arg(2 * ellipse->r2());
        break;
    }
    case FigureType_Triangle:
    {
        auto triangle = flatbuffers::GetRoot<TriangleData>(figure->payload()->c_str());
        if (!triangle) return;
        idItem = QString::number(2);
        type = "Triangle";
        position = QString("(%1, %2)").arg(triangle->x1()).arg(triangle->y1());
        region = QString("(%1 x %2)").arg(triangle->x2()).arg(triangle->y2());
        break;
    }
    case FigureType_Line:
    {
        auto line = flatbuffers::GetRoot<LineData>(figure->payload()->c_str());
        if (!line) return;
        type = "Line";
        position = QString("(%1, %2)").arg(line->x1()).arg(line->y1());
        region = QString("(%1 x %2)").arg(line->x2()).arg(line->y2());
        break;
    }
    default:
        qDebug() << "Неправильный тип";
    }


    for (int replaceRow = 0; replaceRow < rowCount(); ++replaceRow)
    {
        if (item(replaceRow, 1)->text() == type)
        {
            row = replaceRow;
            removeRow(row);
            break;
        }
    }

    insertRow(row);
    setItem(row, 0, new QTableWidgetItem(idItem));
    setItem(row, 1, new QTableWidgetItem(type));
    setItem(row, 2, new QTableWidgetItem(position));
    setItem(row, 3, new QTableWidgetItem(region));

    QPushButton *hideButton = new QPushButton();
    hideButton->setStyleSheet("background-color: white;");
    hideButton->setIcon(QIcon(":/img/visibility.png"));
    hideButton->setProperty("isHidden", false);

    connect(hideButton, &QPushButton::clicked, [ hideButton, this]()
            {
                bool isHidden = hideButton->property("isHidden").toBool();
                QPixmap pixmap(":/img/visibility.png");
                if (isHidden)
                {
                    hideButton->setIcon(QIcon(pixmap));
                }
                else
                {
                    QPixmap originalPixmap(":/img/visibility.png");
                    QPixmap pixmap(originalPixmap.size());
                    pixmap.fill(Qt::transparent);

                    QPainter painter(&pixmap);
                    painter.setOpacity(0.5);
                    painter.drawPixmap(0, 0, originalPixmap);
                    painter.end();
                    hideButton->setIcon(QIcon(pixmap));
                }
                hideButton->setProperty("isHidden", !isHidden);
                hideBtnFunc(hideButton);
            });
    setCellWidget(row, 4, hideButton);

    QPushButton *deleteButton = new QPushButton();
    deleteButton->setStyleSheet("background-color: transparent;");
    connect(deleteButton, &QPushButton::clicked, [this, deleteButton]()
            {
                deleteBtnFunc(deleteButton);
            });
    deleteButton->setIcon(QIcon(":/img/clear.png"));
    setCellWidget(row, 5, deleteButton);

}

void InfoWidget::clear()
{
    int count = rowCount();
    for (int row = 0; row < count; ++row)
    {
        removeRow(0);
    }
}

void InfoWidget::hideBtnFunc(QPushButton *hideBtn)
{
    int row = -1;
    for (int i = 0; i < rowCount(); ++i)
    {
        if (cellWidget(i, 4) == hideBtn)
        {
            row = i;
            break;
        }
    }

    if (row >= 0 && row < rowCount()) emit hideFigure(item(row, 0)->text().toInt());
}

void InfoWidget::deleteBtnFunc(QPushButton *deleteBtn)
{
    int row = -1;
    for (int i = 0; i < rowCount(); ++i)
    {
        if (cellWidget(i, 5) == deleteBtn)
        {
            row = i;
            break;
        }
    }

    if (row >= 0 && row < rowCount())
    {
        emit deleteFigure(item(row, 0)->text().toInt());
        removeRow(row);
    }
}

