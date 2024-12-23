#include <fstream>
#include <vector>
#include <QByteArray>
#include <QFile>
#include <QIODevice>

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
#include <qgsvectorlayer.h>
#include <qgsgeometry.h>
#include <qgslayertree.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsmapcanvas.h>
#include <qgsprojectviewsettings.h>
#include <qgsvectorfilewriter.h>

#include "config.h"
#include "qgspluginlayerregistry.h"

int main(int argc, char *argv[]) {
    // init QGIS app
    QgsApplication app(argc, argv, true);
    // QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
    QgsApplication::setPrefixPath(QGIS_PREFIX_PATH, true);
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

    QgsMapCanvas canvas;
    canvas.setDestinationCrs(qgscrs);
    QgsMapSettings map_settings;
    map_settings.setDestinationCrs(qgscrs);

    // create XYZ base layer
    QString baseXyzUrl = "type=xyz&url=http://47.94.145.6/map/lx/{z}/{x}/{y}.png&zmax=19&zmin=0";
    //QString baseXyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
    QgsRasterLayer baseXyzLayer(baseXyzUrl, BASE_TILE_NAME, "wms");

    if (!baseXyzLayer.isValid()) {
        qWarning() << "XYZ xyzLayer layer invalid!" << baseXyzLayer.error().message();
        return -1;
    }
    // add base layer to project
    project->addMapLayer(&baseXyzLayer);
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
    QgsRasterLayer orthogonalLayer(encodedOrthogonalXyzUrl, MAIN_TILE_NAME, "wms");

    if (!orthogonalLayer.isValid()) {
        qWarning() << "XYZ orthogonalLayer layer invalid!" << orthogonalLayer.error().message();
        return -1;
    }
    // add orthogonal layer to project
    project->addMapLayer(&orthogonalLayer);
    qDebug() << "add orthogonal layer to project";

    // 3D Scene
    // QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
    QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json";
    QgsTiledSceneLayer tiled_scene_layer(realistic3d_tile_url, REAL3D_TILE_NAME,
                                         "cesiumtiles");
    QgsTiledSceneLayer3DRenderer qgs_tiled_scene_layer_3d_renderer;
    tiled_scene_layer.setRenderer3D(&qgs_tiled_scene_layer_3d_renderer);
    if (!tiled_scene_layer.isValid()) {
        qWarning() << "cesiumtiles 3D tiled_scene_layer layer invalid!" << tiled_scene_layer.error().message();
        return -1;
    }
    // add layer to project
    project->addMapLayer(&tiled_scene_layer);
    qDebug() << "add 3d scene layer to project";

    QString layer_name = QString("民警");
    QString icon_base64 =
            "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAolBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpPUAMub////z9f6mt/by9P1Ia+2MovP6+/72+P7AzPiUqfQrVOoFNuYgTOnp7fzi5/xOcO2dr/UuV+p6lPISQOisvPejtPZ/l/IcSOje5Ptaeu9EaOybrvV3kfFTdO7X3vvI0vm9yfi6x/iDm/Jvi/BphvBVdu5Nb+05X+uPpPSOpPN4UAcsAAAAC3RSTlMA0y0M/fny7sF6UvKXmSMAAAGJSURBVDjLhZNXYoMwDEAhkGnZmGFICZBA9l7t/a9WRx6h6Qfvx0g22nLeeGN32Ke0P3THnvOfnhtAkLAwZIn8cHsf194ojgqeESTjRRSPvD+/D4BNSYspg0HLyMSnM/LBjPoT+7+fp6icX9liwa5zFMrc1za8AcX7VFBYheEKqEC5pAMVxwjQ/gYkBec1SDboBUboIGbKfiQvdpwX8ohUxCx+OXG1REJ5E+y+X++EziVyZQTBlijm+ZfGX2pVEXjOGDjRnEFzNhoOY8cNMiOKphF1LapdaDRZ4DrDhBiOAP7hkAMcrSoZOn1mJQEaYVWs79DQSiVoSqsKKT4wLABZkNYD5cImCpJ8TlouMEjLxpbZBolpWu4gWWtBp/kuVNqcAH5kHKcmbRXKCwqVQqISWOORlKbUplmPGJD9XZ3xQzXLtHtJQfF8goIusd1mYNag2e9Bs8aBMSOXVStAqkqdqyZL1cjZoV3y20XUVVWLy41L+6ka2u6x714cu3rb9+pt7ep1L2/n+v8CysQxDA9OhyUAAAAASUVORK5CYII=";
    QgsVectorLayer point_layer(QString("Point?crs=EPSG:3857"), layer_name, QString("memory"));
    QgsVectorDataProvider *point_provider = point_layer.dataProvider();

    QList<QgsField> fields{};

    QgsField f_name("name", QMetaType::Type::QString, QString(), 254);
    fields.append(f_name);
    QgsField f_type("type", QMetaType::Type::QString, QString(), 254);
    fields.append(f_type);
    QgsField f_x("x", QMetaType::Type::Double);
    fields.append(f_x);
    QgsField f_y("y", QMetaType::Type::Double);
    fields.append(f_y);
    QgsField f_z("z", QMetaType::Type::Double);
    fields.append(f_z);

    point_provider->addAttributes(fields);
    point_layer.updatedFields();

    QgsCoordinateReferenceSystem crs_4326("EPSG:4326");
    QgsCoordinateReferenceSystem crs_3857("EPSG:3857");
    QgsCoordinateTransform transformer(crs_4326, crs_3857, project);

    point_layer.startEditing();

    QgsVector3D point3d(111.482116, 40.724241, 1019.501738);
    QgsVector3D qgs_vector_3d = transformer.transform(point3d);

    QgsFeature feature(point_layer.fields());

    QgsPoint qgs_point(qgs_vector_3d.x(), qgs_vector_3d.y(), qgs_vector_3d.z());

    std::unique_ptr<QgsPoint> up_qgs_point(&qgs_point);
    feature.setGeometry(QgsGeometry(up_qgs_point.get()));
    // print(f"{point_name_prefix}{i + 1}", transformed_point.x(), transformed_point.y())
    // feature.setAttributes([f"{point_name_prefix}{i + 1}", "point", qgs_vector_3d.x(), qgs_vector_3d.y()])
    QgsAttributes attributes{};
    attributes.append("张三");
    attributes.append("point");
    attributes.append(qgs_vector_3d.x());
    attributes.append(qgs_vector_3d.y());
    attributes.append(qgs_vector_3d.z());
    feature.setAttributes(attributes);

    QgsFeatureList qgs_feature_list{feature};
    point_provider->addFeatures(qgs_feature_list);

    // # Commit changes to the vector layer
    if (point_layer.commitChanges()) {
        qInfo() << "数据已成功提交到图层";
    } else {
        qCritical() << "数据提交到图层失败:" << point_provider->error().message();
    }


    // # Save the vector layer to a GeoJSON file
    QgsVectorFileWriter::SaveVectorOptions options{};
    options.driverName = "GeoJSON";
    options.fileEncoding = "UTF-8";

    QString qgis_project_path = QString(QGIS_PROJECT_PATH);
    QString layer_persist_prefix = qgis_project_path.append("/").append(layer_name);
    QString geojson_path = layer_persist_prefix.append(".geojson");
    QgsVectorFileWriter::writeAsVectorFormatV3(&point_layer, geojson_path, QgsCoordinateTransformContext(), options);
    // # Load the GeoJSON file
    QgsVectorLayer p_point_layer(geojson_path, layer_name, "ogr");
    if (!p_point_layer.isValid()) {
        qCritical("Failed to load the layer!");
        exit(1);
    }
    // # icon_path = "D:/iProject/pypath/qgis-x/common/icon/民警.png"
    QString icon_path = layer_persist_prefix.append(".png");

    if (!icon_base64.isEmpty()) {
        QByteArray decodedData = QByteArray::fromBase64(icon_base64.toUtf8());
        QFile iconFile(icon_path);
        if (iconFile.open(QIODevice::WriteOnly)) {
            iconFile.write(decodedData);
            iconFile.close();
        } else {
            qWarning() << "Failed to open file for writing:" << icon_path;
        }
    }

    project->addMapLayer(&point_layer);


    QgsRectangle extent;
    auto qgs_map_layers = project->mapLayers().values();
    for (auto qgs_map_layer: qgs_map_layers) {
        QString layerName = qgs_map_layer->name();
        if (layerName != BASE_TILE_NAME && layerName != MAIN_TILE_NAME && !layerName.startsWith(MAIN_TILE_NAME)) {
            QgsRectangle ext = qgs_map_layer->extent();
            qDebug() << "layerName:" << layerName << " ext: " << ext << " width: " << ext.width() << " height: " << ext.
                    height()
                    << " xMinimum:" << extent.xMinimum() << " yMinimum:" << extent.yMinimum() << " xMaximum:" << extent.
                    xMaximum() << " yMaximum:" << extent.yMaximum()
                    << " area: " << ext.area() << " perimeter: " << ext.perimeter() << " center: " << ext.center()
                    << " isEmpty: " << ext.isEmpty() << " isNull: " << ext.isNull() << " isFinite: " << ext.isFinite();

            extent.combineExtentWith(ext);
        }
    }

    qDebug() << "extent: " << extent << " width: " << extent.width() << " height: " << extent.height()
            << " xMinimum:" << extent.xMinimum() << " yMinimum:" << extent.yMinimum() << " xMaximum:" << extent.
            xMaximum() << " yMaximum:" << extent.yMaximum()
            << " area: " << extent.area() << " perimeter: " << extent.perimeter() << " center: " << extent.center()
            << " isEmpty: " << extent.isEmpty() << " isNull: " << extent.isNull() << " isFinite: " << extent.isFinite();


    map_settings.setExtent(extent);
    canvas.setExtent(extent);
    canvas.refresh();


    QgsReferencedRectangle qgs_extent(extent, qgscrs);
    QgsProjectViewSettings *qgs_project_view_settings = project->viewSettings();
    qgs_project_view_settings->setDefaultViewExtent(qgs_extent);
    qgs_project_view_settings->setPresetFullExtent(qgs_extent);

    QString save_qgis_project_path = QString(QGIS_PROJECT_PATH);
    QString projectPath = save_qgis_project_path.append(QGIS_PROJECT_FILE_NAME);

    // save to .qgz file
    qDebug() << "projectPath:" << projectPath;
    if (!project->write(projectPath)) {
        qWarning() << "save projectPath to file " << projectPath << " failed!";
        return -1;
    }

    qDebug() << "save projectPath:" << projectPath << "successes";

    // release qgis resource
    QgsApplication::exitQgis();
    return 0;
}
