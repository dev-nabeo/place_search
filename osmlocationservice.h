#ifndef OSMLOCATIONSERVICE_H
#define OSMLOCATIONSERVICE_H

#include <QObject>
#include <QUrlQuery>
#include "locationservice.h"
#include "osmlocationservicereply.h"

class OsmLocationService : public LocationService
{
    Q_OBJECT
public:
    explicit OsmLocationService(QObject *parent = nullptr);
    virtual ~OsmLocationService();

    LocationServiceReply *search(const QString &searchTerm, int limit);

private:
    QString m_urlPrefix;
    QString m_language;
    QString m_country;

};

#endif // OSMLOCATIONSERVICE_H
