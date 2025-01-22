//
// Created by Lyndon on 2025/1/9.
//

#ifdef ENABLE_APP
#include <qgs3dmapcanvaswidget.h>
#endif
#include "JwLayout3D.h"


// 构造函数
JwLayout3D::JwLayout3D(QgsProject *project, QgsMapCanvas* canvas2d, Qgs3DMapCanvas *canvas3d, const QString &sceneName,
                       const QVariantMap &imageSpec, const QString &projectDir)
        : project(project), canvas2d(canvas2d), canvas3d(canvas3d), sceneName(sceneName), imageSpec(imageSpec), projectDir(projectDir),
          layout(nullptr), mapSettings3d(nullptr), mapWidth(0), mapHeight(0) {
    QString legendTitle = imageSpec["legend_title"].toString();
    this->jw_legend = new JwLegend(legendTitle, project);
}

JwLayout3D::~JwLayout3D() {
    // if (layout) {
    //     delete layout;
    // }
    // if (mapSettings3d) {
    //     delete mapSettings3d;
    // }
    // if (jw_legend) {
    //     delete jw_legend;
    // }
}

// 过滤地图图层
void JwLayout3D::filterMapLayers(const QVector<QString> &removeLayerNames,
                                 const QVector<QString> &removeLayerPrefixes,
                                 Qgs3DMapSettings *mapSettings3d) {
    qDebug() << "filterMapLayers -> removeLayerNames: " << removeLayerNames;
    qDebug() << "filterMapLayers -> removeLayerPrefixes: " << removeLayerPrefixes;

    QMap<QString, QgsMapLayer *> layers = project->mapLayers();
    QList<QgsMapLayer *> filteredLayers;
    for (QgsMapLayer *layer: layers) {
        bool shouldAdd = true;
        if (!removeLayerNames.isEmpty() && removeLayerNames.contains(layer->name())) {
            shouldAdd = false;
        }
        if (shouldAdd && !removeLayerPrefixes.isEmpty()) {
            for (const QString &prefix: removeLayerPrefixes) {
                if (layer->name().startsWith(prefix)) {
                    shouldAdd = false;
                    break;
                }
            }
        }
        if (shouldAdd) {
            filteredLayers.append(layer);
        }
    }
    if (mapSettings3d) {
        qDebug() << "set layers to 3d map settings";
        std::reverse(filteredLayers.begin(), filteredLayers.end());
        for (QgsMapLayer *filtered_layer: filteredLayers) {
            qDebug() << "add layer to layout map: " << filtered_layer->name();
        }
        mapSettings3d->setLayers(filteredLayers);
        qDebug() << "set layers to 3d map settings done";
    }
}

// 设置页面方向
void JwLayout3D::setPageOrientation(const PaperSpecification availablePaper, int pageNum,
                                    QgsLayoutItemPage::Orientation orientation) {
    QgsLayoutPageCollection *pageCollection = layout->pageCollection();
    QgsLayoutItemPage *page = pageCollection->page(pageNum);
    page->setPageSize(availablePaper.getPaperName(), orientation);
}

void JwLayout3D::setTitle(const QVariantMap &titleOfLayinfo) {

    // 添加标题
    QgsLayoutItemLabel *title = new QgsLayoutItemLabel(layout);
    title->setText(titleOfLayinfo["text"].toString());

    // 设置标题字号
    int8_t titleFontSize = imageSpec["title_font_size"].toInt();
    if (titleOfLayinfo.contains("fontSize")) {
        titleFontSize = titleOfLayinfo["fontSize"].toInt();
    }

    // 字体
    QString fontFamily = imageSpec["title_font_family"].toString();
    if (titleOfLayinfo.contains("fontFamily")) {
        fontFamily = titleOfLayinfo["fontFamily"].toString();
    }

    // 字体颜色
    QString fontColor = imageSpec["title_font_color"].toString();
    if (titleOfLayinfo.contains("color")) {
        fontColor = titleOfLayinfo["color"].toString();
        fontColor = ColorTransformUtil::strRgbaToHex(fontColor).first;
    }

    // set the font
    QgsTextFormat *text_format = QtFontUtil::create_font(
            fontFamily,
            titleFontSize,
            fontColor,
            imageSpec["title_is_bold"].toBool(),
            imageSpec["title_is_italic"].toBool(),
            Qgis::TextOrientation::Horizontal,
            imageSpec["title_letter_spacing"].toDouble());

    title->setVAlign(Qt::AlignBottom);
    title->setHAlign(Qt::AlignHCenter);
    title->adjustSizeToText();
    title->setTextFormat(*text_format);
    qDebug() << "title_font_size: " << titleFontSize << " title_font_family: " << fontFamily << " title_font_color: "
             << fontColor << " title_letter_spacing: " << imageSpec["title_letter_spacing"].toDouble();
    title->attemptSetSceneRect(
            QRectF(imageSpec["main_left_margin"].toDouble(), 0.0,
                   mapWidth,
                   imageSpec["main_top_margin"].toDouble() - 10));
    layout->addLayoutItem(title);
}

