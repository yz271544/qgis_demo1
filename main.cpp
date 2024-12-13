#include "QgsProject.h"
#include "QgsApplication.h"
#include <QString.h>
#include <QFileInfo.h>

int main(int argc, char *argv[])
{
    // 初始化QGIS应用，这里设置为不使用图形界面（第二个参数为false）
    QgsApplication app(argc, argv, false);

    // 设置QGIS的相关路径（根据你实际的安装情况调整），比如插件路径等，示例中简单设置为空
    app.setPrefixPath("D:/iProject/cpath/OSGeo4W/apps/qgis", true);

    // 初始化相关的设置等
    app.init();

    // 创建一个QGIS项目实例
    QgsProject *project = QgsProject::instance();

    // 设置项目标题等基础信息
    project->setTitle("My QGIS Project");
    project->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(3857));  // 这里设置坐标参考系为EPSG:3857，可按需改

    // 定义保存的工程文件路径和文件名
    QString projectFilePath = "D:/iProject/cpath/qgis_demo1/common/project/project.qgs";  // 替换为实际要保存的路径

    // 保存项目为工程文件
    bool savedSuccessfully = project->write(projectFilePath);
    if (savedSuccessfully) {
        qDebug() << "Project saved successfully.";
    } else {
        qDebug() << "Failed to save the project.";
    }

    // 退出QGIS应用
    app.exit();

    return 0;
}