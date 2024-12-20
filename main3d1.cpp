#include <execution>
#include <QCoreApplication>
#include <QTextCodec>
#include <QFileInfo>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgslayertree.h>

#include "config.h"
#include "qgspluginlayerregistry.h"

int main(int argc, char *argv[])
{
    // 初始化 QGIS 应用程序
    QgsApplication app(argc, argv, true);
    // QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
    QgsApplication::setPrefixPath(qgisPrefixPath, true);
    QgsApplication::init();
    QgsApplication::initQgis();

    qDebug() << "Plugin path: " << QgsApplication::pluginPath();

    /*QgsPluginLayerRegistry* plugin_layer_registry = QgsApplication::pluginLayerRegistry();
    if (plugin_layer_registry) {
        QStringList plugin_layer_types = plugin_layer_registry->pluginLayerTypes();
        qDebug() << "plugin type:" << plugin_layer_types.join(", ");
    } else {
        qDebug() << "Failed to load plugin registry";
    }*/

    // 创建一个新的 QGIS 工程
    QgsProject *project = QgsProject::instance();

    // 创建 XYZ 图层 (使用公开的 OpenStreetMap URL)
    QString baseXyzUrl = "type=xyz&url=http://47.94.145.6/map/lx/{z}/{x}/{y}.png&zmax=19&zmin=0";
    //QString baseXyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    QString baseLayerName = "BaseMap";
    QgsRasterLayer *baseXyzLayer = new QgsRasterLayer(baseXyzUrl, baseLayerName, "wms");

    if (!baseXyzLayer->isValid()) {
        qWarning() << "XYZ xyzLayer 图层无效!" << baseXyzLayer->error().message();
        return -1;
    }
    // 将图层添加到项目中
    project->addMapLayer(baseXyzLayer);


    // 创建 XYZ 图层 (使用公开的 OpenStreetMap URL)
    QString urlString = "/map/orthogonal/1847168269595754497-健康谷正射";
    QString prefix = "/map/orthogonal/1847168269595754497-";

    std::string chinesePt = "健康谷正射";
    std::cout << "chinesePt:" << chinesePt << std::endl;

    QString chinesePart = "健康谷正射";
    // auto byte_array = chinesePart.toLatin1();
    std::string basic_string = QUrl::toPercentEncoding(chinesePart.toLatin1()).toStdString();
    QString partStr = QString::fromStdString(basic_string);
    qDebug() << "partStr:" << partStr;
    // QString basePart = "/map/orthogonal/test";

    QString encodedOrthogonalXyzUrl = "type=xyz&url=http://47.94.145.6" + prefix +  partStr + "/{z}/{x}/{y}.png";
    //QString xyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    QString orthogonalLayerName = "OrthogonalMap";
    QgsRasterLayer *orthogonalLayer = new QgsRasterLayer(encodedOrthogonalXyzUrl, orthogonalLayerName, "wms");

    if (!orthogonalLayer->isValid()) {
        qWarning() << "XYZ orthogonalLayer 图层无效!" << orthogonalLayer->error().message();
        return -1;
    }
    // 将图层添加到项目中
    project->addMapLayer(orthogonalLayer);


    qDebug() << "qgisProjectPath:" << qgisProjectPath;
    // 保存项目为 .qgz 文件
    QString projectPath = qgisProjectPath;
    qDebug() << "projectPath:" << projectPath;
    if (!project->write(projectPath)) {
        qWarning() << "无法保存项目文件!";
        return -1;
    }

    qDebug() << "项目已成功保存到" << projectPath;

    // 释放 QGIS 资源
    QgsApplication::exitQgis();
    return 0;
}

