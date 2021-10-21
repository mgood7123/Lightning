#ifndef TYPESAFEKEYVALUEPROPERTYANIMATORS_H
#define TYPESAFEKEYVALUEPROPERTYANIMATORS_H

#include "TypeSafePropertyAnimators.h"

#define PROPERTY_ANIMATOR_N(N, T) class KeyValuePropertyAnimator_##N : public PropertyAnimator_##N \
{ \
    Q_OBJECT \
    std::function<void(const T &)> setter; \
public: \
    KeyValuePropertyAnimator_##N (std::function<void(const T &)> setter) : PropertyAnimator_##N (setter) \
    {} \
\
    /* QVariantAnimation interface */ \
protected: \
    virtual QVariant interpolated(const QVariant &from, const QVariant &to, qreal progress) const override { \
        Q_UNUSED(to); \
        Q_UNUSED(progress); \
        return from; \
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
class KeyValuePropertyAnimator_QHash__QString_QVariant : public PropertyAnimator_QHash__QString_QVariant
{
    Q_OBJECT
    std::function<void(const QHash<QString, QVariant> &)> setter;
public:
    KeyValuePropertyAnimator_QHash__QString_QVariant (std::function<void(const QHash<QString, QVariant> &)> setter) : PropertyAnimator_QHash__QString_QVariant (setter)
    {}
\
    /* QVariantAnimation interface */
protected:
    virtual QVariant interpolated(const QVariant &from, const QVariant &to, qreal progress) const override {
        Q_UNUSED(to);
        Q_UNUSED(progress);
        return from;
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
class KeyValuePropertyAnimator_QMap__QString_QVariant : public PropertyAnimator_QMap__QString_QVariant
{
    Q_OBJECT
    std::function<void(const QMap<QString, QVariant> &)> setter;
public:
    KeyValuePropertyAnimator_QMap__QString_QVariant (std::function<void(const QMap<QString, QVariant> &)> setter) : PropertyAnimator_QMap__QString_QVariant (setter)
    {}

    /* QVariantAnimation interface */
protected:
    virtual QVariant interpolated(const QVariant &from, const QVariant &to, qreal progress) const override {
        Q_UNUSED(to);
        Q_UNUSED(progress);
        return from;
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

#endif // TYPESAFEKEYVALUEPROPERTYANIMATORS_H
