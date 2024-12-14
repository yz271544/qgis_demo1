// #include <QApplication>
#include "qgsproject.h"
#include "qgsapplication.h"
#include <QMainWindow>
#include "qgisinterface.h"
#include "qgsproviderregistry.h"
#include "qgsrasterlayer.h"
#include "qgsmapcanvas.h"
#include <QMessageBox>
int main(int argc, char *argv[])
{
    // 初始化QGIS
    QgsApplication app(argc, argv, true);
    app.setPrefixPath("/lyndon/iProject/cpath/QGIS/output", true);
    app.setPluginPath("/lyndon/iProject/cpath/QGIS/output/lib/qgis/plugins");
    app.initQgis();
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
