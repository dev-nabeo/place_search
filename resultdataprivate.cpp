#include "resultdataprivate.h"

ResultDataPrivate::ResultDataPrivate()
    :QSharedData()
{

}

ResultDataPrivate::ResultDataPrivate(const ResultDataPrivate &other)
    :QSharedData()
{
    this->m_name = other.m_name;
    this->m_addressText = other.m_addressText;
    this->m_detail = other.m_detail;
    this->m_coordinate = other.m_coordinate;
}

ResultDataPrivate::~ResultDataPrivate()
{

}

bool ResultDataPrivate::operator==(const ResultDataPrivate &other) const
{
    return(this->m_name == other.m_name
           && this->m_addressText == other.m_addressText
           && this->m_detail == other.m_detail
           && this->m_coordinate == other.m_coordinate);
}

bool ResultDataPrivate::isEmpty() const
{
    return(m_name.isEmpty()
           && m_addressText.isEmpty()
           && m_detail.isEmpty()
           && m_coordinate.isValid());
}
