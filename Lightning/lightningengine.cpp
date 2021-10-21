#include "LightningEngine.h"

LightningEngine::LightningEngine(): commandList(new LightningCommandList()), commandListAttached(false) {}

LightningEngine::LightningEngine(LightningCommandList *list): commandList(list), commandListAttached(true) {}

LightningEngine::~LightningEngine() {
    if (!commandListAttached) {
        delete commandList;
    }
    for (LightningEngine * child: children) {
        delete child;
    }
}

LightningCommandList *LightningEngine::getCommandList() {
    return commandList;
}

QList<LightningEngine *> & LightningEngine::getChildren() {
    return children;
}

void LightningEngine::fatal(const char *string) {
    qFatal("%s", string);
}

void LightningEngine::fatal(const QString &string) {
    auto A = string.toStdString();
    qFatal("%s", A.c_str());
}

qsizetype LightningEngine::commandListSize() {
    return commandList->commands.size();
}

LightningEngine * LightningEngine::subEngine(QString tag, float relative_fromX, float relative_fromY, float relative_toX, float relative_toY)
{
    CoordinateHelper::CoordinateInfo a(
                relative_fromX, relative_fromY,
                relative_toX, relative_toY
    );
    return subEngine(tag, a);
}

LightningEngine * LightningEngine::subEngine(QString tag, CoordinateHelper::CoordinateInfo &absoluteCoordinates) {
    LightningEngine * engine = new LightningEngine();
    children.append(engine);
    engine->setDrawRegion(tag, absoluteCoordinates);
    return engine;
}

void LightningEngine::eraseChildren() {
    for (auto * child : children) {
        child->eraseChildren();
    }
    // children are allocated engines, delete them
    for (LightningEngine * child: children) {
        delete child;
    }
    children = {};
}

void LightningEngine::painter_SetPixelSize(int pixelSize) {
    commandList->addCommand(state_painter_SetPixelSize);
    commandList->addInt(pixelSize);
}

void LightningEngine::painter_SetPenColor(float r, float g, float b, float a) {
    commandList->addCommand(state_painter_SetPenColor);
    commandList->addFloat({r, g, b, a});
}

void LightningEngine::painter_SetPenColor(QColor color) {
    painter_SetPenColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void LightningEngine::painter_DrawText(const QRect &rectangle, int flags, const QString &text) {
    painter_DrawText(QRectF{
                    static_cast<float>(rectangle.x()),
                    static_cast<float>(rectangle.y()),
                    static_cast<float>(rectangle.width()),
                    static_cast<float>(rectangle.height())
                }, flags, text);
}
void LightningEngine::painter_DrawText(const QRectF &rectangle, int flags, const QString &text) {
    commandList->addCommand(draw_painter_DrawText);
    commandList->addFloat({
                              static_cast<float>(rectangle.x()),
                              static_cast<float>(rectangle.y()),
                              static_cast<float>(rectangle.width()),
                              static_cast<float>(rectangle.height())
                          });
    commandList->addInt(flags);
    commandList->addQString(text);
}

void LightningEngine::painter_SetHint(QPainter::RenderHint hint, bool enabled)
{
    commandList->addCommand(state_painter_SetHint);
    commandList->addInt(hint);
    commandList->addBool(enabled);
}

void LightningEngine::painter_SetClipPath(quint64 path_id)
{
    commandList->addCommand(state_painter_SetClipPath);
    commandList->addQUnsignedInt64(path_id);
}

void LightningEngine::painter_FillPath(quint64 path_id, float r, float g, float b, float a)
{
    commandList->addCommand(draw_painter_FillPath);
    commandList->addQUnsignedInt64(path_id);
    commandList->addFloat({r, g, b, a});
}

void LightningEngine::painter_FillPath(quint64 path_id, QColor color)
{
    painter_FillPath(path_id, color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

quint64 LightningEngine::painterPath_create()
{
    auto path_id = next_id;
    commandList->addCommand(state_painterPath_create);
    commandList->addQUnsignedInt64(path_id);
    next_id++;
    return path_id;
}

void LightningEngine::painterPath_DrawRoundedRect(quint64 path_id, const QRectF &size, float rx, float ry)
{
    commandList->addCommand(draw_painterPath_DrawRoundedRect);
    commandList->addQUnsignedInt64(path_id);
    commandList->addFloat({
                              static_cast<float>(size.x()),
                              static_cast<float>(size.y()),
                              static_cast<float>(size.width()),
                              static_cast<float>(size.height()),
                              rx, ry
    });
}

void LightningEngine::painterPath_destroy(quint64 path_id)
{
    commandList->addCommand(state_painterPath_destroy);
    commandList->addQUnsignedInt64(path_id);
}

void LightningEngine::setClearColor(const QColor & color) {
    setClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void LightningEngine::setClearColor(float r, float g, float b, float a) {
    commandList->addCommand(state_setClearColor);
    commandList->addFloat({r, g, b, a});
}

void LightningEngine::clear(LightningEngine::BIT bit) {
    commandList->addCommand(draw_clear);
    commandList->addInt(bit);
}

void LightningEngine::scissor(const QRect &rectangle)
{
    scissor(rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height());
}

void LightningEngine::scissor(int x, int y, int width, int height)
{
    commandList->addCommand(state_scissor);
    commandList->addInt({x, y, width, height});
}

void LightningEngine::setDrawRegion(QString tag, float relative_fromX, float relative_fromY, float relative_toX, float relative_toY)
{
    CoordinateHelper::CoordinateInfo a(
                relative_fromX, relative_fromY,
                relative_toX, relative_toY
    );
    setDrawRegion(tag, a);
}

void LightningEngine::setDrawRegion(QString tag, CoordinateHelper::CoordinateInfo &absoluteCoordinates) {
    commandList->addCommand(state_relative_coordinates);
    commandList->addQString(tag);
    commandList->addAbsoluteCoordinates(absoluteCoordinates);
}
