//
// Created by Lyndon on 2024/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H

#if defined(_WIN32)
    // 定义Windows下相关变量
    const char* qgisPrefixPath = "D:/iProject/cpath/OSGeo4W/apps/qgis";
    const char* qgisProjectPath = "D:/iProject/cpath/qgis_demo1/common/project/project3d1.qgz";
#elif defined(__linux__)
    // 定义Linux下相关变量
    const char* qgisPrefixPath = "/lyndon/iProject/cpath/QGIS/output";
    const char* qgisProjectPath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project3d1.qgz";
#endif

#endif //CONFIG_H