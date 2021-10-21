#include "Lightning_ColorView.h"

const QColor &Lightning_ColorView::getColor() const
{
    return color;
}

void Lightning_ColorView::setColor(const QColor &newColor)
{
    color = newColor;
}

Lightning_ColorView::Lightning_ColorView(float r, float g, float b)
{
    this->color.setRgbF(r, g, b, 1);
}

Lightning_ColorView::Lightning_ColorView(float r, float g, float b, float a)
{
    this->color.setRgbF(r, g, b, a);
}

Lightning_ColorView::Lightning_ColorView(QColor color)
{
    this->color = color;
}

void Lightning_ColorView::onDraw(LightningEngine &engine)
{
    QRectF w = {0, 0, getMeasuredWidth(), getMeasuredHeight()};
    engine.setClearColor(color);
    engine.clear(LightningEngine::COLOR_BIT);
}
