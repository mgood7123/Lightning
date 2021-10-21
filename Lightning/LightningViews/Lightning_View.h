#ifndef LIGHTNING_VIEW_H
#define LIGHTNING_VIEW_H


#include "../Helpers/CoordinateHelper.h"
#include "../Helpers/PropertyAnimatorHelper.h"
#include "../Helpers/WindowDataHelper.h"

#include "../Tools/PixelToNDC.h"
#include "../Tools/chronotimer.h"

#include "../LightningEngine.h"

#define LIGHTNING_CLASS_NAME inline virtual QString className() override { \
    QString tag = getTag(); \
    return LightningEngine::className(this) + (tag.size() == 0 ? "" : " [" + tag + "]"); \
}

class Lightning_View :
        public CoordinateHelper,
        public WindowDataHelper
{
public:
    inline virtual QString className() {
        QString tag = getTag();
        return LightningEngine::className(this) + (tag.size() == 0 ? "" : " [" + tag + "]");
    }
    Lightning_View();
    virtual ~Lightning_View();
    virtual void resize(float w, float h);
    virtual void onDraw(LightningEngine & engine);
    virtual void draw(LightningEngine & engine);
    enum VISIBILITY {
        VISIBLE, INVISIBLE, GONE
    };

    // TODO: consider using custom class with explicit math_parent and wrap_content flags
    static float MATCH_PARENT;
    static float WRAP_CONTENT;

    class LayoutParams {
    public:
        enum GRAVITY {
            GRAVITY_NONE,
            GRAVITY_LEFT,
            GRAVITY_CENTER,
            GRAVITY_RIGHT
        };

        float width = MATCH_PARENT;
        float height = MATCH_PARENT;
        GRAVITY gravity = GRAVITY_NONE;

        LayoutParams(float width, float height);

        template <typename T>
        T * castToType() {
            return dynamic_cast<T*>(this);
        }

        template <typename T>
        static T * castToType(LayoutParams & params) {
            return dynamic_cast<T*>(&params);
        }

        template <typename T>
        static T * castToType(LayoutParams * params) {
            return dynamic_cast<T*>(params);
        }

        virtual ~LayoutParams() = default;
    };
private:
    QSizeF measuredDimensions {0, 0};
    bool calledSetMeasuredDimensions = false;
    static constexpr const char * NO_TAG = "";
    QString tag = NO_TAG;
    bool canDraw = false;
    VISIBILITY visibility = VISIBLE;
    bool invalidated = true;
    int fboWidth = 0, fboHeight = 0;
    LayoutParams * layoutParams = nullptr;
    bool alwaysDraw = false;
public:
    void setAlwaysDraw(bool enabled);

    bool getAlwaysDraw() const;

    void invalidate();

    void setTag(const QString & name);
    QString getTag() const;

    PixelToNDC pixelToNDC;
    Lightning_View * parent = nullptr;

    template <typename T>
    T * castToType() {
        return dynamic_cast<T*>(this);
    }

    template <typename T>
    static T * castToType(Lightning_View & view) {
        return dynamic_cast<T*>(&view);
    }

    template <typename T>
    static T * castToType(Lightning_View * view) {
        return dynamic_cast<T*>(view);
    }

    LayoutParams *getLayoutParams() const;

    void setLayoutParams(LayoutParams *params);

    void setMeasuredDimensions(float width, float height);
    void setMeasuredDimensions(const QSizeF & size);
    QSizeF getMeasuredDimensions();
    float getMeasuredWidth();
    float getMeasuredHeight();

    void measure(float width, float height);
    virtual void onMeasure(float width, float height);

    void buildCoordinates(const QRectF & relativeCoordinates);

    virtual void onAddedToLayout();
    virtual void onRemovedFromLayout();

    virtual bool isLayout() const;

    template<typename T> bool isPowerOf2(T const x) { return x && !( x & (x - 1) ); };

    static
    constexpr int floatToPixel(float pixel);;

    VISIBILITY getVisibility() const;
    void setVisibility(VISIBILITY newVisibility);
};

template<typename T>
T * makeLayoutParams_MATCH_PARENT() {
    return new T(Lightning_View::MATCH_PARENT, Lightning_View::MATCH_PARENT);
}

template<typename T>
T * makeLayoutParams_WRAP_CONTENT() {
    return new T(Lightning_View::WRAP_CONTENT, Lightning_View::WRAP_CONTENT);
}

template<typename T>
T * makeLayoutParams_MATCH_PARENT_WRAP_CONTENT() {
    return new T(Lightning_View::MATCH_PARENT, Lightning_View::WRAP_CONTENT);
}

template<typename T>
T * makeLayoutParams_WRAP_CONTENT_MATCH_PARENT() {
    return new T(Lightning_View::WRAP_CONTENT, Lightning_View::MATCH_PARENT);
}

#endif // LIGHTNING_VIEW_H
