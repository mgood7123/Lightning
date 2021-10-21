#include "Lightning_TextView.h"

Lightning_TextView::Lightning_TextView() : Lightning_TextView(13) {}

Lightning_TextView::Lightning_TextView(const QString &text) : Lightning_TextView(text, 13) {}

Lightning_TextView::Lightning_TextView(const int &textSize) : Lightning_TextView(textSize, QColorConstants::White) {}

Lightning_TextView::Lightning_TextView(const QColor &textColor) : Lightning_TextView(13, textColor) {}
Lightning_TextView::Lightning_TextView(const Qt::GlobalColor &textColor) : Lightning_TextView(13, textColor) {}

Lightning_TextView::Lightning_TextView(const int &textSize, const QColor &textColor) : Lightning_TextView("LightningTextView", textSize, textColor) {}
Lightning_TextView::Lightning_TextView(const int &textSize, const Qt::GlobalColor &textColor) : Lightning_TextView("LightningTextView", textSize, textColor) {}

Lightning_TextView::Lightning_TextView(const QString &text, const int &textSize) : Lightning_TextView(text, textSize, QColorConstants::White) {}

Lightning_TextView::Lightning_TextView(const QString &text, const QColor &textColor) : Lightning_TextView(text, 13, textColor) {}
Lightning_TextView::Lightning_TextView(const QString &text, const Qt::GlobalColor &textColor) : Lightning_TextView(text, 13, textColor) {}

Lightning_TextView::Lightning_TextView(const QString &text, const int &textSize, const QColor &textColor) : text(text), textSize(textSize), textColor(textColor) {}

void Lightning_TextView::onDraw(LightningEngine &engine)
{
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
    engine.painter_DrawText(QRectF{0, 0, getMeasuredWidth(), getMeasuredHeight()}, Qt::TextWordWrap | alignment, text);
}

const QString &Lightning_TextView::getText() const
{
    return text;
}

void Lightning_TextView::setText(const QString &newText)
{
    text = newText;
}

int Lightning_TextView::getTextSize() const
{
    return textSize;
}

void Lightning_TextView::setTextSize(int newTextSize)
{
    textSize = newTextSize;
}

const QColor &Lightning_TextView::getTextColor() const
{
    return textColor;
}

void Lightning_TextView::setTextColor(const QColor &newTextColor)
{
    textColor = newTextColor;
}
