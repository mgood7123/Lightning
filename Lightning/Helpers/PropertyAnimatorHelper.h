#ifndef PROPERTYANIMATORHELPER_H
#define PROPERTYANIMATORHELPER_H

#include "../Tools/TypeSafeKeyValuePropertyAnimators.h"

class PropertyAnimatorHelper {
public:
    static KeyValuePropertyAnimator_QBitArray *getQBitArrayAnimator(QBitArray *valuePointer);

    static KeyValuePropertyAnimator_bool *getBoolAnimator(bool *valuePointer);

    static KeyValuePropertyAnimator_QByteArray *getQByteArrayAnimator(QByteArray *valuePointer);

    static KeyValuePropertyAnimator_QChar *getQCharAnimator(QChar *valuePointer);

    static KeyValuePropertyAnimator_QDate *getQDateAnimator(QDate *valuePointer);

    static KeyValuePropertyAnimator_QDateTime *getQDateTimeAnimator(QDateTime *valuePointer);

    static PropertyAnimator_double *getDoubleAnimator(double *valuePointer);

    static KeyValuePropertyAnimator_double *getDoubleKeyValueAnimator(double *valuePointer);

    static KeyValuePropertyAnimator_QEasingCurve *getQEasingCurveAnimator(QEasingCurve *valuePointer);

    static PropertyAnimator_float *getFloatAnimator(float *valuePointer);

    static KeyValuePropertyAnimator_float *getFloatKeyValueAnimator(float *valuePointer);

    static KeyValuePropertyAnimator_QHash__QString_QVariant *getQHash__QString_QVariant__Animator(QHash<QString, QVariant> *valuePointer);

    static PropertyAnimator_int *getIntAnimator(int *valuePointer);

    static KeyValuePropertyAnimator_int *getIntKeyValueAnimator(int *valuePointer);

    static KeyValuePropertyAnimator_QJsonArray *getQJsonArrayAnimator(QJsonArray *valuePointer);

    static KeyValuePropertyAnimator_QJsonDocument *getQJsonDocumentAnimator(QJsonDocument *valuePointer);

    static KeyValuePropertyAnimator_QJsonObject *getQJsonObjectAnimator(QJsonObject *valuePointer);

    static KeyValuePropertyAnimator_QJsonValue *getQJsonValueAnimator(QJsonValue *valuePointer);

    static KeyValuePropertyAnimator_QLine *getQLineAnimator(QLine *valuePointer);

    static KeyValuePropertyAnimator_QLineF *getQLineFAnimator(QLineF *valuePointer);

    static KeyValuePropertyAnimator_QList__QVariant *getQList__QVariant__Animator(QList<QVariant> *valuePointer);

    static KeyValuePropertyAnimator_QLocale *getQLocaleAnimator(QLocale *valuePointer);

    static PropertyAnimator_qlonglong *getQLongLongAnimator(qlonglong *valuePointer);

    static KeyValuePropertyAnimator_qlonglong *getQLongLongKeyValueAnimator(qlonglong *valuePointer);

    static KeyValuePropertyAnimator_QMap__QString_QVariant *getQMap__QString_QVariant__Animator(QMap<QString, QVariant> *valuePointer);

    static KeyValuePropertyAnimator_QModelIndex *getQModelIndexAnimator(QModelIndex *valuePointer);

    static KeyValuePropertyAnimator_QPersistentModelIndex *getQPersistentModelIndexAnimator(QPersistentModelIndex *valuePointer);

    static KeyValuePropertyAnimator_QPoint *getQPointAnimator(QPoint *valuePointer);

    static KeyValuePropertyAnimator_QPointF *getQPointFAnimator(QPointF *valuePointer);

    static PropertyAnimator_qreal *getQRealAnimator(qreal *valuePointer);

    static KeyValuePropertyAnimator_qreal *getQRealKeyValueAnimator(qreal *valuePointer);

    static KeyValuePropertyAnimator_QRect *getQRectAnimator(QRect *valuePointer);

    static KeyValuePropertyAnimator_QRectF *getQRectFAnimator(QRectF *valuePointer);

    static KeyValuePropertyAnimator_QRegularExpression *getQRegularExpressionAnimator(QRegularExpression *valuePointer);

    static KeyValuePropertyAnimator_QSize *getQSizeAnimator(QSize *valuePointer);

    static KeyValuePropertyAnimator_QSizeF *getQSizeFAnimator(QSizeF *valuePointer);

    static KeyValuePropertyAnimator_QString *getQStringAnimator(QString *valuePointer);

    static KeyValuePropertyAnimator_QStringList *getQStringListAnimator(QStringList *valuePointer);

    static KeyValuePropertyAnimator_QTime *getQTimeAnimator(QTime *valuePointer);

    static PropertyAnimator_uint *getUIntAnimator(uint *valuePointer);

    static KeyValuePropertyAnimator_uint *getUIntKeyValueAnimator(uint *valuePointer);

    static PropertyAnimator_qulonglong *getQULongLongAnimator(qulonglong *valuePointer);

    static KeyValuePropertyAnimator_qulonglong *getQULongLongKeyValueAnimator(qulonglong *valuePointer);

    static KeyValuePropertyAnimator_QUrl *getQUrlAnimator(QUrl *valuePointer);

    static KeyValuePropertyAnimator_QUuid *getQUuidAnimator(QUuid *valuePointer);

    static KeyValuePropertyAnimator_QBitArray *getQBitArrayAnimator(std::function<void (const QBitArray &)> f);

