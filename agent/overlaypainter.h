#ifndef OVERLAYPAINTER_H
#define OVERLAYPAINTER_H

#include <QObject>
#include <QQuickWindow>
#include <QQuickItem>

class OverlayPainter : public QObject
{
    Q_OBJECT
public:
    static OverlayPainter& instance();
    void setWindow(QQuickWindow* window);
    void highlightItem(QQuickItem* item, int mseconds, std::function<void ()> callback);
private:
    OverlayPainter();
    void windowAfterRendering();
    QRectF spiedRect;
    QPointF clickedPoint;
    QQuickWindow* window = nullptr;
    QRectF createHighlighRect(QQuickItem *itm);
};

#endif // OVERLAYPAINTER_H
