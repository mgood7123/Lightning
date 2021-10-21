#include "LightningWindowOpenGL.h"

#include <QOpenGLFramebufferObject>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QPainterPath>

LightningWindowOpenGL::LightningWindowOpenGL(QOpenGLWindow::UpdateBehavior updateBehavior) :
    QOpenGLWindow(updateBehavior),
    commandList(new LightningCommandList),
    engine()
{
    windowData = new LightningWindowData();
    windowData->window = this;
    windowData->screen = screen();
    windowData->dpiScale = devicePixelRatio();

    QSurfaceFormat fmt;
    fmt.setAlphaBufferSize(8);
    fmt.setRedBufferSize(8);
    fmt.setGreenBufferSize(8);
    fmt.setBlueBufferSize(8);
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);

    // Request OpenGL 4.3 core or OpenGL ES 3.2
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 4.3 core context");
        fmt.setVersion(4, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug("Requesting 3.2 context");
        fmt.setVersion(3, 2);
    }

    // request debug context
//    fmt.setOption(QSurfaceFormat::DebugContext);

    setFormat(fmt);
}

LightningWindowOpenGL::~LightningWindowOpenGL()
{
    makeCurrent();
    delete contentView;
    delete logger;
    delete windowData;
    delete commandList;
}

void LightningWindowOpenGL::setContentView(Lightning_View *view) {
    Lightning_View::LayoutParams * l = view->getLayoutParams();
    setContentView(view,  l != nullptr ? l : makeLayoutParams_MATCH_PARENT<Lightning_View::LayoutParams>());
}

void LightningWindowOpenGL::setContentView(Lightning_View *view, Lightning_View::LayoutParams * params) {
    if (contentView != nullptr) {
        contentView->onRemovedFromLayout();
    }
    delete contentView;
    contentView = view;
    if (contentView != nullptr) {
        contentView->setLayoutParams(params);
        contentView->setWindowData(windowData);
        contentView->onAddedToLayout();
    }
}

void LightningWindowOpenGL::handleLoggedMessage(const QOpenGLDebugMessage& debugMessage) {
    qDebug() << debugMessage;
}

void LightningWindowOpenGL::initializeGL()
{
    qDebug() << "Vendor graphic card:" << (const char *) glGetString(GL_VENDOR);
    qDebug() << "Renderer:" << (const char *) glGetString(GL_RENDERER);
    qDebug() << "Version GL:" << (const char *) glGetString(GL_VERSION);
    qDebug() << "Version GLSL:" << (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);

    if (context()->hasExtension("GL_KHR_debug")) {
        logger = new QOpenGLDebugLogger(this);
        connect(logger, &QOpenGLDebugLogger::messageLogged, this, &LightningWindowOpenGL::handleLoggedMessage, Qt::DirectConnection);
        if (logger->initialize()) {
            logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
        } else {
            qWarning("OpenGL context supports GL_KHR_debug extension but initialization of QOpenGLDebugLogger has failed");
        }
    } else {
        qWarning("OpenGL context does not support GL_KHR_debug extension");
    }

    timer.setTimerType(Qt::PreciseTimer);
    timer.connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(8);
}

void LightningWindowOpenGL::resizeGL(int w, int h)
{
    qDebug() << "resizing system to" << windowData->applyDpiScale(w) << "," << windowData->applyDpiScale(h);
    if (contentView != nullptr) {
        contentView->resize(windowData->applyDpiScale(w), windowData->applyDpiScale(h));
    }
}

