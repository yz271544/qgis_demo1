//
// Created by etl on 1/22/25.
//

#ifndef QGIS_DEMO1_CAMERAUTIL_H
#define QGIS_DEMO1_CAMERAUTIL_H

#include <qlogging.h>
#include <qgscameracontroller.h>

class CameraUtil {
public:
    static void LookingAtInfo(QgsCameraController* cameraController);

    static void PoseInfo(QgsCameraController* cameraController);
};


#endif //QGIS_DEMO1_CAMERAUTIL_H
