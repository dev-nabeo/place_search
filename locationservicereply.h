#ifndef LOCATIONSERVICEREPLY_H
#define LOCATIONSERVICEREPLY_H

#include <QObject>
#include <QDebug>

#include "resultdata.h"

class LocationServiceReply : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError,
        CommunicationError,
        ParseError,
        Unsupported,
        UnknownError
    };

    explicit LocationServiceReply(Error error,const QString &errorString, QObject *parent = nullptr);
    virtual ~LocationServiceReply();
    bool isFinished() const;
    Error error() const;
    QString errorString() const;
    QList<ResultData> replydatalist() const;

    virtual void abort();
    void addReplydata(const ResultData &data);

Q_SIGNALS:
    void finished();
    void aborted();
    void error(LocationServiceReply::Error error, const QString &errorString = QString());

protected:
    explicit LocationServiceReply(QObject *parent = nullptr);
    void setError(Error error, const QString &errorString);
    void setFinished(bool finished);

    //void addReplydata(const ResultData &data);
    void setReplydatalist(const QList<ResultData> &datalist);

private:
    Error m_error;
    QString m_errorString;
    bool m_isFinished;
    QList<ResultData> m_replydatalist;

    Q_DISABLE_COPY(LocationServiceReply);
};

#endif // LOCATIONSERVICEREPLY_H
