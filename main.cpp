#include <cstdlib>
#include "qgsproject.h"
#include "qgsapplication.h"
#include "qgsrasterlayer.h"
#include "qgscoordinatereferencesystem.h"
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>

// 确保已包含必要的头文件
// ... [其他必要的包含]

// 初始化QGIS应用，设置为不使用图形界面
int main(int argc, char *argv[])
{
    QString qgis_source;
    char* qgissource = std::getenv("QGISSOURCE");
    if (qgissource != nullptr) {
        qgis_source = QString::fromStdString(qgissource);
    } else {
        qgis_source = "";
    }

    qDebug() << "qgis_source:" << qgis_source;

    QgsApplication app(argc, argv, false);
    QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
    QgsApplication::setPluginPath("/lyndon/iProject/cpath/QGIS/output/lib/qgis/plugins");
    QgsApplication::initQgis();

    QgsProject *project = QgsProject::instance();
    qInfo() << "project instance: " << project;
    project->setTitle("My QGIS Project");
    qInfo() << "set title: " << project->title();
    QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem::fromEpsgId(3857);
    project->setCrs(crs);  // 设置坐标参考系为EPSG:3857
    qInfo() << "set crs: " << project->crs().authid();

    std::string MAP_BASE_URL;
    char* env_url = std::getenv("MAP_BASE_URL");
    if (env_url != nullptr) {
        MAP_BASE_URL = env_url;
    } else {
        MAP_BASE_URL = "http://47.94.145.6";
    }

    // Define XYZ layer URL and name
    std::string BASE_TILE_URL = MAP_BASE_URL + "/map/lx/{z}/{x}/{y}.png";
    QString baseTileUrl = QString::fromStdString(BASE_TILE_URL);
    QString baseTileName = "BaseTile";  // Replace with actual name

    qInfo() << "baseTileUrl: " << baseTileUrl;
    qInfo() << "baseTileName: " << baseTileName;

    // 构建完整的数据源URI字符串
    QString dataSource = QString("type=xyz&url=%1").arg(baseTileUrl);

    // 创建XYZ Raster图层
    auto* baseTileLayer = new QgsRasterLayer(dataSource, baseTileName, "xyz");
    baseTileLayer->setCrs(crs);

    if (baseTileLayer->isValid()) {
        project->addMapLayer(baseTileLayer);
        qInfo() << "XYZ layer added successfully.";
    } else {
        qCritical() << "Error adding XYZ layer";
    }

    // 指定保存的QGS文件路径
    QString projectFilePath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project.qgz";
    QString tempQgsFilePath = QFileInfo(projectFilePath).absolutePath() + "/temp_project.qgs";
    qDebug() << "save qgs file:" << tempQgsFilePath;
    project->write(tempQgsFilePath);

    // 使用QProcess调用zip命令进行压缩
    QString zipCommand = "zip";
    QStringList arguments;
    arguments << "-j" << projectFilePath << tempQgsFilePath;

    QProcess zipProcess;
    zipProcess.start(zipCommand, arguments);
    zipProcess.waitForFinished(-1);

    if (zipProcess.exitStatus() == QProcess::NormalExit && zipProcess.exitCode() == 0) {
        qDebug() << "Successfully created QGZ file:" << projectFilePath;
        QFile::remove(tempQgsFilePath);
    } else {
        qCritical() << "Error creating QGZ file";
    }

    QgsApplication::exitQgis();
    app.exit();

    return 0;
}
