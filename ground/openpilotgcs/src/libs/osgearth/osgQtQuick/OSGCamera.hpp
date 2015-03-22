#ifndef _H_OSGQTQUICK_OSGCAMERA_H_
#define _H_OSGQTQUICK_OSGCAMERA_H_

#include "Export.hpp"

#include <QObject>
#include <QVector3D>

namespace osgViewer {
class View;
} // namespace osgViewer

namespace osgQtQuick {
class OSGNode;

// This class does too much:
// - tracking a geo point and attitude
// - tracking another node
// camera should be simpler and provide only tracking
// - tracking of a modelnode (for ModelView)
// - tracking of a virtual node  (for PFD with Terrain)
//
// TODO
// - expose track mode
// - provide good default distance and attitude for tracker camera
class OSGQTQUICK_EXPORT OSGCamera : public QObject {
    Q_OBJECT Q_PROPERTY(qreal fieldOfView READ fieldOfView WRITE setFieldOfView NOTIFY fieldOfViewChanged)

    Q_PROPERTY(ManipulatorMode manipulatorMode READ manipulatorMode WRITE setManipulatorMode NOTIFY manipulatorModeChanged)

    Q_PROPERTY(osgQtQuick::OSGNode * node READ node WRITE setNode NOTIFY nodeChanged)

    Q_PROPERTY(osgQtQuick::OSGNode * trackNode READ trackNode WRITE setTrackNode NOTIFY trackNodeChanged)
    Q_PROPERTY(TrackerMode trackerMode READ trackerMode WRITE setTrackerMode NOTIFY trackerModeChanged)

    Q_PROPERTY(QVector3D attitude READ attitude WRITE setAttitude NOTIFY attitudeChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

    Q_ENUMS(ManipulatorMode)
    Q_ENUMS(TrackerMode)

public:
    enum ManipulatorMode { Default, Earth, Track, User };

    enum TrackerMode { NodeCenter, NodeCenterAndAzim, NodeCenterAndRotation };

    explicit OSGCamera(QObject *parent = 0);
    virtual ~OSGCamera();

    // fov depends on the scenery space (probaby distance)
    // here are some value: 75�, 60�, 45� many gamers use
    // x-plane uses 45� for 4:3 and 60� for 16:9/16:10
    // flightgear uses 55� / 70�
    qreal fieldOfView() const;
    void setFieldOfView(qreal arg);

    ManipulatorMode manipulatorMode() const;
    void setManipulatorMode(ManipulatorMode);

    OSGNode *node() const;
    void setNode(OSGNode *node);

    OSGNode *trackNode() const;
    void setTrackNode(OSGNode *node);

    TrackerMode trackerMode() const;
    void setTrackerMode(TrackerMode);

    QVector3D attitude() const;
    void setAttitude(QVector3D arg);

    QVector3D position() const;
    void setPosition(QVector3D arg);

    void installCamera(osgViewer::View *view);
    void setViewport(int x, int y, int width, int height);

signals:
    void fieldOfViewChanged(qreal arg);

    void manipulatorModeChanged(ManipulatorMode);

    void nodeChanged(OSGNode *node);

    void trackNodeChanged(OSGNode *node);
    void trackerModeChanged(TrackerMode);

    void attitudeChanged(QVector3D arg);
    void positionChanged(QVector3D arg);

private:
    struct Hidden;
    Hidden *h;
};
} // namespace osgQtQuick

#endif // _H_OSGQTQUICK_OSGCAMERA_H_