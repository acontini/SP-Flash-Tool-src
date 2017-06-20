#ifndef INIITEM_H
#define INIITEM_H

#include <QString>
#include <QVariant>
#include <QStringList>

class IniItem
{
public:
    IniItem(const QString &file_path, const QString &section_name, const QString &item_name);

    void SetFilePath(const QString &file_path) {file_path_ = file_path;}
    void SetSectionName(const QString &section_name) {section_name_ = section_name;}
    void SetItemName(const QString &item_name) { item_name_ = item_name;}

    void SaveIntValue(const int ivalue);
    void SaveStringValue(const QString &svalue);
    void SaveBooleanValue(const bool bvalue);
    void SaveStringListValue(const QStringList &values);

    void Save(QVariant value);

    int GetIntValue();
    bool GetBooleanValue();
    QString GetStringValue();

    QStringList GetStringListValue();

    QVariant Get();

private:
    QString file_path_;
    QString section_name_;
    QString item_name_;
};

#endif // INIITEM_H
