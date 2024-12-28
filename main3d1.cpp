#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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
#include <qgis.h>
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
#include <qgsprovidermetadata.h>
#include <qgs3d.h>
#include <qgs3dmapcanvas.h>
#include <qgswkbtypes.h>
#include <qgslayoutmanager.h>


#include "config.h"
#include "core/qgis/payload/InputPoint.h"
#include "core/utils/QgsUtil.h"
#include "core/utils/ImageUtil.h"
#include "qgspluginlayerregistry.h"
#include "core/qgis/style/StylePoint.h"
#if defined(_WIN32)
#include <windows.h>
#endif


int main(int argc, char *argv[]) {
#if defined(_WIN32)
	// 设置控制台输出为UTF - 8
	SetConsoleOutputCP(CP_UTF8);
	QLocale::setDefault(QLocale(QLocale::Chinese, QLocale::China));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    // init QGIS app
    QgsApplication app(argc, argv, true);
    QgsApplication::setPrefixPath(QGIS_PREFIX_PATH, true);
    //QgsApplication::setPrefixPath("/usr", true);
    QgsApplication::init();
    QgsApplication::initQgis();
    Qgs3D::initialize();

    qDebug() << "Plugin path: " << QgsApplication::pluginPath();
    QString save_qgis_project_path = QString(QGIS_PROJECT_PATH);

    // create QGIS project
    QgsProject *project = QgsProject::instance();
    project->removeAll();
    QString crs = "EPSG:3857";
    QgsCoordinateReferenceSystem qgscrs;
    qgscrs.createFromString(crs);
    project->setCrs(qgscrs);

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

    // create MAIN XYZ layer
    QString urlString = "http://47.94.145.6/map/orthogonal/1847168269595754497-健康谷正射";
    QString encodedOrthogonalXyzUrl = "type=xyz&url=";
    encodedOrthogonalXyzUrl.append(urlString);
    encodedOrthogonalXyzUrl.append("/{z}/{x}/{y}");
    encodedOrthogonalXyzUrl.append(".png");
    qDebug().noquote() << "encodedOrthogonalXyzUrl: " << encodedOrthogonalXyzUrl;
    QgsRasterLayer orthogonalLayer(encodedOrthogonalXyzUrl, MAIN_TILE_NAME, "wms");
    if (!orthogonalLayer.isValid()) {
        qWarning() << "XYZ orthogonalLayer layer invalid!" << orthogonalLayer.error().message();
        return -1;
    }
    project->addMapLayer(&orthogonalLayer);
    qDebug() << "add orthogonal layer to project";

    // 3D Scene
    QString realistic3d_tile_url =
            "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
    QgsTiledSceneLayer tiled_scene_layer(realistic3d_tile_url, REAL3D_TILE_NAME, "cesiumtiles");
    QgsTiledSceneLayer3DRenderer *qgs_tiled_scene_layer_3d_renderer = new QgsTiledSceneLayer3DRenderer();
    qgs_tiled_scene_layer_3d_renderer->setLayer(&tiled_scene_layer);
    tiled_scene_layer.setRenderer3D(qgs_tiled_scene_layer_3d_renderer);
    if (!tiled_scene_layer.isValid()) {
        qWarning() << "cesiumtiles 3D tiled_scene_layer layer invalid!" << tiled_scene_layer.error().message();
        return -1;
    }
    // add layer to project
    project->addMapLayer(&tiled_scene_layer);
    qDebug() << "add 3d scene layer to project";

    QString layer_name = QString("民警");
    // QString icon_base64 =
    //         "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAolBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpPUAMub////z9f6mt/by9P1Ia+2MovP6+/72+P7AzPiUqfQrVOoFNuYgTOnp7fzi5/xOcO2dr/UuV+p6lPISQOisvPejtPZ/l/IcSOje5Ptaeu9EaOybrvV3kfFTdO7X3vvI0vm9yfi6x/iDm/Jvi/BphvBVdu5Nb+05X+uPpPSOpPN4UAcsAAAAC3RSTlMA0y0M/fny7sF6UvKXmSMAAAGJSURBVDjLhZNXYoMwDEAhkGnZmGFICZBA9l7t/a9WRx6h6Qfvx0g22nLeeGN32Ke0P3THnvOfnhtAkLAwZIn8cHsf194ojgqeESTjRRSPvD+/D4BNSYspg0HLyMSnM/LBjPoT+7+fp6icX9liwa5zFMrc1za8AcX7VFBYheEKqEC5pAMVxwjQ/gYkBec1SDboBUboIGbKfiQvdpwX8ohUxCx+OXG1REJ5E+y+X++EziVyZQTBlijm+ZfGX2pVEXjOGDjRnEFzNhoOY8cNMiOKphF1LapdaDRZ4DrDhBiOAP7hkAMcrSoZOn1mJQEaYVWs79DQSiVoSqsKKT4wLABZkNYD5cImCpJ8TlouMEjLxpbZBolpWu4gWWtBp/kuVNqcAH5kHKcmbRXKCwqVQqISWOORlKbUplmPGJD9XZ3xQzXLtHtJQfF8goIusd1mYNag2e9Bs8aBMSOXVStAqkqdqyZL1cjZoV3y20XUVVWLy41L+6ka2u6x714cu3rb9+pt7ep1L2/n+v8CysQxDA9OhyUAAAAASUVORK5CYII=";
    QString icon_base64 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAolBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpPUAMub////z9f6mt/by9P1Ia+2MovP6+/72+P7AzPiUqfQrVOoFNuYgTOnp7fzi5/xOcO2dr/UuV+p6lPISQOisvPejtPZ/l/IcSOje5Ptaeu9EaOybrvV3kfFTdO7X3vvI0vm9yfi6x/iDm/Jvi/BphvBVdu5Nb+05X+uPpPSOpPN4UAcsAAAAC3RSTlMA0y0M/fny7sF6UvKXmSMAAAGJSURBVDjLhZNXYoMwDEAhkGnZmGFICZBA9l7t/a9WRx6h6Qfvx0g22nLeeGN32Ke0P3THnvOfnhtAkLAwZIn8cHsf194ojgqeESTjRRSPvD+/D4BNSYspg0HLyMSnM/LBjPoT+7+fp6icX9liwa5zFMrc1za8AcX7VFBYheEKqEC5pAMVxwjQ/gYkBec1SDboBUboIGbKfiQvdpwX8ohUxCx+OXG1REJ5E+y+X++EziVyZQTBlijm+ZfGX2pVEXjOGDjRnEFzNhoOY8cNMiOKphF1LapdaDRZ4DrDhBiOAP7hkAMcrSoZOn1mJQEaYVWs79DQSiVoSqsKKT4wLABZkNYD5cImCpJ8TlouMEjLxpbZBolpWu4gWWtBp/kuVNqcAH5kHKcmbRXKCwqVQqISWOORlKbUplmPGJD9XZ3xQzXLtHtJQfF8goIusd1mYNag2e9Bs8aBMSOXVStAqkqdqyZL1cjZoV3y20XUVVWLy41L+6ka2u6x714cu3rb9+pt7ep1L2/n+v8CysQxDA9OhyUAAAAASUVORK5CYII=";
    //QString icon_base64 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAulBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpvaMovMAMub////y9P709v4GNub6+/5Oce2cr/WMovQsVerBzfmmt/ZIa+0hTOmltvaCmvLi5/zN1vqquvYSQOjr7/3d4/vT3PobSOgOPefu8f3Y3/vI0vmZrfV+l/J1kPFtifBnhfBVdu47Yus3XusKOef29/64xfivvvegsvWUqfSHnvNwjPBdfO9FaewzWuskT+mEnPJjge9knNjHAAAADHRSTlMA0y0M+/ny7sF6U1IqhgF4AAABmklEQVQ4y4VT55qjMAyEkHqnkSEQICFA+qZttt+2u/d/rTOsjLf8YPiDP1mekTRyLLyRO+gp1Ru4I8/5iY7L4DiJoiRmsNv5FvaGKghTn2r4aRioofclvY/ZlD5hOkP/0yO/ujymbxhz93eT351PqMb6vP9rrk7m3Y7w91niZ8YcuHqXG9z/0DGEJF0j4NNKYXkrLBjWBGom4hcIjrSOgT/CMlMViRuI/hsAq8sYmsTUErhaAYdy/Adgrj/gjgQhe84IqZzesuIx01hlBxKkGDku+2Twtiiyp/yeGvjsOoOYLApoXOrfD+J44PQSsrgAyKufchPuq+KTnqMiG98cATzol5+3u5eaOVL2gj/eM7A9+XS9xOJMNSJuKKY7AMFTSTdXAF4lS1MYkSsA9xN6L1AhX5OINGW+AruUyihA8QIw56ZM06jD4uT7xy2gJT0mObCRRplWT8pKG6BKXWOR5w+m1TIsmZUdpB2WjFvayKXE7LitYdZ3QNYErWGs5Q5YbpqotZw17e2zeM2ats327YvTvnrty9u6/v8BXLQ2Gr2F3XEAAAAASUVORK5CYII=";
    QgsVectorLayer *point_layer = new QgsVectorLayer(
        QStringLiteral("Point?crs=EPSG:3857"), layer_name, QStringLiteral("memory"));
    QgsVectorDataProvider *point_provider = point_layer->dataProvider();

    QList<QgsField> fields;
    fields.append(QgsField(QStringLiteral("name"), QMetaType::Type::QString));
    fields.append(QgsField("type", QMetaType::Type::QString));
    fields.append(QgsField("x", QMetaType::Type::Double));
    fields.append(QgsField("y", QMetaType::Type::Double));
    fields.append(QgsField("z", QMetaType::Type::Double));

    point_provider->addAttributes(fields);
    point_layer->updatedFields();

    QgsCoordinateReferenceSystem crs_4326("EPSG:4326");
    QgsCoordinateReferenceSystem crs_3857("EPSG:3857");
    QgsCoordinateTransform *transformer = new QgsCoordinateTransform(crs_4326, crs_3857, project);

    point_layer->startEditing();
    //QgsPoint point(111.482116, 40.724241, 1019.501738);
    QList<InputPoint> points;
    points.append(InputPoint(QString("等等") ,QgsPoint(111.482116, 40.724241, 1019.501738)));
    points.append(InputPoint(QString("张三"), QgsPoint(111.482624, 40.724823, 1020.262739)));
    points.append(InputPoint(QString("李四"), QgsPoint(111.483443, 40.724982, 1020.800378)));
    points.append(InputPoint(QString("王五"), QgsPoint(111.484411, 40.724352, 1022.498272)));
    points.append(InputPoint(QString("赵六"), QgsPoint(111.483948, 40.725397, 1021.921124)));
    points.append(InputPoint(QString("冯七"), QgsPoint(111.484871, 40.724699, 1024.298574)));
    points.append(InputPoint(QString("马八"), QgsPoint(111.484869,40.726562,1023.229612)));
    qDebug() << "points size:" << points.size();
    for (InputPoint point : points) {
        QgsPointXY *qgs_point_xy = new QgsPointXY(point.getPoint().x(), point.getPoint().y());
        QgsPointXY transformed_point = transformer->transform(*qgs_point_xy);

        QgsPoint *qgs_point = new QgsPoint(transformed_point.x(), transformed_point.y(), point.getPoint().z());

        qDebug() << "qgs_point x:" << qgs_point->x() << " y:" << qgs_point->y() << " z:" << qgs_point->z();

        QgsAttributes *attributes = new QgsAttributes{};
        attributes->append(point.getName());
        attributes->append("point");
        attributes->append(qgs_point->x());
        attributes->append(qgs_point->y());
        attributes->append(qgs_point->z());

        QgsFeature *feature = new QgsFeature(point_layer->fields());
        feature->setAttributes(*attributes);
        QgsGeometry geometry = QgsGeometry::fromPoint(*qgs_point);
        /*QgsPoint vertexOfGeometry = geometry.vertexAt(0);
        qDebug() << "geometry: " << vertexOfGeometry.x() << " " << vertexOfGeometry.y() << " " << vertexOfGeometry.z();*/
        feature->setGeometry(geometry);
        point_provider->addFeature(*feature);
    }

    // # Commit changes to the vector layer
    if (point_layer->commitChanges()) {
        qInfo() << "info: point layer commit changes 数据已成功提交到图层";
    } else {
        qCritical() << "critical：point layer commit changes 数据提交到图层失败:" << point_provider->error().message();
    }
    // # Save the vector layer to a GeoJSON file
    // QgsVectorFileWriter::SaveVectorOptions options{};
    // options.driverName = "GeoJSON";
    // options.fileEncoding = "UTF-8";
    // options.includeZ = true;
    // options.overrideGeometryType = Qgis::WkbType::PointZ;
    //
    // QString layer_persist_prefix = QString().append(save_qgis_project_path).append("/").append(layer_name);
    // QString geojson_path = QString(layer_persist_prefix).append(".geojson");
    // QgsVectorFileWriter *writer = QgsVectorFileWriter::create(geojson_path, fields, Qgis::WkbType::PointZ,
    //                                                           project->crs(), QgsCoordinateTransformContext(), options);
    // writer->writeAsVectorFormatV3(point_layer, geojson_path, project->transformContext(), options);
    // delete writer;
    // // delete point_layer;
    // // # Load the GeoJSON file
    // QgsVectorLayer *p_point_layer = new QgsVectorLayer(geojson_path, layer_name, "ogr");
    QgsVectorLayer *p_point_layer = QgsUtil::write_persisted_layer(layer_name, point_layer, save_qgis_project_path, fields,
                                                                  Qgis::WkbType::PointZ,
                                                                  project->transformContext(),
                                                                  project->crs());
    p_point_layer->setCrs(project->crs());

    if (!p_point_layer->isValid()) {
        qCritical("Failed to load the layer!");
        exit(1);
    }
    // # icon_path = "D:/iProject/pypath/qgis-x/common/icon/民警.png"
    // QString icon_path = QString(layer_persist_prefix).append(".png");
    qDebug() << "icon_base64:" << icon_base64;
    QString icon_path = QString().append(save_qgis_project_path).append("/").append(layer_name).append(".png");
    qDebug() << "icon_path:" << icon_path;
    if (!icon_base64.isEmpty())
    {
        std::pair<QString, QByteArray> base64_image = ImageUtil::parse_base64_image(icon_base64);
        QFile iconFile(icon_path);
        if (iconFile.open(QIODevice::WriteOnly)) {
            iconFile.write(base64_image.second);
            iconFile.close();
        }
        else {
            qWarning() << "Failed to open file for writing:" << icon_path;
        }
    }
    /*if (!icon_base64.isEmpty()) {
        QByteArray decodedData = QByteArray::fromBase64(icon_base64.toUtf8());
        QFile iconFile(icon_path);
        if (iconFile.open(QIODevice::WriteOnly)) {
            iconFile.write(decodedData);
            iconFile.close();
        } else {
            qWarning() << "Failed to open file for writing:" << icon_path;
        }
    }*/

    QString styleInfoJson = "{\"color\":\"rgba(255,255,255,1)\",\"bim\":\"\",\"num\":2,\"loadFlag\":false,\"fontStyle\":{\"borderColor\":\"rgba(255,255,255,1)\",\"loadFlag\":true,\"x\":0,\"y\":35,\"fontSize\":60,\"fontColor\":\"rgba(237, 233, 26, 1)\",\"fontFlag\":true},\"radius\":100}";
    QJsonObject styleInfoJsonObj = QJsonDocument::fromJson(styleInfoJson.toUtf8()).object();
    QJsonObject fontStyleObj = styleInfoJsonObj["fontStyle"].toObject();

    QString layerStyle = "{\"scale\":0.8}";
    QJsonObject layerStyleObj = QJsonDocument::fromJson(layerStyle.toUtf8()).object();

    QgsFeatureRenderer* feature_renderer = StylePoint::get2d_rule_based_renderer(fontStyleObj, layerStyleObj, icon_path, 20.0);

    p_point_layer->setRenderer(feature_renderer);

    p_point_layer->triggerRepaint();

    project->addMapLayer(p_point_layer);




    QgsMapCanvas *canvas = new QgsMapCanvas();
    canvas->setDestinationCrs(qgscrs);
    QgsMapSettings qgs_map_settings = canvas->mapSettings();
    qgs_map_settings.setDestinationCrs(qgscrs);
    qDebug() << "qgs_map_settings extent: " << qgs_map_settings.extent();
    QgsRectangle extent;
    QgsCoordinateReferenceSystem canvasCrs = qgs_map_settings.destinationCrs();
    QgsCoordinateTransformContext transformContext = project->transformContext();
    QList<QgsMapLayer *> b_layers{};
    auto qgs_map_layers = project->mapLayers().values();
    for (auto qgs_map_layer: qgs_map_layers) {
        QString layerName = qgs_map_layer->name();
        QgsRectangle ext = qgs_map_layer->extent();
        QgsCoordinateReferenceSystem layerCrs = qgs_map_layer->crs();
        QgsCoordinateTransform transform(layerCrs, canvasCrs, transformContext);

        if (layerName != BASE_TILE_NAME && layerName != MAIN_TILE_NAME
            && !layerName.startsWith(MAIN_TILE_NAME)
            && !layerName.startsWith(REAL3D_TILE_NAME)
        ) {
            b_layers.append(qgs_map_layer);
            ext = transform.transform(ext);
            QString layer_crs_name = qgs_map_layer->crs().authid();
            qDebug() << "layerName:" << layerName << "crs:" << layer_crs_name << " ext: " << ext
                    << " width: " << ext.width() << " height: " << ext.height()
                    << " xMinimum:" << ext.xMinimum() << " yMinimum:" << ext.yMinimum()
                    << " xMaximum:" << ext.xMaximum() << " yMaximum:" << ext.yMaximum()
                    << " area: " << ext.area() << " perimeter: " << ext.perimeter() << " center: " << ext.center()
                    << " isEmpty: " << ext.isEmpty() << " isNull: " << ext.isNull() << " isFinite: " << ext.isFinite();
            extent.combineExtentWith(ext);
        } else if (layerName.startsWith(REAL3D_TILE_NAME)) {
            QString layer_crs_name = qgs_map_layer->crs().authid();
            ext = transform.transform(ext);
            qDebug() << "layerName:" << layerName << "crs:" << layer_crs_name << " ext: " << ext
                    << " width: " << ext.width() << " height: " << ext.height()
                    << " xMinimum:" << ext.xMinimum() << " yMinimum:" << ext.yMinimum()
                    << " xMaximum:" << ext.xMaximum() << " yMaximum:" << ext.yMaximum()
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
    qgs_map_settings.setLayers(b_layers);
    qgs_map_settings.setExtent(extent, false);
    canvas->setLayers(b_layers);
    canvas->setExtent(extent, false);
    canvas->refresh();
    QgsReferencedRectangle *referenced_rectangle = new QgsReferencedRectangle(extent, project->crs());
    project->viewSettings()->setDefaultViewExtent(*referenced_rectangle);

    QString projectPath = QString().append(save_qgis_project_path).append(QGIS_PROJECT_FILE_NAME);

    // save to .qgz file
    qDebug() << "projectPath:" << projectPath;
    if (!project->write(projectPath)) {
        qWarning() << "save projectPath to file " << projectPath << " failed!";
        return -1;
    }

    qDebug() << "save projectPath:" << projectPath << "successes";

    QgsLayoutManager *layout_manager = project->layoutManager();
    QList<QgsMasterLayoutInterface *> layouts = layout_manager->layouts();
    for (int i = 0; i < layouts.size(); ++i) {
        QgsMasterLayoutInterface *layout = layouts.at(i);
        qDebug() << "remove layout name:" << layout->name();
        layout_manager->removeLayout(layout);
    }
    layout_manager->clear();
    // project->clear();

    //return app.exec();
    return 0;
}
