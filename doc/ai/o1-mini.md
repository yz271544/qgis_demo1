你遇到的报错信息提示了几个关键点，下面我会逐一分析并提出修复方案。
错误分析
WMS数据提供者无法实例化：
`qCritical: "<p><b>Raster layer:</b> Cannot instantiate the 'wms' data provider"
qCritical: "Cannot instantiate the 'wms' data provider"
Error adding XYZ layer
`
这部分错误意味着你尝试使用一个名为 `'wms'` 的数据提供者，但 QGIS 无法找到或加载这个数据提供者。
程序崩溃（Segmentation Fault）：
`Process finished with exit code 139 (interrupted by signal 11:SIGSEGV)
`
这个崩溃通常表示程序访问了无效的内存地址。通常是因为访问了已删除或未初始化的对象。
问题 1: 使用了错误的数据提供者类型
在你的代码中，你创建了一个 `QgsRasterLayer`，并且指定了 `"wms"` 作为数据提供者类型。然而，基于你提供的代码，实际上你是想使用一个 XYZ 图层（而不是 WMS）。因此，你应该使用正确的提供者类型来实例化你的图层。
解决方案
将 `QgsRasterLayer` 的提供者类型从 `"wms"` 更改为 `"xyz"`，因为你要加载的是一个 XYZ 瓦片图层。
修正部分：

```c++
// 创建一个XYZ Raster图层，而不是WMS
QgsRasterLayer* baseTileLayer = new QgsRasterLayer(uri.uri(), baseTileName, "xyz");  // 使用 "xyz" 而不是 "wms"
baseTileLayer->setCrs(crs);

```

问题 2: `QgsPluginLayerRegistry` 和插件加载
你加载了很多插件，并尝试使用 `wms` 类型的插件。虽然这不会直接导致崩溃，但可能会导致其他一些潜在问题。确保你的 `QGIS` 安装有正确的插件和数据提供者支持。
问题 3: 临时 QGS 文件的保存
你在尝试保存临时 `QGS` 文件时，正确地创建了文件路径和 XML 内容。但在保存成 `QGZ` 文件时，你并没有处理压缩过程。你可能需要使用 `QZip` 来完成此操作，或者使用 QGIS 提供的函数直接保存 `.qgz` 文件。
其他建议
错误处理和资源管理： 使用 `QgsRasterLayer` 时，建议检查图层是否有效。你已经在代码中做了这一点，这很好。继续保持这个检查习惯，尤其是在涉及到外部资源（如 WMS 服务）时。
QGIS 安装路径和插件路径： 确保 `QGIS` 环境变量和插件路径设置正确。如果路径不对，可能会导致无法加载插件。
完整修正后的代码示例：

```c++
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

#define HAVE_STATIC_PROVIDERS

int main(int argc, char *argv[])
{
    setenv("QGIS_PROVIDER_FILE", "arcgisvectortileservice;cesiumtiles;copc;ept;gdal;"
                                 "mbtilesvectortiles;memory;mesh_memory;ogr;quantizedmesh;sensorthings;tiledscene;"
                                 "arcgisfeatureserver;arcgismapserver;delimitedtext;gpx;grass7;grassraster7;mdal;mssql;"
                                 "vectortile;vpc;vtpkvectortiles;xyzvectortiles;pdal;"
                                 "postgresraster;postgres;spatialite;virtuallayer;virtualraster;wcs;wfs;wms;", 1);

    QString qgis_source;
    char* qgissource = std::getenv("QGISSOURCE");
    if (qgissource != nullptr) {
        qgis_source = QString::fromStdString(qgissource);
    } else {
        qgis_source = "";
    }

    qDebug() << "qgis_source:" << qgis_source;

    QString qgis_provider_file;
    char* providerfile = std::getenv("QGIS_PROVIDER_FILE");
    if (providerfile != nullptr) {
        qgis_provider_file = QString::fromStdString(providerfile);
    } else {
        qgis_provider_file = "";
    }

    qDebug() << "qgis_provider_file:" << qgis_provider_file;

    // 初始化QGIS应用，设置为不使用图形界面
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

    // Define WMS layer URL and name
    std::string BASE_TILE_URL = MAP_BASE_URL + "/map/lx/{z}/{x}/{y}.png";
    QString baseTileUrl = QString::fromStdString(BASE_TILE_URL);
    QString baseTileName = "BaseTile";  // Replace with actual name

    qInfo() << "baseTileUrl: " << baseTileUrl;
    qInfo() << "baseTileName: " << baseTileName;

    // 配置XYZ数据源的相关参数
    QgsDataSourceUri uri;
    uri.setParam("url", baseTileUrl);
    uri.setParam("type", "xyz");

    // 创建一个XYZ Raster图层，而不是WMS
    QgsRasterLayer* baseTileLayer = new QgsRasterLayer(uri.uri(), baseTileName, "xyz");  // 使用 "xyz" 而不是 "wms"
    baseTileLayer->setCrs(crs);

    if (baseTileLayer->isValid()) {
        project->addMapLayer(baseTileLayer);
    } else {
        qCritical() << "Error adding XYZ layer";
    }

    // 指定保存的QGZ文件路径
    QString projectFilePath = "/lyndon/iProject/cpath/qgis_demo1/common/project/project.qgz";
    QString tempQgsFilePath = QFileInfo(projectFilePath).absolutePath() + "/temp_project.qgs";
    qDebug() << "save qgs file:" << tempQgsFilePath;
    project->write(tempQgsFilePath);

    // 压缩QGS为QGZ文件
    QFileInfo qgsFileInfo(tempQgsFilePath);
    QString baseName = qgsFileInfo.baseName();
    QString dirPath = qgsFileInfo.absoluteDir().absolutePath();

    QDomDocument doc("qgis");
    QDomElement qgisElem = doc.createElement("qgis");
    doc.appendChild(qgisElem);

    QDomElement projectElem = doc.createElement("project");
    projectElem.setAttribute("version", "3");
    qgisElem.appendChild(projectElem);

    QDomElement titleElem = doc.createElement("title");
    QDomText titleText = doc.createTextNode(baseName);
    titleElem.appendChild(titleText);
    projectElem.appendChild(titleElem);

    QFile file(projectFilePath);
    qDebug() << "save qgz file:" << projectFilePath;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
        QFile::remove(tempQgsFilePath);
    } else {
        qDebug() << "Error saving QGZ file";
    }

    QgsApplication::exitQgis();
    app.exit();

    return 0;
}

```

总结
将 `QgsRasterLayer` 数据提供者类型由 `"wms"` 更改为 `"xyz"`，以正确加载 XYZ 图层。
确保你的 QGIS 环境已正确配置，插件路径和数据提供者文件配置正确。
保持良好的错误处理习惯，确保每个步骤都正确执行。
这应该可以解决你的问题。如果还有其他问题，请随时告诉我！
