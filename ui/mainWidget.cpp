#include "mainWidget.h"
#include <QHeaderView>

#include <QtMath>//for addTestData()
#include <vector>


MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , storage_(new DataStorage(this))
    , undoStack_(new QUndoStack(this))
    , gpsModel_(new GPSMeasurementModel(storage_, this))
    , magneticModel_(new MagneticMeasurementModel(storage_, this))
    , reliefModel_(new ReferenceReliefModel(storage_, this))
{
    gpsSpatialAdapter_ =
        new GPSSpatialDataAdapter(gpsModel_, this);

    magneticSpatialAdapter_ =
        new MagneticSpatialDataAdapter(magneticModel_, this);

    gpsHeatMap_ =
        new GPSHeatMapWidget(gpsSpatialAdapter_, this);

    magneticHeatMap_ =
        new MagneticHeatMapWidget(magneticSpatialAdapter_, this);

    connect(gpsModel_, &GPSMeasurementModel::measurementUpdated, this, &MainWidget::onGPSDataChanged);
    connect(magneticModel_, &MagneticMeasurementModel::measurementUpdated, this, &MainWidget::onMagneticDataChanged);
    connect(reliefModel_, &ReferenceReliefModel::measurementUpdated, this, &MainWidget::onReliefDataChanged);
    setupUI();

    undoStack_->setUndoLimit(1000);

    addTestData();
}

MainWidget::~MainWidget()
{
}

void MainWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Undo/Redo buttons
    QHBoxLayout* undoRedoLayout = new QHBoxLayout();
    QPushButton* undoButton = new QPushButton(tr("Undo"), this);
    QPushButton* redoButton = new QPushButton(tr("Redo"), this);
    connect(undoButton, &QPushButton::clicked, undoStack_, &QUndoStack::undo);
    connect(redoButton, &QPushButton::clicked, undoStack_, &QUndoStack::redo);
    undoRedoLayout->addWidget(undoButton);
    undoRedoLayout->addWidget(redoButton);
    undoRedoLayout->addStretch();

    mainLayout->addLayout(undoRedoLayout);

    // GPS Table
    QGroupBox* gpsGroup = new QGroupBox(tr("GPS Measurements"), this);
    QHBoxLayout* gpsGroupLayout = new QHBoxLayout(gpsGroup);

    // --- левая часть: таблица ---
    QVBoxLayout* gpsLeftLayout = new QVBoxLayout();

    gpsTableView_ = new QTableView(this);
    gpsTableView_->setModel(gpsModel_);
    gpsTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    gpsTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    gpsTableView_->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout* gpsButtonLayout = new QHBoxLayout();
    QPushButton* addGPSButton = new QPushButton(tr("Add GPS"), this);
    QPushButton* removeGPSButton = new QPushButton(tr("Remove GPS"), this);

    connect(addGPSButton, &QPushButton::clicked, this, &MainWidget::onAddGPS);
    connect(removeGPSButton, &QPushButton::clicked, this, &MainWidget::onRemoveGPS);

    gpsButtonLayout->addWidget(addGPSButton);
    gpsButtonLayout->addWidget(removeGPSButton);
    gpsButtonLayout->addStretch();

    gpsLeftLayout->addWidget(gpsTableView_);
    gpsLeftLayout->addLayout(gpsButtonLayout);

    // --- правая часть: карта ---
    gpsHeatMap_->setMinimumWidth(300);

    gpsGroupLayout->addLayout(gpsLeftLayout, 2);
    gpsGroupLayout->addWidget(gpsHeatMap_, 1);


    // Magnetic Table
    QGroupBox* magneticGroup = new QGroupBox(tr("Magnetic Measurements"), this);
    QHBoxLayout* magneticGroupLayout = new QHBoxLayout(magneticGroup);

    QVBoxLayout* magneticLeftLayout = new QVBoxLayout();

    magneticTableView_ = new QTableView(this);
    magneticTableView_->setModel(magneticModel_);
    magneticTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    magneticTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    magneticTableView_->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout* magneticButtonLayout = new QHBoxLayout();
    QPushButton* addMagneticButton = new QPushButton(tr("Add Magnetic"), this);
    QPushButton* removeMagneticButton = new QPushButton(tr("Remove Magnetic"), this);

    connect(addMagneticButton, &QPushButton::clicked, this, &MainWidget::onAddMagnetic);
    connect(removeMagneticButton, &QPushButton::clicked, this, &MainWidget::onRemoveMagnetic);

    magneticButtonLayout->addWidget(addMagneticButton);
    magneticButtonLayout->addWidget(removeMagneticButton);
    magneticButtonLayout->addStretch();

    magneticLeftLayout->addWidget(magneticTableView_);
    magneticLeftLayout->addLayout(magneticButtonLayout);

    magneticHeatMap_->setMinimumWidth(300);

    magneticGroupLayout->addLayout(magneticLeftLayout, 2);
    magneticGroupLayout->addWidget(magneticHeatMap_, 1);


    // Relief Table
    QGroupBox* reliefGroup = new QGroupBox(tr("Reference Relief Points"), this);
    QVBoxLayout* reliefLayout = new QVBoxLayout(reliefGroup);

    reliefTableView_ = new QTableView(this);
    reliefTableView_->setModel(reliefModel_);
    reliefTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    reliefTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    reliefTableView_->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout* reliefButtonLayout = new QHBoxLayout();
    QPushButton* addReliefButton = new QPushButton(tr("Add Relief Point"), this);
    QPushButton* removeReliefButton = new QPushButton(tr("Remove Relief Point"), this);
    connect(addReliefButton, &QPushButton::clicked, this, &MainWidget::onAddRelief);
    connect(removeReliefButton, &QPushButton::clicked, this, &MainWidget::onRemoveRelief);

    reliefButtonLayout->addWidget(addReliefButton);
    reliefButtonLayout->addWidget(removeReliefButton);
    reliefButtonLayout->addStretch();

    reliefLayout->addWidget(reliefTableView_);
    reliefLayout->addLayout(reliefButtonLayout);

    // Add all to main layout
    mainLayout->addWidget(gpsGroup);
    mainLayout->addWidget(magneticGroup);
    mainLayout->addWidget(reliefGroup);

    setLayout(mainLayout);
    resize(800, 600);
}