void LightningWindowOpenGL::paintGL()
{
    // erase generated command list
    commandList->eraseAllData();

    // erase data in case we allocate an unusually large amount of data for a period of time
    // and then allocate a small amount of data for a longer duration of time
    // we could cache this but then results in the a situation similar to the MSAA GPU Caching problem
    // where it cache's 1.2 GB of memory for 3 copies of 400 MB MSAA textures of data
    // instead of caching 300 MB of memory for 3 copies of 100 MB non MSAA textures of data
    // if a 400 MB MSAA texture must be used then it is better not to cache this and just rebuild each
    // frame to drastically save memory (but we cannot control GPU caching)
    //
    // in reality this might happen if we have a very large amount of vertex data each frame
    // this might also happen if we have extremely unoptimized code where someone might try to draw everything
    // pixel by pixel for whatever reason
    engine.getCommandList()->eraseAllData();
    // remove all children so that we do not produce duplicates
    engine.eraseChildren();

    // clear to black
    float w = windowData->applyDpiScaleF(width());
    float h = windowData->applyDpiScaleF(height());
    // have at least one root
    auto *r = engine.subEngine("<root>", 0, 0, w, h);

    bool render = true;
    if (!render) {
        r->scissor(0, 0, w, h/2);
        r->setClearColor(1, 0, 1, 1);
        r->clear(LightningEngine::COLOR_BIT);
    } else {
        if (contentView != nullptr) {
            contentView->buildCoordinates({0, 0, w, h});
            contentView->onMeasure(w, h);
            contentView->draw(*r);
        }
    }

    optimize();
    draw();
    printDebugLog = false;
}

