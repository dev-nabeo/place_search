#ifndef OSMLOCATIONSERVICEREPLY_H
#define OSMLOCATIONSERVICEREPLY_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include "locationservicereply.h"
#include "resultdata.h"

class OsmLocationServiceReply : public LocationServiceReply
{
    Q_OBJECT
public:
    explicit OsmLocationServiceReply(QNetworkReply *reply, QObject *parent = nullptr);
    virtual ~OsmLocationServiceReply();

private Q_SLOTS:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);

};

#endif // OSMLOCATIONSERVICEREPLY_H
