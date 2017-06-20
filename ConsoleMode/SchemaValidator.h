#ifndef SCHEMAVALIDATOR_H
#define SCHEMAVALIDATOR_H

#include <QString>
#include <QtXmlPatterns>
#include <QFile>

#include "../Err/Exception.h"

namespace ConsoleMode
{
class MessageHandler;
class SchemaValidator
{
public:
    SchemaValidator(const QString& _schema_file);
    ~SchemaValidator();

    void Validate(const QString& xml_file);
    void LoadFile(const QString &filename);

private:
    QString schema_file;
    QXmlSchema schema_obj;
    MessageHandler *msg_handler;
};

}

#endif // SCHEMAVALIDATOR_H
