#pragma once

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>

#include "core/models/GPSMeasurementModel.h"
#include "core/models/magneticMeasurementModel.h"
#include "core/models/referenceReliefModel.h"
#include "core/storage/dataStorage.h"

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

private:
    DataStorage* storage_;

    GPSMeasurementModel* gpsModel_;
    MagneticMeasurementModel* magneticModel_;
    ReferenceReliefModel* reliefModel_;

    QTableView* gpsTableView_;
    QTableView* magneticTableView_;
    QTableView* reliefTableView_;

    void setupUI();
};
