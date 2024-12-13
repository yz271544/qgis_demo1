cmake_minimum_required(VERSION 3.29)
project(qgis_demo1)

set(CMAKE_CXX_STANDARD 20)

message("Searching for QGIS in $ENV{QGISPROGRAMFILES}/qgis")

# 设置QGIS库的路径
set(QGIS_DIR $ENV{QGISPROGRAMFILES}/qgis)
message("111 QGIS_DIR: ${QGIS_DIR}")
#set(QGIS_INCLUDE_DIR ${QGIS_DIR}/include)
#set(QGIS_LIBRARY_DIR ${QGIS_DIR}/lib)
#
#find_library(QGIS_CORE_LIBRARY NAMES qgis_core HINTS ${QGIS_LIBRARY_DIR})
#
#
#include_directories(${QGIS_INCLUDE_DIR})

# 使用QGIS安装目录下的FindQGIS.cmake来查找QGIS库（前提是这个文件符合CMake查找模块规范）
message("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}")

set(CMAKE_PREFIX_PATH $ENV{QT5DIR})
set(QT_VERSION_MAJOR 5)
set(QT_MIN_VERSION 5.15.2)
set(QT_VERSION_BASE "Qt5")
set(QT_VERSION_BASE_LOWER "qt5")
set(HAS_KDE_QT5_PDF_TRANSFORM_FIX FALSE CACHE BOOL "Using KDE's Qt 5.15 fork with the PDF brush transform fix")
set(HAS_KDE_QT5_SMALL_CAPS_FIX FALSE CACHE BOOL "Using KDE's Qt 5.15 fork with the QFont::SmallCaps fix")
set(HAS_KDE_QT5_FONT_STRETCH_FIX FALSE CACHE BOOL "Using KDE's Qt 5.15 fork with the QFont stretch fix")
#find_package(Qt5 COMPONENTS Core Gui Qml Quick Widgets REQUIRED)
find_package(Qt5Keychain CONFIG REQUIRED)

set(CMAKE_MODULE_PATH ${QGIS_DIR} ${QT5_CMAKE_DIR} ${CMAKE_MODULE_PATH})

find_package(QGIS 3.40.1 COMPONENTS core gui REQUIRED)

include_directories("${QGIS_DIR}/include")
message("Qt include: ${CMAKE_PREFIX_PATH}/include")
include_directories("${CMAKE_PREFIX_PATH}/include")
include_directories("${CMAKE_PREFIX_PATH}/include/QtCore")
include_directories("${CMAKE_PREFIX_PATH}/include/QtGui")
include_directories("${CMAKE_PREFIX_PATH}/include/QtQml")
include_directories("${CMAKE_PREFIX_PATH}/include/QtQuick")
include_directories("${CMAKE_PREFIX_PATH}/include/QtWidgets")
include_directories("${CMAKE_PREFIX_PATH}/include/QtXml")



#add_executable(qgis_demo1 main.cpp
#        test_main.cpp
#        test_main.h)

add_executable(qgis_demo1 test_main.cpp)

# 链接QGIS相关库到可执行文件
target_link_libraries(qgis_demo1 PRIVATE QGIS_core QGIS_gui Qt5_Core Qt5_Gui Qt5_Qml Qt5_Quick Qt5_Widgets)
#target_link_libraries(qgis_demo1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY})
#target_link_libraries(qgis_demo1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets)