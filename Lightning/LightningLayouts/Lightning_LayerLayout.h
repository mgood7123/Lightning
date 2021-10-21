#ifndef LIGHTNING_LAYERLAYOUT_H
#define LIGHTNING_LAYERLAYOUT_H

#include "Lightning_Layout.h"

class Lightning_LayerLayout : public Lightning_Layout
{
    // Lightning_View interface
public:
    LIGHTNING_CLASS_NAME

    virtual void onMeasure(float width, float height) override;
};

#endif // LIGHTNING_LAYERLAYOUT_H
