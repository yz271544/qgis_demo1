//
// Created by etl on 1/22/25.
//

#ifndef QGIS_DEMO1_CAMERAUTIL_H
#define QGIS_DEMO1_CAMERAUTIL_H

#include <qgsrectangle.h>
#include <qlogging.h>
#include <qgscameracontroller.h>

class CameraUtil {
public:
    static void LookingAtInfo(QgsCameraController* cameraController);

    static void PoseInfo(QgsCameraController* cameraController);

    static void ExtentInfo(QgsRectangle extent);

    static void TrapezoidInfo(QVector<QgsPointXY> verticesOfTrapezoid);
};


#endif //QGIS_DEMO1_CAMERAUTIL_H
