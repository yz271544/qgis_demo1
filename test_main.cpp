//
// Created by Lyndon on 2024/12/12.
//

#include "qgsapplication.h"

int main(int argc, char *argv[])
{
    QgsApplication app(argc, argv, false);
    app.setPrefixPath("D:/iProject/cpath/OSGeo4W/apps/qgis", true);
    app.init();
    app.exit();
    return 0;
}