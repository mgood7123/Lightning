#ifndef PAINTHOLDER_H
#define PAINTHOLDER_H

#include <QImage>
#include <QOpenGLPaintDevice>
#include <QPainter>

class PaintHolder {
    QSize size = {-1, -1};
    int width = -1;
    int height = -1;

public:

    QOpenGLPaintDevice * paintDeviceOpenGL = nullptr;
    QPainter * painterGL = nullptr;

    QOpenGLContext * glContext = nullptr;
    QOpenGLFunctions * glES2Functions = nullptr;
    QOpenGLExtraFunctions* glES3Functions = nullptr;

    QPainter * beginGL();

    bool resize(const QSize & size);

    bool resize(const int & width, const int & height);

    void deallocate();

    bool allocate(const QSize & size);

    bool allocate(const int & width, const int & height);

    QSize getSize();

    PaintHolder() = default;

    PaintHolder(const QSize & size);

    PaintHolder(const int & width, const int & height);

    PaintHolder(const PaintHolder & other);

    PaintHolder&operator=(const PaintHolder & other);

    PaintHolder(PaintHolder && other);

    PaintHolder&operator=(PaintHolder && other);

    ~PaintHolder();
};

#endif // PAINTHOLDER_H