// 添加图例
void JwLayout3D::setLegend(const QVariantMap &imageSpec, int legendWidth, int legendHeight,
                           const QString &borderColor, const QSet<QString> &filteredLegendItems) {
    QgsLayoutItemLegend *legend = new QgsLayoutItemLegend(layout);
    qDebug() << "ready to custom legend";
    QPair<double, double> legendWidthHeight = jw_legend->customize(legend, imageSpec, legendWidth, legendHeight,
                                                                   filteredLegendItems);
    qDebug() << "custom legend done";
    legendWidth = legendWidthHeight.first;
    legendHeight = legendWidthHeight.second;
    double legendX = imageSpec["main_left_margin"].toDouble() + mapWidth - legendWidth - 0.5;
    qDebug() << "legend_x:" << legendX << ", main_left_margin: " << imageSpec["main_left_margin"].toDouble()
             << " map_width:" << mapWidth << " legend_width:" << legendWidth;
    double legendY = imageSpec["main_top_margin"].toDouble() + mapHeight - legendHeight - 0.5;
    qDebug() << "legend_y:" << legendY << ", main_top_margin:" << imageSpec["main_top_margin"].toDouble()
             << " map_height:" << mapHeight << " legend_height:" << legendHeight;
    legend->setResizeToContents(true);
    legend->setReferencePoint(QgsLayoutItem::ReferencePoint::LowerRight);
    qDebug() << "set_legend legend_x: " << legendX << ", legend_y: " << legendY << ", legend_width: " << legendWidth
             << ", legend_height: " << legendHeight;
    legend->attemptSetSceneRect(QRectF(legendX, legendY, legendWidth, legendHeight));
    // legend->setFixedSize(QgsLayoutSize());
    legend->setFrameEnabled(true);
    legend->setFrameStrokeWidth(QgsLayoutMeasurement(0.5, Qgis::LayoutUnit::Millimeters));
    legend->setFrameStrokeColor(QColor(borderColor));
    layout->addLayoutItem(legend);
}

void JwLayout3D::setRemarks(const QVariantMap &remarkOfLayinfo, const bool writeQpt) {
    // 获取备注文本
    QString remarkText = remarkOfLayinfo["text"].toString();

    // 获取备注位置信息 [左, 上, 宽, 高]
    QVariantList position = remarkOfLayinfo["position"].toList();
    double positionLeft = position[0].toDouble();
    double positionTop = position[1].toDouble();
    double positionWidth = position[2].toDouble();
    double positionHeight = position[3].toDouble();

    // 计算备注框的宽度和高度
    double remarksWidth = mapWidth * positionWidth / 100.0;
    double remarksHeight = mapHeight * positionHeight / 100.0;

    // 获取字体大小
    double remarkFontSize = remarkOfLayinfo.contains("fontSize") ? remarkOfLayinfo["fontSize"].toDouble()
                                                                 : imageSpec["remark_font_size"].toDouble();

    // 计算文本宽度和高度
    double remarksTextWidth = FontUtil::getTextFontWidth(remarkText, remarkFontSize,
                                                         imageSpec["remark_letter_spacing"].toDouble());
    double remarksTextHeight = FontUtil::getSingleTextSize(remarkFontSize);

    // 调整备注框的宽度和高度
    remarksWidth = qMax(remarksTextWidth, remarksWidth);
    remarksHeight = qMax(remarksTextHeight, remarksHeight);

    // 计算备注框的坐标
    double remarksX = imageSpec["main_left_margin"].toDouble() + 0.25;
    if (positionLeft > 0) {
        remarksX += mapWidth * positionLeft / 100.0;
    }
    double remarksY = imageSpec["main_top_margin"].toDouble() + mapHeight * positionTop / 100.0 - 0.25;

    // 获取背景颜色和边框颜色
    QString bgColor = remarkOfLayinfo.contains("fillColor") && !remarkOfLayinfo["fillColor"].toString().isEmpty()
                      ? ColorTransformUtil::strRgbaToHex(remarkOfLayinfo["fillColor"].toString()).first
                      : imageSpec["remark_bg_color"].toString();
    QString bgFrameColor =
            remarkOfLayinfo.contains("borderColor") && !remarkOfLayinfo["borderColor"].toString().isEmpty()
            ? remarkOfLayinfo["borderColor"].toString()
            : imageSpec["remark_bg_frame_color"].toString();

    // 创建备注框背景
    QgsLayoutItemShape *remarksBg = new QgsLayoutItemShape(layout);
    remarksBg->setShapeType(QgsLayoutItemShape::Rectangle);

    // 设置背景颜色和边框颜色
    QgsFillSymbol *symbol = new QgsFillSymbol();
    symbol->setColor(QColor(bgColor));
    if (auto *symbolLayer = dynamic_cast<QgsSimpleMarkerSymbolLayer *>(symbol->symbolLayer(0))) {
        symbolLayer->setStrokeColor(QColor(bgFrameColor));
    }
    remarksBg->setSymbol(symbol);

    // 设置备注框的位置和大小
    remarksBg->setReferencePoint(QgsLayoutItem::ReferencePoint::UpperLeft);
    remarksBg->attemptSetSceneRect(QRectF(remarksX, remarksY, remarksWidth, remarksHeight));
    layout->addLayoutItem(remarksBg);

    // 创建备注文本
    QgsLayoutItemLabel *remarks = new QgsLayoutItemLabel(layout);
    remarks->setText(remarkText);

    // 设置字体格式
    QgsTextFormat *remarksFont = QtFontUtil::create_font(
            imageSpec["remark_font_family"].toString(),
            remarkFontSize,
            ColorTransformUtil::strRgbaToHex(remarkOfLayinfo.contains("color") ? remarkOfLayinfo["color"].toString()
                                                                               : imageSpec["remark_font_color"].toString()).first,
            imageSpec["remark_is_bold"].toBool(),
            imageSpec["remark_is_italic"].toBool(),
            Qgis::TextOrientation::Horizontal,  // static_cast<Qt::Orientation>(imageSpec["remarkOrientation"].toInt()),
            imageSpec["remark_letter_spacing"].toDouble()
    );
    remarks->setTextFormat(*remarksFont);

    // 设置文本对齐方式
    remarks->setVAlign(Qt::AlignTop);
    remarks->setHAlign(Qt::AlignLeft);

    // 调整文本大小
    remarks->adjustSizeToText();

    // 设置文本位置
    remarks->setReferencePoint(QgsLayoutItem::ReferencePoint::UpperLeft);
    remarks->attemptSetSceneRect(
            QRectF(remarksX + 1, remarksY + 1, remarks->boundingRect().width(), remarks->boundingRect().height()));

    // 添加备注文本到布局
    layout->addLayoutItem(remarks);

    // 刷新布局
    layout->refresh();

    // 保存为 .qpt 文件
    if (writeQpt) {
        QString qptFilePath = projectDir + "/legend.qpt";
        QgsReadWriteContext context;
        layout->saveAsTemplate(qptFilePath, context);
    }
}

