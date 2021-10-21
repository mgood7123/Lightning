#include "Lightning_Layout.h"

bool Lightning_Layout::isLayout() const {
    return true;
}

Lightning_Layout::~Lightning_Layout() {
    removeAllChildren();
}

void Lightning_Layout::addChild(Lightning_View *view) {
    LayoutParams * l = view->getLayoutParams();
    addChild(view,  l != nullptr ? l : new LayoutParams(MATCH_PARENT, MATCH_PARENT));
}

void Lightning_Layout::addChild(Lightning_View *view, LayoutParams * params) {
    if (view == nullptr) {
        qFatal("cannot add a nullptr view");
    }
    if (params == nullptr) {
        qFatal("cannot assign a nullptr LayoutParams");
    }
    view->setLayoutParams(params);
    view->setWindowData(getWindowData());
    view->parent = this;
    children.append(view);
    view->onAddedToLayout();
}

void Lightning_Layout::detachChild(Lightning_View *view) {
    if (view == nullptr) {
        qFatal("cannot detach a nullptr view");
    }
    qsizetype count = children.size();
    for (qsizetype i = 0; i < count; i++) {
        Lightning_View * child = children[i];
        if (child == view) {
            view->onRemovedFromLayout();
            children.remove(i);
            break;
        }
    }
    view->setWindowData(nullptr);
}

void Lightning_Layout::removeChild(Lightning_View *view) {
    if (view == nullptr) {
        qFatal("cannot remove a nullptr view");
    }
    qsizetype count = children.size();
    for (qsizetype i = 0; i < count; i++) {
        Lightning_View * child = children[i];
        if (child == view) {
            view->onRemovedFromLayout();
            delete view;
            children.remove(i);
            break;
        }
        break;
    }
}

void Lightning_Layout::removeAllChildren() {
    qsizetype count = children.size();
    for (qsizetype i = 0; i < count; i++) {
        Lightning_View * view = children[i];
        view->onRemovedFromLayout();
        delete view;
        children.remove(i);
        break;
    }
}

void Lightning_Layout::draw(LightningEngine &engine)
{
    auto & bottom = *engine.subEngine(className() + " (bottom)", absoluteCoordinates);
    auto & top = *engine.subEngine(className() + " (top)", absoluteCoordinates);
    onDraw(top);
    for (Lightning_View * view: children) {
        view->draw(*bottom.subEngine(className() + " (child) " + view->className(), absoluteCoordinates));
    }
}

void Lightning_Layout::setWindowData(const LightningWindowData *windowData)
{
    WindowDataHelper::setWindowData(windowData);
    for (Lightning_View * view: children) {
        view->setWindowData(windowData);
    }
}
