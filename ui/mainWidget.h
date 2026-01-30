#pragma once

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include <QUndoStack>

#include "core/models/GPSMeasurementModel.h"
#include "core/models/magneticMeasurementModel.h"
#include "core/models/referenceReliefModel.h"
#include "core/storage/dataStorage.h"

#include "core/utils/commands.h"

#include "core/spatial/GPSSpatialDataAdapter.h"
#include "core/spatial/magneticSpatialDataAdapter.h"


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private slots:
    void onAddGPS();
    void onRemoveGPS();
    void onAddMagnetic();
    void onRemoveMagnetic();
    void onAddRelief();
    void onRemoveRelief();
    void onGPSDataChanged(int row, const GPSMeasurement& oldMeasurement, const GPSMeasurement& newMeasurement);
    void onMagneticDataChanged(int row, const MagneticMeasurement& oldMeasurement, const MagneticMeasurement& newMeasurement);
    void onReliefDataChanged(int row, const ReferenceReliefPoint& oldPoint, const ReferenceReliefPoint& newPoint);

private:


    DataStorage* storage_;
    QUndoStack* undoStack_;

    GPSMeasurementModel* gpsModel_;
    MagneticMeasurementModel* magneticModel_;
    ReferenceReliefModel* reliefModel_;

    QTableView* gpsTableView_;
    QTableView* magneticTableView_;
    QTableView* reliefTableView_;

    // Map widgets/adapters are created lazily when user requests a map window

    void setupUI();
    void addTestData();
    void blockAllModelsSignals();
    void unlockAllModelsSignals();
    void showGPSMap();
    void showMagneticMap();

};