void JwLayout3D::addRightSideLabel(const QVariantMap &subTitle, int rightSideLabelWidth, int rightSideLabelHeight) {
    // 创建标签项
    QgsLayoutItemLabel *label = new QgsLayoutItemLabel(layout);

    // 设置标签文本
    QString labelText = subTitle["text"].toString();
    label->setText(labelText);

    // 获取字体大小和颜色
    double rightSeamSealFontSize = subTitle.contains("fontSize") ? subTitle["fontSize"].toDouble()
                                                                 : imageSpec["right_seam_seal_font_size"].toDouble();
    QString rightSeamSealFontColor = subTitle.contains("color") ? ColorTransformUtil::strRgbaToHex(
            subTitle["color"].toString()).first : imageSpec["right_seam_seal_font_color"].toString();

    // 计算文本宽度和高度
    double textFontHeight = FontUtil::getTextFontWidth(labelText, rightSeamSealFontSize,
                                                       imageSpec["signatureLetterSpacing"].toDouble());
    double textFontWidth = FontUtil::getSingleTextSize(rightSeamSealFontSize);

    // 打印调试信息
    qDebug() << "add_right_side_label font" << imageSpec["right_seam_seal_family"].toString()
             << rightSeamSealFontSize << rightSeamSealFontColor
             << imageSpec["right_seam_seal_letter_spacing"].toDouble()
             << "text_font_width:" << textFontWidth
             << "text_font_height:" << textFontHeight;

    // 创建字体格式
    QgsTextFormat textFormat;
    QFont font(imageSpec["right_seam_seal_family"].toString(), rightSeamSealFontSize);
    font.setLetterSpacing(QFont::AbsoluteSpacing, imageSpec["right_seam_seal_letter_spacing"].toDouble());
    textFormat.setFont(font);
    textFormat.setColor(QColor(rightSeamSealFontColor));
    textFormat.setOrientation(Qgis::TextOrientation::Vertical); // 设置文本方向为垂直

    // 设置标签的字体格式
    label->setTextFormat(textFormat);

    // 设置垂直居中对齐
    label->setVAlign(Qt::AlignVCenter);

    // 计算标签的位置
    double labelX = imageSpec["main_left_margin"].toDouble() + mapWidth +
                    imageSpec["right_seam_seal_from_right_border_left_margin"].toDouble();
    double labelY = imageSpec["main_top_margin"].toDouble() +
                    imageSpec["right_seam_seal_from_right_border_left_margin"].toDouble();

    // 设置标签的位置和大小
    label->attemptSetSceneRect(QRectF(labelX, labelY, textFontWidth, textFontHeight));

    // 将标签添加到布局
    layout->addLayoutItem(label);
}

void JwLayout3D::addSignatureLabel(const QString &signatureText) {
    // 创建标签项
    QgsLayoutItemLabel *label = new QgsLayoutItemLabel(layout);

    // 设置标签文本
    label->setText(signatureText);

    // 计算文本宽度和高度
    double textFontWidth = FontUtil::getTextFontWidth(signatureText, imageSpec["signature_font_size"].toDouble(),
                                                      imageSpec["signature_letter_spacing"].toDouble());
    double textFontHeight = FontUtil::getSingleTextSize(imageSpec["signature_font_size"].toDouble());

    // 打印调试信息
    qDebug() << "add_signature_label font" << imageSpec["right_seam_seal_family"].toString()
             << imageSpec["right_seam_seal_font_size"].toDouble() << imageSpec["right_seam_seal_font_color"].toString()
             << imageSpec["right_seam_seal_letter_spacing"].toDouble()
             << "text_font_width:" << textFontWidth
             << "text_font_height:" << textFontHeight;

    // 创建字体格式
    QgsTextFormat signatureTextFormat;
    QFont font(imageSpec["signature_family"].toString(), imageSpec["signature_font_size"].toDouble());
    font.setLetterSpacing(QFont::AbsoluteSpacing, 3.0); // 设置字间距
    signatureTextFormat.setFont(font);
    signatureTextFormat.setColor(QColor(imageSpec["signatureFontColor"].toString()));
    signatureTextFormat.setOrientation(Qgis::TextOrientation::Horizontal); // 设置文本方向为水平

    // 设置标签的字体格式
    label->setTextFormat(signatureTextFormat);

    // 设置垂直和水平居中对齐
    label->setVAlign(Qt::AlignVCenter);
    label->setHAlign(Qt::AlignHCenter);

    // 计算标签的位置
    double labelX = imageSpec["main_left_margin"].toDouble() + mapWidth - textFontWidth -
                    imageSpec["signature_from_right_border_right_margin"].toDouble();
    double labelY = imageSpec["main_top_margin"].toDouble() + mapHeight +
                    imageSpec["signature_from_bottom_border_top_margin"].toDouble();

    // 设置标签的位置和大小
    label->attemptSetSceneRect(QRectF(labelX, labelY, textFontWidth, textFontHeight));

    // 将标签添加到布局
    layout->addLayoutItem(label);
}

