#include "PlatformObj.h"

PlatformObj::PlatformObj(QObject *parent) :
    QObject(parent)
{
}

void PlatformObj::onPlatformChanged()
{
    emit signal_platform_change();
}
