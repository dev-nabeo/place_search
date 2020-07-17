#include "resultdata.h"

ResultData::ResultData()
    :d(new ResultDataPrivate)
{

}

ResultData::ResultData(const ResultData &other)
    :d(other.d)
{

}

ResultData::~ResultData()
{

}

ResultData &ResultData::operator=(const ResultData &other)
{
    if(this == &other)
        return *this;
    d = other.d;
    return *this;
}

bool ResultData::operator==(const ResultData &other) const
{
    return (*(d.constData()) == *(other.d.constData()));
}

bool ResultData::operator!=(const ResultData &other) const
{
    return !(other == *this);
}

QString ResultData::name() const
{
    return d->m_name;
}

void ResultData::setName(const QString &name)
{
    d->m_name = name;
}

QString ResultData::addressText() const
{
    return d->m_addressText;
}

void ResultData::setAddressText(const QString &addressText)
{
    d->m_addressText = addressText;
}

QString ResultData::detail() const
{
    return d->m_detail;
}

void ResultData::setDetail(const QString &detail)
{
    d->m_detail = detail;
}

QGeoCoordinate ResultData::coordinate() const
{
    return d->m_coordinate;
}

void ResultData::setCoordinate(const QGeoCoordinate &coordinate)
{
    d->m_coordinate = coordinate;
}