void
JwLayout3D::addArrowToLayout(QgsLayout *layout, const QVector<QgsPointXY> &points, const QColor &color, double width) {
    if (points.isEmpty()) {
        return;
    }
    // 将 QgsPointXY 转换为 QPolygonF
    QPolygonF polygon;
    for (const QgsPointXY &point: points) {
        polygon.append(QPointF(point.x(), point.y()));
    }

    // 创建多线段项
    QgsLayoutItemPolyline *polylineItem = new QgsLayoutItemPolyline(polygon, layout);

    // 创建线符号
    QgsLineSymbol *lineSymbol = QgsLineSymbol::createSimple(QVariantMap{
            {"color", color.name()},
            {"width", QString::number(width)}
    });

    // 创建箭头符号层
    QgsArrowSymbolLayer *arrowSymbolLayer = new QgsArrowSymbolLayer();
    arrowSymbolLayer->setIsCurved(false);
    arrowSymbolLayer->setArrowType(QgsArrowSymbolLayer::ArrowPlain);
    arrowSymbolLayer->setColor(color);
    arrowSymbolLayer->setWidth(width);
    arrowSymbolLayer->setWidthUnit(Qgis::RenderUnit::Points);
    arrowSymbolLayer->setStrokeColor(color);
    arrowSymbolLayer->setFillColor(color);

    // 将箭头符号层添加到线符号
    lineSymbol->changeSymbolLayer(0, arrowSymbolLayer);

    // 设置线符号到多线段项
    polylineItem->setSymbol(lineSymbol);

    // 将多线段项添加到布局
    layout->addLayoutItem(polylineItem);

    // 打印调试信息
    qDebug() << "Arrow added to layout with color:" << color.name() << "and width:" << width;
}

void JwLayout3D::addArrowBasedOnFrontendParams(QgsLayout *layout, const QList<QVariant> &position, double rotate) {
    // 检查位置参数是否有效
    if (position.size() < 4) {
        qWarning() << "Invalid position parameters. Expected 4 values: [left, top, width, height].";
        return;
    }

    // 将相对比例坐标转换为毫米坐标（左上角坐标）
    double left = imageSpec["main_left_margin"].toDouble() + mapWidth * (position[0].toDouble() / 100.0);
    double top = imageSpec["main_top_margin"].toDouble() + mapHeight * (position[1].toDouble() / 100.0);

    // 获取图片相对比例表示的宽和高转换为实际毫米尺寸
    double widthMm = mapWidth * (position[2].toDouble() / 100.0);
    double heightMm = mapHeight * (position[3].toDouble() / 100.0);

    // 计算箭头起始坐标（无旋转时起点坐标）
    double startX = left + widthMm;
    double startY = top + heightMm / 2.0;

    // 计算箭头结束坐标（无旋转时终点坐标）
    double endX = left;
    double endY = top + heightMm / 2.0;

    // 根据旋转角度计算旋转后的起止点坐标
    double angleRad = rotate * M_PI / 180.0; // 将角度转换为弧度
    double dxStart = (startX - left) * std::cos(angleRad) - (startY - top) * std::sin(angleRad);
    double dyStart = (startX - left) * std::sin(angleRad) + (startY - top) * std::cos(angleRad);
    double rotatedStartX = left + dxStart;
    double rotatedStartY = top + dyStart;

    double dxEnd = (endX - left) * std::cos(angleRad) - (endY - top) * std::sin(angleRad);
    double dyEnd = (endX - left) * std::sin(angleRad) + (endY - top) * std::cos(angleRad);
    double rotatedEndX = left + dxEnd;
    double rotatedEndY = top + dyEnd;

    // 构建用于绘制箭头的点坐标列表（毫米坐标形式）
    QVector<QgsPointXY> points = {
            QgsPointXY(rotatedStartX, rotatedStartY),
            QgsPointXY(rotatedEndX, rotatedEndY)
    };

    // 调用添加箭头到布局的函数
    addArrowToLayout(layout, points, Qt::red, 1.0);

    // 打印调试信息
    qDebug() << "Arrow added based on frontend params: position =" << position << ", rotate =" << rotate;
}

void JwLayout3D::init3DLayout(const QString &layoutName) {
    // layout = new QgsLayout(project);
    // // layout->setName(layoutName);
    // qDebug() << "layout setObjectName" << layoutName;
    // layout->setObjectName(layoutName);
    // layout->setUnits(Qgis::LayoutUnit::Millimeters);
    // qDebug() << "initializeDefaults";
    // layout->initializeDefaults();
    // qDebug() << "initializeDefaults done";
    // QgsLayoutManager* layout_manager = project->layoutManager();
    // layout_manager->addLayout(layout);

    layout = new QgsPrintLayout(project);
    layout->setName(layoutName);
    layout->setUnits(Qgis::LayoutUnit::Millimeters);
    layout->initializeDefaults();
    QgsLayoutManager* layout_manager = project->layoutManager();
    layout_manager->addLayout(layout);
}

