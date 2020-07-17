#ifndef YMRLOCATIONSERVICEREPLY_H
#define YMRLOCATIONSERVICEREPLY_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include "locationservicereply.h"

class YmrLocationServiceReply : public LocationServiceReply
{
    Q_OBJECT
public:
    explicit YmrLocationServiceReply(QNetworkReply *reply, QObject *parent = nullptr);
    virtual ~YmrLocationServiceReply();

private Q_SLOTS:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);
};

#endif // YMRLOCATIONSERVICEREPLY_H
