#ifndef LIGHTNINGWINDOWOPENGL_H
#define LIGHTNINGWINDOWOPENGL_H

#include <QOpenGLDebugLogger>
#include <QOpenGLWindow>
#include <QTimer>
#include "Lightning.h"

class LightningWindowOpenGL : public QOpenGLWindow
{
    Q_OBJECT
    QTimer timer;
    LightningCommandList * commandList;
    LightningEngine engine;
    LightningWindowData * windowData = nullptr;
    Lightning_View * contentView = nullptr;

    QOpenGLDebugLogger *logger = nullptr;
    void handleLoggedMessage(const QOpenGLDebugMessage& debugMessage);

    // state tracking
    class Cache {
    public:
        // painter API
        LightningEngine::Cache_Int hint;
        LightningEngine::Cache_Bool hintEnabled;
        LightningEngine::Cache_FloatArray penColor;
        LightningEngine::Cache_Int pixelSize;
        LightningEngine::Cache_FloatArray painterTextRect;
        LightningEngine::Cache_Int painterTextFlags;
        LightningEngine::Cache_QString painterText;
        LightningEngine::Cache_UInt64 painterSetClipPath_path_id;
        LightningEngine::Cache_UInt64 painterFillPath_path_id;
        LightningEngine::Cache_FloatArray painterFillPath_color;

        // painter path API
        LightningEngine::Cache_UInt64 path_id;
        LightningEngine::Cache_UInt64 painterPathDrawRoundedRect_path_id;
        LightningEngine::Cache_FloatArray painterPathDrawRoundedRect_Rect;
        LightningEngine::Cache_Float painterPathDrawRoundedRect_radius_x;
        LightningEngine::Cache_Float painterPathDrawRoundedRect_radius_y;

        // native API
        LightningEngine::Cache_Int clearBit;
        LightningEngine::Cache_FloatArray clearColor;
    };

    // transform engine into a heirarchy

    struct RegionInfo {
        LightningCommandList lightningCommandList;
        bool isTransparent = false;
        bool isOccluded = false;
        float left = 0;
        float top = 0;
        float right = 0;
        float bottom = 0;

        QRectF qRectF() {
            return {left, top, right, bottom};
        }

        bool collisionTest(const RegionInfo & parent) const {
            return (right >= parent.left) && (left < parent.right) &&
                   (bottom >= parent.top) && (top < parent.bottom);
        }

        bool completelyCovers(const RegionInfo & parent) const {
            return (left <= parent.left) && (top <= parent.top) &&
                   (right >= parent.right) && (bottom >= parent.bottom);
        }
    };

    struct RegionTree {
        QString tag;
        RegionInfo object;
        RegionTree * parent = nullptr;
        QList<RegionTree> children;
    };

public:
    bool printDebugLog = false;

    LightningWindowOpenGL(UpdateBehavior updateBehavior = NoPartialUpdate);

    ~LightningWindowOpenGL();

    void setContentView(Lightning_View * view);
    void setContentView(Lightning_View * view, Lightning_View::LayoutParams * params);

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    RegionTree generateRegionTree(LightningEngine *source, RegionTree *parent = nullptr);
    LightningCommandList flattenTree(const RegionTree &tree);
    void optimize();
    void draw();
};


#endif // LIGHTNINGWINDOWOPENGL_H