Qgs3DMapSettings* JwLayout3D::get3DMapSettings(
const QVector<QString> &removeLayerNames,
const QVector<QString> &removeLayerPrefixes
) {
    QgsSettings settings;
    // 创建 3D 地图设置
    mapSettings3d = new Qgs3DMapSettings();
    mapSettings3d->setCrs(project->crs());
    // 过滤图层
    filterMapLayers(removeLayerNames, removeLayerPrefixes, mapSettings3d);

    QgsFlatTerrainGenerator *flatTerrain = new QgsFlatTerrainGenerator;
#if _QGIS_VERSION_INT >= 34100
    flatTerrain->setCrs( mapSettings3d->crs(), project->transformContext() );
#else
    flatTerrain->setCrs( mapSettings3d->crs() );
#endif
    mapSettings3d->setTerrainGenerator( flatTerrain );
    mapSettings3d->setTerrainElevationOffset( project->elevationProperties()->terrainProvider()->offset() );
    /*QgsAbstractTerrainSettings terrainSettings;
    terrainSettings.setElevationOffset(project->elevationProperties()->terrainProvider()->offset());
    mapSettings3d->setTerrainSettings(terrainSettings);*/
    // mapSettings3d->setBackgroundColor(QColor("#ffffff"));
    qDebug() << "filtered map layers";
    const QgsReferencedRectangle projectExtent = project->viewSettings()->fullExtent();
    const QgsRectangle fullExtent = Qgs3DUtils::tryReprojectExtent2D( projectExtent, projectExtent.crs(), mapSettings3d->crs(), project->transformContext() );
    //QgsReferencedRectangle fullExtent = project->viewSettings()->fullExtent();
    qDebug() << "get3DMapSettings fullExtent:";
    CameraUtil::ExtentInfo(fullExtent);
    mapSettings3d->setOrigin(QgsVector3D(fullExtent.center().x(), fullExtent.center().y(), 0));
    qDebug() << "set origin: " << mapSettings3d->origin().toString();
    mapSettings3d->setSelectionColor( canvas2d->selectionColor() );
    qDebug() << "set selection color: " << mapSettings3d->selectionColor();
    mapSettings3d->setBackgroundColor( canvas2d->canvasColor() );
    qDebug() << "set backgroupdColor: " << mapSettings3d->backgroundColor();
    //mapSettings3d->setLayers( canvas2d->layers( true ) );
    mapSettings3d->setTemporalRange( canvas2d->temporalRange() );
    qDebug() << "set temporal range:" << mapSettings3d->temporalRange().isEmpty();
    const Qgis::NavigationMode defaultNavMode = settings.enumValue( QStringLiteral( "map3d/defaultNavigation" ), Qgis::NavigationMode::TerrainBased, QgsSettings::App );
    mapSettings3d->setCameraNavigationMode( defaultNavMode );
    qDebug() << "set camera navigation mode: " << mapSettings3d->cameraNavigationMode();
    mapSettings3d->setCameraMovementSpeed( settings.value( QStringLiteral( "map3d/defaultMovementSpeed" ), 5, QgsSettings::App ).toDouble() );
    qDebug() << "set camera movement speed: " << mapSettings3d->cameraMovementSpeed();
    const Qt3DRender::QCameraLens::ProjectionType defaultProjection = settings.enumValue( QStringLiteral( "map3d/defaultProjection" ), Qt3DRender::QCameraLens::PerspectiveProjection, QgsSettings::App );
    mapSettings3d->setProjectionType( defaultProjection );
    qDebug() << "set project type: " << mapSettings3d->projectionType();
    mapSettings3d->setFieldOfView( settings.value( QStringLiteral( "map3d/defaultFieldOfView" ), 45, QgsSettings::App ).toInt() );
    qDebug() << "set field of view: " << mapSettings3d->fieldOfView();
    mapSettings3d->setTransformContext( QgsProject::instance()->transformContext() );
    qDebug() << "set transform context";
    mapSettings3d->setPathResolver( QgsProject::instance()->pathResolver() );
    qDebug() << "set path resolver";
    mapSettings3d->setMapThemeCollection( QgsProject::instance()->mapThemeCollection() );
    qDebug() << "set map theme collection:" << mapSettings3d->mapThemeCollection();
    mapSettings3d->configureTerrainFromProject( QgsProject::instance()->elevationProperties(), fullExtent );
    qDebug() << "configure terrain from project:" << mapSettings3d->terrainGenerator();
    // new scenes default to a single directional light
    QgsPointLightSettings defaultPointLight;
    QgsRectangle extent = fullExtent;
    QgsPointXY center = extent.center();
    defaultPointLight.setPosition( QgsVector3D( center.x(), center.y(), 1000 ) );
    defaultPointLight.setConstantAttenuation( 0 );
    mapSettings3d->setLightSources({defaultPointLight.clone()});
//    QgsDirectionalLightSettings* directionalLightSettings = new QgsDirectionalLightSettings();
//    mapSettings3d->setLightSources( QList<QgsLightSource *>() << directionalLightSettings );
    qDebug() << "set light sources:" << mapSettings3d->lightSources();
//    mapSettings3d->setOutputDpi( QGuiApplication::primaryScreen()->logicalDotsPerInch() );
    mapSettings3d->setOutputDpi(300);
    qDebug() << "set output dpi:" << mapSettings3d->outputDpi();
    mapSettings3d->setRendererUsage( Qgis::RendererUsage::View );
    qDebug() << "set renderer usage:" << mapSettings3d->rendererUsage();

    QObject::connect( project, &QgsProject::transformContextChanged, mapSettings3d, [this] {
        mapSettings3d->setTransformContext( project->transformContext() );
    } );
    qDebug() << "connect project transform context changed";
    mapSettings3d->setBackgroundColor(QColor("#ffffff"));

    mapSettings3d->setExtent(fullExtent);
    //set3DCanvas(fullExtent);

    return mapSettings3d;
}

void JwLayout3D::set3DCanvas() {
    const QgsReferencedRectangle projectExtent = project->viewSettings()->fullExtent();
    const QgsRectangle fullExtent = Qgs3DUtils::tryReprojectExtent2D( projectExtent, projectExtent.crs(), mapSettings3d->crs(), project->transformContext() );

    QgsRectangle extent = fullExtent;
    qDebug() << "set3DCanvas fullExtent:";
    CameraUtil::ExtentInfo(extent);
    QgsPointXY center = extent.center();
    extent.scale(1);
    qDebug() << "extent scale " << 1 << " center x: " << center.x() << " y:" << center.y();
    const float dist = static_cast< float >( std::max(extent.width(), extent.height()));
    qDebug() << "dist: " << dist;

    /*
     * QOpenGLFunctions created with non-current context
ASSERT: "QOpenGLFunctions::isInitialized(d_ptr)" in file /usr/include/x86_64-linux-gnu/qt5/QtGui/qopenglfunctions.h, line 858
     * */
    canvas3d->setMapSettings(mapSettings3d);
    //qDebug() << "canvas3d setViewFromTop";
    canvas3d->setViewFromTop(extent.center(), dist * 2, 0);
//    QObject::connect( canvas3d->scene(), &Qgs3DMapScene::totalPendingJobsCountChanged, canvas3d, [canvas3d]
//    {
//        qDebug() << "pending jobs:" << canvas3d->scene()->totalPendingJobsCount();
//    } );

    //qDebug() << "pending jobs:" << canvas3d->scene()->totalPendingJobsCount();
}

