#ifndef LIGHTNING_APPLICATION_FPS_LAYOUT_H
#define LIGHTNING_APPLICATION_FPS_LAYOUT_H

#include "Lightning_LayerLayout.h"

class Lightning_Application_FPS_Layout : public Lightning_LayerLayout
{
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
public:
    Lightning_Application_FPS_Layout();
    Lightning_Application_FPS_Layout(const int &textSize);
    Lightning_Application_FPS_Layout(float r, float g, float b);
    Lightning_Application_FPS_Layout(float r, float g, float b, float a);
    Lightning_Application_FPS_Layout(const Qt::GlobalColor &textColor);
    Lightning_Application_FPS_Layout(const QColor &textColor);
    Lightning_Application_FPS_Layout(const int &textSize, float r, float g, float b);
    Lightning_Application_FPS_Layout(const int &textSize, float r, float g, float b, float a);
    Lightning_Application_FPS_Layout(const int &textSize, const Qt::GlobalColor &textColor);
    Lightning_Application_FPS_Layout(const int &textSize, const QColor &textColor);

    int getTextSize() const;
    void setTextSize(int newTextSize);

    const QColor &getTextColor() const;
    void setTextColor(const QColor &newTextColor);



    // Lightning_View interface
public:
    LIGHTNING_CLASS_NAME

    virtual void onDraw(LightningEngine &engine) override;
    virtual void draw(LightningEngine &engine) override;
};

#endif // LIGHTNING_APPLICATION_FPS_LAYOUT_H
