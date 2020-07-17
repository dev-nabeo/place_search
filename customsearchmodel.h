#ifndef CUSTOMSEARCHMODEL_H
#define CUSTOMSEARCHMODEL_H

#include <QAbstractListModel>
#include <QtPositioning/QGeoCoordinate>
#include <QDebug>
#include "resultobject.h"
#include "resultdata.h"
#include "locationservice.h"
#include "locationservicefactory.h"
#include "locationservicereply.h"

class CustomSearchModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(QString method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString service READ service WRITE setService NOTIFY serviceChanged)
    Q_PROPERTY(QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged)
    Q_PROPERTY(QGeoCoordinate searchCoordinate READ searchCoordinate WRITE setSearchCoordinate NOTIFY searchCoordinateChanged)
    Q_PROPERTY(int range READ range WRITE setRange NOTIFY rangeChanged)

public:
    enum Status{
        Null,
        Ready,
        Loading,
        Error
    };
    enum Roles {
        MyDataRole = Qt::UserRole + 1
    };
    explicit CustomSearchModel(QObject *parent = nullptr);
    virtual ~CustomSearchModel();

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Status status() const;
    QString errorString() const;
    int count() const;
    int limit() const;
    void setLimit(int limit);
    QString method() const;
    void setMethod(const QString &method);
    QString service() const;
    void setService(const QString &service);
    QString searchTerm() const;
    void setSearchTerm(const QString &searchTerm);
    QGeoCoordinate searchCoordinate() const;
    void setSearchCoordinate(const QGeoCoordinate &searchCoordinate);
    int range() const;
    void setRange(int range);

    Q_INVOKABLE ResultObject *get(int index);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    void statusChanged();
    void errorStringChanged();
    void countChanged();
    void limitChanged();
    void methodChanged();
    void serviceChanged();
    void searchTermChanged();
    void searchCoordinateChanged();
    void rangeChanged();

public Q_SLOTS:
    void update();
protected Q_SLOTS:
    void updateFinished(LocationServiceReply *reply);
    void updateError(LocationServiceReply *reply,
                     LocationServiceReply::Error error,
                     const QString &errorString);
private:
    void setStatus(Status status);
    void setErrorString(const QString &errorString);
    void setResultLists(const QList<ResultData> &objs);
    void abortRequest();

    QList<ResultObject *> m_resultList;
    LocationService *m_srv;
    LocationServiceReply *m_reply;

    Status m_status;
    QString m_errorString;
    int m_limit;
    QString m_method;
    QString m_service;
    QString m_searchTerm;
    QGeoCoordinate m_searchCoordinate;
    int m_range;

};

#endif // CUSTOMSEARCHMODEL_H
