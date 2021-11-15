#include "scribblearea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QColorDialog>
#include <QPainterPath>

scribblearea::scribblearea(QWidget *parent) : QWidget(parent)
{
    currentPenColor = penColor;
    setNewCursor();
    dot = QImage(":/images/resources/alpha.jpg");
    alpha = QImage(256, 256, QImage::Format_ARGB32);
    alpha.fill(penColor);
    alpha.setAlphaChannel(dot);
    setBrush();
}

void scribblearea::SetWindowBackground(const QColor &color)
{
    pal.setColor(QPalette::Window, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

void scribblearea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        lastImage = image;
        lastpoint = event->pos();
        IsScribbling = true;
    }
}

void scribblearea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && IsScribbling)
        drawLineTo(event->pos());
}

void scribblearea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && IsScribbling)
    {
        //drawLineTo(event->pos());
        IsScribbling = false;
        modified = true;
    }
}

void scribblearea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void scribblearea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void scribblearea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(Qt::transparent);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
    lastImage = *image;
}

void scribblearea::drawLineTo(const QPoint &endpoint)
{
    setBrush();

    //QLine line(lastpoint, endpoint);
    QPainter painter(&image);
    if (erase) painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //painter.setPen(QPen(currentPenColor, (1.f * penWidth * pressure), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter.drawLine(line);

    QPainterPath path;
    int offset = penWidth/2;
    path.moveTo(lastpoint - QPoint(offset, offset));
    path.lineTo(endpoint - QPoint(offset, offset));

    for(int pos = 0; pos <= path.length(); pos += spacing)
    {
        qreal percent = path.percentAtLength(pos);
        painter.drawImage(path.pointAtPercent(percent), brush);
    }

    update(QRect(lastpoint, endpoint).normalized().adjusted(-penWidth, -penWidth, +penWidth, +penWidth));
    //update();
    lastpoint = endpoint;
}

void scribblearea::setPenColor(QColor color)
{
    penColor = color;
    currentPenColor = penColor;
    setNewCursor();
    alpha.fill(color);
    alpha.setAlphaChannel(dot);
    setBrush();
}

void scribblearea::toggleEraser(bool isErasing)
{
    erase = isErasing;
    if (isErasing) setPenWidth(penWidth * 4);
    else setPenWidth(penWidth / 4);
    setBrush();
}

void scribblearea::setPenWidth(int width)
{
    if (width <= 3 || (!erase && width >= 181)) return;
    penWidth = width;
    setNewCursor();
    setBrush();
}

void scribblearea::tabletEvent(QTabletEvent *event)
{
    pressure = 0.4f + event->pressure();

    if (event->type() == QEvent::TabletRelease)
    {
        pressure = 0.9f;
    }
}

void scribblearea::ClearScreen()
{
    lastImage = image;
    image.fill(Qt::transparent);
    update();
    modified = true;
}

void scribblearea::setNewCursor()
{
    cursorMap = QPixmap(penWidth + 15, penWidth + 15);
    cursorMap.fill(Qt::transparent);
    QPainter painter(&cursorMap);
    painter.setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(2, 2, penWidth + 5, penWidth + 5);
    QCursor newCursor(cursorMap);
    setCursor(newCursor);
}

void scribblearea::setBrush()
{
    int scale = (pressure + 0.1) * penWidth;
    brush = alpha.scaled(scale, scale);
    spacing = qMax(1, scale/3);
}

void scribblearea::back()
{
    QImage temp = image;
    image = lastImage;
    lastImage = temp;
    update();
}

bool scribblearea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    if (loadedImage.size().width() >= size().width() || loadedImage.size().height() >= size().height())
            loadedImage = loadedImage.scaled(size(), Qt::KeepAspectRatio);
    resizeImage(&loadedImage, newSize);

    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool scribblearea::saveImage(const QString &fileName)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    visibleImage.fill(pal.color(QPalette::Window));
    QPainter painter(&visibleImage);
    painter.drawImage(0, 0, image);

    if (visibleImage.save(fileName))
    {
        modified = false;
        return true;
    }

    return false;
}
