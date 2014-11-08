#ifndef _H_OSGQTQUICK_OSGCAMERA_H_
#define _H_OSGQTQUICK_OSGCAMERA_H_

#include "osgQtQuick/Export.hpp"

#include <QObject>

//namespace osg {
//class Node;
//} // namespace osg

namespace osgQtQuick {

// TODO split into Camera and CameraManipulator
class OSGQTQUICK_EXPORT OSGCamera : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal fieldOfView READ fieldOfView WRITE setFieldOfView NOTIFY fieldOfViewChanged)

    Q_PROPERTY(qreal roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(qreal yaw READ yaw WRITE setYaw NOTIFY yawChanged)

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)

public:
    explicit OSGCamera(QObject *parent = 0);
    ~OSGCamera();
    
    qreal fieldOfView() const;
    void setFieldOfView(qreal arg);

    qreal roll() const;
    void setRoll(qreal arg);

    qreal pitch() const;
    void setPitch(qreal arg);

    qreal yaw() const;
    void setYaw(qreal arg);

    double latitude() const;
    void setLatitude(double arg);

    double longitude() const;
    void setLongitude(double arg);

    double altitude() const;
    void setAltitude(double arg);

signals:
    void fieldOfViewChanged(qreal arg);

    void rollChanged(qreal arg);
    void pitchChanged(qreal arg);
    void yawChanged(qreal arg);

    void latitudeChanged(double arg);
    void longitudeChanged(double arg);
    void altitudeChanged(double arg);

//    osg::Node* node();
//    void setNode(osg::Node *node);
//    void nodeChanged(osg::Node *node);
    
private:
    struct Hidden;
    Hidden *h;
};

} // namespace osgQtQuick

#endif // _H_OSGQTQUICK_OSGCAMERA_H_