LightningWindowOpenGL::RegionTree LightningWindowOpenGL::generateRegionTree(LightningEngine * source, RegionTree * parent) {
    RegionTree currentTree;
    currentTree.parent = parent;
    LightningCommandList * current = source->getCommandList();
    auto children = source->getChildren();
    if (!current->commands.empty()) {
        // in order to optimize the command list, we must emulate what it would do
        auto commands = current->commands_Iterator();
        auto coords = current->absolute_coordinates_Iterator();
        auto dataBool = current->bool_Iterator(); // just in case
        auto dataFloat = current->float_Iterator();
        auto dataInt = current->int_Iterator();
        auto dataQString = current->qstring_Iterator();
        auto dataQUnsignedInt64 = current->quint64_Iterator();

        Cache regionCache;

        // throw if the first command is not a region
        auto cmd = commands.next();
        if (cmd != LightningEngine::Commands::state_relative_coordinates) {
            qFatal("first command must be a region, please call engine.subEngine(fromX, fromY, toX, toY)");
        }
        auto c = coords.next();
        currentTree.tag = dataQString.next();
        currentTree.object.lightningCommandList.addCommand(LightningEngine::TAG);
        currentTree.object.lightningCommandList.addQString(currentTree.tag);
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", absolute region:" << c;
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", assigning new region command list";
        currentTree.object.left = c.topLeftX;
        currentTree.object.top = c.topLeftY;
        currentTree.object.right = c.bottomRightX;
        currentTree.object.bottom = c.bottomRightY;

        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", created region:" << currentTree.object.qRectF();

        // round x and y to the lower pixel
        int scissor_x = std::floor(currentTree.object.left);
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", currentTree.object.left:" << currentTree.object.left << ", scissor_x:" << scissor_x;

        int scissor_y = std::floor(currentTree.object.top);
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", currentTree.object.top:" << currentTree.object.top << ", scissor_y:" << scissor_y;

        // round width and height to the upper pixel
        int scissor_width = std::ceil(currentTree.object.right - currentTree.object.left);
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", currentTree.object.right:" << currentTree.object.right << ", scissor_width:" << scissor_width;

        int scissor_height = std::ceil(currentTree.object.bottom - currentTree.object.top);
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", currentTree.object.bottom:" << currentTree.object.bottom << ", scissor_height:" << scissor_height;

        // always add scissor command
        currentTree.object.lightningCommandList.addCommand(LightningEngine::state_scissor);
        currentTree.object.lightningCommandList.addInt({scissor_x, scissor_y, scissor_width, scissor_height});
        // return if region is invalid
        if (scissor_width <= 0 || scissor_height <= 0) {
            currentTree.object.isOccluded = true;
            if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", occluding current tree because region is invalid";
            return currentTree;
        }

        // iterate
        // also determine what regions are opaque so we dont have to iterate twice
        while (commands.hasNext()) {
            auto cmd = commands.next();
            switch (cmd) {
            case LightningEngine::Commands::TAG: {
                qFatal("internal error: more than one TAG defined");
                break;
            }
            // defining a region inside a region is an error
            // as this should produce a child command list
            case LightningEngine::Commands::state_relative_coordinates: {
                qFatal("internal error: more than one drawing region defined");
                break;
            }

            // merge duplicate commands inside of regions

            // painter API
            case LightningEngine::Commands::state_painter_SetHint: {
                regionCache.hint = dataInt.next();
                regionCache.hintEnabled = dataBool.next();
                if (regionCache.hint.changed() || regionCache.hintEnabled.changed()) {
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addInt(regionCache.hint.get());
                    currentTree.object.lightningCommandList.addBool(regionCache.hintEnabled.get());
                }
                break;
            }
            case LightningEngine::Commands::state_painter_SetPixelSize: {
                regionCache.pixelSize = dataInt.next();
                if (regionCache.pixelSize.changed()) {
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addInt(regionCache.pixelSize.get());
                }
                break;
            }
            case LightningEngine::Commands::state_painter_SetPenColor: {
                float r = dataFloat.next();
                float g = dataFloat.next();
                float b = dataFloat.next();
                float alpha = dataFloat.next();
                regionCache.penColor = {r, g, b, alpha};
                if (regionCache.penColor.changed()) {
                    if (alpha != 1.0) currentTree.object.isTransparent = true;
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addFloat(regionCache.penColor.get());
                }
                break;
            }
            case LightningEngine::Commands::draw_painter_DrawText: {
                QList data = {
                    currentTree.object.left + dataFloat.next(),
                    currentTree.object.top + dataFloat.next(),
                    currentTree.object.left + dataFloat.next(),
                    currentTree.object.top + dataFloat.next()
                };

                regionCache.painterTextRect = {
                    data[0], data[1], data[2] - data[0], data[3] - data[1]
                };
                regionCache.painterTextFlags = dataInt.next();
                regionCache.painterText = dataQString.next();
                if (regionCache.painterTextRect.changed() || regionCache.painterTextFlags.changed() || regionCache.painterText.changed()) {
                    // text is transparent by default
                    currentTree.object.isTransparent = true;
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addFloat(regionCache.painterTextRect.get());
                    currentTree.object.lightningCommandList.addInt(regionCache.painterTextFlags.get());
                    currentTree.object.lightningCommandList.addQString(regionCache.painterText.get());
                }
                break;
            }
            case LightningEngine::Commands::state_painter_SetClipPath: {
                regionCache.painterSetClipPath_path_id = dataQUnsignedInt64.next();
                if (regionCache.painterSetClipPath_path_id.changed()) {
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addQUnsignedInt64(regionCache.painterSetClipPath_path_id.get());
                }
                break;
            }
            case LightningEngine::Commands::draw_painter_FillPath: {
                regionCache.painterFillPath_path_id = dataQUnsignedInt64.next();
                float r = dataFloat.next();
                float g = dataFloat.next();
                float b = dataFloat.next();
                float alpha = dataFloat.next();
                regionCache.painterFillPath_color = {r, g, b, alpha};
                if (
                        regionCache.painterFillPath_path_id.changed() ||
                        regionCache.painterFillPath_color.changed()
                ) {
                    if (alpha != 1.0) currentTree.object.isTransparent = true;
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addQUnsignedInt64(regionCache.painterFillPath_path_id.get());
                    currentTree.object.lightningCommandList.addFloat(regionCache.painterFillPath_color.get());
                }
                break;
            }

            // painterPath API

            case LightningEngine::Commands::state_painterPath_create: {
                // never merge this
                currentTree.object.lightningCommandList.addCommand(cmd);
                currentTree.object.lightningCommandList.addQUnsignedInt64(dataQUnsignedInt64.next());
                break;
            }
            case LightningEngine::Commands::draw_painterPath_DrawRoundedRect: {
                regionCache.painterPathDrawRoundedRect_path_id = dataQUnsignedInt64.next();
                QList data = {
                    currentTree.object.left + dataFloat.next(),
                    currentTree.object.top + dataFloat.next(),
                    currentTree.object.left + dataFloat.next(),
                    currentTree.object.top + dataFloat.next()
                };

                regionCache.painterPathDrawRoundedRect_Rect = {
                    data[0], data[1], data[2] - data[0], data[3] - data[1]
                };

                regionCache.painterPathDrawRoundedRect_radius_x = dataFloat.next();
                regionCache.painterPathDrawRoundedRect_radius_y = dataFloat.next();
                if (
                        regionCache.painterPathDrawRoundedRect_path_id.changed() ||
                        regionCache.painterPathDrawRoundedRect_Rect.changed() ||
                        regionCache.painterPathDrawRoundedRect_radius_x.changed() ||
                        regionCache.painterPathDrawRoundedRect_radius_y.changed()
                ) {
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addQUnsignedInt64(regionCache.painterPathDrawRoundedRect_path_id.get());
                    currentTree.object.lightningCommandList.addFloat(regionCache.painterPathDrawRoundedRect_Rect.get());
                    currentTree.object.lightningCommandList.addFloat(regionCache.painterPathDrawRoundedRect_radius_x.get());
                    currentTree.object.lightningCommandList.addFloat(regionCache.painterPathDrawRoundedRect_radius_y.get());
                }
                break;
            }
            case LightningEngine::Commands::state_painterPath_destroy: {
                // never merge this
                currentTree.object.lightningCommandList.addCommand(cmd);
                currentTree.object.lightningCommandList.addQUnsignedInt64(dataQUnsignedInt64.next());
                break;
            }

            // native API
            case LightningEngine::Commands::state_setClearColor: {
                float r = dataFloat.next();
                float g = dataFloat.next();
                float b = dataFloat.next();
                float alpha = dataFloat.next();
                regionCache.clearColor = {r, g, b, alpha};
                if (regionCache.clearColor.changed()) {
                    if (alpha != 1.0) currentTree.object.isTransparent = true;
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addFloat(regionCache.clearColor.get());
                }
                break;
            }
            case LightningEngine::Commands::draw_clear: {
                regionCache.clearBit = dataInt.next();
                if (regionCache.clearBit.changed()) {
                    currentTree.object.lightningCommandList.addCommand(cmd);
                    currentTree.object.lightningCommandList.addInt(regionCache.clearBit.get());
                }
                break;
            }
            case LightningEngine::Commands::state_scissor: {
                auto sx = dataInt.next();
                auto sy = dataInt.next();
                auto sw = dataInt.next();
                auto sh = dataInt.next();
                auto right = (scissor_x + scissor_width);
                auto bottom = (scissor_y + scissor_height);
                if (sx < 0) sx = 0;
                if (sy < 0) sy = 0;
                if (sw > right) sw = right;
                if (sh > bottom) sh = bottom;
                auto x = scissor_x + sx;
                auto y = scissor_y + sy;
                auto w = scissor_x + sw;
                auto h = scissor_y + sh;
                currentTree.object.lightningCommandList.addCommand(cmd);
                currentTree.object.lightningCommandList.addInt({x, y, w, h});
                break;
            }

            default:
                source->fatal(QString("Unhandled command: ") + QString::number(cmd));
                break;
            };
        }

        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", created new region";
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", region is transparent:" << currentTree.object.isTransparent;
        for (LightningEngine * child : children) {
            if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", generating child tree";
            RegionTree childTree = generateRegionTree(child, &currentTree);
            if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", determining clipping and occlusion by processing the current tree and the generated child tree";

            // bottom up here

            // occlude

            // current is occluded if item not transparent
            if (!childTree.object.isTransparent) {
                // current is occluded if item completely covers current
                if (childTree.object.completelyCovers(currentTree.object)) {
                    currentTree.object.isOccluded = true;
                    if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", occluding current tree because child tree is not transparent and child tree completely covers current tree";
                }
            }

            // clip

            // An item is clipped (occluded) if it is outside any parent in the parent chain
            if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", determining occlusion...";
            for (auto * parent = childTree.parent; parent != nullptr; parent = parent->parent) {
                if (!childTree.object.collisionTest(parent->object)) {
                    if (printDebugLog) qDebug() << childTree.object.qRectF() << "(TAG:" << childTree.tag << ") is not inside of" << parent->object.qRectF() << "(TAG:" << parent->tag << ")";
                    childTree.object.isOccluded = true;
                    if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", occluding child tree because child tree is outside of a parent";
                    break;
                } else {
                    if (printDebugLog) qDebug() << childTree.object.qRectF() << "(TAG:" << childTree.tag << ") is inside of" << parent->object.qRectF() << "(TAG:" << parent->tag << ")";
                }
            }
            if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", determed occlusion," << childTree.object.qRectF() << "(TAG:" << childTree.tag << ") is" << (childTree.object.isOccluded ? "invisible" : "visible");

            // append stores a copy of childTree
            currentTree.children.append(childTree);
        }
    } else {
        if (children.empty()) {
            if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", current leaf has no commands and no children";
            return currentTree;
        }
        if (printDebugLog) qDebug() << "TAG:" << currentTree.tag << ", current leaf has no commands however it does have children";
        for (LightningEngine * child : children) {
            if (parent != nullptr) {
                return generateRegionTree(child, &currentTree);
            } else {
                if (children.size() != 1) {
                    qFatal("error: engine has multiple root nodes");
                }
                return generateRegionTree(child, nullptr);
            }
        }
    }
    return currentTree;
}

