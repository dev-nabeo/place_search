#include "ymrlocationservicereply.h"

YmrLocationServiceReply::YmrLocationServiceReply(QNetworkReply *reply, QObject *parent)
    :LocationServiceReply(parent)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if (!reply) {
        setError(UnknownError, "");
        return;
    }
    connect(reply, SIGNAL(finished()), this, SLOT(networkReplyFinished()));
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)),
                this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
    connect(this, &LocationServiceReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
}

YmrLocationServiceReply::~YmrLocationServiceReply()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

void YmrLocationServiceReply::networkReplyFinished()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError)
        return;
    QList<ResultData> replydatalist;
    QByteArray replyData = reply->readAll();
    qDebug() << "==========================================";
    qDebug() << QString(replyData);
    qDebug() << "==========================================";
    QJsonDocument document = QJsonDocument::fromJson(replyData);
    QJsonObject object = document.object();
    int err = object.value(QStringLiteral("err")).toInt();
    if(err){
        qDebug() << object.value(QStringLiteral("errcode")).toString();
        setFinished(true);
        return;
    }
    QJsonArray poilist = object.value(QStringLiteral("poilist")).toArray();
    for (int i = 0; i < poilist.count(); ++i) {
        if (!poilist.at(i).isObject())
            continue;
        QJsonObject object = poilist.at(i).toObject();
        QGeoCoordinate coordinate;
        coordinate.setLatitude(object.value(QStringLiteral("lat")).toString().toDouble());
        coordinate.setLongitude(object.value(QStringLiteral("lon")).toString().toDouble());
        coordinate.setAltitude(object.value(QStringLiteral("elevation")).toString().toDouble());
        ResultData replydata;
        replydata.setCoordinate(coordinate);
        replydata.setName(object.value(QStringLiteral("name")).toString());
        replydata.setAddressText(QString());
        replydata.setDetail(object.value(QStringLiteral("detail")).toString());
        replydatalist.append(replydata);
    }
    setReplydatalist(replydatalist);
    setFinished(true);
}

void YmrLocationServiceReply::networkReplyError(QNetworkReply::NetworkError error)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    QString errStr = QString("%1: %2").arg(error).arg(reply->errorString());
    setError(CommunicationError, errStr);
}
