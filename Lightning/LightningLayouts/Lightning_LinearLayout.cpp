#include "Lightning_LinearLayout.h"

Lightning_LinearLayout::Orientation Lightning_LinearLayout::getOrientation() const
{
    return orientation;
}

void Lightning_LinearLayout::setOrientation(Orientation newOrientation)
{
    orientation = newOrientation;
}

void Lightning_LinearLayout::onMeasure(float width, float height)
{
    if (width == 0 || height == 0) {
        setMeasuredDimensions(0, 0);
        return;
    }

    // for a linear layout, we divide the size by the children count

    //23:16:40 Roughy: layout_weight is basically just the normalized weight * view size, except the weights kinda work in reverse so if you have 1.0 and 2.0 the 1.0 will take up 2/3rds'ish instead of the more logical opposite, if I Recall correctly
    //23:16:58 smallville7123: this is what i have at the moment
    //23:21:26 Roughy: if the weights made sense you'd just do ( weight / sum_of_weights ) * total_size_to_fill.
    //23:23:03 Roughy: in the case of Android's reversed weight values ( 1 - (weight / sum_of_weights ) ) I guess
    //23:26:01 smallville7123: so for example, if two views take up 50 each, and parent is 100, then
    //23:26:50 smallville7123: child_1.section = ( 1 - (childAt(0).weight / sum_of_weights ) );      child_2.section = ( 1 - (childAt(1).weight / sum_of_weights ) )
    //23:27:08 smallville7123: _section  *
    //23:28:47 Roughy: that should give you 0.5 for each of them. Multiply by 100 gives you 50 for each

    float weightSum = 0;

    for(Lightning_View *view : children) {
        auto * l = view->getLayoutParams()->castToType<LinearLayoutParams>();
        if (l == nullptr) {
            weightSum += 1;
        } else {
            weightSum += l->weight;
        }
    }

    bool first = true;
    QRectF drawPosition = {0, 0, 0, 0};
    float section = 0;
    float prevSection = 0;
    for (Lightning_View *view : children) {
        float multiplier = 0;
        auto * l = view->getLayoutParams()->castToType<LinearLayoutParams>();
        if (l == nullptr) {
            multiplier = 1 / weightSum;
        } else {
            multiplier = l->weight / weightSum;
        }
        prevSection = section;
        if (orientation == Horizontal) {
            section = multiplier * width;
            if (first) {
                first = false;
                drawPosition.setWidth(section);
                drawPosition.setHeight(height);
            } else {
                drawPosition.translate(prevSection, 0);
                drawPosition.setWidth(drawPosition.width() + section);
            }
            view->buildCoordinates(drawPosition);
            view->measure(section, height);
        } else {
            section = multiplier * height;
            if (first) {
                first = false;
                drawPosition.setWidth(width);
                drawPosition.setHeight(section);
            } else {
                drawPosition.translate(0, prevSection);
                drawPosition.setHeight(drawPosition.height() + section);
            }
            view->buildCoordinates(drawPosition);
            view->measure(width, section);
        }
    }
    setMeasuredDimensions(width, height);
}
