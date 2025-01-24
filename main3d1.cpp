#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <fstream>
#include <vector>
#include <QByteArray>
#include <QFile>
#include <QIODevice>

#include <complex>
// #include <execution>
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
#include <qgsmapviewsmanager.h>
#include <qgsoffscreen3dengine.h>
#include <qgs3dutils.h>

#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <QOffscreenSurface>
#include <QOpenGLContext>

#include "config.h"
#include "core/qgis/payload/InputPoint.h"
#include "core/utils/QgsUtil.h"
#include "core/utils/ImageUtil.h"
#include "qgspluginlayerregistry.h"
#include "core/qgis/style/StylePoint.h"
#include "core/utils/FileUtil.h"


#include "core/qgis/layout/JwLayout.h"
#include "core/qgis/layout/JwLayout3D.h"
#include "core/utils/NodeToMap.h"
#include "core/utils/JsonUtil.h"
#include "core/qgis/d3/ImageCaptureHelper.h"
#include "core/qgis/d3/CameraUtil.h"

#if defined(_WIN32)
#include <windows.h>
#endif


int main(int argc, char* argv[]) {
	qDebug() << "qgis printer server";
#if defined(_WIN32)
	// 设置控制台输出为UTF - 8
	SetConsoleOutputCP(CP_UTF8);
	QLocale::setDefault(QLocale(QLocale::Chinese, QLocale::China));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

	// init QGIS app
	QgsApplication app(argc, argv, true);
	qDebug() << "QGIS_PREFIX_PATH: " << QGIS_PREFIX_PATH;
	QgsApplication::setPrefixPath(QGIS_PREFIX_PATH, true);
	//QgsApplication::setPrefixPath("/usr", true);
	QgsApplication::init();
	QgsApplication::initQgis();
	Qgs3D::initialize();

    // 设置 OpenGL 表面格式
    QSurfaceFormat format;
    format.setVersion(4, 1);  // 设置 OpenGL 版本
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    // 创建离屏表面
    QOffscreenSurface surface;
    surface.setFormat(format);
    surface.create();

    // 创建 OpenGL 上下文
    QOpenGLContext context;
    context.setFormat(format);
    if (!context.create()) {
        qWarning() << "Failed to create OpenGL context";
        return -1;
    }

    // 使上下文成为当前上下文
    if (!context.makeCurrent(&surface)) {
        qWarning() << "Failed to make OpenGL context current";
        return -1;
    }

	qDebug() << "Plugin path: " << QgsApplication::pluginPath();
	QString save_qgis_project_path = QString(QGIS_PROJECT_PATH);

	// create QGIS project
	QgsProject* project = QgsProject::instance();
	project->removeAll();
	QString crs = "EPSG:3857";
	QgsCoordinateReferenceSystem qgscrs;
	qgscrs.createFromString(crs);
	project->setCrs(qgscrs);

	// create XYZ base layer
	 QString baseXyzUrl = "type=xyz&url=http://47.94.145.6/map/lx/{z}/{x}/{y}.png&zmax=19&zmin=0";
	// QString baseXyzUrl = "type=xyz&url=http://172.31.100.34:38083/map/lx/{z}/{x}/{y}.png&zmax=17&zmin=0";
    // QString baseXyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";

	QgsRasterLayer* baseXyzLayer = new QgsRasterLayer(baseXyzUrl, BASE_TILE_NAME, "wms");
	if (!baseXyzLayer->isValid()) {
		qWarning() << "XYZ xyzLayer layer invalid!" << baseXyzLayer->error().message();
		return -1;
	}
	// add base layer to project
	project->addMapLayer(baseXyzLayer);
	qDebug() << "add base layer to project";

	// create MAIN XYZ layer
	QString urlString = "http://47.94.145.6/map/orthogonal/1847168269595754497-健康谷正射";
	// QString urlString = "http://172.31.100.34:38083/map/orthogonal/1847168269595754497-健康谷正射";
	QString encodedOrthogonalXyzUrl = "type=xyz&url=";
	encodedOrthogonalXyzUrl.append(urlString);
	encodedOrthogonalXyzUrl.append("/{z}/{x}/{y}");
	encodedOrthogonalXyzUrl.append(".png");
	qDebug().noquote() << "encodedOrthogonalXyzUrl: " << encodedOrthogonalXyzUrl;
	QgsRasterLayer* orthogonalLayer = new QgsRasterLayer(encodedOrthogonalXyzUrl, MAIN_TILE_NAME, "wms");
	if (!orthogonalLayer->isValid()) {
		qWarning() << "XYZ orthogonalLayer layer invalid!" << orthogonalLayer->error().message();
		return -1;
	}
	project->addMapLayer(orthogonalLayer);
	qDebug() << "add orthogonal layer to project";

	// 3D Scene
	QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
	// QString realistic3d_tile_url = "url=http://172.31.100.34:38083/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
	QgsTiledSceneLayer* tiled_scene_layer = new QgsTiledSceneLayer(realistic3d_tile_url, REAL3D_TILE_NAME, "cesiumtiles");
	// tiled_scene_layer->setCrs(project->crs());
	QgsTiledSceneLayer3DRenderer* qgs_tiled_scene_layer_3d_renderer = new QgsTiledSceneLayer3DRenderer();
	qgs_tiled_scene_layer_3d_renderer->setLayer(tiled_scene_layer);
	tiled_scene_layer->setRenderer3D(qgs_tiled_scene_layer_3d_renderer);
	if (!tiled_scene_layer->isValid()) {
		qWarning() << "cesiumtiles 3D tiled_scene_layer layer invalid!" << tiled_scene_layer->error().message();
		return -1;
	}
	// add layer to project
	project->addMapLayer(tiled_scene_layer);
	qDebug() << "add 3d scene layer to project";

	QString layer_name = QString("民警");
	// QString icon_base64 =
	//         "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAolBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpPUAMub////z9f6mt/by9P1Ia+2MovP6+/72+P7AzPiUqfQrVOoFNuYgTOnp7fzi5/xOcO2dr/UuV+p6lPISQOisvPejtPZ/l/IcSOje5Ptaeu9EaOybrvV3kfFTdO7X3vvI0vm9yfi6x/iDm/Jvi/BphvBVdu5Nb+05X+uPpPSOpPN4UAcsAAAAC3RSTlMA0y0M/fny7sF6UvKXmSMAAAGJSURBVDjLhZNXYoMwDEAhkGnZmGFICZBA9l7t/a9WRx6h6Qfvx0g22nLeeGN32Ke0P3THnvOfnhtAkLAwZIn8cHsf194ojgqeESTjRRSPvD+/D4BNSYspg0HLyMSnM/LBjPoT+7+fp6icX9liwa5zFMrc1za8AcX7VFBYheEKqEC5pAMVxwjQ/gYkBec1SDboBUboIGbKfiQvdpwX8ohUxCx+OXG1REJ5E+y+X++EziVyZQTBlijm+ZfGX2pVEXjOGDjRnEFzNhoOY8cNMiOKphF1LapdaDRZ4DrDhBiOAP7hkAMcrSoZOn1mJQEaYVWs79DQSiVoSqsKKT4wLABZkNYD5cImCpJ8TlouMEjLxpbZBolpWu4gWWtBp/kuVNqcAH5kHKcmbRXKCwqVQqISWOORlKbUplmPGJD9XZ3xQzXLtHtJQfF8goIusd1mYNag2e9Bs8aBMSOXVStAqkqdqyZL1cjZoV3y20XUVVWLy41L+6ka2u6x714cu3rb9+pt7ep1L2/n+v8CysQxDA9OhyUAAAAASUVORK5CYII=";
	QString icon_base64 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAolBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpPUAMub////z9f6mt/by9P1Ia+2MovP6+/72+P7AzPiUqfQrVOoFNuYgTOnp7fzi5/xOcO2dr/UuV+p6lPISQOisvPejtPZ/l/IcSOje5Ptaeu9EaOybrvV3kfFTdO7X3vvI0vm9yfi6x/iDm/Jvi/BphvBVdu5Nb+05X+uPpPSOpPN4UAcsAAAAC3RSTlMA0y0M/fny7sF6UvKXmSMAAAGJSURBVDjLhZNXYoMwDEAhkGnZmGFICZBA9l7t/a9WRx6h6Qfvx0g22nLeeGN32Ke0P3THnvOfnhtAkLAwZIn8cHsf194ojgqeESTjRRSPvD+/D4BNSYspg0HLyMSnM/LBjPoT+7+fp6icX9liwa5zFMrc1za8AcX7VFBYheEKqEC5pAMVxwjQ/gYkBec1SDboBUboIGbKfiQvdpwX8ohUxCx+OXG1REJ5E+y+X++EziVyZQTBlijm+ZfGX2pVEXjOGDjRnEFzNhoOY8cNMiOKphF1LapdaDRZ4DrDhBiOAP7hkAMcrSoZOn1mJQEaYVWs79DQSiVoSqsKKT4wLABZkNYD5cImCpJ8TlouMEjLxpbZBolpWu4gWWtBp/kuVNqcAH5kHKcmbRXKCwqVQqISWOORlKbUplmPGJD9XZ3xQzXLtHtJQfF8goIusd1mYNag2e9Bs8aBMSOXVStAqkqdqyZL1cjZoV3y20XUVVWLy41L+6ka2u6x714cu3rb9+pt7ep1L2/n+v8CysQxDA9OhyUAAAAASUVORK5CYII=";
	//QString icon_base64 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAulBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpvaMovMAMub////y9P709v4GNub6+/5Oce2cr/WMovQsVerBzfmmt/ZIa+0hTOmltvaCmvLi5/zN1vqquvYSQOjr7/3d4/vT3PobSOgOPefu8f3Y3/vI0vmZrfV+l/J1kPFtifBnhfBVdu47Yus3XusKOef29/64xfivvvegsvWUqfSHnvNwjPBdfO9FaewzWuskT+mEnPJjge9knNjHAAAADHRSTlMA0y0M+/ny7sF6U1IqhgF4AAABmklEQVQ4y4VT55qjMAyEkHqnkSEQICFA+qZttt+2u/d/rTOsjLf8YPiDP1mekTRyLLyRO+gp1Ru4I8/5iY7L4DiJoiRmsNv5FvaGKghTn2r4aRioofclvY/ZlD5hOkP/0yO/ujymbxhz93eT351PqMb6vP9rrk7m3Y7w91niZ8YcuHqXG9z/0DGEJF0j4NNKYXkrLBjWBGom4hcIjrSOgT/CMlMViRuI/hsAq8sYmsTUErhaAYdy/Adgrj/gjgQhe84IqZzesuIx01hlBxKkGDku+2Twtiiyp/yeGvjsOoOYLApoXOrfD+J44PQSsrgAyKufchPuq+KTnqMiG98cATzol5+3u5eaOVL2gj/eM7A9+XS9xOJMNSJuKKY7AMFTSTdXAF4lS1MYkSsA9xN6L1AhX5OINGW+AruUyihA8QIw56ZM06jD4uT7xy2gJT0mObCRRplWT8pKG6BKXWOR5w+m1TIsmZUdpB2WjFvayKXE7LitYdZ3QNYErWGs5Q5YbpqotZw17e2zeM2ats327YvTvnrty9u6/v8BXLQ2Gr2F3XEAAAAASUVORK5CYII=";
	QgsVectorLayer* point_layer = new QgsVectorLayer(
		QStringLiteral("Point?crs=EPSG:3857"), layer_name, QStringLiteral("memory"));
	QgsVectorDataProvider* point_provider = point_layer->dataProvider();

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
	QgsCoordinateTransform* transformer = new QgsCoordinateTransform(crs_4326, crs_3857, project);

	point_layer->startEditing();
	//QgsPoint point(111.482116, 40.724241, 1019.501738);
	QList<InputPoint> points;
	points.append(InputPoint(QString("等等"), QgsPoint(111.482116, 40.724241, 1019.501738)));
	points.append(InputPoint(QString("张三"), QgsPoint(111.482624, 40.724823, 1020.262739)));
	points.append(InputPoint(QString("李四"), QgsPoint(111.483443, 40.724982, 1020.800378)));
	points.append(InputPoint(QString("王五"), QgsPoint(111.484411, 40.724352, 1022.498272)));
	points.append(InputPoint(QString("赵六"), QgsPoint(111.483948, 40.725397, 1021.921124)));
	points.append(InputPoint(QString("冯七"), QgsPoint(111.484871, 40.724699, 1024.298574)));
	points.append(InputPoint(QString("马八"), QgsPoint(111.484869, 40.726562, 1023.229612)));
	qDebug() << "points size:" << points.size();
	for (InputPoint point : points) {
		QgsPointXY* qgs_point_xy = new QgsPointXY(point.getPoint().x(), point.getPoint().y());
		QgsPointXY transformed_point = transformer->transform(*qgs_point_xy);

		QgsPoint* qgs_point = new QgsPoint(transformed_point.x(), transformed_point.y(), point.getPoint().z());

		qDebug() << "qgs_point x:" << qgs_point->x() << " y:" << qgs_point->y() << " z:" << qgs_point->z();

		QgsAttributes* attributes = new QgsAttributes{};
		attributes->append(point.getName());
		attributes->append("point");
		attributes->append(qgs_point->x());
		attributes->append(qgs_point->y());
		attributes->append(qgs_point->z());

		QgsFeature* feature = new QgsFeature(point_layer->fields());
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
	}
	else {
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
	QgsVectorLayer* p_point_layer = QgsUtil::write_persisted_layer(layer_name, point_layer, save_qgis_project_path, fields,
		Qgis::WkbType::PointZ,
		project->transformContext(),
		project->crs());

	// delete point_layer;

	p_point_layer->setCrs(project->crs());

	if (!p_point_layer->isValid()) {
		qCritical("Failed to load the layer!");
		exit(1);
	}
	// # icon_path = "D:/iProject/pypath/qgis-x/common/icon/民警.png"
	// QString icon_path = QString(layer_persist_prefix).append(".png");
	// qDebug() << "icon_base64:" << icon_base64;
	QString icon_path = QString().append(save_qgis_project_path).append("/").append(layer_name).append(".png");
	qDebug() << "icon_path:" << icon_path;
	if (!icon_base64.isEmpty())
	{
		std::pair<QString, QByteArray> base64_image = ImageUtil::parse_base64_image(icon_base64);
		QFile iconFile(icon_path);
		if (iconFile.open(QIODevice::WriteOnly)) {
			iconFile.write(base64_image.second);
			iconFile.flush();
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
	// QgsFeatureRenderer* feature_renderer = StylePoint::get2d_single_symbol_renderer();
	p_point_layer->setRenderer(feature_renderer);

	if (ENABLE_3D) {
		QgsAbstract3DRenderer* renderer3d = StylePoint::get3d_single_raster_symbol_renderer(*p_point_layer, fontStyleObj, layerStyleObj, icon_path, 20.0);
		p_point_layer->setRenderer3D(renderer3d);
	}

	p_point_layer->triggerRepaint();

	project->addMapLayer(p_point_layer);

	QgsMapCanvas* canvas = new QgsMapCanvas();
	canvas->setDestinationCrs(qgscrs);
	QgsMapSettings qgs_map_settings = canvas->mapSettings();
	qgs_map_settings.setDestinationCrs(qgscrs);
	qDebug() << "qgs_map_settings extent: " << qgs_map_settings.extent();
	QgsRectangle extent;
	QgsCoordinateReferenceSystem canvasCrs = qgs_map_settings.destinationCrs();
	QgsCoordinateTransformContext transformContext = project->transformContext();
	QList<QgsMapLayer*> b_layers{};
	auto qgs_map_layers = project->mapLayers().values();
	for (auto qgs_map_layer : qgs_map_layers) {
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
		}
		else if (layerName.startsWith(REAL3D_TILE_NAME)) {
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
	qDebug() << "extent: " << extent << " width: " << QString::number(extent.width(),'f',3) << " height: " << QString::number(extent.height(),'f',3)
		<< " xMinimum:" << QString::number(extent.xMinimum(),'f',3) << " yMinimum:" << QString::number(extent.yMinimum(),'f',3)
		<< " xMaximum:" << QString::number(extent.xMaximum(),'f',3) << " yMaximum:" << QString::number(extent.yMaximum(),'f',3)
		<< " area: " << QString::number(extent.area(),'f',3) << " perimeter: " << QString::number(extent.perimeter(),'f',3)
		<< " center -> x:" << QString::number(extent.center().x(), 'f', 3) << " y:" << QString::number(extent.center().y(), 'f', 3)
		<< " isEmpty: " << extent.isEmpty() << " isNull: " << extent.isNull() << " isFinite: " << extent.isFinite();
	qgs_map_settings.setLayers(b_layers);
	qgs_map_settings.setExtent(extent, false);
	canvas->setLayers(b_layers);
	canvas->setExtent(extent, false);
	canvas->refresh();
	QgsReferencedRectangle* referenced_rectangle = new QgsReferencedRectangle(extent, project->crs());
	project->viewSettings()->setDefaultViewExtent(*referenced_rectangle);

#if defined(_WIN32)
	YAML::Node config = YAML::LoadFile("D:/iProject/cpath/qgis_demo1/config/settings.yaml");
#elif defined(__linux__)
	YAML::Node config = YAML::LoadFile("/lyndon/iProject/cpath/qgis_demo1/config/settings.yaml");
#endif
	// QMap<QString, YAML::Node>* specification_map = new QMap<QString, YAML::Node>();
	// if (config["specification"]) {
	// 	for (YAML::const_iterator it = config["specification"].begin(); it != config["specification"].end(); ++it) {
	// 		QString name = QString((*it)["name"].as<std::string>().c_str());
	// 		specification_map->insert(name, *it);
	// 	}
	// }
	//
	// // 遍历 specification_map
	// for (auto it = specification_map->begin(); it != specification_map->end(); ++it) {
	// 	QString key = it.key();  // 获取当前的 key
	// 	YAML::Node value = it.value();  // 获取当前的 value
	//
	// 	// 检查 value 中是否存在 "local" 节点
	// 	if (value["local"]) {
	// 		// 获取 "local" 节点的值并打印
	// 		QString localValue = QString(value["local"].as<std::string>().c_str());
	// 		qDebug() << "Key:" << key << ", Local:" << localValue;
	// 	} else {
	// 		qDebug() << "Key:" << key << ", Local: (not found)";
	// 	}
	// }

	QVariantMap localConfig = NodeToMap::mapToVariantMap(config);
	qDebug() << "localConfig: " << localConfig;
	// YAML::Node localConfig = config["specification"]["现场位置图"];
	QList<QVariant> specVariants = localConfig["specification"].toList();
	// qDebug() << "specVariants: " << specVariants;
	QVariantMap imageSpec = specVariants[0].toMap();
	// qDebug() << "imageSpec:" << imageSpec;


	qDebug() << "add 2d layout";
	JwLayout* jwLayout = new JwLayout(project, canvas, "test", imageSpec, save_qgis_project_path);

	QString layout_type = "现场位置图";
	QString joined_layout_name = QString(layout_type).append("-").append("A3");

	QString plottingWeb = "{\"selectPath\":true,\"path\":\"健康谷正射\",\"tileIndex\":[[846789,407356],[856152,402089]],\"sceneId\":\"1847168269595754497\",\"sceneName\":\"test\",\"topicCategory\":\"\",\"geojson\":{\"type\":\"Feature\",\"geometry\":{\"type\":\"Polygon\",\"coordinates\":[[[111.45614558807182,40.718542891344214],[111.45614558807182,40.73911269545787],[111.51314153018527,40.73911269545787],[111.51314153018527,40.718542891344214],[111.45614558807182,40.718542891344214]]]},\"properties\":{}},\"savePath\":\"C:/Users/Administrator/Desktop\",\"sceneType\":\"01\",\"layinfo\":{\"title\":{\"text\":\"test郑州二期警务部署图\",\"position\":[],\"borderColor\":\"rgba(0,0,0,1)\",\"fillColor\":\"rgba(255,0,0,1)\",\"fontSize\":28,\"color\":\"rgba(0,0,0,1)\",\"fontBorderColor\":\"\"},\"subTitle\":{\"text\":\"右侧索引标题\",\"color\":\"rgba(0,0,0,1)\",\"fontSize\":16},\"remark\":[{\"text\":\"指挥: 这里填写指挥信息\",\"position\":[0,0,28,10],\"borderColor\":\"\",\"fillColor\":\"rgba(0,151,233,1)\",\"fontSize\":18,\"color\":\"rgba(0,0,0,1)\",\"url\":\"C:/security2.0/ToDWG/tmp/0.png\"},{\"text\":\"备注: 这里填写备注信息\",\"position\":[0,90,28,10],\"borderColor\":\"\",\"fillColor\":\"rgba(0,51,133,1)\",\"fontSize\":18,\"color\":\"rgba(0,0,0,1)\",\"url\":\"C:/security2.0/ToDWG/tmp/0.png\"}],\"north\":{\"position\":[97,0,2,8],\"rotate\":30},\"arrows\":[{\"position\":[80,80,10,2],\"rotate\":30}],\"scaleBar\":true},\"paper\":\"a3\",\"pictureUnit\":\"制图单位：xxx 制\",\"mapType\":{\"map\":true,\"electron\":false}}";
	QJsonObject plottingWebObj = QJsonDocument::fromJson(plottingWeb.toUtf8()).object();
	// qDebug() << "plottingWebObj: " << plottingWebObj;
	QVariantMap plottingWebVariants = JsonUtil::jsonObjectToVariantMap(plottingWebObj);
	// qDebug() << "plottingWebVariants: " << plottingWebVariants;
	PaperSpecification availablePaper("A3");

	QVector<QString> removeLayerNames = QVector<QString>();
	QVector<QString> removeLayerPrefixes = QVector<QString>();
	removeLayerPrefixes.append(REAL3D_TILE_NAME);
	jwLayout->addPrintLayout(QString("2d"), joined_layout_name, plottingWebVariants, availablePaper, false, removeLayerNames, removeLayerPrefixes);
	qDebug() << "add 2d layout done";


	qDebug() << "add 3d layout";
	QString joined_3d_layout_name = QString(layout_type).append("-3D").append("-").append("A3");
	QVector<QString> remove3DLayerNames = QVector<QString>();
	remove3DLayerNames.append(BASE_TILE_NAME);
	QVector<QString> remove3DLayerPrefixes = QVector<QString>();
	remove3DLayerPrefixes.append(MAIN_TILE_NAME);
	qDebug() << "constructor 3d canvas";
    // 初始化 Qgs3DMapCanvas
	Qgs3DMapCanvas* canvas3d = new Qgs3DMapCanvas();
    canvas3d->show(); // 即使是无头模式，也需要调用 show() 来初始化 OpenGL 上下文
	qDebug() << "constructor 3d JwLayout3D";
	JwLayout3D* jwLayout3d = new JwLayout3D(project, canvas, canvas3d, "test", imageSpec, save_qgis_project_path);

//	QgsOffscreen3DEngine engine;
//	engine.setSize(QSize( 4960, 3507 ));
//	engine->setFrustumCullingEnabled(true);
//	engine->setRenderCaptureEnabled(true);
//	engine->requestCaptureImage();
    qDebug() << "construct the QgsOffscreen3DEngine";
    jwLayout3d->get3DMapSettings(remove3DLayerNames, remove3DLayerPrefixes);
    jwLayout3d->set3DCanvas();
//    qDebug() << "construct the Qgs3DMapSettings";
//    Qgs3DMapScene* scene = new Qgs3DMapScene(*map_settings_3d, &engine);
//    engine.setRootEntity( scene );
//	qDebug() << "construct the Qgs3DMapScene";
//    QVector<QgsPointXY> verticesOfTrapezoid = scene->viewFrustum2DExtent();
//    CameraUtil::TrapezoidInfo(verticesOfTrapezoid);
//
//    scene->viewZoomFull();
//
//    QgsCameraController* canvas3dCameraController = canvas3d->cameraController();
//
//    QgsCameraController* cameraController = scene->cameraController();

//    qDebug() << "before modify camera controller scene state:" << scene->sceneState();
//    qDebug() << "before modify camera controller pending jobs:" << scene->totalPendingJobsCount();
//    qDebug() << "scene CameraController info";
//    CameraUtil::LookingAtInfo(cameraController);
//    CameraUtil::PoseInfo(cameraController);
//    qDebug() << "canvas3d CameraController info";
//    CameraUtil::LookingAtInfo(canvas3dCameraController);
//    CameraUtil::PoseInfo(canvas3dCameraController);

    // 设置保存路径
	QString capture_scene_image_path = QString().append(save_qgis_project_path).append("/").append("capture_scene_image.png");
	qDebug() << "save image path: " << capture_scene_image_path;

//    canvas3d->saveAsImage(capture_scene_image_path, "png");
//
//    // 监听渲染完成信号
//    QObject::connect(canvas3d, &QgsOffscreen3DEngine::imageCaptured, [=](const QImage& image) {
//        qDebug() << "Image captured successfully!";
//        // 可以在这里处理捕获的图像
//    });
//
//    // 使用定时器延迟捕获，确保场景加载完成
//    QTimer::singleShot(30000,  [=]() {
//        // 延迟 1 秒后调用 saveAsImage
//        canvas3d->saveAsImage(capture_scene_image_path, "png");
//    });

//    Qgs3DUtils::captureSceneImage( engine, scene ); // 保存截图 init
//	if (scene->sceneState() == Qgs3DMapScene::Ready && scene->totalPendingJobsCount() == 0) {
//		qDebug() << "Scene is ready, capturing image...";
//
//        QgsVector3D lookAtCenterPoint = QgsVector3D(100, 500, 220.0);
//
//        float distance = extent.width() / 1.2; // 根据场景范围调整相机距离
//        //float distance = 1857.9;
//        qDebug() << "distance: " << distance << " extent.width(): " << extent.width();
//        cameraController->setLookingAtPoint(lookAtCenterPoint, distance, 38.0, 20.0);
//
//        QgsCameraPose cameraPose;
//        QgsVector3D cameraPostPoint = QgsVector3D(100, 500, 220.0);
//        cameraPose.setCenterPoint(cameraPostPoint);
//        cameraPose.setDistanceFromCenterPoint(distance);
//        cameraPose.setPitchAngle(38.0);
//        cameraPose.setHeadingAngle(20.0);
//
//        cameraController->setCameraPose(cameraPose);
//        canvas3dCameraController->setLookingAtPoint(lookAtCenterPoint, distance, 38.0, 20.0);
//        canvas3dCameraController->setCameraPose(cameraPose);
//
//        CameraUtil::LookingAtInfo(cameraController);
//        CameraUtil::PoseInfo(cameraController);
//        CameraUtil::LookingAtInfo(canvas3dCameraController);
//        CameraUtil::PoseInfo(canvas3dCameraController);
//        qDebug() << "extent: ";
//        CameraUtil::ExtentInfo(extent);
//
//		QImage img = Qgs3DUtils::captureSceneImage(engine, scene);
//		bool capture_image_status = img.save(capture_scene_image_path);
//		qDebug() << "Save capture_scene_image: " << capture_image_status;
//	}

//	// 创建辅助对象并捕获图像
//	ImageCaptureHelper* helper = new ImageCaptureHelper(engine, qgs_3d_map_scene, capture_scene_image_path);

//	QObject::connect(qgs_3d_map_scene, &Qgs3DMapScene::sceneStateChanged, [helper, qgs_3d_map_scene]() {
//		if (qgs_3d_map_scene->sceneState() == Qgs3DMapScene::Ready) {
//			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " start capture image";
//			helper->captureImage();
//			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " capture image done";
//		}
//	});

	// qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " start capture image";
	// helper->captureImage();
	// qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " capture image done";

	// QImage capture_scene_image = Qgs3DUtils::captureSceneImage(*qgs_offscreen_3d_engine, qgs_3d_map_scene);
	// qDebug() << "capture_scene_image size:" << capture_scene_image.size();
	// QString capture_scene_image_path = QString().append(save_qgis_project_path).append("/").append("capture_scene_image.png");
	// qDebug() << "capture_scene_image_path: " << capture_scene_image_path;
	// bool capture_image_status = capture_scene_image.save(capture_scene_image_path);
	// qDebug() << "save capture_scene_image: " << capture_image_status;

	qDebug() << "JwLayout3D addPrintLayout";
	jwLayout3d->addPrintLayout(QString("3d"), joined_3d_layout_name, plottingWebVariants, availablePaper, false);
	qDebug() << "add 3d layout done";
//
    QString d3_scene_png = QString().append(save_qgis_project_path).append("/").append("d3_scene.png");
    qDebug() << "d3_scene_png: " << d3_scene_png;
    jwLayout3d->exportLayoutToImage(d3_scene_png);

    QString d3_scene_pdf = QString().append(save_qgis_project_path).append("/").append("d3_scene.pdf");
    qDebug() << "d3_scene_pdf: " << d3_scene_pdf;
    jwLayout3d->exportLayoutToPdf(d3_scene_pdf);

	qDebug() << "验证布局是否存在";
	QgsLayoutManager* layout_manager = project->layoutManager();
	QList<QgsMasterLayoutInterface*> layouts = layout_manager->layouts();
	qDebug() << " layouts count: " << layouts.count();
	for (int i = 0; i < layouts.size(); ++i) {
		QgsMasterLayoutInterface* layout = layouts.at(i);
		qDebug() << "layout name:" << layout->name();
	}


	// save to .qgz file
	QString projectPath = QString().append(save_qgis_project_path).append(QGIS_PROJECT_FILE_NAME);
	qDebug() << "projectPath:" << projectPath;
	if (!project->write(projectPath)) {
		qWarning() << "save projectPath to file " << projectPath << " failed!";
		return -1;
	}

	qDebug() << "save projectPath:" << projectPath << "successes";

	// 清理关系
	// qDebug() << "清理关系";
	// QgsRelationManager* relationManager = project->relationManager();
	// if (relationManager) {
	// 	QMap<QString, QgsRelation> qgs_relations = relationManager->relations();
	// 	qDebug() << " relation count: " << qgs_relations.count();
	// 	for (auto it = qgs_relations.constBegin(); it != qgs_relations.constEnd(); ++it) {
	// 		qDebug() << "Relation ID:" << it.key() << " Name:" << it.value().name();
	// 		relationManager->removeRelation(it.key());
	// 	}
	// }

	// 清理布局
	// qDebug() << "清理布局";
	// QgsLayoutManager* layout_manager = project->layoutManager();
	// QList<QgsMasterLayoutInterface*> layouts = layout_manager->layouts();
	// qDebug() << " layouts count: " << layouts.count();
	// for (int i = 0; i < layouts.size(); ++i) {
	// 	QgsMasterLayoutInterface* layout = layouts.at(i);
	// 	qDebug() << "remove layout name:" << layout->name();
	// 	layout_manager->removeLayout(layout);
	// }

	// 清理图层
	// qDebug() << "清理图层";
	// auto layers = project->mapLayers();
	// qDebug() << "layers count:" << layers.size();
	// for (auto it = layers.constBegin(); it != layers.constEnd(); ++it) {
	// 	qDebug() << "Layer ID:" << it.key() << " Name:" << it.value()->name();
	// 	project->removeMapLayer(it.key());
	// 	delete it.value();
	// }


	// auto layers = project->mapLayers();
	// qDebug() << "layers count:" << layers.size();
	// for (auto it = layers.constBegin(); it != layers.constEnd(); ++it) {
	// 	qDebug() << "Layer ID:" << it.key() << " Name:" << it.value()->name();
	// 	QgsVectorLayer* vectorLayer = dynamic_cast<QgsVectorLayer*>(it.value());
	// 	if (vectorLayer) {
	// 		qDebug() << "Checking relations for layer:" << vectorLayer->name();
	// 		//delete feature_renderer;
	// 		// for (int i = 0; i < vectorLayer->fields().count(); ++i) {
	// 		// 	qDebug() << "Field index:" << i << " Name:" << vectorLayer->fields().at(i).name();
	// 		// 	QList<QgsRelation> relations = vectorLayer->referencingRelations(i);
	// 		// 	qDebug() << "Field index:" << i << " has referencing relations count:" << relations.size();
	// 		// 	if (!relations.isEmpty()) {
	// 		// 		qDebug() << "Field index:" << i << " has referencing relations:";
	// 		// 		for (const QgsRelation& relation : relations) {
	// 		// 			qDebug() << "Relation name:" << relation.name();
	// 		// 			qDebug() << "Referenced layer ID:" << relation.referencedLayerId();
	// 		// 		}
	// 		// 	}
	// 		// }
	// 	}
	// 	qDebug() << "removeMapLayer" << it.key();
	// 	project->removeMapLayer(it.key());
	// }

//#if !defined(_WIN32)
//	layout_manager->clear();
//	qDebug() << "removeAllMapLayers";
//	project->removeAllMapLayers();
//	qDebug() << "project -> clear()";
//	/*project->clear();*/
//	qDebug() << "remove all layouts";
//	qDebug() << "delete project";
//	delete project;
//	qDebug() << "delete project done";
//
//	QString delete_file_test = "D:/iProject/cpath/qgis_demo1/common/project/民警.geojson";
//	//bool delete_file_status = FileUtil::delete_file_with_status(delete_file_test);
//	QFile file(delete_file_test);
//	qDebug() << "ready delete file: " << delete_file_test;
//	bool delete_file_status = file.remove();
//	qDebug() << "delete file: " << delete_file_test << " status:" << delete_file_status;
//#endif

    // 释放上下文
    context.doneCurrent();

	//return app.exec();
	return 0;
}