void JwLayout3D::set3DMap(
        const PaperSpecification &availablePaper,
        int mapFrameWidth,
        const QString &mapFrameColor,
        bool isDoubleFrame,
        double mapRotation) {
    QDomImplementation DomImplementation;
    QDomDocumentType documentType = DomImplementation.createDocumentType(
        QStringLiteral("qgis"), QStringLiteral("http://mrcc.com/qgis.dtd"), QStringLiteral("SYSTEM")
    );
    qDebug() << "create QDomDocument:" << documentType.name();

    QDomDocument doc(documentType);

    QDomElement elem3DMap = doc.createElement(QStringLiteral("view"));
    elem3DMap.setAttribute(QStringLiteral("isOpen"), 1);
    qDebug() << "create QDomElement:" << elem3DMap.tagName();

    //mapSettings3d->setExtent(fullExtent);

//    Qgs3DAxisSettings axis;
//    axis.setMode(Qgs3DAxisSettings::Mode::Off);
//    qDebug() << "mapSettings3d set3DAxisSettings";
//    mapSettings3d->set3DAxisSettings(axis);
//    qDebug() << "mapSettings3d setTransformContext";
//    mapSettings3d->setTransformContext(project->transformContext());
//    qDebug() << "mapSettings3d setPathResolver";
//    mapSettings3d->setPathResolver(project->pathResolver());
//    qDebug() << "mapSettings3d setMapThemeCollection";
//    mapSettings3d->setMapThemeCollection(project->mapThemeCollection());
//
//    qDebug() << "connect project mapSettings3d";
//    QObject::connect(project, &QgsProject::transformContextChanged, mapSettings3d, [this] {
//        mapSettings3d->setTransformContext(project->transformContext());
//    });
//
//    QgsFlatTerrainGenerator *flatTerrain = new QgsFlatTerrainGenerator;
//    QString crs = "EPSG:4979";
//    QgsCoordinateReferenceSystem scene3dcrs;
//    scene3dcrs.createFromString(crs);
//#if _QGIS_VERSION_INT >= 34100
//    flatTerrain->setCrs( scene3dcrs, project->transformContext() );
//#else
//    flatTerrain->setCrs(scene3dcrs);
//#endif
//    qDebug() << "mapSettings3d setTerrainGenerator";
//    mapSettings3d->setTerrainGenerator(flatTerrain);
//    qDebug() << "mapSettings3d setTerrainElevationOffset";
//    mapSettings3d->setTerrainElevationOffset(project->elevationProperties()->terrainProvider()->offset());

    // QgsPointLightSettings defaultPointLight;
    // qDebug() << "mapSettings3d setPosition";
    // defaultPointLight.setPosition(QgsVector3D( 0, 0, 1000 ) );
    // qDebug() << "mapSettings3d setConstantAttenuation";
    // defaultPointLight.setConstantAttenuation( 0 );
    // qDebug() << "mapSettings3d setLightSources";
    // mapSettings3d->setLightSources({defaultPointLight.clone() } );
    // if ( QScreen *screen = QGuiApplication::primaryScreen() )
    // {
    //     qDebug() << "mapSettings3d setOutputDpi:" << screen->physicalDotsPerInch();
    //     mapSettings3d->setOutputDpi( screen->physicalDotsPerInch() );
    // }
    // else
    // {
    //     qDebug() << "mapSettings3d setOutputDpi 300";
    //     mapSettings3d->setOutputDpi( 300 );
    // }


    // qDebug() << "mapSettings3d setTerrainRenderingEnabled true";
    // mapSettings3d->setTerrainRenderingEnabled(true);
    // qDebug() << "mapSettings3d setShowExtentIn2DView true";
    // mapSettings3d->setShowExtentIn2DView(true);
//    qDebug() << "canvas3d setMapSettings";
//    canvas3d->setMapSettings(mapSettings3d);
//    qDebug() << "canvas3d setMapSettings done";
//    QgsRectangle extent = fullExtent;
//    qDebug() << "extent scale";
//    extent.scale(1.3);
//    const float dist = static_cast< float >( std::max(extent.width(), extent.height()));
//    qDebug() << "canvas3d setViewFromTop";
//    canvas3d->setViewFromTop(extent.center(), dist * 2, 0);
//    qDebug() << "connect canvas3d totalPendingJobsCountChanged";
//    QObject::connect(canvas3d->scene(), &Qgs3DMapScene::totalPendingJobsCountChanged, canvas3d, [this] {
//        qDebug() << "pending jobs:" << canvas3d->scene()->totalPendingJobsCount();
//    });
//    qDebug() << "canvas3d show";
//    canvas3d->show();
//
    // 创建 3D 地图项
    //mapItem3d = QgsLayoutItem3DMap::create(layout);
    mapItem3d = new QgsLayoutItem3DMap(layout);
    // qDebug() << "mapItem3d setIsTemporal";
    // mapItem3d->setIsTemporal(true);
    qDebug() << "mapItem3d setMapSettings";
    mapItem3d->setMapSettings(mapSettings3d);
    // 设置地图项大小
    mapWidth = availablePaper.getPaperSize().second - imageSpec["main_left_margin"].toDouble() -
               imageSpec["main_right_margin"].toDouble();
    mapHeight = availablePaper.getPaperSize().first - imageSpec["main_top_margin"].toDouble() -
                imageSpec["main_bottom_margin"].toDouble();
    // 设置地图项位置
    qDebug() << "设置地图项位置:" << imageSpec["main_left_margin"].toDouble() << imageSpec["main_top_margin"].toDouble()
             << mapWidth << mapHeight;
    mapItem3d->attemptSetSceneRect(
            QRectF(imageSpec["main_left_margin"].toDouble(), imageSpec["main_top_margin"].toDouble(),
                   mapWidth, mapHeight));
    // QgsLayoutSize fixedSize(mapWidth, mapHeight, Qgis::LayoutUnit::Millimeters);
    // mapItem3d->attemptResize(fixedSize);
    // 设置相机视角
    qDebug() << "设置相机视角";
    // QgsCameraPose cameraPose;
    // cameraPose.setDistanceFromCenterPoint(1788.7f); // 设置相机距离
    // mapItem3d->setCameraPose(cameraPose);
    // 添加地图项到布局
    qDebug() << "add 3d map to layout";
    layout->addLayoutItem(mapItem3d);
    qDebug() << "add 3d map to layout done";
}


