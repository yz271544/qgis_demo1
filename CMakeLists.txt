cmake_minimum_required(VERSION 3.6)
project(qgis_demo1)

set(CMAKE_CXX_STANDARD 20)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

if (WIN32)
    message("Win32 Searching for QGIS in $ENV{QGISPROGRAMFILES}/qgis")
    # 设置QGIS库的路径
    set(QGIS_DIR $ENV{QGISPROGRAMFILES}/qgis)
    set(CMAKE_MODULE_PATH ${QGIS_DIR} ${QT5_CMAKE_DIR} ${CMAKE_MODULE_PATH})

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


elseif(UNIX)
    if($ENV{QGISSOURCE})
        message("debug env")
        message("NIX QGIS SOURCE DIR: $ENV{QGISSOURCE}")
        message("NIX Searching for QGIS in $ENV{QGISPROGRAMFILES}/cmake")
        # 设置QGIS库的路径
        set(QGIS_INSTALL_DIR "$ENV{QGISSOURCE}/apps")
        set(QGIS_DIR_CMAKE "$ENV{QGISSOURCE}/cmake")
        set(QGIS_BUILD_PATH "$ENV{QGISSOURCE}")
        set(QGIS_PREFIX_PATH "$ENV{QGISSOURCE}/output")
        message("QGIS_INSTALL_DIR: ${QGIS_INSTALL_DIR}")
        message("QGIS_DIR_CMAKE: ${QGIS_DIR_CMAKE}")
        set(CMAKE_MODULE_PATH ${QGIS_INSTALL_DIR} ${QGIS_DIR_CMAKE} ${QT5_CMAKE_DIR} ${CMAKE_MODULE_PATH})
        message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
    else()
        message("release env")
        set(CMAKE_MODULE_PATH /usr/share/qgis)
        message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
    endif()

    find_package(Qt5 REQUIRED COMPONENTS
            Core
            Gui
            Qml
            Quick
            Widgets
            Xml
            Multimedia
    )
    find_package(Qt5Keychain CONFIG REQUIRED)
else()
    message("Unsupported platform")
endif()


find_package(QGIS COMPONENTS core gui REQUIRED)

#message("111 QGIS_DIR: ${QGIS_DIR}")
#set(QGIS_INCLUDE_DIR ${QGIS_DIR}/include)
#set(QGIS_LIBRARY_DIR ${QGIS_DIR}/lib)
#
#find_library(QGIS_CORE_LIBRARY NAMES qgis_core HINTS ${QGIS_LIBRARY_DIR})
#
#
#include_directories(${QGIS_INCLUDE_DIR})



#include_directories("${QGIS_DIR}/include")
#message("Qt include: ${CMAKE_PREFIX_PATH}/include")
#include_directories("${CMAKE_PREFIX_PATH}/include")
#include_directories("${CMAKE_PREFIX_PATH}/include/QtCore")
#include_directories("${CMAKE_PREFIX_PATH}/include/QtGui")
#include_directories("${CMAKE_PREFIX_PATH}/include/QtQml")
#include_directories("${CMAKE_PREFIX_PATH}/include/QtQuick")
#include_directories("${CMAKE_PREFIX_PATH}/include/QtWidgets")
#include_directories("${CMAKE_PREFIX_PATH}/include/QtXml")

message("QGIS_INCLUDE_DIR: ${QGIS_INCLUDE_DIR}")
include_directories(${QGIS_INCLUDE_DIR})
message("Qt5Core_INCLUDE_DIRS: ${Qt5Core_INCLUDE_DIRS}")
include_directories(${Qt5Core_INCLUDE_DIRS})
message("Qt5Gui_INCLUDE_DIRS: ${Qt5Gui_INCLUDE_DIRS}")
include_directories(${Qt5Gui_INCLUDE_DIRS})
message("Qt5Qml_INCLUDE_DIRS: ${Qt5Qml_INCLUDE_DIRS}")
include_directories(${Qt5Qml_INCLUDE_DIRS})
message("Qt5Quick_INCLUDE_DIRS: ${Qt5Quick_INCLUDE_DIRS}")
include_directories(${Qt5Quick_INCLUDE_DIRS})
message("Qt5Widgets_INCLUDE_DIRS: ${Qt5Widgets_INCLUDE_DIRS}")
include_directories(${Qt5Widgets_INCLUDE_DIRS})
message("Qt5Multimedia_INCLUDE_DIRS: ${Qt5Multimedia_INCLUDE_DIRS}")
include_directories(${Qt5Multimedia_INCLUDE_DIRS})
message("Qt5Xml_INCLUDE_DIRS: ${Qt5Xml_INCLUDE_DIRS}")
include_directories(${Qt5Xml_INCLUDE_DIRS})

# Add the -fPIC flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Add library directories
link_directories(${QGIS_PREFIX_PATH}/lib /usr/lib/x86_64-linux-gnu)

#add_executable(qgis_demo1 main.cpp
#        test_main.cpp
#        test_main.h)
add_executable(qgis_demo1 main.cpp)
add_executable(qgis_demo2 main2.cpp)
#add_executable(qgis_demo1 test_main.cpp)

# 链接QGIS相关库到可执行文件
#target_link_libraries(qgis_demo1 PRIVATE QGIS_core QGIS_gui Qt5_Core Qt5_Gui Qt5_Qml Qt5_Quick Qt5_Widgets)
#target_link_libraries(qgis_demo1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY})
target_link_libraries(qgis_demo1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml)
target_link_libraries(qgis_demo2 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml)