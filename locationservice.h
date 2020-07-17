#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include "locationservicereply.h"

class LocationService : public QObject
{
    Q_OBJECT
public:
    explicit LocationService(QObject *parent = nullptr);
    virtual ~LocationService();

    virtual LocationServiceReply *search(const QString &searchTerm, int limit);
    virtual LocationServiceReply *nearby(const QGeoCoordinate &coordinate, int range, int limit);

Q_SIGNALS:
    void finished(LocationServiceReply *reply);
    void error(LocationServiceReply *reply, LocationServiceReply::Error error, QString errorString = QString());

private Q_SLOTS:
    void replyFinished();
    void replyError(LocationServiceReply::Error errorCode, const QString &errorString);

protected:
    QNetworkAccessManager *m_networkManager;
    QByteArray m_userAgent;
};

#endif // LOCATIONSERVICE_H
