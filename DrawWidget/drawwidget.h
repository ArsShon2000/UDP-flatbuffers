#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include "api_generated.h"
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>


class CursorGraphicsItem
{
public:
     CursorGraphicsItem() {}

protected:
    void setupItem(QGraphicsItem* item);

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event, QGraphicsItem* item);

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event, QGraphicsItem* item);

    void mousePressEvent(QGraphicsSceneMouseEvent* event, QGraphicsItem* item);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, QGraphicsItem* item);
};

class CursorRectItem : public QGraphicsRectItem, public CursorGraphicsItem
{
public:
    CursorRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
class CursorPolygonItem : public QGraphicsPolygonItem, public CursorGraphicsItem
{
public:
    CursorPolygonItem(const QPolygonF& polygon, QGraphicsItem* parent = nullptr);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
class CursorEllipseItem : public QGraphicsEllipseItem, public CursorGraphicsItem
{
public:
    CursorEllipseItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
class CursorLineItem : public QGraphicsLineItem, public CursorGraphicsItem
{
public:
    CursorLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem* parent = nullptr);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};


class DrawWidget : public QGraphicsView
{
    Q_OBJECT
public:
    DrawWidget(QWidget* parent = nullptr);

    void drawFigure(const Figure* figure, int figureType);
    void deleteFigure(int type);
    void changeOpacityFigure(int type);
    void clear();


private:
    QGraphicsScene* scene;
    QGraphicsRectItem *currentRect = nullptr;
    QGraphicsEllipseItem *currentEllipse = nullptr;
    QGraphicsPolygonItem *currentTriangle = nullptr;
    QGraphicsLineItem *currentLine = nullptr;
};

#endif // DRAWWIDGET_H
