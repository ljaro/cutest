#include "overlaypainter.h"

#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTimer>

OverlayPainter::OverlayPainter()
{

}

OverlayPainter& OverlayPainter::instance()
{
    static OverlayPainter painter;
    return painter;
}

void OverlayPainter::setWindow(QQuickWindow *window)
{
    this->window = window;
    QObject::connect(this->window, &QQuickWindow::afterRendering,
                     this, &OverlayPainter::windowAfterRendering, Qt::DirectConnection);
}

void OverlayPainter::highlightItem(QQuickItem *item, int mseconds, std::function<void ()> callback)
{
    spiedRect = createHighlighRect(item);
    window->update();

    QTimer::singleShot(mseconds, this, [=](){
        spiedRect = QRectF();
        window->update();
        callback();
    });
}

QRectF OverlayPainter::createHighlighRect(QQuickItem* itm)
{
    QRectF rect = itm->boundingRect();
    QPointF tl = itm->mapToScene(rect.topLeft());
    QPointF br = itm->mapToScene(rect.bottomRight());
    return QRectF(tl, br);
}

void OverlayPainter::windowAfterRendering()
{
    Q_ASSERT(QOpenGLContext::currentContext() == window->openglContext());

    QOpenGLPaintDevice device(window->size() * window->devicePixelRatio());
    device.setDevicePixelRatio(window->devicePixelRatio());
    QPainter p(&device);

    p.save();
    p.setPen(QColor(255,10,10,55));
    QBrush b;
    b.setStyle(Qt::BrushStyle::Dense3Pattern);
    b.setColor(QColor(155,255,110,130));
    p.fillRect(spiedRect, b);

    QPen pen;
    pen.setColor(QColor(255,0,0,255));
    pen.setWidth(10);
    p.setPen(pen);
    p.drawPoint(clickedPoint);

    p.restore();
}
