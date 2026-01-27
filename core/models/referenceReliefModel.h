#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include "core/storage/dataStorage.h"

class ReferenceReliefModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ReferenceReliefModel(DataStorage* storage, QObject* parent = nullptr);
    ~ReferenceReliefModel();

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
    bool addPoint(const ReferenceReliefPoint& point);
    bool removePoint(int row);
    bool updatePoint(int row, const ReferenceReliefPoint& point);
    ReferenceReliefPoint getPoint(int row) const;

    enum Columns {
        ColLatitude = 0,
        ColLongitude,
        ColElevation,
        ColSource,
        ColumnCount
    };

signals:
    void measurementUpdated(int row, const ReferenceReliefPoint& oldPoint, const ReferenceReliefPoint& newPoint);

private slots:
    void onDataChanged();

private:
    DataStorage* storage_;
    QString sourceToString(ReferenceReliefPoint::Source source) const;
    ReferenceReliefPoint::Source stringToSource(const QString& str) const;
};
