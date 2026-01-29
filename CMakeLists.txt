cmake_minimum_required(VERSION 3.19)

project(GeoMagnSurvey LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(GeoMagnSurvey
    WIN32 MACOSX_BUNDLE

    # ---- entry point ----
    main.cpp

    # ---- Commands ----
    core/utils/commands.h
    core/utils/commands.cpp

    # ---- UI ----
    ui/mainWidget.h
    ui/mainWidget.cpp
    ui/mainWidget.ui

    # ---- Storage ----
    core/storage/dataStorage.h
    core/storage/dataStorage.cpp

    # ---- Data types ----
    core/datatypes/geoCoordinate.h
    core/datatypes/measurementPoint.h
    core/datatypes/gpsMeasurement.h
    core/datatypes/magneticMeasurement.h
    core/datatypes/referenceReliefPoint.h

    # ---- Models ----
    core/models/GPSMeasurementModel.h
    core/models/GPSMeasurementModel.cpp
    core/models/magneticMeasurementModel.h
    core/models/magneticMeasurementModel.cpp
    core/models/referenceReliefModel.h
    core/models/referenceReliefModel.cpp
    core/spatial/abstractSpatialDataAdapter.h
    core/spatial/spatialPoint.h
    core/spatial/magneticSpatialDataAdapter.h core/spatial/magneticSpatialDataAdapter.cpp
    core/spatial/GPSSpatialDataAdapter.h core/spatial/GPSSpatialDataAdapter.cpp
    ui/views/mapViews/abstractHeatMapWidget.h
    ui/views/mapViews/abstractHeatMapWidget.cpp
    ui/views/mapViews/GPSHeatMapWidget.h ui/views/mapViews/GPSHeatMapWidget.cpp
    ui/views/mapViews/magneticHeatMapWidget.h ui/views/mapViews/magneticHeatMapWidget.cpp

)

qt_add_translations(
    TARGETS GeoMagnSurvey
    TS_FILES GeoMagnSurvey_ru_RU.ts
)

target_link_libraries(GeoMagnSurvey
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# Чтобы include работали красиво
target_include_directories(GeoMagnSurvey
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

include(GNUInstallDirs)

install(TARGETS GeoMagnSurvey
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET GeoMagnSurvey
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