    static KeyValuePropertyAnimator_bool *getBoolAnimator(std::function<void (const bool &)> f);

    static KeyValuePropertyAnimator_QByteArray *getQByteArrayAnimator(std::function<void (const QByteArray &)> f);

    static KeyValuePropertyAnimator_QChar *getQCharAnimator(std::function<void (const QChar &)> f);

    static KeyValuePropertyAnimator_QDate *getQDateAnimator(std::function<void (const QDate &)> f);

    static KeyValuePropertyAnimator_QDateTime *getQDateTimeAnimator(std::function<void (const QDateTime &)> f);

    static PropertyAnimator_double *getDoubleAnimator(std::function<void (const double &)> f);

    static KeyValuePropertyAnimator_double *getDoubleKeyValueAnimator(std::function<void (const double &)> f);

    static KeyValuePropertyAnimator_QEasingCurve *getQEasingCurveAnimator(std::function<void (const QEasingCurve &)> f);

    static PropertyAnimator_float *getFloatAnimator(std::function<void (const float &)> f);

    static KeyValuePropertyAnimator_float *getFloatKeyValueAnimator(std::function<void (const float &)> f);

    static KeyValuePropertyAnimator_QHash__QString_QVariant *getQHash__QString_QVariant__Animator(std::function<void (const QHash<QString, QVariant> &)> f);

    static PropertyAnimator_int *getIntAnimator(std::function<void (const int &)> f);

    static KeyValuePropertyAnimator_int *getIntKeyValueAnimator(std::function<void (const int &)> f);

    static KeyValuePropertyAnimator_QJsonArray *getQJsonArrayAnimator(std::function<void (const QJsonArray &)> f);

    static KeyValuePropertyAnimator_QJsonDocument *getQJsonDocumentAnimator(std::function<void (const QJsonDocument &)> f);

    static KeyValuePropertyAnimator_QJsonObject *getQJsonObjectAnimator(std::function<void (const QJsonObject &)> f);

    static KeyValuePropertyAnimator_QJsonValue *getQJsonValueAnimator(std::function<void (const QJsonValue &)> f);

    static KeyValuePropertyAnimator_QLine *getQLineAnimator(std::function<void (const QLine &)> f);

    static KeyValuePropertyAnimator_QLineF *getQLineFAnimator(std::function<void (const QLineF &)> f);

    static KeyValuePropertyAnimator_QList__QVariant *getQList__QVariant__Animator(std::function<void (const QList<QVariant> &)> f);

    static KeyValuePropertyAnimator_QLocale *getQLocaleAnimator(std::function<void (const QLocale &)> f);

    static PropertyAnimator_qlonglong *getQLongLongAnimator(std::function<void (const qlonglong &)> f);

    static KeyValuePropertyAnimator_qlonglong *getQLongLongKeyValueAnimator(std::function<void (const qlonglong &)> f);

    static KeyValuePropertyAnimator_QMap__QString_QVariant *getQMap__QString_QVariant__Animator(std::function<void (const QMap<QString, QVariant> &)> f);

    static KeyValuePropertyAnimator_QModelIndex *getQModelIndexAnimator(std::function<void (const QModelIndex &)> f);

    static KeyValuePropertyAnimator_QPersistentModelIndex *getQPersistentModelIndexAnimator(std::function<void (const QPersistentModelIndex &)> f);

    static KeyValuePropertyAnimator_QPoint *getQPointAnimator(std::function<void (const QPoint &)> f);

    static KeyValuePropertyAnimator_QPointF *getQPointFAnimator(std::function<void (const QPointF &)> f);

    static PropertyAnimator_qreal *getQRealAnimator(std::function<void (const qreal &)> f);

    static KeyValuePropertyAnimator_qreal *getQRealKeyValueAnimator(std::function<void (const qreal &)> f);

    static KeyValuePropertyAnimator_QRect *getQRectAnimator(std::function<void (const QRect &)> f);

    static KeyValuePropertyAnimator_QRectF *getQRectFAnimator(std::function<void (const QRectF &)> f);

    static KeyValuePropertyAnimator_QRegularExpression *getQRegularExpressionAnimator(std::function<void (const QRegularExpression &)> f);

    static KeyValuePropertyAnimator_QSize *getQSizeAnimator(std::function<void (const QSize &)> f);

    static KeyValuePropertyAnimator_QSizeF *getQSizeFAnimator(std::function<void (const QSizeF &)> f);

    static KeyValuePropertyAnimator_QString *getQStringAnimator(std::function<void (const QString &)> f);

    static KeyValuePropertyAnimator_QStringList *getQStringListAnimator(std::function<void (const QStringList &)> f);

    static KeyValuePropertyAnimator_QTime *getQTimeAnimator(std::function<void (const QTime &)> f);

    static PropertyAnimator_uint *getUIntAnimator(std::function<void (const uint &)> f);

    static KeyValuePropertyAnimator_uint *getUIntKeyValueAnimator(std::function<void (const uint &)> f);

    static PropertyAnimator_qulonglong *getQULongLongAnimator(std::function<void (const qulonglong &)> f);

    static KeyValuePropertyAnimator_qulonglong *getQULongLongKeyValueAnimator(std::function<void (const qulonglong &)> f);

    static KeyValuePropertyAnimator_QUrl *getQUrlAnimator(std::function<void (const QUrl &)> f);

    static KeyValuePropertyAnimator_QUuid *getQUuidAnimator(std::function<void (const QUuid &)> f);
};

#endif // PROPERTYANIMATORHELPER_H
