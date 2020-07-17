#include "ymrlocationservice.h"

YmrLocationService::YmrLocationService(QObject *parent)
    :LocationService(parent),
      m_page(0),
      m_limit(10),
      m_range(1)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_urlPrefix = "https://api.yamareco.com/api/v1";
}

YmrLocationService::~YmrLocationService()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

LocationServiceReply *YmrLocationService::search(const QString &searchTerm, int limit)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_limit = limit;
    m_searchTerm = searchTerm;
    m_reply = searchPoi();

    return m_reply;
}

LocationServiceReply *YmrLocationService::nearby(const QGeoCoordinate &coordinate, int range, int limit)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_limit = limit;
    m_coordinate = coordinate;
    m_range = range;
    m_reply = nearbyPoi();

    return m_reply;
}

void YmrLocationService::replyFinished()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    LocationServiceReply *reply = qobject_cast<LocationServiceReply*>(sender());
    if(m_reply == reply){
        qDebug() << "First Responce" << m_page;
    }
    else{
        qDebug() << "Next Responce" << m_page;
        reply->deleteLater();
        for(int i = 0; i < reply->replydatalist().count(); i++){
            m_reply->addReplydata(reply->replydatalist().at(i));
        }
    }
    if( m_reply->replydatalist().count() < m_limit
            && m_page < 5
            && reply->replydatalist().count() == 20){
        searchPoi();
        return;
    }
    else{
        if (m_reply){
            m_page = 0;
            emit finished(m_reply);
        }
    }
    return;
}

void YmrLocationService::replyNearFinished()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    LocationServiceReply *reply = qobject_cast<LocationServiceReply*>(sender());
    if(m_reply == reply){
        qDebug() << "First Responce" << m_page;
    }
    else{
        qDebug() << "Next Responce" << m_page;
        reply->deleteLater();
        for(int i = 0; i < reply->replydatalist().count(); i++){
            m_reply->addReplydata(reply->replydatalist().at(i));
        }
    }
    if( m_reply->replydatalist().count() < m_limit
            && m_page < 5
            && reply->replydatalist().count() == 20){
        nearbyPoi();
        return;
    }
    else{
        if (m_reply){
            m_page = 0;
            emit finished(m_reply);
        }
    }
    return;
}

LocationServiceReply *YmrLocationService::searchPoi()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_page++;
    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QUrl url(QString("%1/searchPoi").arg(m_urlPrefix));
    //QUrl url = QUrl::fromLocalFile("../place_search/debug_data/ymrdebug.json"); //for debug
    request.setUrl(url);
    QUrlQuery query;
    QByteArray postData;
    query.addQueryItem(QStringLiteral("page"), QString::number(m_page));
    query.addQueryItem(QStringLiteral("name"), m_searchTerm);
    query.addQueryItem(QStringLiteral("type_id"), QStringLiteral("0"));
    postData = query.toString(QUrl::FullyEncoded).toUtf8();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QNetworkReply *reply = m_networkManager->post(request, postData);
    //QNetworkReply *reply = m_networkManager->get(request); //for debug
    YmrLocationServiceReply *locationServiceReply = new YmrLocationServiceReply(reply, this);

    connect(locationServiceReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(locationServiceReply, SIGNAL(error(LocationServiceReply::Error,QString)),
            this, SLOT(replyError(LocationServiceReply::Error,QString)));

    return locationServiceReply;
}

LocationServiceReply *YmrLocationService::nearbyPoi()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_page++;
    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QUrl url(QString("%1/nearbyPoi").arg(m_urlPrefix));
    //QUrl url = QUrl::fromLocalFile("../place_search/debug_data/ymrdebug.json"); //for debug
    request.setUrl(url);
    QUrlQuery query;
    QByteArray postData;
    query.addQueryItem(QStringLiteral("page"), QString::number(m_page));
    query.addQueryItem(QStringLiteral("range"), QString::number(m_range));
    query.addQueryItem(QStringLiteral("lat"), QString::number(m_coordinate.latitude()));
    query.addQueryItem(QStringLiteral("lon"), QString::number(m_coordinate.longitude()));
    query.addQueryItem(QStringLiteral("type_id"), QStringLiteral("0"));
    postData = query.toString(QUrl::FullyEncoded).toUtf8();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QNetworkReply *reply = m_networkManager->post(request, postData);
    //QNetworkReply *reply = m_networkManager->get(request); //for debug
    YmrLocationServiceReply *locationServiceReply = new YmrLocationServiceReply(reply, this);

    connect(locationServiceReply, SIGNAL(finished()), this, SLOT(replyNearFinished()));
    connect(locationServiceReply, SIGNAL(error(LocationServiceReply::Error,QString)),
            this, SLOT(replyError(LocationServiceReply::Error,QString)));

    return locationServiceReply;
}
