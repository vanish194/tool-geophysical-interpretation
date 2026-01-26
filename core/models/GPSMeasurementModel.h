#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include "core/storage/dataStorage.h"

class GPSMeasurementModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GPSMeasurementModel(DataStorage* storage, QObject* parent = nullptr);
    ~GPSMeasurementModel();

    // QAbstractTableModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Custom methods
    bool addMeasurement(const GPSMeasurement& measurement);
    bool removeMeasurement(int row);
    bool updateMeasurement(int row, const GPSMeasurement& measurement);
    GPSMeasurement getMeasurement(int row) const;

    enum Columns {
        ColProfile = 0,
        ColPicket,
        ColLatitude,
        ColLongitude,
        ColAltitude,
        ColTimestamp,
        ColSource,
        ColumnCount
    };

private slots:
    void onDataChanged();

private:
    DataStorage* storage_;
    QString sourceToString(GPSMeasurement::Source source) const;
    GPSMeasurement::Source stringToSource(const QString& str) const;
};
