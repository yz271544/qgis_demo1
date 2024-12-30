// #include <QApplication>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#if defined(_WIN32)
#include <windows.h>
#endif
#include <qgspluginlayerregistry.h>

#include "qgsproject.h"
#include "qgsapplication.h"
#include <QMainWindow>
#include "qgisinterface.h"
#include "qgsproviderregistry.h"
#include "qgsrasterlayer.h"
#include "qgsmapcanvas.h"
#include <QMessageBox>
int main(int argc, char* argv[])
{
	// 初始化QGIS
	QgsApplication app(argc, argv, true);
	QgsApplication::setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
	QgsApplication::setPluginPath("/lyndon/iProject/cpath/QGIS/output/lib/qgis/plugins");
	QgsApplication::init();
	QgsApplication::initQgis();
	QgsPluginLayerRegistry* plugin_layer_registry = QgsApplication::pluginLayerRegistry();
	QStringList plugin_layer_types = plugin_layer_registry->pluginLayerTypes();
	qDebug() << "plugin type:" << plugin_layer_types;
	QgsPluginLayerType* wms_plugin_layer = plugin_layer_registry->pluginLayerType("wms");
	qDebug() << "wms plugin type:" << wms_plugin_layer;

	// 创建主窗口
	QMainWindow mainWindow;
	mainWindow.setWindowTitle("QGIS-with-Amap");
	// 创建地图Canvas
	QgsMapCanvas* mapCanvas = new QgsMapCanvas(&mainWindow);
	mapCanvas->setCanvasColor(Qt::white);
	mapCanvas->setVisible(true);
	mapCanvas->setFrameStyle(0);
	mainWindow.setCentralWidget(mapCanvas);
	// 创建栅格图层
	QgsRasterLayer* amapLayer = new QgsRasterLayer("http://47.94.145.6/map/lx/{z}/{x}/{y}.png", "Amap", "wms");
	if (amapLayer->isValid())
	{
		QgsProject::instance()->addMapLayer(amapLayer);
		QgsRectangle extent = amapLayer->extent();
		mapCanvas->setExtent(extent);
	}
	else
	{
		QMessageBox::critical(&mainWindow, "Error", "Could not load Amap layer.");
		return -1;
	}
	// 设置地图范围和投影
	QgsCoordinateReferenceSystem crs("EPSG:3857");
	mapCanvas->setDestinationCrs(crs);
	mapCanvas->enableAntiAliasing(true);
	// 显示主窗口
	mainWindow.show();
	// 执行QGIS事件循环
	return app.exec();
}
