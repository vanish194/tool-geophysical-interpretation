#pragma once

#include "core/datatypes/geoCoordinate.h"

#include <utility>

class ReferenceReliefPoint
{
public:
    enum class Source
    {
        Unknown,
        Survey
    };

    ReferenceReliefPoint(const GeoCoordinate& coordinate,
                         double elevation,
                         Source source = Source::Survey)
        : coordinate_(coordinate)
        , elevation_(elevation)
        , source_(source)
    {}

    const GeoCoordinate& coordinate() const noexcept { return coordinate_; }
    double elevation() const noexcept { return elevation_; }
    Source source() const noexcept { return source_; }

    bool setCoordinate(const GeoCoordinate& coordinate) noexcept
    {
        coordinate_ = coordinate;
        return true;
    }

    bool setElevation(double elevation) noexcept
    {
        elevation_ = elevation;
        return true;
    }

    bool setSource(Source source) noexcept
    {
        source_ = source;
        return true;
    }

private:
    GeoCoordinate coordinate_;
    double elevation_ = 0.0;
    Source source_ = Source::Unknown;
};
