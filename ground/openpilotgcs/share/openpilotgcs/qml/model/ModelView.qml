import QtQuick 2.4
import osgQtQuick 1.0
import PfdQmlEnums 1.0

OSGViewport {
    anchors.fill: parent
    focus: true
    sceneData: transformNode
    camera: camera

    OSGTransformNode {
        id: transformNode
        modelData: fileNode
        rotate: Qt.vector3d(AttitudeState.Pitch, AttitudeState.Roll, -AttitudeState.Yaw)
    }

    OSGFileNode {
        id: fileNode
        source: qmlWidget.modelFile
        async: false
        optimizeMode: OSGFileNode.OptimizeAndCheck
    }

    OSGCamera {
        id: camera
        fieldOfView: 90
        //manipulatorMode: OSGCamera.Track
        //trackNode: fileNode
    }
}