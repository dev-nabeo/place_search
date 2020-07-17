#include "locationservicefactory.h"

LocationServiceFactory::LocationServiceFactory()
{

}

LocationServiceFactory::~LocationServiceFactory()
{

}

LocationService *LocationServiceFactory::create(const QString &name, QObject *parent)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    LocationService *retservice = nullptr;
    if(name=="OSM"){
        retservice = new OsmLocationService(parent);
    }
    else if(name == "GSI"){
        retservice = new GsiLocationService(parent);
    }
    else if(name == "YMR"){
        retservice = new YmrLocationService(parent);
    }
    else{
        retservice = new LocationService(parent);
    }
    return retservice;
}
