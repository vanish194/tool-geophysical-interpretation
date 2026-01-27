#pragma once

#include "core/datatypes/measurementPoint.h"
#include "core/datatypes/geoCoordinate.h"

#include <QDateTime>

class MagneticMeasurement
{
public:
    enum class Source
    {
        Unknown,
        Field_Sensor,
        Reference_Sensor,
        Manual
    };

    MagneticMeasurement() = default;

    MagneticMeasurement(double value, const QDateTime& timestamp)
        : value_(value)
        , source_(Source::Field_Sensor)
        , timestamp_(timestamp)
    {}

    MagneticMeasurement(const MeasurementPoint& point,
                        double value,
                        Source source,
                        const QDateTime& timestamp)
        : point_(point)
        , value_(value)
        , source_(source)
        , timestamp_(timestamp)
    {}

    const MeasurementPoint& point() const noexcept { return point_; }
    double value() const noexcept { return value_; }
    Source source() const noexcept { return source_; }
    const QDateTime& timestamp() const noexcept { return timestamp_; }

    bool setPoint(const MeasurementPoint& point) noexcept
    {
        point_ = point;
        return true;
    }

    bool setValue(double value) noexcept
    {
        value_ = value;
        return true;
    }

    bool setSource(Source source) noexcept
    {
        source_ = source;
        return true;
    }

    bool setTimestamp(const QDateTime& timestamp) noexcept
    {
        timestamp_ = timestamp;
        return true;
    }

private:
    MeasurementPoint point_;
    double value_ = 0.0;
    Source source_ = Source::Unknown;
    QDateTime timestamp_;
};
