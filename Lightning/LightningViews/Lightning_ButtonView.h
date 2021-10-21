#ifndef LIGHTNING_BUTTONVIEW_H
#define LIGHTNING_BUTTONVIEW_H

#include "Lightning_View.h"

class Lightning_ButtonView : public Lightning_View
{
protected:
    QString text;
    int textSize;
    QColor textColor;
    QRect lastW;
public:
    LIGHTNING_CLASS_NAME

    Lightning_ButtonView();
    Lightning_ButtonView(const QString &text);
    Lightning_ButtonView(const int &textSize);
    Lightning_ButtonView(float r, float g, float b);
    Lightning_ButtonView(float r, float g, float b, float a);
    Lightning_ButtonView(const Qt::GlobalColor &textColor);
    Lightning_ButtonView(const QColor &textColor);
    Lightning_ButtonView(const int &textSize, float r, float g, float b);
    Lightning_ButtonView(const int &textSize, float r, float g, float b, float a);
    Lightning_ButtonView(const int &textSize, const Qt::GlobalColor &textColor);
    Lightning_ButtonView(const int &textSize, const QColor &textColor);
    Lightning_ButtonView(const QString &text, const int &textSize);
    Lightning_ButtonView(const QString &text, float r, float g, float b);
    Lightning_ButtonView(const QString &text, float r, float g, float b, float a);
    Lightning_ButtonView(const QString &text, const Qt::GlobalColor &textColor);
    Lightning_ButtonView(const QString &text, const QColor &textColor);
    Lightning_ButtonView(const QString &text, const int &textSize, float r, float g, float b);
    Lightning_ButtonView(const QString &text, const int &textSize, float r, float g, float b, float a);
    Lightning_ButtonView(const QString &text, const int &textSize, const Qt::GlobalColor &textColor);
    Lightning_ButtonView(const QString &text, const int &textSize, const QColor &textColor);

    const QString &getText() const;
    void setText(const QString &newText);

    int getTextSize() const;
    void setTextSize(int newTextSize);

    const QColor &getTextColor() const;
    void setTextColor(const QColor &newTextColor);

    // Lightning_View interface
private:
    virtual void onDraw(LightningEngine &engine) override;
};

#endif // LIGHTNING_BUTTONVIEW_H
