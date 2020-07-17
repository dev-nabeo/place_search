#ifndef GSILOCATIONSERVICE_H
#define GSILOCATIONSERVICE_H

#include <QObject>
#include <QUrlQuery>
#include "locationservice.h"
#include "gsilocationservicereply.h"

class GsiLocationService : public LocationService
{
    Q_OBJECT
public:
    explicit GsiLocationService(QObject *parent = nullptr);
    virtual ~GsiLocationService();

    LocationServiceReply *search(const QString &searchTerm, int limit);

private:
    QString m_urlPrefix;
};

#endif // GSILOCATIONSERVICE_H
