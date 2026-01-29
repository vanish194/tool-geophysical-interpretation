#pragma once

#include <QWidget>
#include "core/spatial/abstractSpatialDataAdapter.h"

class AbstractHeatMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractHeatMapWidget(AbstractSpatialDataAdapter* adapter,
                                   QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

    // --- переопределяется в потомках ---
    virtual QColor colorAt(double t) const = 0; // t ∈ [0..1]
    virtual QString units() const;

private slots:
    void onDataChanged();

private:
    void drawHeatMap(QPainter& p, const QRect& area);
    void drawLegend(QPainter& p, const QRect& area);

private:
    AbstractSpatialDataAdapter* adapter_ = nullptr;

    double minValue_ = 0.0;
    double maxValue_ = 0.0;
};
