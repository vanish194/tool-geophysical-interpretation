#pragma once

#include "core/datatypes/measurementPoint.h"
#include "core/datatypes/geoCoordinate.h"

#include <QDateTime>
#include <utility>

class GPSMeasurement
{
public:
    enum class Source
    {
        Unknown,
        GPS_Receiver,
        Manual
    };

    GPSMeasurement(const MeasurementPoint& point,
                   const GeoCoordinate& coordinate,
                   const QDateTime& timestamp,
                   Source source = Source::Unknown)
        : point_(point)
        , coordinate_(coordinate)
        , timestamp_(timestamp)
        , source_(source)
    {}

    const MeasurementPoint& point() const noexcept { return point_; }
    const GeoCoordinate& coordinate() const noexcept { return coordinate_; }
    const QDateTime& timestamp() const noexcept { return timestamp_; }
    Source source() const noexcept { return source_; }

    bool setPoint(const MeasurementPoint& point) noexcept
    {
        point_ = point;
        return true;
    }

    bool setCoordinate(const GeoCoordinate& coordinate) noexcept
    {
        coordinate_ = coordinate;
        return true;
    }

    bool setTimestamp(const QDateTime& timestamp) noexcept
    {
        timestamp_ = timestamp;
        return true;
    }

    bool setSource(Source source) noexcept
    {
        source_ = source;
        return true;
    }

private:
    MeasurementPoint point_;
    GeoCoordinate coordinate_;
    QDateTime timestamp_;
    Source source_ = Source::Unknown;
};


