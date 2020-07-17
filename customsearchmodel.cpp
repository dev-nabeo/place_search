#include "customsearchmodel.h"

CustomSearchModel::CustomSearchModel(QObject *parent)
    : QAbstractListModel(parent),
      m_status(CustomSearchModel::Null),
      m_errorString(QString()),
      m_limit(10),
      m_method(QString()),
      m_service(QString()),
      m_searchTerm(QString()),
      m_searchCoordinate(QGeoCoordinate()),
      m_range(1)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    setResultLists(QList<ResultData>());
    m_srv = nullptr;
    m_reply = nullptr;
}

CustomSearchModel::~CustomSearchModel()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    qDeleteAll(m_resultList);
    m_resultList.clear();
    if(m_srv != nullptr)
        delete m_srv;
    if(m_reply != nullptr)
        delete m_reply;
}

int CustomSearchModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_resultList.count();
}

QVariant CustomSearchModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(index.row() >= m_resultList.count())
        return QVariant();
    if(role == CustomSearchModel::MyDataRole) {
        QObject *resultobject = m_resultList.at(index.row());
        Q_ASSERT(resultobject);
        return QVariant::fromValue(resultobject);
    }
    return QVariant();
}

QHash<int, QByteArray> CustomSearchModel::roleNames() const
{
    QHash<int, QByteArray> roleNames = QAbstractItemModel::roleNames();
    roleNames.insert( CustomSearchModel::MyDataRole, "result");
    return roleNames;
}

CustomSearchModel::Status CustomSearchModel::status() const
{
    return  m_status;
}

QString CustomSearchModel::errorString() const
{
    return m_errorString;
}

int CustomSearchModel::count() const
{
    return m_resultList.count();
}

int CustomSearchModel::limit() const
{
    return m_limit;
}

void CustomSearchModel::setLimit(int limit)
{
    if(m_limit != limit){
        m_limit = limit;
        emit limitChanged();
    }
}

QString CustomSearchModel::method() const
{
    return m_method;
}

void CustomSearchModel::setMethod(const QString &method)
{
    if(m_method != method){
        m_method = method;
        emit methodChanged();
    }
}

QString CustomSearchModel::service() const
{
    return m_service;
}

void CustomSearchModel::setService(const QString &service)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if(m_service != service){
        m_service = service;
        LocationServiceFactory lservice;
        if(m_srv)
            delete m_srv;
        m_srv = lservice.create(m_service, this);
        Q_ASSERT(m_srv);
        connect(m_srv, SIGNAL(finished(LocationServiceReply*)),
                this, SLOT(updateFinished(LocationServiceReply*)));
        connect(m_srv, SIGNAL(error(LocationServiceReply*, LocationServiceReply::Error, QString)),
                this, SLOT(updateError(LocationServiceReply*, LocationServiceReply::Error, QString)));
        emit serviceChanged();
    }
}

QString CustomSearchModel::searchTerm() const
{
    return m_searchTerm;
}

void CustomSearchModel::setSearchTerm(const QString &searchTerm)
{
    if(m_searchTerm != searchTerm){
        m_searchTerm = searchTerm;
        emit searchTermChanged();
    }
}

QGeoCoordinate CustomSearchModel::searchCoordinate() const
{
    return m_searchCoordinate;
}

void CustomSearchModel::setSearchCoordinate(const QGeoCoordinate &searchCoordinate)
{
    if(m_searchCoordinate != searchCoordinate){
        m_searchCoordinate = searchCoordinate;
        emit searchCoordinateChanged();
    }
}

int CustomSearchModel::range() const
{
    return m_range;
}

void CustomSearchModel::setRange(int range)
{
    if(m_range != range){
        m_range = range;
        emit rangeChanged();
    }
}

ResultObject *CustomSearchModel::get(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if(index < 0 || index >= m_resultList.count() ){
        return nullptr;
    }
    return m_resultList.at(index);
}

void CustomSearchModel::reset()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    beginResetModel();
    if(!m_resultList.isEmpty()){
        setResultLists(QList<ResultData>());
        emit countChanged();
    }
    endResetModel();
    abortRequest();
    setErrorString(QString());
    setStatus(CustomSearchModel::Null);
    return;
}

void CustomSearchModel::cancel()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    abortRequest();
    setErrorString(QString());
    setStatus(m_resultList.isEmpty() ? CustomSearchModel::Null : CustomSearchModel::Ready);
    return;
}

void CustomSearchModel::update()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if(m_method.isEmpty()){
        setErrorString("method is not set.");
        return;
    }
    if(m_service.isEmpty()){
        setErrorString("service is not set.");
        return;
    }
    if(!m_srv){
        setErrorString("location service is not set.");
        return;
    }
    abortRequest();
    setErrorString(QString());
    if( m_method == "nearby"){
        if(!m_searchCoordinate.isValid()){
            setErrorString("Coordinate is invalid.");
            return;
        }
        setStatus(CustomSearchModel::Loading);
        m_reply = m_srv->nearby(m_searchCoordinate, m_range, m_limit);
        if(m_reply->isFinished()){
            if( m_reply->error() == LocationServiceReply::NoError){
                updateFinished(m_reply);
            }
            else{
                updateError(m_reply, m_reply->error(), m_reply->errorString());
            }
        }
    }
    else if(m_method == "search"){
        if(m_searchTerm.isEmpty()){
            setErrorString("searchTerm is invalid.");
            return;
        }
        setStatus(CustomSearchModel::Loading);
        m_reply = m_srv->search(m_searchTerm, m_limit);
        if(m_reply->isFinished()){
            if( m_reply->error() == LocationServiceReply::NoError){
                updateFinished(m_reply);
            }
            else{
                updateError(m_reply, m_reply->error(), m_reply->errorString());
            }
        }
    }
    else{
        setErrorString(QString("%1 is not supported.").arg(m_method));
        return;
    }
    return;
}

void CustomSearchModel::updateFinished(LocationServiceReply *reply)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if (reply != m_reply || reply->error() != LocationServiceReply::NoError)
        return;
    reply->deleteLater();
    m_reply = 0;
    int oldcount = m_resultList.count();
    setResultLists(reply->replydatalist());
    setErrorString(QString());
    setStatus(CustomSearchModel::Ready);
    if(oldcount != m_resultList.count())
        emit countChanged();
}

void CustomSearchModel::updateError(LocationServiceReply *reply, LocationServiceReply::Error error, const QString &errorString)
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if (reply != m_reply)
        return;
    reply->deleteLater();
    m_reply = 0;
    int oldCount = m_resultList.count();
    if (oldCount > 0) {
        setResultLists(reply->replydatalist());
        emit countChanged();
    }
    Q_UNUSED(error)
    setErrorString(errorString);
    setStatus(CustomSearchModel::Error);
}

void CustomSearchModel::setStatus(CustomSearchModel::Status status)
{
    if(m_status != status){
        m_status = status;
        emit statusChanged();
    }
}

void CustomSearchModel::setErrorString(const QString &errorString)
{
    if(m_errorString != errorString){
        m_errorString = errorString;
        emit errorStringChanged();
    }
}

void CustomSearchModel::setResultLists(const QList<ResultData> &objs)
{
    beginResetModel();
    qDeleteAll(m_resultList);
    m_resultList.clear();
    for(int i = 0; i < objs.count(); ++i ){
        ResultObject *obj = new ResultObject(objs.at(i), this);
        m_resultList.append(obj);
    }
    endResetModel();
}

void CustomSearchModel::abortRequest()
{
    qDebug() << __PRETTY_FUNCTION__ << "ENTRY";
    if(m_reply){
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = 0;
    }

}
