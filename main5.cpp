//
// Created by etl on 1/22/25.
//
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
#include <qgslayout.h>
#include <qgslayoutitemmap.h>
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
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <qgslayoutexporter.h>
#include <qgs3dmapscene.h>
#include <qgsprintlayout.h>
#include <qgslayoutitem3dmap.h>

#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>



int main(int argc, char *argv[])
{
    // 初始化QGIS应用（无头模式）
    QgsApplication app(argc, argv, true);
    QgsApplication::init();
    QgsApplication::initQgis();


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

    QString save_qgis_project_path = QString("/lyndon/iProject/cpath/qgis_demo1/common/project");
    // create QGIS project
    QgsProject* project = QgsProject::instance();
    project->removeAll();
    QString crs = "EPSG:3857";
    QgsCoordinateReferenceSystem qgscrs;
    qgscrs.createFromString(crs);
    project->setCrs(qgscrs);

    // 加载3D数据图层
    QgsVectorLayer* layer = new QgsVectorLayer("/lyndon/iProject/cpath/qgis_demo1/common/input/dem/太原/太原市DEM.tif", "3D Layer", "ogr");
    if (!layer->isValid()) {
        qWarning("Failed to load layer!");
        return 1;
    }
    project->addMapLayer(layer);

    // 创建3D地图设置
    Qgs3DMapSettings mapSettings;
    mapSettings.setLayers({layer});  // 添加图层到3D场景
    mapSettings.setCrs(layer->crs());  // 设置坐标系

    QgsOffscreen3DEngine engine;
    engine.setSize(QSize( 4960, 3507 ));
    // 创建3D场景
    Qgs3DMapScene scene(mapSettings, &engine);

    // 创建布局并添加3D视图
    QgsLayoutManager* layoutManager = project->layoutManager();

    QgsPrintLayout* layout = new QgsPrintLayout(project);
    layout->setName("3D Layout");
    layout->setUnits(Qgis::LayoutUnit::Millimeters);
    layout->initializeDefaults();

    layoutManager->addLayout(layout);
    QgsLayoutItem3DMap* mapItem3d = new QgsLayoutItem3DMap(layout);
    mapItem3d->attemptSetSceneRect(QRectF(0, 0,800, 600));
    layout->addLayoutItem(mapItem3d);

    // 保存工程文件
    // save to .qgz file
    QString projectPath = QString().append(save_qgis_project_path).append("/demo5.qgz");
    qDebug() << "projectPath:" << projectPath;
    if (!project->write(projectPath)) {
        qWarning() << "save projectPath to file " << projectPath << " failed!";
        return -1;
    }

    qDebug() << "save projectPath:" << projectPath << "successes";

    // 导出布局为图片
    QgsLayoutExporter exporter(layout);
    exporter.exportToImage("/lyndon/iProject/cpath/qgis_demo1/common/output/demo5.png", QgsLayoutExporter::ImageExportSettings());

    // 释放上下文
    context.doneCurrent();
    // 清理并退出
    QgsApplication::exitQgis();
    return 0;
}