//
// Created by Lyndon on 2025/1/7.
//

#ifndef JWLAYOUT_H
#define JWLAYOUT_H

#include <qgis.h>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <qgsproject.h>
#include <QgsLayout.h>
#include <QgsPrintLayout.h>
#include <QgsLayoutItemMap.h>
#include <QgsLayoutPageCollection.h>
#include <qgslayoutmanager.h>
#include <QgsLayoutItemLegend.h>
#include <QgsLayoutItemLabel.h>
#include <QgsLayoutItemPicture.h>
#include <QgsLayoutItemScaleBar.h>
#include <QgsLayoutItemPolyline.h>
#include <QgsLayoutPoint.h>
#include <QgsLayoutSize.h>
#include <QgsLayoutMeasurement.h>
#include <QgsCoordinateReferenceSystem.h>
#include <QgsFillSymbol.h>
#include <QgsLineSymbol.h>
#include <QgsArrowSymbolLayer.h>
#include <QgsReadWriteContext.h>
#include <QgsMapCanvas.h>
#include <Qgs3DMapSettings.h>
#include <QgsLayoutItem3DMap.h>
#include <QgsCameraPose.h>
#include <QgsVector3D.h>
#include <QgsLayoutItemShape.h>
#include <qgssymbollayer.h>
#include <qgssymbollayerwidget.h>
#include <qgsmarkersymbollayer.h>

#include "JwLegend.h"
#include "../../enums/PaperSpecification.h"
#include "../../utils/QgsUtil.h"
#include "../../utils/ColorTransformUtil.h"

class JwLayout {
public:
    JwLayout(QgsProject* project, QgsMapCanvas* canvas, const QString& sceneName,
             const QVariantMap& imageSpec, const QString& projectDir);

    ~JwLayout();

    void filterMapLayers(const QVector<QString>& removeLayerNames = QVector<QString>(),
                         const QVector<QString>& removeLayerPrefixes = QVector<QString>(),
                         Qgs3DMapSettings* mapSettings3d = nullptr);

    void setPageOrientation(const PaperSpecification availablePaper, int pageNum,
                            QgsLayoutItemPage::Orientation orientation = QgsLayoutItemPage::Landscape);
    void setTitle(const QVariantMap& titleOfLayinfo);
    void setLegend(const QVariantMap& imageSpec, int legendWidth = 40, int legendHeight = 80,
                   const QString& borderColor = "#000000", const QSet<QString>& filteredLegendItems = QSet<QString>());

    void setRemarks(const QVariantMap& remarkOfLayinfo, const bool writeQpt);

    void addRightSideLabel(const QVariantMap& subTitle, int rightSideLabelWidth, int rightSideLabelHeight);

    void addSignatureLabel(const QString& signatureText);

    void addScaleBar(QgsLayout* layout, QgsLayoutItemMap* mapItem);

    void addArrowToLayout(QgsLayout* layout, const QVector<QgsPointXY>& points, const QColor& color, double width);

    void addArrowBasedOnFrontendParams(QgsLayout* layout, const QList<QVariant>& position, double rotate);

    void init2DLayout(const QString& layoutName);
    void setMap(const PaperSpecification& availablePaper, int mapFrameWidth = 1,
                const QString& mapFrameColor = "#000000", bool isDoubleFrame = false,
                const QVector<QString>& removeLayerNames = QVector<QString>(),
                const QVector<QString>& removeLayerPrefixes = QVector<QString>(),
                double mapRotation = 0.0);

    void init3DLayout(const QString& layoutName);
    void set3DMap(const PaperSpecification& availablePaper, int mapFrameWidth = 1,
                  const QString& mapFrameColor = "#000000", bool isDoubleFrame = false,
                  const QVector<QString>& removeLayerNames = QVector<QString>(),
                  const QVector<QString>& removeLayerPrefixes = QVector<QString>(),
                  double mapRotation = 0.0);


    void addNorthArrow(const QVariantMap& northArrowPath);

    void addPrintLayout(const QString& layoutType, const QString& layoutName,
                        const QVariantMap& plottingWeb, const PaperSpecification availablePaper,
                        bool writeQpt = false, const QVector<QString>& removeLayerNames = QVector<QString>(),
                        const QVector<QString>& removeLayerPrefixes = QVector<QString>());

    void loadQptTemplate(const QString& qptFilePath, const QString& layoutTemplateName);
    void updateLayoutExtent(const QString& layoutName);
    QPair<double, double> getLegendDimensions(const QString& layoutName);

    void addPrintLayout(const QString& layoutType, const QString& layoutName,
                              const QVariantMap& plottingWeb, const PaperSpecification& availablePaper,
                              bool writeQpt, const QVector<QString>& removeLayerNames,
                              const QVector<QString>& removeLayerPrefixes);
private:
    QgsProject* project;
    QString projectDir;
    QgsMapCanvas* canvas;
    QString sceneName;
    JwLegend* jw_legend;
    QgsPrintLayout* layout;
    QVariantMap imageSpec;
    QgsLayoutItemMap* mapItem;
    QgsLayoutItem3DMap* mapItem3d;
    Qgs3DMapSettings* mapSettings3d;
    double mapWidth;
    double mapHeight;
};



#endif //JWLAYOUT_H
