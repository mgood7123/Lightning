#include "paintholder.h"

QPainter *PaintHolder::beginGL() {
    if (painterGL == nullptr) return nullptr;
    painterGL->begin(paintDeviceOpenGL);
    return painterGL;
}

bool PaintHolder::resize(const QSize &size) {
    return allocate(size);
}

bool PaintHolder::resize(const int &width, const int &height) {
    return allocate(width, height);
}

void PaintHolder::deallocate()
{
    if (painterGL == nullptr) return;
    delete painterGL;
    painterGL = nullptr;
    glES3Functions = nullptr;
    glES2Functions = nullptr;
    glContext = nullptr;
    delete paintDeviceOpenGL;
    paintDeviceOpenGL = nullptr;
}

bool PaintHolder::allocate(const QSize &size)
{
    return allocate(size.width(), size.height());
}

bool PaintHolder::allocate(const int &width, const int &height)
{
    if (this->width != width || this->height != height) {
        this->width = width;
        this->height = height;
        size = {width, height};
        deallocate();
        if (width == 0 || height == 0) return false;
        paintDeviceOpenGL = new QOpenGLPaintDevice(width, height);
        painterGL = new QPainter();
        glContext = paintDeviceOpenGL->context();
        glES2Functions = glContext->functions();
        glES3Functions = glContext->extraFunctions();
        return true;
    }
    return false;
}

QSize PaintHolder::getSize() {
    return size;
}

PaintHolder::PaintHolder(const QSize &size) {
    allocate(size);
}

PaintHolder::PaintHolder(const int &width, const int &height) {
    allocate(width, height);
}

PaintHolder::PaintHolder(const PaintHolder &other) {
    resize(other.size);
}

PaintHolder &PaintHolder::operator=(const PaintHolder &other) {
    resize(other.size);
    return *this;
}

PaintHolder::PaintHolder(PaintHolder &&other) {
    size = other.size;
    other.size = {0, 0};

    glES3Functions = other.glES3Functions;
    other.glES3Functions = nullptr;

    glES2Functions = other.glES2Functions;
    other.glES2Functions = nullptr;

    glContext = other.glContext;
    other.glContext = nullptr;

    paintDeviceOpenGL = other.paintDeviceOpenGL;
    other.paintDeviceOpenGL = nullptr;
}

PaintHolder &PaintHolder::operator=(PaintHolder &&other) {
    deallocate();

    size = other.size;
    other.size = {0, 0};

    glES3Functions = other.glES3Functions;
    other.glES3Functions = nullptr;

    glES2Functions = other.glES2Functions;
    other.glES2Functions = nullptr;

    glContext = other.glContext;
    other.glContext = nullptr;

    return *this;
}

PaintHolder::~PaintHolder() {
    deallocate();
}
