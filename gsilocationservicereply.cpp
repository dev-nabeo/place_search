#include "gsilocationservicereply.h"

GsiLocationServiceReply::GsiLocationServiceReply(QNetworkReply *reply, QObject *parent)
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

GsiLocationServiceReply::~GsiLocationServiceReply()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

void GsiLocationServiceReply::networkReplyFinished()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError)
        return;
    QList<ResultData> replydatalist;
    QByteArray replyData = reply->readAll();
    qDebug() << "==========================================";
    qDebug() << QString(replyData);  //qPrintable??
    qDebug() << "==========================================";
    QXmlStreamReader xmlReader;
    xmlReader.addData(replyData);
    QString convertedName;
    if (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == "results"){
            while(xmlReader.readNextStartElement()){
                if(xmlReader.name() == "converted"){
                    convertedName = xmlReader.readElementText();
                }
                else if(xmlReader.name() == "candidate"){
                    QGeoCoordinate coordinate;
                    ResultData replydata;
                    while(xmlReader.readNextStartElement()){
                        if(xmlReader.name() == "address"){
                            replydata.setAddressText(xmlReader.readElementText());
                        }
                        else if(xmlReader.name() == "longitude"){
                            coordinate.setLongitude(xmlReader.readElementText().toDouble());
                        }
                        else if(xmlReader.name() == "latitude"){
                            coordinate.setLatitude(xmlReader.readElementText().toDouble());
                        }
                        else{
                             xmlReader.skipCurrentElement();
                        }
                    }
                    replydata.setName(convertedName);
                    // not converted WGS84
                    replydata.setCoordinate(convertTokyoToWGS84(coordinate));
                    replydata.setDetail(QString());
                    replydatalist.append(replydata);
                }
                else{
                     xmlReader.skipCurrentElement();
                }
            }
        }
    }

    setReplydatalist(replydatalist);
    setFinished(true);
}

void GsiLocationServiceReply::networkReplyError(QNetworkReply::NetworkError error)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    QString errStr = QString("%1: %2").arg(error).arg(reply->errorString());
    setError(CommunicationError, errStr);
}

QGeoCoordinate GsiLocationServiceReply::convertTokyoToWGS84(const QGeoCoordinate &input)
{
    QGeoCoordinate coordinate;
    // 参考 簡易計算 https://www.jalan.net/jw/jwp0200/jww0203.do
    coordinate.setLatitude(input.latitude() - input.latitude()*0.00010695 + input.longitude()*0.000017464 + 0.0046017);
    coordinate.setLongitude(input.longitude() - input.latitude()*0.000046038 - input.longitude()* 0.000083043 + 0.010040);
    return coordinate;
}
