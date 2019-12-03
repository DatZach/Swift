/*
 *  JsonObject.cpp
 *  JSON Abstraction
 */

#include <QFile>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <Project/JsonObject.hpp>

JsonObject::JsonObject(const QString& filepath)
    : filepath(filepath),
      json()
{

}

JsonObject::JsonObject(const QVariantMap& map)
    : filepath(""),
      json(map)
{

}

bool JsonObject::LoadJson()
{
    QJson::Parser parser;
    bool ok;

    QFile projectFile(filepath);
    if (!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // Load JSON
    json = parser.parse(projectFile.readAll(), &ok).toMap();

    return ok;
}

bool JsonObject::SaveJson(const QVariantMap& map)
{
    QJson::Serializer serializer;
    bool ok;

    QByteArray value = serializer.serialize(map, &ok);
    if (!ok)
        return false;

    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(value);

    return true;
}

bool JsonObject::Load()
{
	return false;
}

bool JsonObject::Save()
{
	return false;
}

QVariant* JsonObject::GetValue(const QString& value)
{
	QString valueSafe = value.toLower();

    for(QString key : json.keys())
    {
		if (valueSafe == key.toLower())
            return &json[key];
    }

    return nullptr;
}

QVariantMap JsonObject::GetMap(const QString &value)
{
    QVariant* qValue = GetValue(value);
    if (qValue == nullptr)
        return QVariantMap();

    return qValue->toMap();
}

QStringList JsonObject::GetStringList(const QString& value, const QStringList& defaultValue)
{
    QVariant* qValue = GetValue(value);
    if (qValue == nullptr)
        return defaultValue;

    return qValue->toStringList();
}

QString JsonObject::GetString(const QString& value, const QString& defaultValue)
{
    QVariant* qValue = GetValue(value);
    if (qValue == nullptr)
        return defaultValue;

    return qValue->toString();
}

bool JsonObject::GetBoolean(const QString& value, bool defaultValue)
{
    QVariant* qValue = GetValue(value);
    if (qValue == nullptr)
        return defaultValue;

    return qValue->toBool();
}

int JsonObject::GetInteger(const QString& value, int defaultValue)
{
    QVariant* qValue = GetValue(value);
    if (qValue == nullptr)
        return defaultValue;

    bool ok;
    int returnValue = qValue->toInt(&ok);

    return ok ? returnValue : defaultValue;
}

double JsonObject::GetDouble(const QString& value, double defaultValue)
{
    QVariant* qValue = GetValue(value);
    if (qValue == nullptr)
        return defaultValue;

    bool ok;
    double returnValue = qValue->toDouble(&ok);

    return ok ? returnValue : defaultValue;
}
