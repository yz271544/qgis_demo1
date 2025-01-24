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
#include <QScreen>
#include <qgs3dmapcanvas.h>
#include <qgsproject.h>
#include <qgslayout.h>
#include <qgsprintlayout.h>
#include <qgslayoutitemmap.h>
#include <qgslayoutpagecollection.h>
#include <qgslayoutmanager.h>
#include <qgslayoutitemlegend.h>
#include <qgslayoutitemlabel.h>
#include <qgslayoutitempicture.h>
#include <qgslayoutitemscalebar.h>
#include <qgslayoutitempolyline.h>
#include <qgslayoutpoint.h>
#include <qgslayoutsize.h>
#include <qgslayoutmeasurement.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsfillsymbol.h>
#include <qgslinesymbol.h>
#include <qgsarrowsymbollayer.h>
#include <qgsreadwritecontext.h>
#include <qgsmapcanvas.h>
#include <qgs3dmapsettings.h>
#include <qgslayoutitem3dmap.h>
#include <qgscamerapose.h>
#include <qgsvector3d.h>
#include <qgslayoutitemshape.h>
#include <qgssymbollayer.h>
#include <qgssymbollayerwidget.h>
#include <qgsmarkersymbollayer.h>
#include <qgsprojectviewsettings.h>
#include <qgsflatterraingenerator.h>
#include <qgsprojectelevationproperties.h>
#include <qgsterrainprovider.h>
#include <qgspointlightsettings.h>
#include <qgs3dmapscene.h>

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

    // void init3DLayout(const QString& layoutName);
    // void init3DCanvas();
    // void set3DMap(const PaperSpecification& availablePaper, int mapFrameWidth = 1,
    //               const QString& mapFrameColor = "#000000", bool isDoubleFrame = false,
    //               const QVector<QString>& removeLayerNames = QVector<QString>(),
    //               const QVector<QString>& removeLayerPrefixes = QVector<QString>(),
    //               double mapRotation = 0.0);


    void addNorthArrow(const QVariantMap& northArrowPath);

    void addPrintLayout(const QString& layoutType, const QString& layoutName,
                        const QVariantMap& plottingWeb, const PaperSpecification& availablePaper,
                        bool writeQpt = false, const QVector<QString>& removeLayerNames = QVector<QString>(),
                        const QVector<QString>& removeLayerPrefixes = QVector<QString>());

    void loadQptTemplate(const QString& qptFilePath, const QString& layoutTemplateName);
    void updateLayoutExtent(const QString& layoutName);
    QPair<double, double> getLegendDimensions(const QString& layoutName);


private:
    QgsProject* project;
    QString projectDir;
    QgsMapCanvas* canvas;
    QString sceneName;
    JwLegend* jw_legend;
    QgsPrintLayout* layout;
    QVariantMap imageSpec;
    QgsLayoutItemMap* mapItem;
    double mapWidth;
    double mapHeight;
};



#endif //JWLAYOUT_H
