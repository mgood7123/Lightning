#ifndef LIGHTNING_COLORVIEW_H
#define LIGHTNING_COLORVIEW_H

#include "Lightning_View.h"

class Lightning_ColorView : public Lightning_View
{
    QColor color;
public:
    LIGHTNING_CLASS_NAME

    Lightning_ColorView(float r, float g, float b);
    Lightning_ColorView(float r, float g, float b, float a);
    Lightning_ColorView(QColor color);

    const QColor &getColor() const;
    void setColor(const QColor &newColor);



    // Lightning_View interface
private:
    virtual void onDraw(LightningEngine &engine) override;
};

#endif // LIGHTNING_COLORVIEW_H
