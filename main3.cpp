#include <iomanip>
#include <iostream>
#include <cstdlib>
#include "qgsproject.h"
#include "qgsapplication.h"
#include "qgsrasterlayer.h"
#include "qgsdatasourceuri.h"
#include "qgspluginlayerregistry.h"
#include <QString>
#include <QFileInfo>

// 以下用于处理QGZ文件相关（基于QDomDocument等）
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include "config.h"

using namespace std;
#define HAVE_STATIC_PROVIDERS

int main(int argc, char *argv[])
{
    // Set the QT_QPA_PLATFORM environment variable to wayland
    // setenv("QT_QPA_PLATFORM", "wayland", 1);
    auto platform = std::getenv("QT_QPA_PLATFORM");
    if (getenv == nullptr) {
        _putenv_s("QT_QPA_PLATFORM", "wayland");
    } else {
        qDebug() << "QT_QPA_PLATFORM:" << platform;
    }

    _putenv_s("QGIS_PROVIDER_FILE", "arcgisvectortileservice;cesiumtiles;copc;ept;gdal;"
                                 "mbtilesvectortiles;memory;mesh_memory;ogr;quantizedmesh;sensorthings;tiledscene;"
                                 "arcgisfeatureserver;arcgismapserver;delimitedtext;gpx;grass7;grassraster7;mdal;mssql;"
                                 "vectortile;vpc;vtpkvectortiles;xyzvectortiles;pdal;"
                                 "postgresraster;postgres;spatialite;virtuallayer;virtualraster;wcs;wfs;wms;");


    QString qgis_source;
    char* qgissource = std::getenv("QGISSOURCE");
    if (qgissource!= nullptr) {
        qgis_source = QString::fromStdString(qgissource);
    } else {
        qgis_source = "";
    }

    qDebug() << "qgis_source:" << qgis_source;


    QString qgis_provider_file;
    char* providerfile = std::getenv("QGIS_PROVIDER_FILE");
    if (providerfile!= nullptr) {
        qgis_provider_file = QString::fromStdString(providerfile);
    } else {
        qgis_provider_file = "";
    }

    qDebug() << "qgis_provider_file:" << qgis_provider_file;

    // 初始化QGIS应用，这里设置为不使用图形界面（第二个参数为false）
    QgsApplication app(argc, argv, false);
    // QgsApplication::initQgis();
    // 设置QGIS的相关路径（根据你实际的安装情况调整），比如插件路径等，示例中简单设置为空
    // app.setPrefixPath("D:/iProject/cpath/OSGeo4W/apps/qgis", true);
    QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
    // app.setPrefixPath("/usr", false);
    // Set QGIS plugin path
    QgsApplication::setPluginPath("/lyndon/iProject/cpath/QGIS/output/lib/qgis/plugins");
    // app.``setPluginPath("/usr/lib/qgis/plugins");
    QgsApplication::initQgis();
    // 初始化相关的设置等
    QgsApplication::init();

    QgsPluginLayerRegistry* plugin_layer_registry = QgsApplication::pluginLayerRegistry();
    if (plugin_layer_registry) {
        for (const QString& plugin_name : qgis_provider_file.split(";")) {
            plugin_layer_registry->addPluginLayerType(new QgsPluginLayerType(plugin_name));
        }
        QStringList plugin_layer_types = plugin_layer_registry->pluginLayerTypes();
        qDebug() << "plugin type:" << plugin_layer_types.join(", ");
        QgsPluginLayerType* wms_plugin_layer = plugin_layer_registry->pluginLayerType("wms");
        if (wms_plugin_layer) {
            qDebug() << "WMS plugin type loaded successfully";
        } else {
            qDebug() << "Failed to load WMS plugin type";
        }
    } else {
        qDebug() << "Failed to load plugin registry";
    }

    QgsPluginLayerType* wms_plugin_layer = plugin_layer_registry->pluginLayerType("wms");
    qDebug() << "wms plugin type:" << wms_plugin_layer;

    // 创建一个QGIS项目实例
    QgsProject *project = QgsProject::instance();
    qInfo() << "project instance: " << project;
    // 设置项目标题等基础信息
    project->setTitle("My QGIS Project");
    qInfo() << "set title: " << project->title();
    QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem::fromEpsgId(3857);
    project->setCrs(crs);  // 这里设置坐标参考系为EPSG:3857，可按需改
    qInfo() << "set crs: " << project->crs().authid();

    std::string MAP_BASE_URL;
    char* env_url = std::getenv("MAP_BASE_URL");
    if (env_url!= nullptr) {
        MAP_BASE_URL = env_url;
    } else {
        MAP_BASE_URL = "http://47.94.145.6";
    }

    // Define WMS layer URL and name
    std::string BASE_TILE_URL = MAP_BASE_URL + "/map/lx/{z}/{x}/{y}.png";
    QString baseTileUrl = QString::fromStdString(BASE_TILE_URL);
    QString baseTileName = "BaseTile";  // Replace with actual name

    qInfo() << "baseTileUrl: " << baseTileUrl;
    qInfo() << "baseTileName: " << baseTileName;

    // 配置XYZ数据源的相关参数（使用你提供的URL模板）
    QgsDataSourceUri uri;
    uri.setParam("url", baseTileUrl);
    uri.setParam("type", "xyz");

    // Create WMS raster layer
    QgsRasterLayer* baseTileLayer = new QgsRasterLayer(uri.uri(), baseTileName, "xyz");
    baseTileLayer->setCrs(crs);

    // baseTileLayer->setDataProvider(QString::fromStdString("wms"));

    // 创建RasterLayer并添加到项目中
    if (baseTileLayer->isValid()) {
        project->addMapLayer(baseTileLayer);
    } else {
        qCritical() << "qCritical:" << baseTileLayer->error().message();
        qCritical() << "qCritical:" << baseTileLayer->error().summary();
        qDebug() << "Error adding XYZ layer";
    }
    // 指定要保存的QGZ文件路径
    QString projectFilePath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project.qgz";
    // 先将项目保存为临时的QGS文件（QGIS项目文件格式）
    QString tempQgsFilePath = QFileInfo(projectFilePath).absolutePath() + "/temp_project.qgs";
    qDebug() << "save qgs file:" << tempQgsFilePath;
    project->write(tempQgsFilePath);

    // 以下是将QGS文件打包成QGZ文件的过程（QGZ实际就是压缩的QGS）
    QFileInfo qgsFileInfo(tempQgsFilePath);
    QString baseName = qgsFileInfo.baseName();
    QString dirPath = qgsFileInfo.absoluteDir().absolutePath();

    // 创建QDomDocument用于构建QGZ的内部结构（基于XML的配置）
    QDomDocument doc("qgis");
    QDomElement qgisElem = doc.createElement("qgis");
    doc.appendChild(qgisElem);

    // 构建必要的节点来表示项目信息（这里简单示例，可根据QGIS项目文件规范完善）
    QDomElement projectElem = doc.createElement("project");
    projectElem.setAttribute("version", "3");  // 假设是QGIS 3版本的项目，按需调整
    qgisElem.appendChild(projectElem);

    QDomElement titleElem = doc.createElement("title");
    QDomText titleText = doc.createTextNode(baseName);
    titleElem.appendChild(titleText);
    projectElem.appendChild(titleElem);

    // 保存为QGZ文件（实际是压缩成ZIP格式并改名）
    QFile file(projectFilePath);
    qDebug() << "save qgz file:" << projectFilePath;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
        // 这里可以删除临时的QGS文件
        QFile::remove(tempQgsFilePath);
    } else {
        qDebug() << "Error saving QGZ file";
    }

    // // 定义保存的工程文件路径和文件名
    // QString projectFilePath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project.qgz";  // 替换为实际要保存的路径
    //
    // // 保存项目为工程文件
    // qInfo() << "ready to write project file: " << projectFilePath;
    // bool savedSuccessfully = project->write(projectFilePath);
    // if (savedSuccessfully) {
    //     qDebug() << "Project saved successfully.";
    // } else {
    //     qDebug() << "Failed to save the project.";
    // }

    // 退出QGIS应用，释放相关资源
    QgsApplication::exitQgis();

    // 退出QGIS应用
    app.exit();

    return 0;
}