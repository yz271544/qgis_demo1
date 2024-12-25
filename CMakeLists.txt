cmake_minimum_required(VERSION 3.6)
project(qgis_demo1)

set(CMAKE_CXX_STANDARD 20)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")


add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

if (WIN32)
    if(MSVC)
        add_compile_options(/wd4819)
        add_definitions(-D_USE_MATH_DEFINES)
    endif()
    message("Win32 Searching for QGIS in $ENV{QGISPROGRAMFILES}/qgis")
    # 设置QGIS库的路径
    set(QGIS_PREFIX_PATH $ENV{QGISPROGRAMFILES}/qgis)
    set(QGIS_DIR $ENV{QGISPROGRAMFILES}/qgis)
    set(QGIS_INCLUDE_DIR ${QGIS_DIR}/include)
    set(QGIS_LIB_PATH ${QGIS_DIR}/lib)
    set(QT5_CMAKE_DIR D:/iProject/cpath/OSGeo4W/apps/Qt5/lib/cmake/Qt5)
    message("QT5_CMAKE_DIR: ${QT5_CMAKE_DIR}")
    set(QT5_DIR D:/iProject/cpath/OSGeo4W/apps/Qt5/lib)
#    set(QT5_GUI_CMAKE_DIR D:/iProject/cpath/OSGeo4W/apps/Qt5/lib/cmake/Qt5Gui)
#    set(QT5_QML_CMAKE_DIR D:/iProject/cpath/OSGeo4W/apps/Qt5/lib/cmake/Qt5Qml)
#    set(QT5_XML_CMAKE_DIR D:/iProject/cpath/OSGeo4W/apps/Qt5/lib/cmake/Qt5Xml)
#    set(CMAKE_MODULE_PATH ${QGIS_DIR} ${QT5_DIR} ${QT5_GUI_CMAKE_DIR} ${QT5_CMAKE_DIR} ${QT5_QML_CMAKE_DIR} ${QT5_XML_CMAKE_DIR} ${CMAKE_MODULE_PATH})
    set(CMAKE_MODULE_PATH ${QGIS_DIR} ${QGIS_LIB_PATH} ${QT5_DIR} ${QT5_CMAKE_DIR} ${CMAKE_MODULE_PATH})
    message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
    # 使用QGIS安装目录下的FindQGIS.cmake来查找QGIS库（前提是这个文件符合CMake查找模块规范）
    message("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}")

    set(CMAKE_PREFIX_PATH $ENV{QT5DIR})
    message("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}")
    set(QT_VERSION_MAJOR 5)
    set(QT_MIN_VERSION 5.15.2)
    set(QT_VERSION_BASE "Qt5")
    set(QT_VERSION_BASE_LOWER "qt5")
    set(HAS_KDE_QT5_PDF_TRANSFORM_FIX FALSE CACHE BOOL "Using KDE's Qt 5.15 fork with the PDF brush transform fix")
    set(HAS_KDE_QT5_SMALL_CAPS_FIX FALSE CACHE BOOL "Using KDE's Qt 5.15 fork with the QFont::SmallCaps fix")
    set(HAS_KDE_QT5_FONT_STRETCH_FIX FALSE CACHE BOOL "Using KDE's Qt 5.15 fork with the QFont stretch fix")
    find_package(Qt5 COMPONENTS Core Gui Qml Quick Widgets Xml Multimedia REQUIRED)
    find_package(Qt5Keychain CONFIG REQUIRED)

    find_path(GDAL_INCLUDE_DIR NAMES gdal.h ogr_api.h gdalwarper.h cpl_conv.h cpl_string.h HINTS $ENV{QGISPROGRAMFILES}/gdal-dev/include)
    find_library(GDAL_LIBRARY NAMES gdal HINTS $ENV{QGISPROGRAMFILES}/gdal-dev/lib)