void JwLayout3D::addNorthArrow(const QVariantMap &north) {
    // 创建指北针图片项
    QgsLayoutItemPicture *northArrow = new QgsLayoutItemPicture(layout);

    // 设置指北针图片路径
    QString northArrowPath = "";
    if (imageSpec.contains("north_arrow_path")) {
        // write icon file to project directory from base64
        northArrowPath = projectDir + "/north_arrow.png";
        QFile file(northArrowPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QByteArray::fromBase64(imageSpec["north_arrow_path"].toByteArray()));
            file.close();
        }
    } else {
        northArrowPath = projectDir + imageSpec["system_north_arrow_path"].toString();
    }
    northArrow->setPicturePath(northArrowPath);

    // 设置指北针大小和位置
    double northWidth = imageSpec["north_arrow_width"].toDouble();
    double northHeight = imageSpec["north_arrow_height"].toDouble();
    double northX = (imageSpec["main_left_margin"].toDouble() + mapWidth - northWidth);
    double northY = imageSpec["main_top_margin"].toDouble() + 1.0;

    if (!north.isEmpty() and north.contains("position")) {
        if (north.value("position").toList().size() == 4) {
            northWidth = mapWidth * north.value("position").toList().at(2).toDouble() / 100;
            northHeight = mapHeight * north.value("position").toList().at(3).toDouble() / 100;
            northX = imageSpec["main_left_margin"].toDouble() +
                     mapWidth * north.value("position").toList().at(0).toDouble() / 100;
            northY = imageSpec["main_top_margin"].toDouble() +
                     mapHeight * north.value("position").toList().at(1).toDouble() / 100;
        }
    }

    northArrow->attemptSetSceneRect(QRectF(northX, northY, northWidth, northHeight));

    // 设置指北针旋转角度 （单位为度）
    //double northRotation = north.rotate if north and north.rotate else self.image_spec.north_rotate
    double northRotation = imageSpec["north_rotate"].toDouble();
    if (!north.isEmpty() and north.contains("rotate")) {
        northRotation = north.value("rotate").toDouble();
    }
    northArrow->setPictureRotation(northRotation);

    // 添加指北针到布局
    layout->addLayoutItem(northArrow);
}

void JwLayout3D::loadQptTemplate(const QString &qptFilePath, const QString &layoutTemplateName) {
    // 检查文件是否存在
    if (!QFile::exists(qptFilePath)) {
        qWarning() << "QPT file does not exist:" << qptFilePath;
        return;
    }

    // 读取 QPT 文件内容
    QFile file(qptFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open QPT file:" << qptFilePath;
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qWarning() << "Failed to parse QPT file:" << qptFilePath;
        file.close();
        return;
    }
    file.close();

    // 创建布局并加载模板
    QgsPrintLayout *layout = new QgsPrintLayout(project);
    QgsReadWriteContext context;
    QList<QgsLayoutItem *> qgs_layout_items = layout->loadFromTemplate(doc, context);
    if (qgs_layout_items.isEmpty()) {
        qWarning() << "Failed to load layout from QPT file:" << qptFilePath;
        delete layout;
        return;
    }

    // 设置布局名称
    layout->setName(layoutTemplateName);

    // 将布局添加到项目的布局管理器中
    project->layoutManager()->addLayout(layout);
    qDebug() << "Loaded layout from QPT file:" << qptFilePath;
}

void JwLayout3D::updateLayoutExtent(const QString &layoutName) {
    // 获取布局管理器
    QgsLayoutManager *layoutManager = project->layoutManager();

    // 查找指定名称的布局
    QgsPrintLayout *layout = dynamic_cast<QgsPrintLayout *>(layoutManager->layoutByName(layoutName));
    if (!layout) {
        qWarning() << "Layout not found:" << layoutName;
        return;
    }

    // 获取主窗口的地图画布, 更新布局中所有地图项的范围
    QList<QGraphicsItem *> graphics_items = layout->items();
    for (QGraphicsItem *graphics_item: graphics_items) {
        QgsLayoutItem *layout_item = dynamic_cast<QgsLayoutItem *>(graphics_item);
        if (layout_item) {
            if (QgsLayoutItemMap *map_item = dynamic_cast<QgsLayoutItemMap *>(layout_item)) {
                // map_item->setExtent(canvas3d->extent());
            }
        }
    }

    // 刷新布局
    layout->refresh();
    qDebug() << "Updated extent for layout:" << layoutName;
}

// 获取图例尺寸
QPair<double, double> JwLayout3D::getLegendDimensions(const QString &layoutName) {
    QgsMasterLayoutInterface *layoutInterface = project->layoutManager()->layoutByName(layoutName);

    if (layoutInterface) {
        if (layoutInterface->layoutType() == QgsMasterLayoutInterface::PrintLayout) {
            QgsPrintLayout *layout = static_cast<QgsPrintLayout *>(layoutInterface);
            for (QGraphicsItem *graphicsItem: layout->items()) {
                QgsLayoutItem *item = dynamic_cast<QgsLayoutItem *>(graphicsItem);
                if (item) {
                    if (QgsLayoutItemLegend *legend = dynamic_cast<QgsLayoutItemLegend *>(item)) {
                        QRectF boundingRect = legend->boundingRect();
                        return qMakePair(boundingRect.width(), boundingRect.height());
                    }
                }
            }
        }
    }
    return qMakePair(0.0, 0.0);
};


