#include "Lightning_DebugLayout.h"
#include "../LightningWindowOpenGL.h"

Lightning_DebugLayout::Lightning_DebugLayout()
{
    container = new Lightning_LinearLayout();
    Lightning_LayerLayout::addChild(container, makeLayoutParams_MATCH_PARENT<LayoutParams>());

    auto * l = makeLayoutParams_MATCH_PARENT<Lightning_LinearLayout::LinearLayoutParams>();
    l->weight = 1;

    auto * l2 = makeLayoutParams_MATCH_PARENT<Lightning_LinearLayout::LinearLayoutParams>();
    l2->weight = 7;

    contentView = new Lightning_LayerLayout();
    optionsButton = new Lightning_ButtonView(QString("Options"));
    container->addChild(optionsButton, l);
    container->addChild(contentView, l2);
    textColor = Qt::white;
    textSize = 13;
}

void Lightning_DebugLayout::addChild(Lightning_View *view)
{
    contentView->addChild(view);
}

void Lightning_DebugLayout::addChild(Lightning_View *view, LayoutParams *params)
{
    contentView->addChild(view, params);
}

void Lightning_DebugLayout::detachChild(Lightning_View *view)
{
    contentView->detachChild(view);
}

void Lightning_DebugLayout::removeChild(Lightning_View *view)
{
    contentView->removeChild(view);
}

void Lightning_DebugLayout::removeAllChildren()
{
    contentView->removeAllChildren();
}

int Lightning_DebugLayout::getTextSize() const
{
    return textSize;
}

void Lightning_DebugLayout::setTextSize(int newTextSize)
{
    textSize = newTextSize;
}

const QColor &Lightning_DebugLayout::getTextColor() const
{
    return textColor;
}

void Lightning_DebugLayout::setTextColor(const QColor &newTextColor)
{
    textColor = newTextColor;
}

void Lightning_DebugLayout::onDraw(LightningEngine &engine)
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
    text += "Frames: " + QString::number(frames) + "\n";

    auto * window = getWindowData()->window;

    text += "Application: RenderMode: ";
    text += window->renderModeToString();
    text += "\n";

    text += "Application: commands submitted to engine since start: ";
    text += QString::number(window->commands_submitted_to_engine_since_start);
    text += "\n";

    text += "Application: commands submitted to driver since start: ";
    text += QString::number(window->commands_submitted_to_driver_since_start);
    text += "\n";

    text += "Application: commands submitted to engine last frame: ";
    text += QString::number(window->commands_submitted_to_engine_last_frame);
    text += "\n";

    text += "Application: commands submitted to driver last frame: ";
    text += QString::number(window->commands_submitted_to_driver_last_frame);
    text += "\n";

    // text renders as blocks when drawn to a fbo, work around this by using QImage

    engine.painter_SetHint(QPainter::Antialiasing, true);
    engine.painter_SetPixelSize(applyDpiScale(textSize));
    engine.painter_SetPenColor(textColor);

    auto * l = getLayoutParams();
    Qt::AlignmentFlag alignment = Qt::AlignmentFlag::AlignLeft;
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

    engine.painter_DrawText(w, Qt::TextWrapAnywhere | alignment | Qt::AlignmentFlag::AlignTop, text);
}

void Lightning_DebugLayout::draw(LightningEngine &engine)
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
