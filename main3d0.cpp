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

#include "config.h"
#include "qgspluginlayerregistry.h"
#if defined(_WIN32)
#include <windows.h>
#endif
int main(int argc, char* argv[]) {

#if defined(_WIN32)
	// 设置控制台输出为UTF - 8
	SetConsoleOutputCP(CP_UTF8);
	QLocale::setDefault(QLocale(QLocale::Chinese, QLocale::China));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

	// init QGIS app
	QgsApplication app(argc, argv, true);
	QgsApplication::setPrefixPath(QGIS_PREFIX_PATH, true);
	QgsApplication::init();
	QgsApplication::initQgis();

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
	QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json&http-header:referer=";
	//QString realistic3d_tile_url = "url=http://47.94.145.6/map/realistic3d/1847168269595754497-jkg/tileset.json";
	QgsTiledSceneLayer tiled_scene_layer(realistic3d_tile_url, REAL3D_TILE_NAME, "cesiumtiles");
	QgsTiledSceneLayer3DRenderer* qgs_tiled_scene_layer_3d_renderer = new QgsTiledSceneLayer3DRenderer();
	qgs_tiled_scene_layer_3d_renderer->setLayer(&tiled_scene_layer);
	tiled_scene_layer.setRenderer3D(qgs_tiled_scene_layer_3d_renderer);
	auto renderer3d_of_layer = tiled_scene_layer.renderer3D();
	if (renderer3d_of_layer == nullptr) {
		qDebug() << "renderer3d_of_layer is nullptr";
	}
	else {
		qDebug() << "renderer3d_of_layer is not nullptr";
	}
	if (!tiled_scene_layer.isValid()) {
		qWarning() << "cesiumtiles 3D tiled_scene_layer layer invalid!" << tiled_scene_layer.error().message();
		return -1;
	}
	// add layer to project
	project->addMapLayer(&tiled_scene_layer);
	qDebug() << "add 3d scene layer to project";

	QString projectPath = QString().append(save_qgis_project_path).append(QGIS_PROJECT_FILE_NAME);

	// save to .qgz file
	qDebug() << "projectPath:" << projectPath;
	if (!project->write(projectPath)) {
		qWarning() << "save projectPath to file " << projectPath << " failed!";
		return -1;
	}

	qDebug() << "save projectPath:" << projectPath << "successes";


	return 0;
}
