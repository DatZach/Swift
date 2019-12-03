/*
 *  JsonObject.hpp
 *  JSON Abstraction
 */

#ifndef __JSON_OBJECT_HPP
#define __JSON_OBJECT_HPP

#include <QVariant>
#include <QVariantMap>
#include <QStringList>

class JsonObject
{
protected:
    QString filepath;
    QVariantMap json;

    bool LoadJson();
    bool SaveJson(const QVariantMap& map);

public:
    JsonObject(const QString& filepath);
    JsonObject(const QVariantMap& map);
	virtual bool Load();
	virtual bool Save();

    QVariant* GetValue(const QString& value);
    QVariantMap GetMap(const QString& value);
    QStringList GetStringList(const QString& value, const QStringList& defaultValue);
    QString GetString(const QString& value, const QString& defaultValue);
    bool GetBoolean(const QString& value, bool defaultValue);
    int GetInteger(const QString& value, int defaultValue);
    double GetDouble(const QString& value, double defaultValue);
};

#endif
