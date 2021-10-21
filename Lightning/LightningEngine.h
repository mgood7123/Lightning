#ifndef LIGHTNINGENGINE_H
#define LIGHTNINGENGINE_H

#include "LightningCommandList.h"
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QColor>
#include <cxxabi.h>

class LightningEngine {
    QList<LightningEngine*> children;
    LightningCommandList * commandList;
    bool commandListAttached = false;

    quint64 next_id = 0;
public:

    template <typename T>
    static QString className(const T & name) {
        int status;
        char * demangled = abi::__cxa_demangle(typeid(name).name(),0,0,&status);
        QString result(demangled);
        free(demangled);
        return result;
    }

    template <typename T>
    static QString className(const T *& name) {
        int status;
        char * demangled = abi::__cxa_demangle(typeid(*name).name(),0,0,&status);
        QString result(demangled);
        free(demangled);
        return result;
    }

    template <typename T>
    class Cache {
        T value;
        bool first = false;
        bool will_change = false;
    public:

        Cache<T> & operator=(const T & value) {
            set(value);
            return *this;
        }

        void set(const T & value) {
            if (first) {
                will_change = this->value != value;
            } else {
                will_change = true;
            }
            this->value = value;
            first = true;
        }

        bool changed() {
            return will_change;
        }

        const T & get() {
            return value;
        }
    };

    typedef Cache<bool> Cache_Bool;
    typedef Cache<int> Cache_Int;
    typedef Cache<float> Cache_Float;
    typedef Cache<QString> Cache_QString;
    typedef Cache<quint64> Cache_UInt64;
    typedef Cache<QList<bool>> Cache_BoolArray;
    typedef Cache<QList<int>> Cache_IntArray;
    typedef Cache<QList<float>> Cache_FloatArray;
    typedef Cache<QList<QString>> Cache_QStringArray;
    typedef Cache<QList<quint64>> Cache_UInt64Array;

    LightningEngine();
    LightningEngine(LightningCommandList * list);
    ~LightningEngine();

    LightningCommandList * getCommandList();
    QList<LightningEngine *> & getChildren();

    static void fatal(const char * string);
    static void fatal(const QString & string);

    qsizetype commandListSize();

    LightningEngine * subEngine(QString tag, float relative_fromX, float relative_fromY, float relative_toX, float relative_toY);
    LightningEngine * subEngine(QString tag, CoordinateHelper::CoordinateInfo & absoluteCoordinates);

    void eraseChildren();

    enum BIT {
        COLOR_BIT
    };

    enum Commands {
        TAG,

        // painter API
        state_painter_SetPixelSize,
        state_painter_SetPenColor,
        state_painter_SetHint,
        state_painter_SetClipPath,
        draw_painter_DrawText,
        draw_painter_FillPath,

        // painterPath API
        state_painterPath_create,
        draw_painterPath_DrawRoundedRect,
        state_painterPath_destroy,

        // native API
        state_setClearColor,
        draw_clear,

        // region and window
        state_scissor,
        state_relative_coordinates,
    };

    // painter API
    void painter_SetPixelSize(int pixelSize);
    void painter_SetPenColor(float r, float g, float b, float a);
    void painter_SetPenColor(QColor color);
    void painter_DrawText(const QRect &rectangle, int flags, const QString &text);
    void painter_DrawText(const QRectF &rectangle, int flags, const QString &text);
    void painter_SetHint(QPainter::RenderHint hint, bool enabled);
    void painter_SetClipPath(quint64 path_id);
    void painter_FillPath(quint64 path_id, float r, float g, float b, float a);
    void painter_FillPath(quint64 path_id, QColor color);

    // painterPath API
    quint64 painterPath_create();
    void painterPath_DrawRoundedRect(quint64 path_id, const QRectF &size, float rx, float ry);
    void painterPath_destroy(quint64 path_id);

    // native API
    void setClearColor(const QColor & color);
    void setClearColor(float r, float g, float b, float a);
    void clear(LightningEngine::BIT bit);

    // window and region
    void scissor(const QRect &rectangle);
    void scissor(int x, int y, int width, int height);
private:
    // subEngine
    void setDrawRegion(QString tag, float relative_fromX, float relative_fromY, float relative_toX, float relative_toY);
    void setDrawRegion(QString tag, CoordinateHelper::CoordinateInfo &absoluteCoordinates);
};

#endif // LIGHTNINGENGINE_H
