#include "SchemaValidator.h"
#include <QtGlobal>

#include "../Logger/Log.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace ConsoleMode
{

class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler()
        : QAbstractMessageHandler(0)
    {
    }

    QString statusMessage() const
    {
        return m_description;
    }

    QString line() const
    {
        return QString::number(m_sourceLocation.line());
    }

    QString column() const
    {
        return QString::number(m_sourceLocation.column());
    }

protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation)
    {
        Q_UNUSED(type);
        Q_UNUSED(identifier);

        m_messageType = type;
        m_description = description;
        m_sourceLocation = sourceLocation;
    }

private:
    QtMsgType m_messageType;
    QString m_description;
    QSourceLocation m_sourceLocation;
};

SchemaValidator::SchemaValidator(const QString& _schema_file)
    : schema_file(_schema_file),schema_obj(),msg_handler(new MessageHandler())
{
    schema_obj.setMessageHandler(msg_handler);
    QFile file(QDir::toNativeSeparators(schema_file));
    if(file.exists()) {
        file.open(QFile::ReadOnly | QIODevice::Text);
        schema_obj.load(&file);
    } else {
        LOGE("schema file (%s) does NOT exsit!", schema_file.toLocal8Bit().data());
    }

    Q_ASSERT(schema_obj.isValid());
}

SchemaValidator::~SchemaValidator()
{
    if(msg_handler!=NULL)
    {
        delete msg_handler;
    }
}

void SchemaValidator::Validate(const QString& xml_file)
{
    LOG("validating XML schema...");
    QXmlSchemaValidator validator(schema_obj);
    QFile file(xml_file);
    file.open(QFile::ReadOnly);
    if(!validator.validate(&file))
    {
        QString msg("XML Schema validation failed: ");
        msg.append(msg_handler->statusMessage());
        msg.append("(line:").append(msg_handler->line());
        msg.append(", column:").append(msg_handler->column()).append(")");

        LOG(msg.toLocal8Bit().constData());
        THROW_APP_EXCEPTION(-1, msg.toStdString());
    }
    LOG("XML schema validation passed!");
}

}
