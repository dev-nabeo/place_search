#include "locationservicereply.h"

LocationServiceReply::LocationServiceReply(LocationServiceReply::Error error,const QString &errorString, QObject *parent)
    : QObject(parent),
      m_error(error),
      m_errorString(errorString),
      m_isFinished(true)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

LocationServiceReply::~LocationServiceReply()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

bool LocationServiceReply::isFinished() const
{
    return m_isFinished;
}

LocationServiceReply::Error LocationServiceReply::error() const
{
    return m_error;
}

QString LocationServiceReply::errorString() const
{
    return m_errorString;
}

QList<ResultData> LocationServiceReply::replydatalist() const
{
    return m_replydatalist;
}

void LocationServiceReply::abort()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if (!isFinished())
        setFinished(true);
    emit aborted();
}

LocationServiceReply::LocationServiceReply(QObject *parent)
    : QObject(parent),
      m_error(LocationServiceReply::NoError),
      m_isFinished(false)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
}

void LocationServiceReply::setError(LocationServiceReply::Error error, const QString &errorString)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_error = error;
    m_errorString = errorString;
    emit this->error(error, errorString);
    setFinished(true);
}

void LocationServiceReply::setFinished(bool finished)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    m_isFinished = finished;
    if(m_isFinished)
        emit this->finished();
}

void LocationServiceReply::addReplydata(const ResultData &data)
{
    m_replydatalist.append(data);
}

void LocationServiceReply::setReplydatalist(const QList<ResultData> &datalist)
{
    m_replydatalist = datalist;
}
