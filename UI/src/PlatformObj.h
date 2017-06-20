#ifndef PLATFORMOBJ_H
#define PLATFORMOBJ_H

#include <QObject>
#include "../../Setting/PlatformSetting.h"

class PlatformObj : public QObject, public APCore::IPlatformOb
{
    Q_OBJECT
public:
    explicit PlatformObj(QObject *parent = 0);

    virtual void onPlatformChanged();

signals:
    void signal_platform_change();

public slots:

};

#endif // PLATFORMOBJ_H
