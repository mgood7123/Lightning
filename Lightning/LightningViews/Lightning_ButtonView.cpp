#include "Lightning_ButtonView.h"

const QString &Lightning_ButtonView::getText() const
{
    return text;
}

void Lightning_ButtonView::setText(const QString &newText)
{
    text = newText;
}

int Lightning_ButtonView::getTextSize() const
{
    return textSize;
}

void Lightning_ButtonView::setTextSize(int newTextSize)
{
    textSize = newTextSize;
}

const QColor &Lightning_ButtonView::getTextColor() const
{
    return textColor;
}

void Lightning_ButtonView::setTextColor(const QColor &newTextColor)
{
    textColor = newTextColor;
}

Lightning_ButtonView::Lightning_ButtonView() : Lightning_ButtonView(13) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text) : Lightning_ButtonView(text, 13) {}

Lightning_ButtonView::Lightning_ButtonView(const int &textSize) : Lightning_ButtonView(textSize, Qt::white) {}

Lightning_ButtonView::Lightning_ButtonView(float r, float g, float b) : Lightning_ButtonView(13, r, g, b) {}

Lightning_ButtonView::Lightning_ButtonView(float r, float g, float b, float a) : Lightning_ButtonView(13, r, g, b, a) {}

Lightning_ButtonView::Lightning_ButtonView(const Qt::GlobalColor &textColor) : Lightning_ButtonView(13, textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const QColor &textColor) : Lightning_ButtonView(13, textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const int &textSize, float r, float g, float b) : Lightning_ButtonView("LightningButtonView", textSize, r, g, b) {}

Lightning_ButtonView::Lightning_ButtonView(const int &textSize, float r, float g, float b, float a) : Lightning_ButtonView("LightningButtonView", textSize, r, g, b, a) {}

Lightning_ButtonView::Lightning_ButtonView(const int &textSize, const Qt::GlobalColor &textColor) : Lightning_ButtonView("LightningButtonView", textSize, textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const int &textSize, const QColor &textColor) : Lightning_ButtonView("LightningButtonView", textSize, textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const int &textSize) : Lightning_ButtonView(text, textSize, Qt::white) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, float r, float g, float b) : Lightning_ButtonView(text, 13, r, g, b) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, float r, float g, float b, float a) : Lightning_ButtonView(text, 13, r, g, b, a) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const Qt::GlobalColor &textColor) : Lightning_ButtonView(text, 13, textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const QColor &textColor) : Lightning_ButtonView(text, 13, textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const int &textSize, float r, float g, float b) : text(text), textSize(textSize), textColor(QColor::fromRgbF(r, g, b)) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const int &textSize, float r, float g, float b, float a) : text(text), textSize(textSize), textColor(QColor::fromRgbF(r, g, b, a)) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const int &textSize, const Qt::GlobalColor &textColor) : text(text), textSize(textSize), textColor(textColor) {}

Lightning_ButtonView::Lightning_ButtonView(const QString &text, const int &textSize, const QColor &textColor) : text(text), textSize(textSize), textColor(textColor) {}

void Lightning_ButtonView::onDraw(LightningEngine &engine)
{
    float radius = 20;
    float a = radius/2;
    QRectF w = {0, 0, getMeasuredWidth(), getMeasuredHeight()};
    QRectF r = w.adjusted(a, a, -a, -a);
    if (r.width() > 0 && r.height() > 0) {
        engine.painter_SetHint(QPainter::Antialiasing, true);

        auto path_id = engine.painterPath_create();
        engine.painterPath_DrawRoundedRect(path_id, r, a, a);
        engine.painter_SetClipPath(path_id);
        engine.painter_FillPath(path_id, QColor("#404040"));
        engine.painterPath_destroy(path_id);

        engine.painter_SetPixelSize(applyDpiScale(textSize));
        engine.painter_SetPenColor(textColor);
        engine.painter_DrawText(w, Qt::TextWordWrap | Qt::AlignmentFlag::AlignCenter, text);
    }
}
