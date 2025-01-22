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
