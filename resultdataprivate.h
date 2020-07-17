#ifndef RESULTDATAPRIVATE_H
#define RESULTDATAPRIVATE_H

#include <QSharedDataPointer>
#include <QtPositioning/QGeoCoordinate>

class ResultDataPrivate : public QSharedData
{
public:
    ResultDataPrivate();
    ResultDataPrivate(const ResultDataPrivate &other);
    ~ResultDataPrivate();
    bool operator==(const ResultDataPrivate &other) const;
    bool isEmpty() const;

    QString m_name;
    QString m_addressText;
    QString m_detail;
    QGeoCoordinate m_coordinate;
};

#endif // RESULTDATAPRIVATE_H
