//
// Created by Lyndon on 2025/1/7.
//

#ifndef APP_H
#define APP_H
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <QString>
#include <qgsapplication.h>

#include "../enums/PaperSpecification.h"

class App {
public:
    App();
    ~App();

private:
    QString scene_name;
    QgsProject* project;
    QgsMapCanvas* canvas;
    QgsMapSettings* mapSettings;
    QString projectDir;
    QgsCoordinateTransformContext* transformContext;
    QgsApplication& qgis;
    QgsPageSizeRegistry& pageSizeRegistry;
    QList<PaperSpecification>* available_papers;
};



#endif //APP_H
