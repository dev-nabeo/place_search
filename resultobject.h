#ifndef RESULTOBJECT_H
#define RESULTOBJECT_H

#include <QObject>
#include "resultdata.h"

class ResultObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString addressText READ addressText WRITE setAddressText NOTIFY addressTextChanged)
    Q_PROPERTY(QString detail READ detail WRITE setDetail NOTIFY detailChanged)
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged)

public:
    explicit ResultObject(QObject *parent = nullptr);
    explicit ResultObject(const ResultData &data,QObject *parent = nullptr);
    virtual ~ResultObject();

    QString name() const;
    void setName(const QString &name);
    QString addressText() const;
    void setAddressText(const QString &addressText);
    QString detail() const;
    void setDetail(const QString &detail);
    QGeoCoordinate coordinate() const;
    void setCoordinate(const QGeoCoordinate &coordinate);

    void setResultData(const ResultData &data);

signals:
    void nameChanged();
    void addressTextChanged();
    void detailChanged();
    void coordinateChanged();

private:
    QString m_name;
    QString m_addressText;
    QString m_detail;
    QGeoCoordinate m_coordinate;
};

#endif // RESULTOBJECT_H