void MainWidget::addTestData()
{
    const int profilesCount = 200;
    const int picketsPerProfile = 200;

    const double step = 0.0002;

    const double baseLat = 55.0;
    const double baseLon = 37.0;
    const double baseAlt = 120.0;

    const QDateTime baseTime = QDateTime::currentDateTime();

    // центры аномалий (профиль, пикет)
    const QVector<QPointF> anomalies = {
        {100, 100},   // центр
        {40, 60},     // левая верхняя
        {160, 140}    // правая нижняя
    };

    const double anomalyRadius = 25.0;

    std::vector<GPSMeasurement> gpsVec;
    std::vector<MagneticMeasurement> magneticVec;
    std::vector<ReferenceReliefPoint> reliefVec;
    gpsVec.reserve(profilesCount * picketsPerProfile);
    magneticVec.reserve(profilesCount * picketsPerProfile);
    reliefVec.reserve(profilesCount * picketsPerProfile);

    for (int profile = 0; profile < profilesCount; ++profile) {
        for (int picket = 0; picket < picketsPerProfile; ++picket) {

            const double lat = baseLat + profile * step;
            const double lon = baseLon + picket * step;

            // ---------- разлом ----------
            const double faultShift = (profile > 100) ? 20.0 : 0.0;

            // ---------- аномалии ----------
            double anomalyValue = 0.0;
            for (const auto& c : anomalies) {
                const double dx = picket - c.x();
                const double dy = profile - c.y();
                const double r2 = dx * dx + dy * dy;
                anomalyValue += qExp(-r2 / (2.0 * anomalyRadius * anomalyRadius)) * 60.0;
            }

            // ---------- GPS ----------
            GPSMeasurement gpsMeas(
                MeasurementPoint(profile + 1, picket + 1),
                GeoCoordinate(
                    lat,
                    lon,
                    baseAlt
                        + profile * 0.1
                        + faultShift
                    ),
                baseTime.addSecs(profile * picketsPerProfile + picket),
                GPSMeasurement::Source::GPS_Receiver
                );
            gpsVec.push_back(gpsMeas);

            // ---------- Magnetic ----------
            MagneticMeasurement magneticMeas(
                MeasurementPoint(profile + 1, picket + 1),
                40.0
                    + picket * 0.15
                    - profile * 0.1
                    + qSin(picket * 0.1) * 8.0
                    + faultShift * 1.5
                    + anomalyValue,
                MagneticMeasurement::Source::Field_Sensor,
                baseTime.addSecs(profile * picketsPerProfile + picket)
                );
            magneticVec.push_back(magneticMeas);

            // ---------- Relief ----------
            const double reliefValue =
                baseAlt
                + profile * 0.2
                + qSin(picket * 0.05) * 5.0
                + faultShift;

            ReferenceReliefPoint reliefMeas(
                GeoCoordinate(lat, lon, reliefValue),
                reliefValue,
                ReferenceReliefPoint::Source::Survey
                );
            reliefVec.push_back(reliefMeas);
        }
    }

    // Bulk-insert into storage to minimize signals and model resets
    storage_->addGPSBatch(gpsVec);
    storage_->addMagneticBatch(magneticVec);
    storage_->addReliefBatch(reliefVec);
}

