#pragma once

#include <QObject>
#include <QVector>

#include "core/spatial/spatialPoint.h"

class AbstractSpatialDataAdapter : public QObject
{
    Q_OBJECT

public:
    explicit AbstractSpatialDataAdapter(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    ~AbstractSpatialDataAdapter() override = default;

    virtual const QVector<SpatialPoint>& points() const = 0;


    virtual bool isEmpty() const
    {
        return points().isEmpty();
    }

signals:
    void dataChanged();
};

struct AvgAccumulator
{
    double sum = 0.0;
    int count = 0;
};
