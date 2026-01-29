#include "core/models/referenceReliefModel.h"
#include <QColor>
#include <QDebug>

ReferenceReliefModel::ReferenceReliefModel(DataStorage* storage, QObject* parent)
    : QAbstractTableModel(parent)
    , storage_(storage)
{
    connect(storage_, &DataStorage::reliefChanged,
            this, &ReferenceReliefModel::onDataChanged);
}

ReferenceReliefModel::~ReferenceReliefModel()
{
}

int ReferenceReliefModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(storage_->relief().size());
}

int ReferenceReliefModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant ReferenceReliefModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount())
        return QVariant();

    const auto& points = storage_->relief();
    if (index.row() >= static_cast<int>(points.size()))
        return QVariant();

    const auto& point = points[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case ColLatitude:
            return point.coordinate().latitude();
        case ColLongitude:
            return point.coordinate().longitude();
        case ColElevation:
            return point.elevation();
        case ColSource:
            return sourceToString(point.source());
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    else if (role == Qt::BackgroundRole) {
        if (point.source() == ReferenceReliefPoint::Source::Unknown) {
            return QColor(255, 200, 200); // Light red for unknown
        }
    }

    return QVariant();
}

QVariant ReferenceReliefModel::headerData(int section, Qt::Orientation orientation,
                                          int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case ColLatitude:   return tr("Latitude");
        case ColLongitude:  return tr("Longitude");
        case ColElevation:  return tr("Elevation");
        case ColSource:     return tr("Source");
        default:            return QVariant();
        }
    }

    return section + 1;
}

bool ReferenceReliefModel::setData(const QModelIndex& index, const QVariant& value,
                                   int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() >= rowCount() || index.column() >= columnCount())
        return false;

    auto oldPoint = storage_->reliefAt(index.row());
    auto point = oldPoint;
    bool success = false;

    switch (index.column()) {
    case ColLatitude: {
        double latitude = value.toDouble(&success);
        if (success) {
            GeoCoordinate coord = point.coordinate();
            success = coord.setLatitude(latitude);
            if (success) {
                point.setCoordinate(coord);
            }
        }
        break;
    }
    case ColLongitude: {
        double longitude = value.toDouble(&success);
        if (success) {
            GeoCoordinate coord = point.coordinate();
            success = coord.setLongitude(longitude);
            if (success) {
                point.setCoordinate(coord);
            }
        }
        break;
    }
    case ColElevation: {
        double elevation = value.toDouble(&success);
        if (success) {
            point.setElevation(elevation);
        }
        break;
    }
    case ColSource: {
        auto source = stringToSource(value.toString());
        point.setSource(source);
        success = true;
        break;
    }
    }

    if (success) {
        emit measurementUpdated(index.row(), oldPoint, point);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags ReferenceReliefModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (index.isValid()) {
        flags |= Qt::ItemIsEditable;
        flags |= Qt::ItemIsSelectable;
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

bool ReferenceReliefModel::addPoint(const ReferenceReliefPoint& point)
{
    const int oldSize = rowCount();
    beginInsertRows(QModelIndex(), oldSize, oldSize);
    storage_->blockSignals(true);
    storage_->addRelief(point);
    storage_->blockSignals(false);
    endInsertRows();

    if (rowCount() != oldSize + 1) {
        qWarning() << "ReferenceReliefModel::addPoint - insert failed: size mismatch";
        return false;
    }

    return true;
}

bool ReferenceReliefModel::removePoint(int row)
{
    if (row < 0 || row >= rowCount())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    storage_->blockSignals(true);
    bool success = storage_->removeRelief(row);
    storage_->blockSignals(false);
    endRemoveRows();

    if (!success) {
        qWarning() << "ReferenceReliefModel::removePoint - storage failed to remove row" << row;
    }

    return success;
}

bool ReferenceReliefModel::updatePoint(int row, const ReferenceReliefPoint& point)
{
    if (row < 0 || row >= rowCount())
        return false;

    storage_->blockSignals(true);
    bool success = storage_->updateRelief(row, point);
    storage_->blockSignals(false);
    if (success) {
        emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    }

    return success;
}

ReferenceReliefPoint ReferenceReliefModel::getPoint(int row) const
{
    if (row < 0 || row >= rowCount())
        return ReferenceReliefPoint(GeoCoordinate(), 0.0);

    return storage_->reliefAt(row);
}

void ReferenceReliefModel::onDataChanged()
{
    beginResetModel();
    endResetModel();
}

QString ReferenceReliefModel::sourceToString(ReferenceReliefPoint::Source source) const
{
    switch (source) {
    case ReferenceReliefPoint::Source::Survey: return tr("Survey");
    case ReferenceReliefPoint::Source::Unknown: return tr("Unknown");
    default: return tr("Unknown");
    }
}

ReferenceReliefPoint::Source ReferenceReliefModel::stringToSource(const QString& str) const
{
    if (str == tr("Survey")) return ReferenceReliefPoint::Source::Survey;
    return ReferenceReliefPoint::Source::Unknown;
}