void JwLayout3D::addPrintLayout(const QString &layoutType, const QString &layoutName,
                                const QVariantMap &plottingWeb, const PaperSpecification &availablePaper,
                                bool writeQpt) {
    // 初始化布局
    qDebug() << "初始化3d布局";
    init3DLayout(layoutName);

    // 设置纸张类型和大小
    qDebug() << "Setting page orientation and size";
    setPageOrientation(availablePaper, 0);

    // 获取地图边框颜色、宽度和双边框设置
    QVariantMap layInfo = plottingWeb["layinfo"].toMap();
    QVariantMap bgPic = layInfo["bgPic"].toMap();
    QString mapFrameColor = bgPic.contains("borderColor") ? bgPic["borderColor"].toString()
                                                          : imageSpec["main_border_color"].toString();
    int mapFrameWidth = bgPic.contains("borderWidth") ? bgPic["borderWidth"].toInt()
                                                      : imageSpec["main_border_size"].toInt();
    bool mapDoubleFrame = bgPic.contains("doubleFrame") ? bgPic["doubleFrame"].toBool()
                                                        : imageSpec["main_double_frame"].toBool();
    double mapRotation = layInfo.contains("north") ? layInfo["north"].toMap()["rotate"].toDouble()
                                                   : imageSpec["north_rotate"].toDouble();

    // 设置地图
    qInfo() << "Added 3D map to layout";
    set3DMap(availablePaper, mapFrameWidth, mapFrameColor, mapDoubleFrame, mapRotation);

    // 设置标题
    if (layInfo.contains("title") && !layInfo["title"].toMap().isEmpty()) {
        QMap<QString, QVariant> titleVariants = layInfo["title"].toMap();
        qInfo() << "设置标题:" << titleVariants["text"].toString();
        setTitle(titleVariants);
    }

    // 添加图例
    qDebug() << "添加图例";
    QVariantMap legendInfo = layInfo["legend"].toMap();
    QString legendFrameColor = legendInfo.contains("borderColor") ? legendInfo["borderColor"].toString()
                                                                  : imageSpec["legend_frame_color"].toString();
    QSet<QString> filteredLegendItems;
    if (plottingWeb.contains("legend") && plottingWeb["legend"].toMap().contains("items")) {
        for (const QVariant &item: plottingWeb["legend"].toMap()["items"].toList()) {
            filteredLegendItems.insert(item.toString());
        }
    }

    qDebug() << "filteredLegendItems Length:" << filteredLegendItems.size();

    int legend_width = 40;
    if (layInfo.contains("legendWidth")) {
        legend_width = layInfo["legendWidth"].toInt();
    } else if (imageSpec.contains("legend_width")) {
        legend_width = imageSpec["legend_width"].toInt();
    }
    int legend_height = 80;
    if (layInfo.contains("legendHeight")) {
        legend_width = layInfo["legendHeight"].toInt();
    } else if (imageSpec.contains("legend_height")) {
        legend_width = imageSpec["legend_height"].toInt();
    }
    qDebug() << "legend_width:" << legend_width << ", legend_height:" << legend_height << ", legendFrameColor:"
             << legendFrameColor;
    setLegend(imageSpec, legend_width, legend_height, legendFrameColor, filteredLegendItems);

    // 添加备注
    if (layInfo.contains("remark")) {
        QList<QVariant> remark_v_list = layInfo["remark"].toList();
        qDebug() << "添加备注: " << remark_v_list.length() << "个";
        for (const QVariant &remark: remark_v_list) {
            setRemarks(remark.toMap(), false);
        }
    }

    // 添加指北针
    if (layInfo.contains("north")) {
        qDebug() << "添加指北针";
        addNorthArrow(layInfo["north"].toMap());
    }

    // 添加右侧索引标题
    if (layInfo.contains("subTitle")) {
        QMap<QString, QVariant> subTitleVariants = layInfo["subTitle"].toMap();
        qDebug() << "添加右侧索引标题:" << subTitleVariants["text"].toString();;
        addRightSideLabel(subTitleVariants, 7, 100);
    }

    // 添加签名
    if (plottingWeb.contains("pictureUnit") && !plottingWeb["pictureUnit"].toString().isEmpty()) {
        qDebug() << "添加签名: " << plottingWeb["pictureUnit"].toString();
        addSignatureLabel(plottingWeb["pictureUnit"].toString());
    }

    // 添加箭头
    if (layInfo.contains("arrows")) {
        QList<QVariant> arrows = layInfo["arrows"].toList();
        qDebug() << "添加箭头: " << arrows.length() << "个";
        for (const QVariant &arrow: arrows) {
            QVariantMap arrowInfo = arrow.toMap();
            addArrowBasedOnFrontendParams(layout, arrowInfo["position"].toList(), arrowInfo["rotate"].toDouble());
        }
    }

    // 保存为 .qpt 文件
    if (writeQpt) {
        qDebug() << "保存为 .qpt 文件";
        QString qptFilePath = projectDir + "/" + layoutName + ".qpt";
        QgsReadWriteContext context;
        layout->saveAsTemplate(qptFilePath, context);
        qDebug() << "Saved layout as QPT template:" << qptFilePath;
    }
}

void JwLayout3D::exportLayoutToImage(const QString &outputFilePath) {
    // 创建布局导出器
    QgsLayoutExporter exporter(layout);

    // 设置导出选项
    QgsLayoutExporter::ImageExportSettings settings;
    settings.dpi = 300; // 设置DPI

    qDebug() << "export to image: " << outputFilePath;
    // 导出为PNG图片
    QgsLayoutExporter::ExportResult result = exporter.exportToImage(outputFilePath, settings);

    if (result == QgsLayoutExporter::Success) {
        qDebug() << "Layout exported successfully to:" << outputFilePath;
    } else {
        qWarning() << "Failed to export layout to:" << outputFilePath;
    }
}