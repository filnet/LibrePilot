/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "pfdqmlgadgetwidget.h"
#include "extensionsystem/pluginmanager.h"
#include "uavobjectmanager.h"
#include "uavobject.h"
#include "utils/svgimageprovider.h"

#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>

PfdQmlGadgetWidget::PfdQmlGadgetWidget(QWidget *parent) :
    QQuickWidget(parent),
    m_speedUnit("m/s"),
    m_speedFactor(1.0),
    m_altitudeUnit("m"),
    m_altitudeFactor(1.0),
    m_terrainEnabled(false),
    m_terrainFile(""),
    m_positionMode(Pfd::Predefined),
    m_latitude(46.671478),
    m_longitude(10.158932),
    m_altitude(2000),
    m_timeMode(Pfd::Local),
    m_dateTime(QDateTime()),
    m_minAmbientLight(0.03),
    m_modelFile(""),
    m_backgroundImageFile("")
{
    qDebug() << "PfdQmlGadgetWidget - <init>";
    setResizeMode(SizeRootObjectToView);

    // setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    QStringList objectsToExport;
    objectsToExport <<
        "VelocityState" <<
        "PositionState" <<
        "AttitudeState" <<
        "AccelState" <<
        "VelocityDesired" <<
        "PathDesired" <<
        "GPSPositionSensor" <<
        "HomeLocation" <<
        "GCSTelemetryStats" <<
        "SystemAlarms" <<
        "NedAccel" <<
        "FlightBatteryState" <<
        "ActuatorDesired" <<
        "TakeOffLocation" <<
        "PathPlan" <<
        "WaypointActive" <<
        "OPLinkStatus" <<
        "FlightStatus" <<
        "SystemStats" <<
        "StabilizationDesired" <<
        "VtolPathFollowerSettings" <<
        "HwSettings" <<
        "ManualControlCommand" <<
        "SystemSettings" <<
        "RevoSettings" <<
        "MagState" <<
        "FlightBatterySettings";

    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();

    foreach(const QString &objectName, objectsToExport) {
        UAVObject *object = objManager->getObject(objectName);

        if (object) {
            engine()->rootContext()->setContextProperty(objectName, object);
        } else {
            qWarning() << "Failed to load object" << objectName;
        }
    }

#if 0
    qDebug() << "is OpenGLContext persistent" << isPersistentOpenGLContext();
    // window->setPersistentOpenGLContext(!window->isPersistentOpenGLContext());
    qDebug() << "is SceneGraph persistent" << isPersistentSceneGraph();
#endif

    // to expose settings values
    engine()->rootContext()->setContextProperty("qmlWidget", this);

    connect(this, SIGNAL(statusChanged(QQuickWidget::Status)), this, SLOT(onStatusChanged(QQuickWidget::Status)));
}

PfdQmlGadgetWidget::~PfdQmlGadgetWidget()
{}

void PfdQmlGadgetWidget::setQmlFile(QString fn)
{
    if (m_qmlFileName == fn) {
        return;
    }
    qDebug() << Q_FUNC_INFO << fn;

    m_qmlFileName = fn;

    if (fn.isEmpty()) {
        setSource(QUrl());

        engine()->removeImageProvider("svg");
        // engine()->rootContext()->setContextProperty("svgRenderer", NULL);

        // calling clearComponentCache() causes crashes (see https://bugreports.qt-project.org/browse/QTBUG-41465)
        // but not doing it causes almost systematic crashes when switching PFD gadget to "Model View (Without Terrain)" configuration
        engine()->clearComponentCache();
    } else {
        SvgImageProvider *svgProvider = new SvgImageProvider(fn);
        engine()->addImageProvider("svg", svgProvider);

        // it's necessary to allow qml side to query svg element position
        engine()->rootContext()->setContextProperty("svgRenderer", svgProvider);

        QUrl url = QUrl::fromLocalFile(fn);
        engine()->setBaseUrl(url);
        setSource(url);
    }

    foreach(const QQmlError &error, errors()) {
        qDebug() << error.description();
    }
}

