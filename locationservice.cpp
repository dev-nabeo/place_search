#include "locationservice.h"

LocationService::LocationService(QObject *parent)
    : QObject(parent),
      m_networkManager(new QNetworkAccessManager(this))
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_userAgent = "Qt application";
}

LocationService::~LocationService()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

LocationServiceReply *LocationService::search(const QString &query, int limit)
{
    Q_UNUSED(query);
    Q_UNUSED(limit);
    return new LocationServiceReply(LocationServiceReply::Unsupported, QString("This function is not supported"), this);
}

LocationServiceReply *LocationService::nearby(const QGeoCoordinate &coordinate, int range, int limit)
{
    Q_UNUSED(coordinate);
    Q_UNUSED(range);
    Q_UNUSED(limit);
    return new LocationServiceReply(LocationServiceReply::Unsupported, QString("This function is not supported"), this);
}

void LocationService::replyFinished()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    LocationServiceReply *reply = qobject_cast<LocationServiceReply*>(sender());
    if (reply)
        emit finished(reply);
}

void LocationService::replyError(LocationServiceReply::Error errorCode, const QString &errorString)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    LocationServiceReply *reply = qobject_cast<LocationServiceReply*>(sender());
    if (reply)
        emit error(reply, errorCode, errorString);
}