void MainWidget::blockAllModelsSignals()
{
    magneticModel_->blockSignals(true);
    gpsModel_->blockSignals(true);
    reliefModel_->blockSignals(true);

}

void MainWidget::unlockAllModelsSignals()
{
    magneticModel_->blockSignals(false);
    gpsModel_->blockSignals(false);
    reliefModel_->blockSignals(false);
}



void MainWidget::onAddGPS()
{
    // Создаем диалог для ввода данных GPS
    GPSMeasurement gps(
        MeasurementPoint(1, 1),
        GeoCoordinate(0.0, 0.0, 0.0),
        QDateTime::currentDateTime(),
        GPSMeasurement::Source::Manual
        );
    undoStack_->push(new AddGPSCommand(storage_, gps));
}

void MainWidget::onRemoveGPS()
{
    QModelIndexList selected = gpsTableView_->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a row to remove"));
        return;
    }

    int row = selected.first().row();
    undoStack_->push(new RemoveGPSCommand(storage_, row));
}

void MainWidget::onAddMagnetic()
{
    MagneticMeasurement magnetic(
        MeasurementPoint(1, 1),
        0.0,
        MagneticMeasurement::Source::Manual,
        QDateTime::currentDateTime()
        );
    undoStack_->push(new AddMagneticCommand(storage_, magnetic));
}

void MainWidget::onRemoveMagnetic()
{
    QModelIndexList selected = magneticTableView_->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a row to remove"));
        return;
    }

    int row = selected.first().row();
    undoStack_->push(new RemoveMagneticCommand(storage_, row));
}

void MainWidget::onAddRelief()
{
    ReferenceReliefPoint relief(
        GeoCoordinate(0.0, 0.0, 0.0),
        0.0,
        ReferenceReliefPoint::Source::Survey
        );
    undoStack_->push(new AddReliefCommand(storage_, relief));
}

void MainWidget::onRemoveRelief()
{
    QModelIndexList selected = reliefTableView_->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a row to remove"));
        return;
    }

    int row = selected.first().row();
    undoStack_->push(new RemoveReliefCommand(storage_, row));
}

void MainWidget::onGPSDataChanged(int row, const GPSMeasurement& oldMeasurement, const GPSMeasurement& newMeasurement)
{
    undoStack_->push(new UpdateGPSCommand(storage_, row, oldMeasurement, newMeasurement));
}

void MainWidget::onMagneticDataChanged(int row, const MagneticMeasurement& oldMeasurement, const MagneticMeasurement& newMeasurement)
{
    undoStack_->push(new UpdateMagneticCommand(storage_, row, oldMeasurement, newMeasurement));
}

void MainWidget::onReliefDataChanged(int row, const ReferenceReliefPoint& oldPoint, const ReferenceReliefPoint& newPoint)
{
    undoStack_->push(new UpdateReliefCommand(storage_, row, oldPoint, newPoint));
}
