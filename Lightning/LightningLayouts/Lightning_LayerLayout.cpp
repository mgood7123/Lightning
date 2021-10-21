#include "Lightning_LayerLayout.h"

void Lightning_LayerLayout::onMeasure(float width, float height)
{
    if (width == 0 || height == 0) {
        setMeasuredDimensions(0, 0);
        return;
    }
    for (Lightning_View *view : children) {
        view->buildCoordinates({0, 0, width, height});
        view->measure(width, height);
    }
    setMeasuredDimensions(width, height);
}
