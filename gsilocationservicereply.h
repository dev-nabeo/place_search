#ifndef GSILOCATIONSERVICEREPLY_H
#define GSILOCATIONSERVICEREPLY_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QXmlStreamReader>
#include "locationservicereply.h"

class GsiLocationServiceReply : public LocationServiceReply
{
    Q_OBJECT
public:
    explicit GsiLocationServiceReply(QNetworkReply *reply, QObject *parent = nullptr);
    virtual ~GsiLocationServiceReply();

private Q_SLOTS:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);

private:
    QGeoCoordinate convertTokyoToWGS84(const QGeoCoordinate &input);
};

#endif // GSILOCATIONSERVICEREPLY_H
