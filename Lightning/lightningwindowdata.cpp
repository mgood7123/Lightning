#include "LightningWindowData.h"

int LightningWindowData::applyDpiScale(qreal value) const {
    return static_cast<int>(dpiScale * value);
}

int LightningWindowData::applyDpiScale(int value) const {
    return applyDpiScale(qreal(value));
}

qreal LightningWindowData::applyDpiScaleF(qreal value) const {
    return dpiScale * value;
}

qreal LightningWindowData::applyDpiScaleF(int value) const {
    return applyDpiScaleF(qreal(value));
}
