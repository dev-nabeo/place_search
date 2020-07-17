#include "gsilocationservice.h"

GsiLocationService::GsiLocationService(QObject *parent)
    :LocationService(parent)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_urlPrefix = "http://geocode.csis.u-tokyo.ac.jp/cgi-bin/simple_geocode.cgi";
}

GsiLocationService::~GsiLocationService()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

LocationServiceReply *GsiLocationService::search(const QString &searchTerm, int limit)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    Q_UNUSED(limit);
    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);
    QUrl url(m_urlPrefix);
    //QUrl url = QUrl::fromLocalFile("../place_search/debug_data/gsidebug.xml");
    QStringList  searchTermList = searchTerm.split(" ", Qt::SkipEmptyParts);
    qDebug() << "addr=" << searchTermList.at(0);
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("addr"), searchTermList.at(0));
    query.addQueryItem(QStringLiteral("charset"), QStringLiteral("UTF8"));
    query.addQueryItem(QStringLiteral("geosys"), QStringLiteral("tokyo"));
    query.addQueryItem(QStringLiteral("series"), QStringLiteral("PLACE"));
    if(searchTermList.count() > 1 ){
        qDebug() << "constraint=" << searchTermList.at(1);
        query.addQueryItem(QStringLiteral("constraint"), searchTermList.at(1));
    }
    url.setQuery(query);
    request.setUrl(url);

    QNetworkReply *reply = m_networkManager->get(request);
    GsiLocationServiceReply *locationServiceReply = new GsiLocationServiceReply(reply, this);

    connect(locationServiceReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(locationServiceReply, SIGNAL(error(LocationServiceReply::Error,QString)),
            this, SLOT(replyError(LocationServiceReply::Error,QString)));

    return locationServiceReply;
}
