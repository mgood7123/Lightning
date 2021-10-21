#include "Lightning_Application_FPS_Layout.h"

#include <chrono>

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout() : Lightning_Application_FPS_Layout(13) {}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const int &textSize) : Lightning_Application_FPS_Layout(textSize, QColorConstants::White) {}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(float r, float g, float b) : Lightning_Application_FPS_Layout(13, r, g, b) {}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(float r, float g, float b, float a) : Lightning_Application_FPS_Layout(13, r, g, b, a) {}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const Qt::GlobalColor &textColor) : Lightning_Application_FPS_Layout(13, textColor) {}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const QColor &textColor) : Lightning_Application_FPS_Layout(13, textColor) {}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const int &textSize, float r, float g, float b) : textSize(textSize), textColor(QColor::fromRgbF(r, g, b)) {
    setAlwaysDraw(true);
}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const int &textSize, float r, float g, float b, float a) : textSize(textSize), textColor(QColor::fromRgbF(r, g, b, a)) {
    setAlwaysDraw(true);
}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const int &textSize, const Qt::GlobalColor &textColor) : textSize(textSize), textColor(textColor) {
    setAlwaysDraw(true);
}

Lightning_Application_FPS_Layout::Lightning_Application_FPS_Layout(const int &textSize, const QColor &textColor) : textSize(textSize), textColor(textColor) {
    setAlwaysDraw(true);
}

int Lightning_Application_FPS_Layout::getTextSize() const
{
    return textSize;
}

void Lightning_Application_FPS_Layout::setTextSize(int newTextSize)
{
    textSize = newTextSize;
}

const QColor &Lightning_Application_FPS_Layout::getTextColor() const
{
    return textColor;
}

void Lightning_Application_FPS_Layout::setTextColor(const QColor &newTextColor)
{
    textColor = newTextColor;
}

void Lightning_Application_FPS_Layout::onDraw(LightningEngine &engine)
{
    float targetFPS = getWindowData()->screen->refreshRate();
    QString text;
    text += "Raw FPS: " + QString::number((int)raw_fps) + "\n";
    text += "Exponential FPS: " + QString::number((int)exponential_fps) + "\n";
    text += "Target FPS: " + QString::number(targetFPS) + "\n";
    text += "Time: " + QString::number(lastFrameTime.difference.millisecondsTotal()) + " ms\n";
    text += "Target Time: " + QString::number(1000 / targetFPS) + " ms\n";
    text += "Elapsed: " + QString::number(lastFrameTime.elapsed.secondsTotal()) + ".";
    text += QString::number(lastFrameTime.elapsed.milliseconds()).rightJustified(3, '0') + "  seconds\n";
    text += "Frames: " + QString::number(frames);

    // text renders as blocks when drawn to a fbo, work around this by using QImage

    engine.painter_SetHint(QPainter::Antialiasing, true);
    engine.painter_SetPixelSize(applyDpiScale(textSize));
    engine.painter_SetPenColor(textColor);

    auto * l = getLayoutParams();
    Qt::AlignmentFlag alignment;
    switch (l->gravity) {
        case Lightning_View::LayoutParams::GRAVITY_NONE:
        case Lightning_View::LayoutParams::GRAVITY_LEFT:
            alignment = Qt::AlignmentFlag::AlignLeft;
            break;
        case Lightning_View::LayoutParams::GRAVITY_CENTER:
            alignment = Qt::AlignmentFlag::AlignCenter;
            break;
        case Lightning_View::LayoutParams::GRAVITY_RIGHT:
            alignment = Qt::AlignmentFlag::AlignRight;
            break;
    }

    QRectF w = {0, 0, getMeasuredWidth(), getMeasuredHeight()};

    engine.painter_DrawText(w, Qt::TextWordWrap | alignment | Qt::AlignmentFlag::AlignTop, text);
}

void Lightning_Application_FPS_Layout::draw(LightningEngine &engine)
{
    if (!frameTime.started) {
        frameTime.reset();
        fpsUpdateTime.reset();
        frames = 0;
    }
    Lightning_LayerLayout::draw(engine);
    frames++;
    frameTime.elapsedTime();
    fpsUpdateTime.elapsedTime();
    if (fpsUpdateTime.elapsed.milliseconds() >= 25) {

        fpsUpdateTime.reset();

        raw_fps = (1000.0 / frameTime.difference.milliseconds());

        // calculate exponential fps to provide a
        // smoothed out the reading
        if (fpsContainer.size() == capacity) {
            fpsContainer.removeFirst();
        }
        fpsContainer.append(raw_fps);
        for (auto fps : fpsContainer) {
            N = N < avg_scale ? N+1 : N;
            exponential_fps = (exponential_fps*(N-1) + fps)/N;
        }
    }
    lastFrameTime = frameTime;
    frameTime.elapsedTime();
}
