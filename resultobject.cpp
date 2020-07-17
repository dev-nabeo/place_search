#include "resultobject.h"

ResultObject::ResultObject(QObject *parent)
    : QObject(parent)
{
    setResultData(ResultData());
}

ResultObject::ResultObject(const ResultData &data, QObject *parent)
    : QObject(parent)
{
    setResultData(data);
}

ResultObject::~ResultObject()
{

}

QString ResultObject::name() const
{
    return m_name;
}

void ResultObject::setName(const QString &name)
{
     if(name != m_name){
         m_name = name;
         emit nameChanged();
     }
}

QString ResultObject::addressText() const
{
    return m_addressText;
}

void ResultObject::setAddressText(const QString &addressText)
{
    if(addressText != m_addressText){
        m_addressText = addressText;
        emit addressTextChanged();
    }
}

QString ResultObject::detail() const
{
    return m_detail;
}

void ResultObject::setDetail(const QString &detail)
{
    if(detail != m_detail){
        m_detail = detail;
        emit detailChanged();
    }
}

QGeoCoordinate ResultObject::coordinate() const
{
    return m_coordinate;
}

void ResultObject::setCoordinate(const QGeoCoordinate &coordinate)
{
    if(coordinate != m_coordinate){
        m_coordinate = coordinate;
        emit coordinateChanged();
    }
}

void ResultObject::setResultData(const ResultData &data)
{
    setName(data.name());
    setAddressText(data.addressText());
    setDetail(data.detail());
    setCoordinate(data.coordinate());
}
