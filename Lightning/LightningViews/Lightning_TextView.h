#ifndef LIGHTNING_TEXTVIEW_H
#define LIGHTNING_TEXTVIEW_H

#include "Lightning_View.h"

class Lightning_TextView : public Lightning_View
{
protected:
    QString text;
    int textSize;
    QColor textColor;
public:
    LIGHTNING_CLASS_NAME

    Lightning_TextView();
    Lightning_TextView(const QString &text);
    Lightning_TextView(const int &textSize);
    Lightning_TextView(const QColor &textColor);
    Lightning_TextView(const Qt::GlobalColor &textColor);
    Lightning_TextView(const int &textSize, const QColor &textColor);
    Lightning_TextView(const int &textSize, const Qt::GlobalColor &textColor);
    Lightning_TextView(const QString &text, const int &textSize);
    Lightning_TextView(const QString &text, const QColor &textColor);
    Lightning_TextView(const QString &text, const Qt::GlobalColor &textColor);
    Lightning_TextView(const QString &text, const int &textSize, const QColor &textColor);

    const QString &getText() const;
    void setText(const QString &newText);

    int getTextSize() const;
    void setTextSize(int newTextSize);

    const QColor &getTextColor() const;
    void setTextColor(const QColor &newTextColor);

    // Lightning_View interface
public:
    virtual void onDraw(LightningEngine &engine) override;
};


#endif // LIGHTNING_TEXTVIEW_H
