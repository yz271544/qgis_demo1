#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QCoreApplication>
#include <QFileInfo>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgslayertree.h>

#include "config.h"
#include "qgspluginlayerregistry.h"

#if defined(_WIN32)
#include <windows.h>
#endif

int main(int argc, char* argv[])
{
	// 初始化 QGIS 应用程序
	QgsApplication app(argc, argv, true);
	// QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
	QgsApplication::setPrefixPath(QGIS_PREFIX_PATH, true);
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
	QgsProject* project = QgsProject::instance();

	// 创建 XYZ 图层 (使用公开的 OpenStreetMap URL)
	QString xyzUrl = "type=xyz&url=http://47.94.145.6/map/lx/{z}/{x}/{y}.png&zmax=19&zmin=0";
	//QString xyzUrl = "type=xyz&http://tile.openstreetmap.org/{z}/{x}/{y}.png&zmax=19&zmin=0";
	QString layerName = "OpenStreetMap";
	QgsRasterLayer* xyzLayer = new QgsRasterLayer(xyzUrl, layerName, "wms");

	if (!xyzLayer->isValid()) {
		qWarning() << "XYZ 图层无效!" << xyzLayer->error().message();
		return -1;
	}

	// 将图层添加到项目中
	project->addMapLayer(xyzLayer);
	qDebug() << "qgisProjectPath:" << QGIS_PREFIX_PATH;
	// 保存项目为 .qgz 文件
	QString projectPath = QGIS_PREFIX_PATH;
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

