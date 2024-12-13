#include <cstdlib>
#include "qgsproject.h"
#include "qgsapplication.h"
#include "qgsrasterlayer.h"
#include <QString>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    // Set the QT_QPA_PLATFORM environment variable to wayland
    // setenv("QT_QPA_PLATFORM", "wayland", 1);

    // 初始化QGIS应用，这里设置为不使用图形界面（第二个参数为false）
    QgsApplication app(argc, argv, false);

    // 设置QGIS的相关路径（根据你实际的安装情况调整），比如插件路径等，示例中简单设置为空
    // app.setPrefixPath("D:/iProject/cpath/OSGeo4W/apps/qgis", true);
    app.setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);


    // Set QGIS plugin path
    app.setPluginPath("/lyndon/iProject/cpath/QGIS/output/lib/qgis/plugins");


    // 初始化相关的设置等
    app.init();

    // 创建一个QGIS项目实例
    QgsProject *project = QgsProject::instance();
    qInfo() << "project instance: " << project;
    // 设置项目标题等基础信息
    project->setTitle("My QGIS Project");
    qInfo() << "set title: " << project->title();
    project->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(3857));  // 这里设置坐标参考系为EPSG:3857，可按需改
    qInfo() << "set crs: " << project->crs().authid();

    std::string MAP_BASE_URL;
    char* env_url = std::getenv("MAP_BASE_URL");
    if (env_url!= nullptr) {
        MAP_BASE_URL = env_url;
    } else {
        MAP_BASE_URL = "http://ALIETL:80";
    }

    // Define WMS layer URL and name
    std::string BASE_TILE_URL = "type=xyz&url=" + MAP_BASE_URL + "/map/lx/{z}/{x}/{y}.png";
    QString baseTileUrl = QString::fromStdString(BASE_TILE_URL);
    QString baseTileName = "BaseTile";  // Replace with actual name

    qInfo() << "baseTileUrl: " << baseTileUrl;
    qInfo() << "baseTileName: " << baseTileName;
    // Create WMS raster layer
    QgsRasterLayer *baseTileLayer = new QgsRasterLayer(baseTileUrl, baseTileName, "wms");

    // Check if the layer is valid
    if (baseTileLayer->isValid()) {
        // Add the layer to the project
        project->addMapLayer(baseTileLayer);
        qDebug() << "WMS layer added successfully.";
    } else {
        qDebug() << "Failed to add WMS layer.";
    }

    // 定义保存的工程文件路径和文件名
    QString projectFilePath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project.qgs";  // 替换为实际要保存的路径

    // 保存项目为工程文件
    qInfo() << "ready to write project file: " << projectFilePath;
    bool savedSuccessfully = project->write(projectFilePath);
    if (savedSuccessfully) {
        qDebug() << "Project saved successfully.";
    } else {
        qDebug() << "Failed to save the project.";
    }

    // 退出QGIS应用
    app.exit();

    return 0;
}