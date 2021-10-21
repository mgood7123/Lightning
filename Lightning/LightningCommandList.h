#ifndef LIGHTNINGCOMMANDLIST_H
#define LIGHTNINGCOMMANDLIST_H

#include <QtGlobal>
#include <QList>

#include "Helpers/CoordinateHelper.h"

class LightningCommandList
{
public:
    template <typename T>
    class Iterator {
        QList<T> & list;
        qsizetype size;
        qsizetype index = 0;
    public:
        Iterator(QList<T> & list) : list(list), size(list.size()) {}
        void skip(qsizetype amount) {
            index += amount;
        }
        void skipOne() {
            index++;
        }
        bool hasNext() const {
            return index < size;
        }
        const T & next() {
            return list.at(index++);
        }
        qsizetype getIndex() const {
            return index;
        }

        void reset() {
            index = 0;
        }
    };

    typedef quint16 COMMAND_T;
    QList<COMMAND_T> commands;
    QList<CoordinateHelper::CoordinateInfo> absolute_coordinates;
    QList<bool> data_bool;
    QList<float> data_float;
    QList<int> data_int;
    QList<QString> data_qstring;
    QList<quint64> data_quint64;

    Iterator<COMMAND_T> commands_Iterator();
    Iterator<CoordinateHelper::CoordinateInfo> absolute_coordinates_Iterator();
    Iterator<bool> bool_Iterator();
    Iterator<float> float_Iterator();
    Iterator<int> int_Iterator();
    Iterator<QString> qstring_Iterator();
    Iterator<quint64> quint64_Iterator();


    void eraseAllData();

    void addCommand(const COMMAND_T & command);

    void addAbsoluteCoordinates(const CoordinateHelper::CoordinateInfo & absoluteCoordinates);

    void addBool(const bool & data);

    void addBool(const QList<bool> & data);

    void addInt(const int & data);

    void addInt(const QList<int> & data);

    void addFloat(const float & data);

    void addFloat(const QList<float> & data);

    void addQString(const QString & data);

    void addQString(const QList<QString> & data);

    void addQUnsignedInt64(const quint64 & data);

    void addQUnsignedInt64(const QList<quint64> & data);

    void addCommandList(const LightningCommandList & commandList);
};

#endif // LIGHTNINGCOMMANDLIST_H
