#ifndef TYPESAFEPROPERTYANIMATORS_H
#define TYPESAFEPROPERTYANIMATORS_H

#define PROPERTY_ANIMATOR_N(N, T) class PropertyAnimator_##N : public QVariantAnimation \
{ \
    Q_OBJECT \
    std::function<void(const T &)> setter; \
public: \
    typedef QPair<qreal, T> KeyValue; \
    typedef QList<KeyValue> KeyValues; \
    PropertyAnimator_##N (std::function<void(const T &)> setter) { \
        this->setter = setter; \
    } \
\
    KeyValues keyValuesConstructor() const { \
        return KeyValues(); \
    } \
\
    void setStartValue(const T & value) { \
        QVariantAnimation::setStartValue(value); \
    } \
\
    void setEndValue(const T & value) { \
        QVariantAnimation::setEndValue(value); \
    } \
\
    void setKeyValueAt(qreal step, const T &value) { \
        QVariantAnimation::setKeyValueAt(step, value); \
    } \
\
    void setKeyValues(const KeyValues &keyValues) { \
        QVariantAnimation::KeyValues keys; \
        keys.reserve(keyValues.size()); \
        for (const KeyValue & key : keyValues) { \
            keys.append({key.first, key.second}); \
        } \
        QVariantAnimation::setKeyValues(keys); \
    } \
\
    /* QVariantAnimation interface */ \
protected: \
    virtual void updateCurrentValue(const QVariant &value) override \
    { \
        setter(value.value<T>()); \
    } \
};

#define PROPERTY_ANIMATOR(T) PROPERTY_ANIMATOR_N(T, T)

#include <QVariantAnimation>
#include <QBitArray>
PROPERTY_ANIMATOR(QBitArray)
PROPERTY_ANIMATOR(bool)
#include <QByteArray>
PROPERTY_ANIMATOR(QByteArray)
PROPERTY_ANIMATOR(QChar)
#include <QDate>
PROPERTY_ANIMATOR(QDate)
#include <QDateTime>
PROPERTY_ANIMATOR(QDateTime)
PROPERTY_ANIMATOR(double)
#include <QEasingCurve>
PROPERTY_ANIMATOR(QEasingCurve)
PROPERTY_ANIMATOR(float)
#include <QHash>
class PropertyAnimator_QHash__QString_QVariant : public QVariantAnimation
{
    Q_OBJECT
    std::function<void(const QHash<QString, QVariant> &)> setter;
public:
    typedef QPair<qreal, QHash<QString, QVariant>> KeyValue;
    typedef QList<KeyValue> KeyValues;
    PropertyAnimator_QHash__QString_QVariant (std::function<void(const QHash<QString, QVariant> &)> setter) {
        this->setter = setter;
    }

    void setStartValue(const QHash<QString, QVariant> & value) {
        QVariantAnimation::setStartValue(value);
    }

    void setEndValue(const QHash<QString, QVariant> & value) {
        QVariantAnimation::setEndValue(value);
    }

    void setKeyValueAt(qreal step, const QHash<QString, QVariant> &value) {
        QVariantAnimation::setKeyValueAt(step, value);
    }

    void setKeyValues(const KeyValues &keyValues) {
        QVariantAnimation::KeyValues keys;
        keys.reserve(keyValues.size());
        for (const KeyValue & key : keyValues) {
            keys.append({key.first, key.second});
        }
        QVariantAnimation::setKeyValues(keys);
    }

    /* QVariantAnimation interface */
protected:
    virtual void updateCurrentValue(const QVariant &value) override
    {
        setter(value.value<QHash<QString, QVariant>>());
    }
};
PROPERTY_ANIMATOR(int)
#include <QJsonArray>
PROPERTY_ANIMATOR(QJsonArray)
#include <QJsonDocument>
PROPERTY_ANIMATOR(QJsonDocument)
#include <QJsonObject>
PROPERTY_ANIMATOR(QJsonObject)
#include <QJsonValue>
PROPERTY_ANIMATOR(QJsonValue)
#include <QLine>
PROPERTY_ANIMATOR(QLine)
#include <QLineF>
PROPERTY_ANIMATOR(QLineF)
#include <QList>
PROPERTY_ANIMATOR_N(QList__QVariant, QList<QVariant>)
#include <QLocale>
PROPERTY_ANIMATOR(QLocale)
PROPERTY_ANIMATOR(qlonglong)
#include <QMap>
class PropertyAnimator_QMap__QString_QVariant : public QVariantAnimation
{
    Q_OBJECT
    std::function<void(const QMap<QString, QVariant> &)> setter;
public:
    typedef QPair<qreal, QMap<QString, QVariant>> KeyValue;
    typedef QList<KeyValue> KeyValues;
    PropertyAnimator_QMap__QString_QVariant (std::function<void(const QMap<QString, QVariant> &)> setter) {
        this->setter = setter;
    }
\
    void setStartValue(const QMap<QString, QVariant> & value) {
        QVariantAnimation::setStartValue(value);
    }
\
    void setEndValue(const QMap<QString, QVariant> & value) {
        QVariantAnimation::setEndValue(value);
    }
\
    void setKeyValueAt(qreal step, const QMap<QString, QVariant> &value) {
        QVariantAnimation::setKeyValueAt(step, value);
    }

    void setKeyValues(const KeyValues &keyValues) {
        QVariantAnimation::KeyValues keys;
        keys.reserve(keyValues.size());
        for (const KeyValue & key : keyValues) {
            keys.append({key.first, key.second});
        }
        QVariantAnimation::setKeyValues(keys);
    }
\
    /* QVariantAnimation interface */
protected:
    virtual void updateCurrentValue(const QVariant &value) override
    {
        setter(value.value<QMap<QString, QVariant>>());
    }
};
#include <QModelIndex>
PROPERTY_ANIMATOR(QModelIndex)
#include <QPersistentModelIndex>
PROPERTY_ANIMATOR(QPersistentModelIndex)
#include <QPoint>
PROPERTY_ANIMATOR(QPoint)
#include <QPointF>
PROPERTY_ANIMATOR(QPointF)
PROPERTY_ANIMATOR(qreal)
#include <QRect>
PROPERTY_ANIMATOR(QRect)
#include <QRectF>
PROPERTY_ANIMATOR(QRectF)
#include <QRegularExpression>
PROPERTY_ANIMATOR(QRegularExpression)
#include <QSize>
PROPERTY_ANIMATOR(QSize)
#include <QSizeF>
PROPERTY_ANIMATOR(QSizeF)
#include <QString>
PROPERTY_ANIMATOR(QString)
#include <QStringList>
PROPERTY_ANIMATOR(QStringList)
#include <QTime>
PROPERTY_ANIMATOR(QTime)
PROPERTY_ANIMATOR(uint)
PROPERTY_ANIMATOR(qulonglong)
#include <QUrl>
PROPERTY_ANIMATOR(QUrl)
#include <QUuid>
PROPERTY_ANIMATOR(QUuid)

#undef PROPERTY_ANIMATOR
#undef PROPERTY_ANIMATOR_N

#endif // TYPESAFEPROPERTYANIMATORS_H
