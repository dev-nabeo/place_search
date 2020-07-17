#ifndef RESULTDATA_H
#define RESULTDATA_H

#include <QSharedDataPointer>
#include "resultdataprivate.h"

class ResultData
{
public:
    ResultData();
    ResultData(const ResultData &other);
    ~ResultData();

    ResultData &operator=(const ResultData &other);
    bool operator==(const ResultData &other) const;
    bool operator!=(const ResultData &other) const;

    QString name() const;
    void setName(const QString &name);
    QString addressText() const;
    void setAddressText(const QString &addressText);
    QString detail() const;
    void setDetail(const QString &detail);
    QGeoCoordinate coordinate() const;
    void setCoordinate(const QGeoCoordinate &coordinate);

private:
    QSharedDataPointer<ResultDataPrivate> d;
};

#endif // RESULTDATA_H