elseif(UNIX)
    message("NIX system - QGISSOURCE: ${QGISSOURCE}")
    if (NOT DEFINED $ENV{QGISSOURCE} AND $ENV{QGISSOURCE} STREQUAL "")
        message("release env")
        set(CMAKE_MODULE_PATH /usr/share/qgis)
        message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
    else()
        message("debug env")
        message("NIX QGIS SOURCE DIR: $ENV{QGISSOURCE}")
        # 设置QGIS库的路径
        set(QGIS_INSTALL_DIR "$ENV{QGISSOURCE}/output" CACHE STRING "QGIS install directory")
        set(QGIS_DIR_CMAKE "$ENV{QGISSOURCE}/output/share/qgis" CACHE STRING "QGIS cmake directory")
        set(QGIS_BUILD_PATH "$ENV{QGISSOURCE}" CACHE STRING "QGIS build directory")
        set(QGIS_PREFIX_PATH "$ENV{QGISSOURCE}/output" CACHE STRING "QGIS prefix path")
        set(QGIS_LIB_PATH "$ENV{QGISSOURCE}/output/lib" CACHE STRING "QGIS lib path")
        message("QGIS_INSTALL_DIR: ${QGIS_INSTALL_DIR}")
        message("QGIS_DIR_CMAKE: ${QGIS_DIR_CMAKE}")
        set(CMAKE_MODULE_PATH ${QGIS_INSTALL_DIR} ${QGIS_DIR_CMAKE} ${QT5_CMAKE_DIR} ${QGIS_LIB_PATH} ${CMAKE_MODULE_PATH})
        message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

#        find_path(QGIS_3D_INCLUDE_DIR NAMES qgis_3d.h HINTS ${QGIS_INSTALL_DIR}/include/qgis)
#        find_library(QGIS_3D_LIBRARY NAMES qgis_3d HINTS ${QGIS_LIB_PATH})

    endif()
    message("start find Qt package")
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

    find_path(GDAL_INCLUDE_DIR NAMES gdal.h ogr_api.h gdalwarper.h cpl_conv.h cpl_string.h HINTS /usr/include/gdal)
    find_library(GDAL_LIBRARY NAMES gdal HINTS /usr/lib /usr/lib/x86_64-linux-gnu)
else()
    message("Unsupported platform")
endif()

message("start find qgis package")
find_package(QGIS REQUIRED)
find_path(QGIS_3D_INCLUDE_DIR NAMES qgis_3d.h HINTS ${QGIS_INCLUDE_DIR})
find_library(QGIS_3D_LIBRARY NAMES qgis_3d HINTS ${QGIS_LIB_PATH})

message("QGIS_3D_LIBRARY: ${QGIS_3D_LIBRARY}")

message("QGIS_INCLUDE_DIR: ${QGIS_INCLUDE_DIR}")
include_directories(${QGIS_INCLUDE_DIR})
message("QGIS_3D_INCLUDE_DIR: ${QGIS_3D_INCLUDE_DIR}")
include_directories(${QGIS_3D_INCLUDE_DIR})
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

message("GDAL_INCLUDE_DIR: ${GDAL_INCLUDE_DIR}")
include_directories(${GDAL_INCLUDE_DIR})


if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Add library directories
link_directories(${QGIS_PREFIX_PATH}/lib /usr/lib/x86_64-linux-gnu)

#add_executable(qgis_demo1 main.cpp
#        test_main.cpp
#        test_main.h)
add_executable(qgis_demo1 main.cpp
        config.h)
add_executable(qgis_demo2 main2.cpp)
add_executable(qgis_demo3 main3.cpp)
add_executable(qgis_demo4 main4.cpp)
add_executable(qgis_demo3d0 main3d0.cpp
        JwGeometry.cpp)
add_executable(qgis_demo3d1 main3d1.cpp
        JwGeometry.cpp)

set_target_properties(qgis_demo3d1 PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

#add_executable(qgis_demo1 test_main.cpp)

target_link_libraries(qgis_demo1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${GDAL_LIBRARY})
target_link_libraries(qgis_demo2 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${GDAL_LIBRARY})
target_link_libraries(qgis_demo3 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${GDAL_LIBRARY})
target_link_libraries(qgis_demo4 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${GDAL_LIBRARY})
target_link_libraries(qgis_demo3d0 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${GDAL_LIBRARY})
target_link_libraries(qgis_demo3d1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${GDAL_LIBRARY})