#include "GPSSpatialDataAdapter.h"

GPSSpatialDataAdapter::GPSSpatialDataAdapter(
    GPSMeasurementModel* model,
    QObject* parent)
    : AbstractSpatialDataAdapter(parent)
    , model_(model)
{
    Q_ASSERT(model_);
    //НУЖНО ПЕРЕСТАВИТЬ МОМЕНТ ОТРИСОВКИ
    connect(model_, &QAbstractItemModel::dataChanged,
            this, &GPSSpatialDataAdapter::rebuild);
    connect(model_, &QAbstractItemModel::modelReset,
            this, &GPSSpatialDataAdapter::rebuild);
    connect(model_, &QAbstractItemModel::rowsInserted,
            this, &GPSSpatialDataAdapter::rebuild);
    connect(model_, &QAbstractItemModel::rowsRemoved,
            this, &GPSSpatialDataAdapter::rebuild);

    rebuild();
}

const QVector<SpatialPoint>& GPSSpatialDataAdapter::points() const
{
    return points_;
}

void GPSSpatialDataAdapter::rebuild()
{
    points_.clear();

    QMap<QPair<int, int>, AvgAccumulator> acc;

    for (int row = 0; row < model_->rowCount(); ++row) {
        const auto m = model_->getMeasurement(row);

        QPair<int, int> key{
            m.point().profile(),
            m.point().picket()
        };

        auto& cell = acc[key];
        cell.sum += m.coordinate().altitude();
        cell.count++;
    }

    points_.reserve(acc.size());

    for (auto it = acc.begin(); it != acc.end(); ++it) {
        SpatialPoint p;
        p.x = it.key().first;
        p.y = it.key().second;
        p.value = it.value().sum / it.value().count;

        points_.push_back(p);
    }

    emit dataChanged();
}
