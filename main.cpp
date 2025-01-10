#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <cstdlib>
#include "qgsproject.h"
#include "qgsapplication.h"
#include "qgsrasterlayer.h"
#include "qgsdatasourceuri.h"
#include <QString>
#include <QFileInfo>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>  // For logging
#include <qgspluginlayerregistry.h>
#include <QProcess>
#include <QTextCodec>

#include "config.h"

#if defined(_WIN32)
#include <windows.h>
#endif

// #define HAVE_STATIC_PROVIDERS

int main(int argc, char* argv[])
{
#if defined(_WIN32)
	// 设置控制台输出为UTF - 8
	SetConsoleOutputCP(CP_UTF8);
	QLocale::setDefault(QLocale(QLocale::Chinese, QLocale::China));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

	// Initialize QGIS application without graphical interface
	QgsApplication app(argc, argv, true);
	QgsApplication::setPrefixPath(QGIS_PREFIX_PATH, true);
	//QgsApplication::setPrefixPath("/usr", true);
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
	// QString baseXyzUrl = "type=xyz&url=http://172.31.100.34:38083/map/lx/{z}/{x}/{y}.png&zmax=17&zmin=0";
	//QString baseXyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";

	QgsRasterLayer* baseXyzLayer = new QgsRasterLayer(baseXyzUrl, BASE_TILE_NAME, "wms");
	if (!baseXyzLayer->isValid()) {
		qWarning() << "XYZ xyzLayer layer invalid!" << baseXyzLayer->error().message();
		return -1;
	}
	// add base layer to project
	project->addMapLayer(baseXyzLayer);
	qDebug() << "add base layer to project";

	// 指定保存的QGS文件路径
	QString projectFilePath = "../common/project/project.qgz";
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

	// if (zipProcess.exitStatus() == QProcess::NormalExit && zipProcess.exitCode() == 0) {
	// 	qDebug() << "Successfully created QGZ file:" << projectFilePath;
	// 	QFile::remove(tempQgsFilePath);
	// }
	// else {
	// 	qCritical() << "Error creating QGZ file";
	// }

	QgsApplication::exitQgis();
	app.exit();

	return 0;

}
