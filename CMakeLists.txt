cmake_minimum_required(VERSION 3.6)
project(qgis_demo1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")


add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

if (WIN32)
    if(MSVC)
        add_compile_options(/wd4819)
        add_definitions(-D_USE_MATH_DEFINES)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        # Ensure consistent runtime library settings
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
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
    find_package(Qt5 COMPONENTS Core Gui Qml Quick Widgets Xml Svg Multimedia Test REQUIRED)
    find_package(Qt5Keychain CONFIG REQUIRED)

    find_path(GDAL_INCLUDE_DIR NAMES gdal.h ogr_api.h gdalwarper.h cpl_conv.h cpl_string.h HINTS $ENV{QGISPROGRAMFILES}/gdal-dev/include)
    find_library(GDAL_LIBRARY NAMES gdal HINTS $ENV{QGISPROGRAMFILES}/gdal-dev/lib)

    message("find yaml-cpp")
    set(YAML_CPP_PATH "D:/iProject/cpath/OSGeo4W/apps/yaml-cpp")
#    find_package(yaml-cpp REQUIRED PATHS YAML_CPP_PATH)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        find_path(YAML_CPP_INCLUDE_DIR
                NAMES
                yaml-cpp/null.h
                yaml-cpp/yaml.h
                yaml-cpp/traits.h
                PATHS
                "D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include"
        )
        set(YAML_CPP_NODE_INCLUDE_DIR ${YAML_CPP_INCLUDE_DIR}/node)
        set(YAML_CPP_NODE_DETAIL_INCLUDE_DIR ${YAML_CPP_NODE_INCLUDE_DIR}/detail)
        set(YAML_CPP_CONTRIB_INCLUDE_DIR ${YAML_CPP_INCLUDE_DIR}/contrib)
        find_library(YAML_CPP_LIBRARIES
                NAMES yaml-cppd.lib
                PATHS
                "D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib"
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        message("PROGRAMFILES: $ENV{PROGRAMFILES}")
        message("PROGRAMFILES(x86): $ENV{PROGRAMFILES\(x86\)}")
        message("PROGRAM_FILES_X86: ${PROGRAM_FILES_X86}")

        set(PROGRAM_FILES_X86 $ENV{PROGRAMFILES\(x86\)})
        set(YAML_CPP_DIR ${PROGRAM_FILES_X86}/YAML_CPP)
        find_path(YAML_CPP_INCLUDE_DIR
                NAMES
                yaml-cpp/null.h
                yaml-cpp/yaml.h
                yaml-cpp/traits.h
                PATHS
                ${YAML_CPP_DIR}/include
        )
        set(YAML_CPP_NODE_INCLUDE_DIR ${YAML_CPP_INCLUDE_DIR}/node)
        set(YAML_CPP_NODE_DETAIL_INCLUDE_DIR ${YAML_CPP_NODE_INCLUDE_DIR}/detail)
        set(YAML_CPP_CONTRIB_INCLUDE_DIR ${YAML_CPP_INCLUDE_DIR}/contrib)
        find_library(YAML_CPP_LIBRARIES
                NAMES yaml-cpp.lib
                PATHS
                ${YAML_CPP_DIR}/lib
        )
    endif()
    message("yaml-cpp found successfully.")
    message("YAML_CPP_INCLUDE_DIR: ${YAML_CPP_INCLUDE_DIR}")
    message("YAML_CPP_LIBRARIES: ${YAML_CPP_LIBRARIES}")


    #    if (yaml-cpp_FOUND)
#        message("yaml-cpp found successfully.")
#        message("YAML_CPP_INCLUDE_DIR: ${YAML_CPP_INCLUDE_DIR}")
#        message("yaml-cpp_LIBRARIES: ${YAML_CPP_LIBRARIES}")
#    else()
#        message(FATAL_ERROR "Failed to find yaml-cpp.")
#    endif()

elseif(UNIX)
    message("NIX system - QGISSOURCE: ${QGISSOURCE}")
    if (NOT DEFINED $ENV{QGISSOURCE} AND $ENV{QGISSOURCE} STREQUAL "")
        message("release env")
        set(CMAKE_MODULE_PATH /usr/share/qgis)
        message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

        message("find yaml-cpp")
        find_package(yaml-cpp REQUIRED)
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
        message("find yaml-cpp")
        find_package(yaml-cpp REQUIRED)
    endif()

    # 检查 CMAKE_BUILD_TYPE 的值
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QGIS_PREFIX_PATH "/lyndon/iProject/cpath/QGIS/output")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(QGIS_PREFIX_PATH "/usr")
    else()
        # 可以添加一个默认值或错误处理
        message(FATAL_ERROR "Unsupported CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    endif()

    message("start find Qt package")
    find_package(Qt5 REQUIRED COMPONENTS
            Core
            Gui
            Qml
            Quick
            Widgets
            Xml
            Svg
            Multimedia
            Test
    )
    find_package(Qt5Keychain CONFIG REQUIRED)

    find_path(GDAL_INCLUDE_DIR NAMES gdal.h ogr_api.h gdalwarper.h cpl_conv.h cpl_string.h HINTS /usr/include/gdal)
    find_library(GDAL_LIBRARY NAMES gdal HINTS /usr/lib /usr/lib/x86_64-linux-gnu)
else()
    message("Unsupported platform")
endif()

# 生成 config.h 头文件
configure_file(config.h.in ${CMAKE_HOME_DIRECTORY}/config.h)
configure_file(constants.h.in ${CMAKE_HOME_DIRECTORY}/constants.h)
configure_file(constants.cpp.in ${CMAKE_HOME_DIRECTORY}/constants.cpp)


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
message("Qt5Svg_INCLUDE_DIRS: ${Qt5Svg_INCLUDE_DIRS}")
include_directories(${Qt5Svg_INCLUDE_DIRS})


message("GDAL_INCLUDE_DIR: ${GDAL_INCLUDE_DIR}")
include_directories(${GDAL_INCLUDE_DIR})

include_directories(${YAML_CPP_INCLUDE_DIR} ${YAML_CPP_NODE_INCLUDE_DIR} ${YAML_CPP_CONTRIB_INCLUDE_DIR})


if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Add library directories
link_directories(${QGIS_PREFIX_PATH}/lib /usr/lib/x86_64-linux-gnu ${YAML_CPP_LIBRARIES} ${YAML_CPP_NODE_DETAIL_INCLUDE_DIR})

#add_executable(qgis_demo1 main.cpp
#        test_main.cpp
#        test_main.h)
add_executable(qgis_demo1 main.cpp
        tests/test_json.cpp
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h)
add_executable(qgis_demo2
        main2.cpp
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h)
add_executable(qgis_demo3
        main3.cpp
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h)
add_executable(qgis_demo4
        main4.cpp
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h)
add_executable(qgis_demo3d0
        main3d0.cpp
        JwGeometry.cpp core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h)
add_executable(qgis_demo3d1 main3d1.cpp
        JwGeometry.cpp
        config.h
        constants.h
        constants.cpp
        core/qgis/style/StylePoint.cpp
        core/qgis/style/StylePoint.h
        core/qgis/payload/InputPoint.cpp
        core/qgis/payload/InputPoint.h
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h
        core/utils/QgsUtil.cpp
        core/utils/QgsUtil.h
        core/utils/FileUtil.cpp
        core/utils/FileUtil.h
        core/utils/ImageUtil.cpp
        core/utils/ImageUtil.h
        core/enums/PaperSpecification.cpp
        core/enums/PaperSpecification.h
        core/qgis/layout/JwLayout.cpp
        core/qgis/layout/JwLayout.h
        core/qgis/layout/JwLegend.cpp
        core/qgis/layout/JwLegend.h
        core/utils/FontUtil.cpp
        core/utils/FontUtil.h
        core/utils/NodeToMap.cpp
        core/utils/NodeToMap.h
        core/utils/JsonUtil.cpp
        core/utils/JsonUtil.h
)

add_executable(test_main test_main.cpp
        test_main.h
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h)

set_target_properties(qgis_demo3d1 PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

set_target_properties(test_main PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

#add_executable(qgis_demo1 test_main.cpp)

target_link_libraries(qgis_demo1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Test ${GDAL_LIBRARY})
target_link_libraries(qgis_demo2 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Test ${GDAL_LIBRARY})
target_link_libraries(qgis_demo3 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Test ${GDAL_LIBRARY})
target_link_libraries(qgis_demo4 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Test ${GDAL_LIBRARY})
target_link_libraries(qgis_demo3d0 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Test ${GDAL_LIBRARY})
target_link_libraries(qgis_demo3d1 PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Svg Qt5::Test ${GDAL_LIBRARY} ${YAML_CPP_LIBRARIES})
target_link_libraries(test_main PRIVATE ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} ${QGIS_3D_LIBRARY} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml Qt5::Test ${GDAL_LIBRARY})

# 测试json程序
add_executable(test_json tests/test_json.cpp
        core/utils/JsonUtil.cpp
        core/utils/JsonUtil.h)
target_link_libraries(test_json Qt5::Core Qt5::Test)
add_test(NAME JsonTest COMMAND test_json)

# 测试utils程序
add_executable(test_utils
        tests/test_utils.cpp
        core/utils/ColorTransformUtil.cpp
        core/utils/ColorTransformUtil.h
        core/utils/QgsUtil.cpp
        core/utils/QgsUtil.h
        core/utils/FileUtil.cpp
        core/utils/FileUtil.h
        core/utils/ImageUtil.cpp
        core/utils/ImageUtil.h)
target_link_libraries(test_utils ${QGIS_CORE_LIBRARY} ${QGIS_GUI_LIBRARY} Qt5::Core Qt5::Gui Qt5::Test)
add_test(NAME UtilsTest COMMAND test_utils)

add_executable(test_yaml tests/test_yaml.cpp)
target_link_libraries(test_yaml Qt5::Core Qt5::Test ${YAML_CPP_LIBRARIES})
add_test(NAME YamlTest COMMAND test_yaml)

add_executable(test_enum tests/test_enum.cpp
        core/enums/PaperSpecification.cpp
        core/enums/PaperSpecification.h)
target_link_libraries(test_enum Qt5::Core Qt5::Test)
add_test(NAME YamlEnum COMMAND test_enum)