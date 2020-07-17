#ifndef LOCATIONSERVICEFACTORY_H
#define LOCATIONSERVICEFACTORY_H

#include <QObject>
#include "locationservice.h"
#include "osmlocationservice.h"
#include "ymrlocationservice.h"
#include "gsilocationservice.h"

class LocationServiceFactory
{
public:
    LocationServiceFactory();
    ~LocationServiceFactory();
    LocationService *create(const QString &name, QObject *parent);
};

#endif // LOCATIONSERVICEFACTORY_H
