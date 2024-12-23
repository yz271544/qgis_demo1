//
// Created by Lyndon on 2024/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H


inline const char *BASE_TILE_NAME = "BaseTile";
inline const char *MAIN_TILE_NAME = "MainTile";
inline const char *REAL3D_TILE_NAME="Real3DTile";

#if defined(_WIN32)
    // for Windows
    const char* QGIS_PREFIX_PATH = "D:/iProject/cpath/OSGeo4W/apps/qgis";
    const char* QGIS_PROJECT_PATH = "D:/iProject/cpath/qgis_demo1/common/project";
    const char* QGIS_PROJECT_FILE_PATH = "/project3d1.qgz";
#elif defined(__linux__)
    // for Linux
    inline const char* QGIS_PREFIX_PATH = "/lyndon/iProject/cpath/QGIS/output";
    inline const char* QGIS_PROJECT_PATH = "/lyndon/iProject/cpath/qgis_demo1/common/project";
    inline const char* QGIS_PROJECT_FILE_NAME = "/project3d1.qgz";
#endif

#endif //CONFIG_H
