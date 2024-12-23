//
// Created by Lyndon on 2024/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H


const char *BASE_TILE_NAME = "BaseTile";
const char *MAIN_TILE_NAME = "MainTile";
const char *REAL3D_TILE_NAME="Real3DTile";

#if defined(_WIN32)
    // for Windows
    const char* qgisPrefixPath = "D:/iProject/cpath/OSGeo4W/apps/qgis";
    const char* qgisProjectPath = "D:/iProject/cpath/qgis_demo1/common/project/project3d1.qgz";
#elif defined(__linux__)
    // for Linux
    const char* qgisPrefixPath = "/lyndon/iProject/cpath/QGIS/output";
    const char* qgisProjectPath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project3d1.qgz";
#endif

#endif //CONFIG_H
