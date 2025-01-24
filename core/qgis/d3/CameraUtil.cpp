//
// Created by etl on 1/22/25.
//

#include "CameraUtil.h"

void CameraUtil::LookingAtInfo(QgsCameraController *cameraController) {
    QgsVector3D sceneLookingAtPoint3d = cameraController->lookingAtPoint();
    qDebug() << "scene camera lookingAtPoint: x:" << QString::number(sceneLookingAtPoint3d.x(), 'f', 10)
             << " y:" << QString::number(sceneLookingAtPoint3d.y(), 'f', 10)
             << " z:" << QString::number(sceneLookingAtPoint3d.z(), 'f', 10);
}

void CameraUtil::PoseInfo(QgsCameraController *cameraController) {
    QgsCameraPose cameraPose = cameraController->cameraPose();
    qDebug() << "scene camera cameraPose: x:" << QString::number(cameraPose.centerPoint().x() , 'f', 10)
             << " y:" << QString::number(cameraPose.centerPoint().y() , 'f', 10)
             << " z:" << QString::number(cameraPose.centerPoint().z(), 'f', 10)
             << " pitchAngle:" << cameraPose.pitchAngle()
             << " headingAngle:" << cameraPose.headingAngle()
             << " distanceFromCenterPoint:" << cameraPose.distanceFromCenterPoint();
}

void CameraUtil::ExtentInfo(QgsRectangle extent) {
    qDebug() << "extent: " << extent << " width: " << QString::number(extent.width(),'f',3) << " height: " << QString::number(extent.height(),'f',3)
             << " xMinimum:" << QString::number(extent.xMinimum(),'f',3) << " yMinimum:" << QString::number(extent.yMinimum(),'f',3)
             << " xMaximum:" << QString::number(extent.xMaximum(),'f',3) << " yMaximum:" << QString::number(extent.yMaximum(),'f',3)
             << " area: " << QString::number(extent.area(),'f',3) << " perimeter: " << QString::number(extent.perimeter(),'f',3)
             << " center -> x:" << QString::number(extent.center().x(), 'f', 3) << " y:" << QString::number(extent.center().y(), 'f', 3)
             << " isEmpty: " << extent.isEmpty() << " isNull: " << extent.isNull() << " isFinite: " << extent.isFinite();
}

void CameraUtil::TrapezoidInfo(QVector<QgsPointXY> verticesOfTrapezoid) {
    for (int i = 0; i < verticesOfTrapezoid.size(); i++) {
        QgsPointXY point = verticesOfTrapezoid.at(i);
        qDebug() << "verticesOfTrapezoid[" << i << "]: x:" << QString::number(point.x(), 'f', 10)
                 << " y:" << QString::number(point.y(), 'f', 10);
    }
}