#include <execution>
#include <QCoreApplication>
#include <QTextCodec>
#include <QUrl>
#include <QUrlQuery>
#include <QFileInfo>
#include <qgstiledscenelayer.h>
#include <qgstiledscenelayer3drenderer.h>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgslayertree.h>

#include "config.h"
#include "qgspluginlayerregistry.h"


QString encodeUrl(QString originalUrl) {
    // QString originalUrl = "https://example.com/路径/文件.html?参数1=值1&参数2=值2";
    qDebug() << "originalUrl: " << originalUrl;
    QUrl url(originalUrl);
    QString path = url.path();
    qDebug() << "path: " << path;
    // for (QString chars : path.split("/")) {
    //     qDebug() << "chars: " << chars;
    //     QString encodedChars = QUrl::toPercentEncoding(chars);
    //     qDebug() << "encodedChars: " << encodedChars;
    //     QString part = path.replace(chars, encodedChars);
    //     qDebug() << "part: " << part;
    //     url.setPath(part);
    // }
    url.setPath(path);

    // QString encodedPath = QUrl::toPercentEncoding(path);
    // qDebug() << "encodedPath: " << encodedPath;
    // url.setPath(encodedPath);
    qDebug() << "url: " << url.toString();
    QString finalEncodedUrl = url.toString();
    // qDebug() << "TEST originalUrl：" << originalUrl;
    // qDebug() << "TEST Encoded URL: " << finalEncodedUrl;
    return finalEncodedUrl;
}



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

    // QString originalUrl = "https://example.com/路径/文件.html?参数1=值1&参数2=值2";
    // QString encoded_test_url= encodeUrl(originalUrl);
    // qDebug() << "encoded_test_url:" << encoded_test_url;



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
    qDebug() << "add base layer to project";

    // 创建 XYZ 图层 (使用公开的 OpenStreetMap URL)
    QString urlString = "http://47.94.145.6/map/orthogonal/1847168269595754497-健康谷正射";

    // QString encoded_url_string = encodeUrl(urlString);
    // qDebug() << "encoded_url_string: " << encoded_url_string;
    // QString encodedOrthogonalXyzUrl = "type=xyz&url=http://47.94.145.6" + prefix +  partStr + "/{z}/{x}/{y}.png";
    QString encodedOrthogonalXyzUrl = "type=xyz&url=" + urlString + "/{z}/{x}/{y}.png";
    qDebug() << "encodedOrthogonalXyzUrl: " << encodedOrthogonalXyzUrl;
    //QString xyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    QString orthogonalLayerName = "OrthogonalMap";
    QgsRasterLayer *orthogonalLayer = new QgsRasterLayer(encodedOrthogonalXyzUrl, orthogonalLayerName, "wms");

    if (!orthogonalLayer->isValid()) {
        qWarning() << "XYZ orthogonalLayer 图层无效!" << orthogonalLayer->error().message();
        return -1;
    }
    // 将图层添加到项目中
    project->addMapLayer(orthogonalLayer);
    qDebug() << "add orthogonal layer to project";

    // 3D Scene
    // QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
    QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json";
    QgsTiledSceneLayer *tiled_scene_layer = new QgsTiledSceneLayer(realistic3d_tile_url, "Real3DTile", "cesiumtiles");
    tiled_scene_layer->setRenderer3D(new QgsTiledSceneLayer3DRenderer());
    if (!tiled_scene_layer->isValid()) {
        qWarning() << "cesiumtiles 3D tiled_scene_layer 图层无效!" << tiled_scene_layer->error().message();
        return -1;
    }
    // 将图层添加到项目中
    project->addMapLayer(tiled_scene_layer);
    qDebug() << "add 3d scene layer to project";

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

