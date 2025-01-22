//
// Created by Lyndon on 2025/1/9.
//

#ifndef JWLAYOUT3D_H
#define JWLAYOUT3D_H



#include <qgis.h>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QScreen>
#include <QSplashScreen>
#include <QStringLiteral>

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
#include <qgsmapviewsmanager.h>
#include <qgsconfig.h>
#include <qgs3dutils.h>
#include <qgssettings.h>
#include <qgsdirectionallightsettings.h>
#include <qgslayoutexporter.h>

//#include <qgis/app/3d/qgs3dmapcanvaswidget.h>
#include "JwLegend.h"
#include "../../enums/PaperSpecification.h"
#include "../../utils/QgsUtil.h"
#include "../../utils/ColorTransformUtil.h"
#include "../d3/CameraUtil.h"

class JwLayout3D {
public:
    JwLayout3D(QgsProject* project, QgsMapCanvas* canvas2d, Qgs3DMapCanvas* canvas, const QString& sceneName,
             const QVariantMap& imageSpec, const QString& projectDir);

    ~JwLayout3D();

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

    void addArrowToLayout(QgsLayout* layout, const QVector<QgsPointXY>& points, const QColor& color, double width);

    void addArrowBasedOnFrontendParams(QgsLayout* layout, const QList<QVariant>& position, double rotate);

    void init3DLayout(const QString& layoutName);

    Qgs3DMapSettings* get3DMapSettings(
        const QVector<QString> &removeLayerNames,
        const QVector<QString> &removeLayerPrefixes
    );

    void set3DCanvas();

    void set3DMap(
        const PaperSpecification& availablePaper,
        int mapFrameWidth = 1,
        const QString& mapFrameColor = "#000000",
        bool isDoubleFrame = false,
        double mapRotation = 0.0
    );


    void addNorthArrow(const QVariantMap& northArrowPath);

    void addPrintLayout(const QString& layoutType, const QString& layoutName,
                        const QVariantMap& plottingWeb, const PaperSpecification& availablePaper,
                        bool writeQpt = false);

    void loadQptTemplate(const QString& qptFilePath, const QString& layoutTemplateName);
    void updateLayoutExtent(const QString& layoutName);
    QPair<double, double> getLegendDimensions(const QString& layoutName);

    void exportLayoutToImage(const QString &outputFilePath);

#ifdef ENABLE_APP
    void create3DMapCanvasWidget(QString view3dName);
    void write3DMapViewSettings( Qgs3DMapCanvasWidget *widget, QDomDocument &doc, QDomElement &elem3DMap );
#endif

private:
    QgsProject* project;
    QString projectDir;
    QgsMapCanvas* canvas2d;
    Qgs3DMapCanvas* canvas3d;
    QString sceneName;
    JwLegend* jw_legend;
    // QgsLayout* layout;
    QgsPrintLayout* layout;
    QVariantMap imageSpec;
    QgsLayoutItem3DMap* mapItem3d;
    Qgs3DMapSettings* mapSettings3d;
    double mapWidth;
    double mapHeight;
};




#endif //JWLAYOUT3D_H
