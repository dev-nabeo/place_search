#include "osmlocationservicereply.h"

OsmLocationServiceReply::OsmLocationServiceReply(QNetworkReply *reply, QObject *parent)
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

OsmLocationServiceReply::~OsmLocationServiceReply()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

void OsmLocationServiceReply::networkReplyFinished()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QList<ResultData> replydatalist;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (document.isObject()) {
        QJsonObject object = document.object();
        QGeoCoordinate coordinate;
        coordinate.setLatitude(object.value(QStringLiteral("lat")).toString().toDouble());
        coordinate.setLongitude(object.value(QStringLiteral("lon")).toString().toDouble());
        ResultData replydata;
        replydata.setCoordinate(coordinate);
        replydata.setAddressText(object.value(QStringLiteral("display_name")).toString());
        QJsonObject nao = object.value(QStringLiteral("namedetails")).toObject();
        replydata.setName(nao.value(QStringLiteral("name")).toString());
        replydata.setDetail(QString());
        replydatalist.append(replydata);
    }
    else if(document.isArray()){
        QJsonArray results = document.array();
        for (int i = 0; i < results.count(); ++i) {
            if (!results.at(i).isObject())
                continue;
            QJsonObject object = results.at(i).toObject();
            QGeoCoordinate coordinate;
            coordinate.setLatitude(object.value(QStringLiteral("lat")).toString().toDouble());
            coordinate.setLongitude(object.value(QStringLiteral("lon")).toString().toDouble());
            ResultData replydata;
            replydata.setCoordinate(coordinate);
            replydata.setAddressText(object.value(QStringLiteral("display_name")).toString());
            QJsonObject nao = object.value(QStringLiteral("namedetails")).toObject();
            replydata.setName(nao.value(QStringLiteral("name")).toString());
            replydata.setDetail(QString());
            replydatalist.append(replydata);
        }
    }
    setReplydatalist(replydatalist);
    setFinished(true);
}

void OsmLocationServiceReply::networkReplyError(QNetworkReply::NetworkError error)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    QString errStr = QString("%1: %2").arg(error).arg(reply->errorString());
    setError(CommunicationError, errStr);
}


