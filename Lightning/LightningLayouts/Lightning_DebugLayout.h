#ifndef LIGHTNING_DEBUGLAYOUT_H
#define LIGHTNING_DEBUGLAYOUT_H

#include "Lightning_LayerLayout.h"
#include "Lightning_LinearLayout.h"
#include "../LightningViews/Lightning_ButtonView.h"

class Lightning_DebugLayout : public Lightning_LayerLayout
{
public:
    ChronoTimer frameTime;
    ChronoTimer fpsUpdateTime;
    ChronoTimer lastFrameTime;

    quint64 frames = 0;
    float raw_fps = 0;

    QList<float> fpsContainer;
    float N = 0;
    float avg_scale = 30;
    float capacity = 20;
    float exponential_fps = 0;

    int textSize;
    QColor textColor;
    Lightning_LinearLayout * container = nullptr;
    Lightning_LayerLayout * contentView = nullptr;
    Lightning_ButtonView * optionsButton = nullptr;
public:
    Lightning_DebugLayout();
    Lightning_DebugLayout(const int &textSize);
    Lightning_DebugLayout(float r, float g, float b);
    Lightning_DebugLayout(float r, float g, float b, float a);
    Lightning_DebugLayout(const Qt::GlobalColor &textColor);
    Lightning_DebugLayout(const QColor &textColor);
    Lightning_DebugLayout(const int &textSize, float r, float g, float b);
    Lightning_DebugLayout(const int &textSize, float r, float g, float b, float a);
    Lightning_DebugLayout(const int &textSize, const Qt::GlobalColor &textColor);
    Lightning_DebugLayout(const int &textSize, const QColor &textColor);

    int getTextSize() const;
    void setTextSize(int newTextSize);

    const QColor &getTextColor() const;
    void setTextColor(const QColor &newTextColor);

    // Lightning_View interface
public:
    LIGHTNING_CLASS_NAME

    virtual void addChild(Lightning_View *view) override;
    virtual void addChild(Lightning_View *view, LayoutParams *params) override;
    virtual void detachChild(Lightning_View *view) override;
    virtual void removeChild(Lightning_View *view) override;
    virtual void removeAllChildren() override;
    virtual void onDraw(LightningEngine &engine) override;
    virtual void draw(LightningEngine &engine) override;
};

#endif // LIGHTNING_DEBUGLAYOUT_H
