#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>

class scribblearea : public QWidget
{
    Q_OBJECT
public:
    explicit scribblearea(QWidget *parent = nullptr);

    QPalette pal;

    void SetWindowBackground(const QColor &color);
    void setPenColor(QColor color);
    void toggleEraser(bool isErasing);
    void setPenWidth(int width);
    void ClearScreen();
    int getPenWidth() { return penWidth;}
    QColor getPenColor(){return penColor;}
    void back();
    bool isModified() {return modified;}
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;

private:
    QPoint lastpoint;
    bool erase = false;
    bool IsScribbling = false;
    QColor penColor = Qt::white;
    QColor currentPenColor;
    QImage image;
    QImage lastImage;
    QImage alpha;
    QImage dot;
    QImage brush;
    QPixmap cursorMap;
    int penWidth = 20;
    int spacing = 2;
    float pressure = 0.8f;
    bool modified = false;

    void drawLineTo(const QPoint &endpoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void zoom(float scale, float direction);
    void updateImage();
    void setNewCursor();
    void setBrush();
signals:

};

#endif // SCRIBBLEAREA_H
