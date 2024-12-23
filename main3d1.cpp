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

int main(int argc, char *argv[]) {
    // init QGIS app
    QgsApplication app(argc, argv, true);
    // QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
    QgsApplication::setPrefixPath(qgisPrefixPath, true);
    QgsApplication::init();
    QgsApplication::initQgis();

    qDebug() << "Plugin path: " << QgsApplication::pluginPath();

    // create QGIS project
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

    // create XYZ base layer
    QString baseXyzUrl = "type=xyz&url=http://47.94.145.6/map/lx/{z}/{x}/{y}.png&zmax=19&zmin=0";
    //QString baseXyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    auto *baseXyzLayer = new QgsRasterLayer(baseXyzUrl, BASE_TILE_NAME, "wms");

    if (!baseXyzLayer->isValid()) {
        qWarning() << "XYZ xyzLayer layer invalid!" << baseXyzLayer->error().message();
        return -1;
    }
    // add base layer to project
    project->addMapLayer(baseXyzLayer);
    qDebug() << "add base layer to project";

    QTextStream out(stdout);
    out.setCodec("UTF-8");
    QString ch = QString::fromUtf8("健康谷正射");
    qDebug() << "ch:" << ch;

    // create XYZ layer
    QString urlString = QString::fromUtf8("http://47.94.145.6/map/orthogonal/1847168269595754497-健康谷正射");
    qDebug() << "urlString:" << urlString;
    // QTextStream(stdout) << "urlString:" << urlString << endl;
    QString encodedOrthogonalXyzUrl = "type=xyz&url=";
    encodedOrthogonalXyzUrl.append(urlString);
    encodedOrthogonalXyzUrl.append("/{z}/{x}/{y}");
    encodedOrthogonalXyzUrl.append(".png");
    // QTextStream(stdout) << "encodedOrthogonalXyzUrl: " << encodedOrthogonalXyzUrl << Qt::endl;
    qDebug().noquote() << "encodedOrthogonalXyzUrl: " << encodedOrthogonalXyzUrl;
    //QString xyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    auto *orthogonalLayer = new QgsRasterLayer(encodedOrthogonalXyzUrl, MAIN_TILE_NAME, "wms");

    if (!orthogonalLayer->isValid()) {
        qWarning() << "XYZ orthogonalLayer layer invalid!" << orthogonalLayer->error().message();
        return -1;
    }
    // add orthogonal layer to project
    project->addMapLayer(orthogonalLayer);
    qDebug() << "add orthogonal layer to project";

    // 3D Scene
    // QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
    QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json";
    auto *tiled_scene_layer = new QgsTiledSceneLayer(realistic3d_tile_url, REAL3D_TILE_NAME,
                                                                   "cesiumtiles");
    tiled_scene_layer->setRenderer3D(new QgsTiledSceneLayer3DRenderer());
    if (!tiled_scene_layer->isValid()) {
        qWarning() << "cesiumtiles 3D tiled_scene_layer layer invalid!" << tiled_scene_layer->error().message();
        return -1;
    }
    // add layer to project
    project->addMapLayer(tiled_scene_layer);
    qDebug() << "add 3d scene layer to project";

    QgsRectangle extent;
    auto qgs_map_layers = project->mapLayers().values();
    for (auto qgs_map_layer: qgs_map_layers) {
        QString layerName = qgs_map_layer->name();
        if (layerName != BASE_TILE_NAME && layerName != MAIN_TILE_NAME && !layerName.startsWith(MAIN_TILE_NAME)) {
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
    // save to .qgz file
    QString projectPath = qgisProjectPath;
    qDebug() << "projectPath:" << projectPath;
    if (!project->write(projectPath)) {
        qWarning() << "save projectPath to file " << projectPath << " failed!";
        return -1;
    }

    qDebug() << "projectPath:" << projectPath;

    // release qgis resource
    QgsApplication::exitQgis();
    return 0;
}
