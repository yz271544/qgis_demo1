#include <complex>
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
#include <qgscoordinatereferencesystem.h>
#include <qgsmapcanvas.h>
#include <qgsprojectviewsettings.h>

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





int main(int argc, char *argv[]) {
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
    project->removeAll();
    QString crs = "EPSG:3857";
    QgsCoordinateReferenceSystem qgscrs;
    qgscrs.createFromString(crs);
    project->setCrs(qgscrs);

    auto canvas = new QgsMapCanvas();
    canvas->setDestinationCrs(qgscrs);
    auto map_settings = new QgsMapSettings();
    map_settings->setDestinationCrs(qgscrs);



    // 创建 XYZ 图层 (使用公开的 OpenStreetMap URL)
    QString baseXyzUrl = "type=xyz&url=http://47.94.145.6/map/lx/{z}/{x}/{y}.png&zmax=19&zmin=0";
    //QString baseXyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    auto *baseXyzLayer = new QgsRasterLayer(baseXyzUrl, BASE_TILE_NAME, "wms");

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
    auto *orthogonalLayer = new QgsRasterLayer(encodedOrthogonalXyzUrl, MAIN_TILE_NAME, "wms");

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
    auto *tiled_scene_layer = new QgsTiledSceneLayer(realistic3d_tile_url, REAL3D_TILE_NAME,
                                                                   "cesiumtiles");
    tiled_scene_layer->setRenderer3D(new QgsTiledSceneLayer3DRenderer());
    if (!tiled_scene_layer->isValid()) {
        qWarning() << "cesiumtiles 3D tiled_scene_layer 图层无效!" << tiled_scene_layer->error().message();
        return -1;
    }
    // 将图层添加到项目中
    project->addMapLayer(tiled_scene_layer);
    qDebug() << "add 3d scene layer to project";

    QgsRectangle extent;
    auto qgs_map_layers = project->mapLayers().values();
    for (auto qgs_map_layer: qgs_map_layers) {
        QString layerName = qgs_map_layer->name();
        if (layerName != BASE_TILE_NAME && layerName != MAIN_TILE_NAME && !layerName.startsWith(MAIN_TILE_NAME)) {
            // 将当前图层的范围合并到总的范围中
            QgsRectangle ext = qgs_map_layer->extent();
            qDebug() << "layerName:" << layerName << " ext: " << ext << " width: " << ext.width() << " height: " << ext.height()
            << " xMinimum:" << extent.xMinimum() << " yMinimum:" << extent.yMinimum() << " xMaximum:" << extent.xMaximum() << " yMaximum:" << extent.yMaximum()
            << " area: " << ext.area() << " perimeter: " << ext.perimeter() << " center: " << ext.center()
            << " isEmpty: " << ext.isEmpty() << " isNull: " << ext.isNull() << " isFinite: " << ext.isFinite();

            extent.combineExtentWith(ext);
        }
    }

    qDebug() << "extent: " << extent << " width: " << extent.width() << " height: " << extent.height()
    << " xMinimum:" << extent.xMinimum() << " yMinimum:" << extent.yMinimum() << " xMaximum:" << extent.xMaximum() << " yMaximum:" << extent.yMaximum()
    << " area: " << extent.area() << " perimeter: " << extent.perimeter() << " center: " << extent.center()
    << " isEmpty: " << extent.isEmpty() << " isNull: " << extent.isNull() << " isFinite: " << extent.isFinite();



    map_settings->setExtent(extent);
    canvas->setExtent(extent);
    canvas->refresh();


    QgsReferencedRectangle qgs_extent(extent, qgscrs);
    QgsProjectViewSettings* qgs_project_view_settings = project->viewSettings();
    qgs_project_view_settings->setDefaultViewExtent(qgs_extent);
    qgs_project_view_settings->setPresetFullExtent(qgs_extent);





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
