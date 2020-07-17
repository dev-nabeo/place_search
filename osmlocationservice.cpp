#include "osmlocationservice.h"

OsmLocationService::OsmLocationService(QObject *parent)
    :LocationService(parent),
      m_language("ja"),
      m_country("JP")
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_urlPrefix = "http://nominatim.openstreetmap.org";
}

OsmLocationService::~OsmLocationService()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

LocationServiceReply *OsmLocationService::search(const QString &searchTerm, int limit)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);
    QUrl url(QString("%1/search").arg(m_urlPrefix));
    //QUrl url = QUrl::fromLocalFile("../place_search/debug_data/osmdebug.json");
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("q"), searchTerm);
    query.addQueryItem(QStringLiteral("format"), QStringLiteral("json"));
    query.addQueryItem(QStringLiteral("accept-language"), m_language);
    query.addQueryItem(QStringLiteral("countrycodes"), m_country);
    query.addQueryItem(QStringLiteral("limit"), QString::number(limit));
    query.addQueryItem(QStringLiteral("namedetails"), QStringLiteral("1"));
    url.setQuery(query);
    request.setUrl(url);

    QNetworkReply *reply = m_networkManager->get(request);
    OsmLocationServiceReply *locationServiceReply = new OsmLocationServiceReply(reply, this);

    connect(locationServiceReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(locationServiceReply, SIGNAL(error(LocationServiceReply::Error,QString)),
            this, SLOT(replyError(LocationServiceReply::Error,QString)));

    return locationServiceReply;


}
