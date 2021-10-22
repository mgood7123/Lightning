#ifndef LIGHTNING_LAYOUT_H
#define LIGHTNING_LAYOUT_H

#include "../LightningViews/Lightning_View.h"

class Lightning_Layout : public Lightning_View
{
public:
    virtual ~Lightning_Layout();
    QList<Lightning_View*> children;

    virtual void addChild(Lightning_View * view);
    virtual void addChild(Lightning_View * view, LayoutParams * params);

    virtual void detachChild(Lightning_View * view);

    virtual void removeChild(Lightning_View * view);

    virtual void removeAllChildren();

    // LightningView interface
public:
    LIGHTNING_CLASS_NAME

    virtual void draw(LightningEngine & engine) override;
    virtual bool isLayout() const override final;

    // WindowDataHelper interface
public:
    virtual void setWindowData(const LightningWindowData *windowData) override;
};

#endif // LIGHTNING_LAYOUT_H
