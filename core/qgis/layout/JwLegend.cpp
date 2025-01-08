//
// Created by Lyndon on 2025/1/7.
//

#include "JwLegend.h"


QPair<double, double> JwLegend::customize(
    QgsLayoutItemLegend* legend,
    QMap<QString, QVariant> imageSpec,
    double legendWidth,
    int32_t legendHeight,
    const QSet<QString>& filteredLegendItems)
{
    qDebug() << "customize legend, title:" << legendTitle;
    // Set the legend title
    legend->setTitle(legendTitle);
    // Control which layers are included in the legend
    legend->setAutoUpdateModel(false);
    // Remove all existing layers from the legend
    QgsLegendModel* legendModel = legend->model();
    while (legendModel->rowCount() > 0)
    {
        legendModel->removeRow(0);
    }

    QList<double> column_letter_width = {legendWidth};
    double legend_element_total_height = 0.0;
    QSet<QgsMapLayer*> filtered_legend_layer_items;
    QSet<QString> level_domain_text_set;

    // Add specific layers to the legend, excluding BaseTile and MainTile
    QMap<QString, QgsMapLayer*> layers = project->mapLayers();

    if (!filteredLegendItems.empty())
    {
        for (QgsMapLayer* layer : layers)
        {
            if (filteredLegendItems.contains(layer->name()))
            {
                filtered_legend_layer_items.insert(layer);
            }
        }
    }
    else
    {
        for (QgsMapLayer* layer : layers)
        {
            filtered_legend_layer_items.insert(layer);
        }
    }
    qDebug() << "filtered_legend_layer_items size: " << filtered_legend_layer_items.size();
    for (QgsMapLayer* layer : filtered_legend_layer_items)
    {
        if (layer->name() != "BASE_TILE_NAME" && layer->name() != MAIN_TILE_NAME && layer->name() != REAL3D_TILE_NAME &&
            !layer->name().startsWith(MAIN_TILE_NAME) && !layer->name().startsWith(REAL3D_TILE_NAME))
        {
            legend->model()->rootGroup()->addLayer(layer);
            if (auto* vectorLayer = dynamic_cast<QgsVectorLayer*>(layer))
            {
                QgsFeatureIterator layer_features_iterator = vectorLayer->getFeatures();
                if (layer->name().startsWith("等级域"))
                {
                    legend_element_total_height += (FontUtil::getSingleTextSize(12) +
                        FontUtil::getSingleTextSize(3.0));
                    QgsFeature feature;
                    while (layer_features_iterator.nextFeature(feature))
                    {
                        QgsAttributes feature_attrs = feature.attributes();
                        level_domain_text_set.insert(layer->name() + feature_attrs[0].toString());
                        column_letter_width.append(FontUtil::getTextFontWidth(feature_attrs[0].toString(), 12, 0.0));
                    }
                }
                else
                {
                    column_letter_width.append(FontUtil::getTextFontWidth(layer->name(), 12, 0.0));
                    legend_element_total_height += (FontUtil::getSingleTextSize(12) +
                        FontUtil::getSingleTextSize(2.5));
                }
            }
        }
    }


    // 遍历 level_domain_text_set ，将不同等级域的 item 高度进行累加
    for ([[maybe_unused]] const QString& text : level_domain_text_set)
    {
        legend_element_total_height += (FontUtil::getSingleTextSize(12) + FontUtil::getSingleTextSize(2.5));
    }


    // 核算图例的高度
    qDebug() << "核算图例的高度";
    int legend_height = (FontUtil::getSingleTextSize(16) + FontUtil::getSingleTextSize(3.5));


    // Customize the appearance of legend items
    if (ENABLE_POINT_CLUSTER)
    {
        for (QgsMapLayer* layer : filtered_legend_layer_items)
        {
            if (layer->name() != BASE_TILE_NAME && layer->name() != MAIN_TILE_NAME && layer->name() != REAL3D_TILE_NAME
                &&
                !layer->name().startsWith(MAIN_TILE_NAME) && !layer->name().startsWith(REAL3D_TILE_NAME))
            {
                legend_height += FontUtil::getSingleTextSize(2.0);

                if (auto* vectorLayer = dynamic_cast<QgsVectorLayer*>(layer))
                {
                    // 获取 layer: LayerVectorLayer 的自定义字段 type 的值
                    QString layer_custom_type = get_type(vectorLayer);

                    // 将点 point 图层，聚合点的 FontMarker 类型的 symbol 过滤剔除掉
                    /// 获取图层的渲染器
                    QgsFeatureRenderer* layer_renderer = vectorLayer->renderer();
                    /// 获取图例的符号项清单
                    QList<QgsLegendSymbolItem> legend_symbol_items = layer_renderer->legendSymbolItems();
                    if (legend_symbol_items.empty()) {
                        continue;
                    }


                    /// 遍历图例中获取所有的图例节点
                    for (QObject* obj : legend->model()->rootGroup()->children())
                    {
                        auto* tr = qobject_cast<QgsLayerTreeLayer*>(obj);
                        if (tr) {
                            /// 如果是点类型的图层，并且图例节点的图层 id 与当前图层 id 相同
                            /// tr: LayerTreeLayer
                            if (layer_custom_type == "point" && tr->layerId() == layer->id()) {
                                // custom symbol
                                /// 遍历图例的符号项
                                for (QgsLegendSymbolItem legend_symbol_item : legend_symbol_items) {
                                    /// 从符号项中获取符号：一般类型为 QgsFillSymbol, QgsLineSymbol, QgsMarkerSymbol，抽象类: QgsSymbol
                                    QgsSymbol* symbol = legend_symbol_item.symbol();
                                    /// 获取符号子集
                                    QgsSymbolLayerList symbol_layers = symbol->symbolLayers();
                                    QgsSymbolLayerList filtered_symbol_layers;
                                    /// 遍历符号子集
                                    for (QgsSymbolLayer* symbol_layer : symbol_layers) {
                                        /// 剔除掉 FontMarker 类型的 symbol
                                        if (symbol_layer->layerType()!= "FontMarker") {
                                            /// 这里避免崩溃，采用 clone() 方法
                                            std::cout << "这里避免崩溃，采用 clone() 方法" << std::endl;
                                            filtered_symbol_layers.append(symbol_layer->clone());
                                        }
                                    }
                                    /// 本项目中，点图例都采用可聚合+嵌入式规则渲染器，这里用 QgsMarkerSymbol 重新创建符号集
                                    std::cout << "重新创建符号集" << std::endl;
                                    auto* filtered_marker_symbol = new QgsMarkerSymbol(filtered_symbol_layers);
                                    /// 重新设置符号项的符号
                                    std::cout << "重新设置符号项的符号" << std::endl;
                                    legend_symbol_item.setSymbol(filtered_marker_symbol);
                                    /// 创建一个符号节点
                                    // symbol_legend_node = QgsSymbolLegendNode(tr, legend_symbol_item);
                                    // symbol_legend_node.setCustomSymbol(filtered_marker_symbol);
                                    std::cout << "设置图例节点的自定义符号" << std::endl;
                                    QgsMapLayerLegendUtils::setLegendNodeCustomSymbol(tr, 0, filtered_marker_symbol);
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    // 核算图例的高度
    legendHeight += (FontUtil::getSingleTextSize(12) + FontUtil::getSingleTextSize(2.5));
    for (double width : column_letter_width)
    {
        if (width > legendWidth)
        {
            legendWidth = width;
        }
    }
    legendHeight += legend_element_total_height;
    qDebug() << "calc legend_width:" << legendWidth << ",legend_height:" << legendHeight;

    return qMakePair(legendWidth, legendHeight);
}

/**
 * 获取 layer: LayerVectorLayer 的自定义字段 type 的值
 * @param layer
 * @return
 */
QString JwLegend::get_type(QgsVectorLayer* layer)
{
    QString layer_custom_type = "";
    // 获取字段索引 (name)
    QString field_name = "type";

    int field_index = layer->fields().indexOf(field_name);
    // 检查字段是否有效
    if (field_index == -1)
    {
        std::cerr << "Field " << field_name.toStdString() << " not found in layer " << layer->name().toStdString()
            << std::endl;
        return "unknown";
    }
    else
    {
        // 获取字段值
        QgsFeatureIterator it = layer->getFeatures();
        QgsFeature feature;
        if (it.nextFeature(feature))
        {
            layer_custom_type = feature.attribute(field_index).toString();
        }
    }
    return layer_custom_type;
}
