#include "LightningCommandList.h"

LightningCommandList::Iterator<LightningCommandList::COMMAND_T> LightningCommandList::commands_Iterator()
{
    return {commands};
}

LightningCommandList::Iterator<CoordinateHelper::CoordinateInfo> LightningCommandList::absolute_coordinates_Iterator()
{
    return {absolute_coordinates};
}

LightningCommandList::Iterator<bool> LightningCommandList::bool_Iterator()
{
    return {data_bool};
}

LightningCommandList::Iterator<float> LightningCommandList::float_Iterator()
{
    return {data_float};
}

LightningCommandList::Iterator<int> LightningCommandList::int_Iterator()
{
    return {data_int};
}

LightningCommandList::Iterator<QString> LightningCommandList::qstring_Iterator()
{
    return {data_qstring};
}

LightningCommandList::Iterator<quint64> LightningCommandList::quint64_Iterator()
{
    return {data_quint64};
}

void LightningCommandList::eraseAllData() {
    commands = {};
    absolute_coordinates = {};
    data_bool = {};
    data_float = {};
    data_int = {};
    data_qstring = {};
    data_quint64 = {};
}

void LightningCommandList::addCommand(const COMMAND_T & command) {
    commands.append(command);
}

void LightningCommandList::addAbsoluteCoordinates(const CoordinateHelper::CoordinateInfo &absoluteCoordinates)
{
    absolute_coordinates.append(absoluteCoordinates);
}

void LightningCommandList::addBool(const bool & data) {
    data_bool.append(data);
}

void LightningCommandList::addBool(const QList<bool> &data) {
    data_bool.append(data);
}

void LightningCommandList::addInt(const int & data) {
    data_bool.append(false);
    data_int.append(data);
}

void LightningCommandList::addInt(const QList<int> &data) {
    data_bool.insert(data_bool.end(), data.size(), false);
    data_int.append(data);
}

void LightningCommandList::addFloat(const float & data) {
    data_bool.append(true);
    data_float.append(data);
}

void LightningCommandList::addFloat(const QList<float> &data) {
    data_bool.insert(data_bool.end(), data.size(), true);
    data_float.append(data);
}

void LightningCommandList::addQString(const QString &data)
{
    data_qstring.append(data);
}

void LightningCommandList::addQString(const QList<QString> &data)
{
    data_qstring.append(data);
}

void LightningCommandList::addQUnsignedInt64(const quint64 &data)
{
    data_quint64.append(data);
}

void LightningCommandList::addQUnsignedInt64(const QList<quint64> &data)
{
    data_quint64.append(data);
}

void LightningCommandList::addCommandList(const LightningCommandList &commandList) {
    commands.append(commandList.commands);
    absolute_coordinates.append(commandList.absolute_coordinates);
    data_bool.append(commandList.data_bool);
    data_float.append(commandList.data_float);
    data_int.append(commandList.data_int);
    data_qstring.append(commandList.data_qstring);
    data_quint64.append(commandList.data_quint64);
}
