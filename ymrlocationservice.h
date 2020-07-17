#ifndef YMRLOCATIONSERVICE_H
#define YMRLOCATIONSERVICE_H

#include <QObject>
#include <QUrlQuery>
#include "locationservice.h"
#include "ymrlocationservicereply.h"

class YmrLocationService : public LocationService
{
    Q_OBJECT
public:
    explicit YmrLocationService(QObject *parent = nullptr);
    virtual ~YmrLocationService();

    LocationServiceReply *search(const QString &searchTerm, int limit);
    LocationServiceReply *nearby(const QGeoCoordinate &coordinate, int range, int limit);

private Q_SLOTS:
    void replyFinished();
    void replyNearFinished();

private:
    LocationServiceReply *searchPoi();
    LocationServiceReply *nearbyPoi();
    QString m_urlPrefix;
    int m_page;
    int m_limit;
    QString m_searchTerm;
    int m_range;
    QGeoCoordinate m_coordinate;
    LocationServiceReply *m_reply;
};

#endif // YMRLOCATIONSERVICE_H
