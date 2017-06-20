#ifndef FLASHTOOLAPP_H
#define FLASHTOOLAPP_H

#include <QObject>
#include <QApplication>

class FlashToolApp : public QApplication
{
    Q_OBJECT
public:
    explicit FlashToolApp(int &argc, char **argv);

    virtual bool notify(QObject *receiver, QEvent *e);

signals:

public slots:

};

#endif // FLASHTOOLAPP_H