void PfdQmlGadgetWidget::onStatusChanged(QQuickWidget::Status status)
{
    switch (status) {
    case Null:
        qDebug() << "PfdQmlGadgetWidget - status Null";
        break;
    case Ready:
        qDebug() << "PfdQmlGadgetWidget - status Ready";
        break;
    case Loading:
        qDebug() << "PfdQmlGadgetWidget - status Loading";
        break;
    case Error:
        qDebug() << "PfdQmlGadgetWidget - status Error";
        foreach(const QQmlError &error, errors()) {
            qDebug() << error.description();
        }
        break;
    }
}

void PfdQmlGadgetWidget::setSpeedUnit(QString unit)
{
    if (m_speedUnit != unit) {
        m_speedUnit = unit;
        emit speedUnitChanged(speedUnit());
    }
}

void PfdQmlGadgetWidget::setSpeedFactor(double factor)
{
    if (m_speedFactor != factor) {
        m_speedFactor = factor;
        emit speedFactorChanged(speedFactor());
    }
}

void PfdQmlGadgetWidget::setAltitudeUnit(QString unit)
{
    if (m_altitudeUnit != unit) {
        m_altitudeUnit = unit;
        emit altitudeUnitChanged(altitudeUnit());
    }
}

void PfdQmlGadgetWidget::setAltitudeFactor(double factor)
{
    if (m_altitudeFactor != factor) {
        m_altitudeFactor = factor;
        emit altitudeFactorChanged(altitudeFactor());
    }
}

void PfdQmlGadgetWidget::setTerrainEnabled(bool arg)
{
    if (m_terrainEnabled != arg) {
        m_terrainEnabled = arg;
        qDebug() << "PfdQmlGadgetWidget - setTerrainEnabled" << terrainEnabled();
        emit terrainEnabledChanged(terrainEnabled());
    }
}

void PfdQmlGadgetWidget::setTerrainFile(const QString &arg)
{
    if (m_terrainFile != arg) {
        m_terrainFile = arg;
        qDebug() << "PfdQmlGadgetWidget - setTerrainFile" << terrainFile();
        emit terrainFileChanged(terrainFile());
    }
}

void PfdQmlGadgetWidget::setPositionMode(Pfd::PositionMode arg)
{
    if (m_positionMode != arg) {
        m_positionMode = arg;
        emit positionModeChanged(positionMode());
    }
}

void PfdQmlGadgetWidget::setLatitude(double arg)
{
    if (m_latitude != arg) {
        m_latitude = arg;
        emit latitudeChanged(latitude());
    }
}

void PfdQmlGadgetWidget::setLongitude(double arg)
{
    if (m_longitude != arg) {
        m_longitude = arg;
        emit longitudeChanged(longitude());
    }
}

void PfdQmlGadgetWidget::setAltitude(double arg)
{
    if (m_altitude != arg) {
        m_altitude = arg;
        emit altitudeChanged(altitude());
    }
}

void PfdQmlGadgetWidget::setTimeMode(Pfd::TimeMode arg)
{
    if (m_timeMode != arg) {
        m_timeMode = arg;
        emit timeModeChanged(timeMode());
    }
}

void PfdQmlGadgetWidget::setDateTime(QDateTime arg)
{
    if (m_dateTime != arg) {
        m_dateTime = arg;
        emit dateTimeChanged(dateTime());
    }
}

void PfdQmlGadgetWidget::setMinimumAmbientLight(double arg)
{
    if (m_minAmbientLight != arg) {
        m_minAmbientLight = arg;
        emit minimumAmbientLightChanged(minimumAmbientLight());
    }
}

void PfdQmlGadgetWidget::setModelFile(const QString &arg)
{
    if (m_modelFile != arg) {
        m_modelFile = arg;
        qDebug() << "PfdQmlGadgetWidget - setModelFile" << modelFile();
        emit modelFileChanged(modelFile());
    }
}

void PfdQmlGadgetWidget::setBackgroundImageFile(const QString &arg)
{
    if (m_backgroundImageFile != arg) {
        m_backgroundImageFile = arg;
        qDebug() << "PfdQmlGadgetWidget - setBackgroundImageFile" << backgroundImageFile();
        emit backgroundImageFileChanged(backgroundImageFile());
    }
}
