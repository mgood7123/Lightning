#ifndef WINDOWDATAHELPER_H
#define WINDOWDATAHELPER_H

#include "../LightningWindowData.h"

class WindowDataHelper {
    // points to read-only data managed by LightningEngine
    const LightningWindowData * windowData = nullptr;

public:
    virtual void setWindowData(const LightningWindowData * windowData);

    const LightningWindowData * getWindowData();

    int applyDpiScale(qreal value) const;

    int applyDpiScale(int value) const;

    qreal applyDpiScaleF(qreal value) const;

    qreal applyDpiScaleF(int value) const;
};

#endif // WINDOWDATAHELPER_H
