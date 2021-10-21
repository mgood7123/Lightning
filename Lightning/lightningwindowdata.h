#ifndef LIGHTNINGWINDOWDATA_H
#define LIGHTNINGWINDOWDATA_H

#include <QWindow>
#include <QScreen>

class LightningWindowData
{
public:
    QWindow * window = nullptr;
    QScreen * screen = nullptr;
    qreal dpiScale = 1;

    int applyDpiScale(qreal value) const;

    int applyDpiScale(int value) const;

    qreal applyDpiScaleF(qreal value) const;

    qreal applyDpiScaleF(int value) const;
};

#endif // LIGHTNINGWINDOWDATA_H
