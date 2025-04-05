#include "drawwidget.h"
#include <QColor>

DrawWidget::DrawWidget(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene(this))
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(0, 0, parent->width(), parent->height());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void DrawWidget::drawFigure(const Figure *figure, int figureType)
{
    switch (figureType)
    {
    case FigureType_Rect:
    {
        if (currentRect != nullptr)
        {
            scene->removeItem(currentRect);
            delete currentRect;
            currentRect = nullptr;
        }
        auto rect = flatbuffers::GetRoot<RectData>(figure->payload()->c_str());
        currentRect = new CursorRectItem(rect->x(), rect->y(), rect->width(), rect->height());
        scene->addItem(currentRect);
        currentRect->setPen(QPen(Qt::black));
        currentRect->setBrush(QBrush(QColor(rect->color_hex()->c_str())));
        currentRect->setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    }
    case FigureType_Ellipse:
    {
        if (currentEllipse != nullptr)
        {
            scene->removeItem(currentEllipse);
            delete currentEllipse;
            currentEllipse = nullptr;
        }

        auto ellipse = flatbuffers::GetRoot<EllipseData>(figure->payload()->c_str());
        currentEllipse = new CursorEllipseItem(ellipse->x() - ellipse->r1(), ellipse->y() - ellipse->r2(), 2 * ellipse->r1(), 2 * ellipse->r2());
        scene->addItem(currentEllipse);
        currentEllipse->setPen(QPen(Qt::black));
        currentEllipse->setBrush(QBrush(QColor(ellipse->color_hex()->c_str())));
        currentEllipse->setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    }
    case FigureType_Triangle:
    {
        if (currentTriangle != nullptr)
        {
            scene->removeItem(currentTriangle);
            delete currentTriangle;
            currentTriangle = nullptr;
        }
        auto triangle = flatbuffers::GetRoot<TriangleData>(figure->payload()->c_str());
        QPolygonF polygon;
        polygon << QPointF(triangle->x1(), triangle->y1())
                << QPointF(triangle->x2(), triangle->y2())
                << QPointF(triangle->x3(), triangle->y3());
        currentTriangle = new CursorPolygonItem(polygon);
        scene->addItem(currentTriangle);
        currentTriangle->setPen(QPen(Qt::black));
        currentTriangle->setBrush(QBrush(QColor(triangle->color_hex()->c_str())));
        currentTriangle->setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    }
    case FigureType_Line:
    {
        if (currentLine != nullptr)
        {
            scene->removeItem(currentLine);
            delete currentLine;
            currentLine = nullptr;
        }
        auto line = flatbuffers::GetRoot<LineData>(figure->payload()->c_str());
        QPolygonF polygon;

        currentLine = new CursorLineItem(line->x1(), line->y1(), line->x2(), line->y2());
        scene->addItem(currentLine);
        currentLine->setPen(QPen(QColor(line->color_hex()->c_str())));
        currentLine->setFlag(QGraphicsItem::ItemIsMovable, true);
        break;
    }
    default:
        qDebug() << "Неправильный тип";
    }
}

void DrawWidget::deleteFigure(int type)
{
    switch (type)
    {
    case FigureType_Rect:
    {
        if (currentRect != nullptr)
        {
            scene->removeItem(currentRect);
            delete currentRect;
            currentRect = nullptr;
        }
        break;
    }
    case FigureType_Ellipse:
    {
        if (currentEllipse != nullptr)
        {
            scene->removeItem(currentEllipse);
            delete currentEllipse;
            currentEllipse = nullptr;
        }
        break;
    }
    case FigureType_Triangle:
    {
        if (currentTriangle != nullptr)
        {
            scene->removeItem(currentTriangle);
            delete currentTriangle;
            currentTriangle = nullptr;
        }
        break;
    }
    case FigureType_Line:
    {
        if (currentLine != nullptr)
        {
            scene->removeItem(currentLine);
            delete currentLine;
            currentLine = nullptr;
        }
        break;
    }
    default:
        qDebug() << "Неправильный тип";
    }
}

void DrawWidget::changeOpacityFigure(int type)
{
    switch (type)
    {
    case FigureType_Rect:
    {
        if (currentRect != nullptr)
        {
            if (currentRect->isVisible()) currentRect->hide();
            else currentRect->setVisible(true);
        }
        break;
    }
    case FigureType_Ellipse:
    {
        if (currentEllipse != nullptr)
        {
            if (currentEllipse->isVisible()) currentEllipse->hide();
            else currentEllipse->setVisible(true);
        }
        break;
    }
    case FigureType_Triangle:
    {
        if (currentTriangle != nullptr)
        {
            if (currentTriangle->isVisible()) currentTriangle->hide();
            else currentTriangle->setVisible(true);
        }
        break;
    }
    case FigureType_Line:
    {
        if (currentLine != nullptr)
        {
            if (currentLine->isVisible()) currentLine->hide();
            else currentLine->setVisible(true);
        }
        break;
    }
    default:
        qDebug() << "Неправильный тип";
    }
}

void DrawWidget::clear()
{
    deleteFigure(0);
    deleteFigure(1);
    deleteFigure(2);
    deleteFigure(3);

}

// ##############################################################################################################

void CursorGraphicsItem::setupItem(QGraphicsItem *item)
{
    item->setAcceptHoverEvents(true);
}

void CursorGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event, QGraphicsItem *item)
{
    item->setCursor(Qt::OpenHandCursor);
}

void CursorGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event, QGraphicsItem *item)
{
    item->unsetCursor();
}

void CursorGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *item)
{
    if (event->button() == Qt::LeftButton)
    {
        item->setCursor(Qt::ClosedHandCursor);
    }
}

void CursorGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *item)
{
    if (event->button() == Qt::LeftButton)
    {
        item->setCursor(Qt::OpenHandCursor);
    }
}

CursorRectItem::CursorRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{
    setupItem(this);
}

void CursorRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverEnterEvent(event, this);
    QGraphicsRectItem::hoverEnterEvent(event);
}

void CursorRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverLeaveEvent(event, this);
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void CursorRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mousePressEvent(event, this);
    QGraphicsRectItem::mousePressEvent(event);
}

void CursorRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mouseReleaseEvent(event, this);
    QGraphicsRectItem::mouseReleaseEvent(event);
}

CursorPolygonItem::CursorPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsPolygonItem(polygon, parent)
{
    setupItem(this);
}

void CursorPolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverEnterEvent(event, this);
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void CursorPolygonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverLeaveEvent(event, this);
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

void CursorPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mousePressEvent(event, this);
    QGraphicsPolygonItem::mousePressEvent(event);
}

void CursorPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mouseReleaseEvent(event, this);
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

CursorEllipseItem::CursorEllipseItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x, y, width, height, parent)
{
    setupItem(this);
}

void CursorEllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverEnterEvent(event, this);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void CursorEllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverLeaveEvent(event, this);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

void CursorEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mousePressEvent(event, this);
    QGraphicsEllipseItem::mousePressEvent(event);
}

void CursorEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mouseReleaseEvent(event, this);
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

CursorLineItem::CursorLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
    : QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    setupItem(this);
}

void CursorLineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverEnterEvent(event, this);
    QGraphicsLineItem::hoverEnterEvent(event);
}

void CursorLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    CursorGraphicsItem::hoverLeaveEvent(event, this);
    QGraphicsLineItem::hoverLeaveEvent(event);
}

void CursorLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mousePressEvent(event, this);
    QGraphicsLineItem::mousePressEvent(event);
}

void CursorLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CursorGraphicsItem::mouseReleaseEvent(event, this);
    QGraphicsLineItem::mouseReleaseEvent(event);
}
