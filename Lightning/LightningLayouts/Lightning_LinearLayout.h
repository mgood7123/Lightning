#ifndef LIGHTNING_LINEARLAYOUT_H
#define LIGHTNING_LINEARLAYOUT_H

#include "Lightning_Layout.h"

class Lightning_LinearLayout : public Lightning_Layout
{
public:
    enum Orientation {
        Horizontal, Vertical
    };
    Orientation getOrientation() const;
    void setOrientation(Orientation newOrientation);
private:
    Orientation orientation = Vertical;
public:
    LIGHTNING_CLASS_NAME

    class LinearLayoutParams : public Lightning_View::LayoutParams {
    public:
        // constructors
        using Lightning_View::LayoutParams::LayoutParams;
        float weight = 1;
    };

    // Lightning_View interface
public:
    virtual void onMeasure(float width, float height) override;
};


#endif // LIGHTNING_LINEARLAYOUT_H