LightningCommandList LightningWindowOpenGL::flattenTree(const RegionTree & tree) {
    LightningCommandList flattened;
    if (printDebugLog) qDebug() << "TAG: " << tree.tag << ", tree.object is occluded and should not be drawn:" << tree.object.isOccluded;
    // if this tree is occluded then its children might not be
    if (!tree.object.isOccluded) {
        auto & cmd = tree.object.lightningCommandList;
        auto size = cmd.commands.size();
        if (printDebugLog) qDebug().nospace() << "TAG: " << tree.tag << ", adding " << size << " command" << (size == 1 ? '\0' : 's');
        flattened.addCommandList(cmd);
    }
    for (const RegionTree & regionTree : tree.children) {
        flattened.addCommandList(flattenTree(regionTree));
    }
    return flattened;
}

void LightningWindowOpenGL::optimize() {
    if (engine.commandListSize() == 0 && engine.getChildren().size() == 0) return;

    RegionTree tree = generateRegionTree(&engine);

    // flatten regions to command list

    LightningCommandList flattened = flattenTree(tree);

    // final command list to send to draw state which get translated to GPU calls

    commandList->eraseAllData();
    commandList->addCommandList(flattened);
}

void LightningWindowOpenGL::draw() {
    if (commandList == nullptr) return;
    auto size = commandList->commands.size();
    if (size == 0) return;

    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setSamples(8);
    QOpenGLFramebufferObject fbo(width(), height(), fboFormat);
    fbo.bind();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (printDebugLog) qDebug() << "// setup";
    if (printDebugLog) qDebug() << "QOpenGLPaintDevice x(width(), height());";
    QOpenGLPaintDevice x(width(), height());
    if (printDebugLog) qDebug() << "x.setPaintFlipped(true);";
    x.setPaintFlipped(true);
    if (printDebugLog) qDebug() << "QPainter painter(&x);";
    QPainter painter(&x);

    if (printDebugLog) qDebug() << "auto font = painter.font();";
    auto font = painter.font();
    if (printDebugLog) qDebug() << "auto pen = painter.pen();";
    auto pen = painter.pen();

    // get iterators
    auto commands = commandList->commands_Iterator();
    auto coords = commandList->absolute_coordinates_Iterator();
    auto dataBool = commandList->bool_Iterator(); // just in case
    auto dataFloat = commandList->float_Iterator();
    auto dataInt = commandList->int_Iterator();
    auto dataQString = commandList->qstring_Iterator();
    auto dataQUnsignedInt64 = commandList->quint64_Iterator();

    if (printDebugLog) qDebug() << "QColor clearColor;";
    QColor clearColor;
    QMap<quint64, void*> pointer_id_map;

    QList<int> clipRegion;

    #define GET_POINTER(type, id, name, error_message) type * name = reinterpret_cast<type *>(pointer_id_map.value(id, nullptr)); if (name == nullptr) LightningEngine::fatal(error_message)

    if (printDebugLog) qDebug() << "// draw";
    if (printDebugLog) qDebug().nospace() << "processing " << size << " command" << (size == 1 ? '\0' : 's');

    QString currentTag;

    // iterate
    while (commands.hasNext()) {
        auto cmd = commands.next();
        switch (cmd) {
        case LightningEngine::Commands::TAG: {
            currentTag = dataQString.next();
            break;
        }
        // painter API
        case LightningEngine::Commands::state_painter_SetHint: {
            int hint = dataInt.next();
            bool enabled = dataBool.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ painter.setRenderHint(QPainter::RenderHint(" << hint << "), " << enabled << ");";
            painter.setRenderHint(QPainter::RenderHint(hint), enabled);
            break;
        }
        case LightningEngine::Commands::state_painter_SetPixelSize: {
            int pixelSize = dataInt.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ font.setPixelSize(" << pixelSize << ");";
            font.setPixelSize(pixelSize);
            if (printDebugLog) qDebug() << "painter.setFont(font);";
            painter.setFont(font);
            break;
        }
        case LightningEngine::Commands::state_painter_SetPenColor: {
            float r = dataFloat.next();
            float g = dataFloat.next();
            float b = dataFloat.next();
            float a = dataFloat.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ pen.setColor(QColor::fromRgbF(" << r << ", " << g << ", " << b << ", " << a << "));";
            pen.setColor(QColor::fromRgbF(r, g, b, a));
            if (printDebugLog) qDebug() << "painter.setPen(pen);";
            painter.setPen(pen);
            break;
        }
        case LightningEngine::Commands::draw_painter_DrawText: {
            float x = dataFloat.next();
            float y = dataFloat.next();
            float w = dataFloat.next();
            float h = dataFloat.next();
            int flags = dataInt.next();
            QString text = dataQString.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ painter.drawText(QRectF(" << x << ", " << y << ", " << w << ", " << h << "), " << flags << ", " << text << ");";
            painter.drawText(QRectF(x, y, w, h), flags, text);
            break;
        }
        case LightningEngine::Commands::state_painter_SetClipPath: {
            auto id = dataQUnsignedInt64.next();
            GET_POINTER(QPainterPath, id, path, "path is nullptr, did you forget to call `painterPath_create()` ?");
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ painter.setClipPath(*path_" << id << ");";
            painter.setClipPath(*path);
            break;
        }
        case LightningEngine::Commands::draw_painter_FillPath: {
            auto id = dataQUnsignedInt64.next();
            GET_POINTER(QPainterPath, id, path, "path is nullptr, did you forget to call `painterPath_create()` ?");
            float r = dataFloat.next();
            float g = dataFloat.next();
            float b = dataFloat.next();
            float a = dataFloat.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ painter.fillPath(*path_" << id << ", QColor::fromRgbF(" << r << ", " << g << ", " << b << ", " << a << "));";
            painter.fillPath(*path, QColor::fromRgbF(r, g, b, a));
            break;
        }
        // painterPath API
        case LightningEngine::Commands::state_painterPath_create: {
            auto id = dataQUnsignedInt64.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ QPainterPath * path_" << id << " = new QPainterPath();";
            pointer_id_map.insert(id, new QPainterPath());
            break;
        }
        case LightningEngine::Commands::draw_painterPath_DrawRoundedRect: {
            auto id = dataQUnsignedInt64.next();
            GET_POINTER(QPainterPath, id, path, "path is nullptr, did you forget to call `painterPath_create()` ?");
            float x = dataFloat.next();
            float y = dataFloat.next();
            float w = dataFloat.next();
            float h = dataFloat.next();
            float rx = dataFloat.next();
            float ry = dataFloat.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ path_" << id << "->addRoundedRect(" << x << ", " << y << ", " << w << ", " << h << ", " << rx << ", " << ry << ");";
            path->addRoundedRect(x, y, w, h, rx, ry);
            break;
        }
        case LightningEngine::Commands::state_painterPath_destroy: {
            auto id = dataQUnsignedInt64.next();
            GET_POINTER(QPainterPath, id, path, "path is nullptr, did you forget to call `painterPath_create()` ?");
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ delete path_" << id << ";";
            delete path;
            break;
        }
        // native API
        case LightningEngine::Commands::state_setClearColor: {
            float r = dataFloat.next();
            float g = dataFloat.next();
            float b = dataFloat.next();
            float a = dataFloat.next();
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ clearColor.setRgbF(" << r << ", " << g << ", " << b << ", " << a << ");";
            clearColor.setRgbF(r, g, b, a);
            break;
        }
        case LightningEngine::Commands::draw_clear: {
            int bit = dataInt.next();
            switch(bit) {
            case LightningEngine::BIT::COLOR_BIT: {
                if (printDebugLog) qDebug() << "QPainterPath path;";
                QPainterPath path;
                if (printDebugLog) qDebug() << "path.addRect(painter.window());";
                path.addRect(painter.window());
                if (printDebugLog) qDebug() << "painter.fillPath(path, clearColor);";
                painter.fillPath(path, clearColor);
                break;
            }
            default:
                engine.fatal(QString("Unknown bit: ") + QString::number(bit));
                break;
            }
            break;
        }
        // window and region
        case LightningEngine::Commands::state_scissor: {
            clipRegion = {dataInt.next(), dataInt.next(), dataInt.next(), dataInt.next()};
            if (printDebugLog) qDebug().nospace() << "/* TAG: " << currentTag << " */ painter.setClipRect(" << clipRegion[0] << ", " << clipRegion[1] << ", " << clipRegion[2] << ", " << clipRegion[3] <<");";
            painter.setClipRect(clipRegion[0], clipRegion[1], clipRegion[2], clipRegion[3]);
            break;
        }
        case LightningEngine::Commands::state_relative_coordinates: {
            engine.fatal("state_relative_coordinates should not be in final list");
            break;
        }
        default:
            engine.fatal(QString("Unhandled command: ") + QString::number(cmd));
            break;
        };
    }

    if (printDebugLog) qDebug().nospace() << "processed " << size << " command" << (size == 1 ? '\0' : 's');

    #undef GET_POINTER
    if (printDebugLog) qDebug() << "painter.end();";
    painter.end();

    bool flip = true;

    if (flip) {
        QOpenGLFramebufferObject::blitFramebuffer(
                    0, {0, 0, fbo.width(), fbo.height()},
                    &fbo, {0, fbo.height(), fbo.width(), -fbo.height()});
    } else {
        QOpenGLFramebufferObject::blitFramebuffer(
                    0, {0, 0, fbo.width(), fbo.height()},
                    &fbo, {0, 0, fbo.width(), fbo.height()});
    }
}
