#include "Lightning_View.h"

float Lightning_View::MATCH_PARENT = -1.0;
float Lightning_View::WRAP_CONTENT = -2.0;

Lightning_View::Lightning_View()
{
}

Lightning_View::~Lightning_View() {
    delete layoutParams;
}

void Lightning_View::resize(float w, float h)
{
}

void Lightning_View::onDraw(LightningEngine & engine)
{
}

void Lightning_View::draw(LightningEngine &engine) {
    onDraw(*engine.subEngine(className(), absoluteCoordinates));
}

Lightning_View::VISIBILITY Lightning_View::getVisibility() const
{
    return visibility;
}

void Lightning_View::setVisibility(VISIBILITY newVisibility)
{
    visibility = newVisibility;
}

void Lightning_View::setAlwaysDraw(bool enabled) {
    alwaysDraw = enabled;
}

bool Lightning_View::getAlwaysDraw() const {
    return alwaysDraw;
}

void Lightning_View::invalidate() {
    invalidated = true;
    if (parent != nullptr) {
        parent->invalidate();
    }
}

void Lightning_View::setTag(const QString & name) {
    tag = name;
}

QString Lightning_View::getTag() const {
    return tag;
}

Lightning_View::LayoutParams *Lightning_View::getLayoutParams() const {
    return layoutParams;
}

void Lightning_View::setLayoutParams(LayoutParams *params) {
    // important optimization:
    // if params are self, then
    //   do not delete layoutParams
    //     it will invalidate/corrupt the passed params
    //     since it points to layoutParams, resulting in use-after-free
    //
    //   avoid this useless assignment:
    //     LayoutParams * params = layoutParams;
    //     layoutParams = params;
    if (params != layoutParams) {
        delete layoutParams;
        layoutParams = params;
    }
}

void Lightning_View::measure(float width, float height)
{
    if (visibility == GONE) return;
    calledSetMeasuredDimensions = false;
    onMeasure(width, height);
    if (!calledSetMeasuredDimensions) {
        qFatal("invalid measurement, did you forget to call `setMeasuredDimensions(const QSizeF &);` or `setMeasuredDimensions(float, float)` ?");
    }
}

void Lightning_View::setMeasuredDimensions(float width, float height)
{
    setMeasuredDimensions({width, height});
}

void Lightning_View::setMeasuredDimensions(const QSizeF &size)
{
    if (size.width() == MATCH_PARENT || size.height() == MATCH_PARENT) {
        qFatal("cannot set measured dimensions to MATCH_PARENT");
    } else if (size.width() == WRAP_CONTENT || size.height() == WRAP_CONTENT) {
        qFatal("cannot set measured dimensions to WRAP_CONTENT");
    }
    calledSetMeasuredDimensions = true;
    canDraw = !size.isEmpty();
    if (measuredDimensions != size) {
        measuredDimensions = size;
        invalidate();
    }
}

QSizeF Lightning_View::getMeasuredDimensions()
{
    return measuredDimensions;
}

float Lightning_View::getMeasuredWidth()
{
    return measuredDimensions.width();
}

float Lightning_View::getMeasuredHeight()
{
    return measuredDimensions.height();
}

void Lightning_View::onMeasure(float width, float height)
{
    LayoutParams * params = getLayoutParams();
    QSizeF measuredDimensions = QSizeF{params->width, params->height};
    if (params->width == MATCH_PARENT) {
        measuredDimensions.setWidth(width);
    } else if (params->width == WRAP_CONTENT) {
        measuredDimensions.setWidth(0);
    }
    if (params->height == MATCH_PARENT) {
        measuredDimensions.setHeight(height);
    } else if (params->height == WRAP_CONTENT) {
        measuredDimensions.setHeight(0);
    }
    setMeasuredDimensions(measuredDimensions);
}

void Lightning_View::onAddedToLayout()
{
//    startAnimation();
}

void Lightning_View::onRemovedFromLayout()
{
//    stopAnimation();
}

bool Lightning_View::isLayout() const {
    return false;
}

void Lightning_View::buildCoordinates(const QRectF &relativeCoordinates) {
    this->relativeCoordinates = relativeCoordinates;
    if (parent == nullptr) {
        absoluteCoordinates = this->relativeCoordinates;
    } else {
        float atlx = parent->absoluteCoordinates.topLeftX + this->relativeCoordinates.topLeftX;
        float atly = parent->absoluteCoordinates.topLeftY + this->relativeCoordinates.topLeftY;
        float abrx = parent->absoluteCoordinates.topLeftX + this->relativeCoordinates.bottomRightX;
        float abry = parent->absoluteCoordinates.topLeftY + this->relativeCoordinates.bottomRightY;
        absoluteCoordinates = CoordinateInfo(atlx, atly, abrx, abry);
    }
}

Lightning_View::LayoutParams::LayoutParams(float width, float height) : width(width), height(height) {}
