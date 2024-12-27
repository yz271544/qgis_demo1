//
// Created by Lyndon on 2024/12/27.
//

#ifndef QGSUTIL_H
#define QGSUTIL_H

#include <QCoreApplication>
#include <QFont>
#include <iostream>
#include <memory>
#include <qgspallabeling.h>
#include <qgstextformat.h>
#include <qgsvectorfilewriter.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerlabeling.h>

class QtFontUtil {
public:
    static QgsTextFormat create_font(const std::string& font_family, double font_size, const std::string& font_color,
                                bool is_bold, bool is_italic, Qgis::TextOrientation orientation, double spacing);
};


class QgsUtil {
public:
    static void show_layer_label(QgsVectorLayer* layer, const std::string& style);


    static QgsVectorLayerSimpleLabeling* get_layer_label(const std::string& style, const std::string& label_of_field_name);


    static float d300_pixel_to_mm(float pixel_size);


    static QgsCoordinateTransform coordinate_transformer_4326_to_3857(QgsProject* project);


    static QgsVectorLayer* write_persisted_layer(const QString& layer_name, QgsVectorLayer* layer,
                                         const QString& project_dir, const QgsFields& fields,
                                         Qgis::WkbType qgs_wkb_type,
                                         const QgsCoordinateTransformContext& cts,
                                         const QgsCoordinateReferenceSystem& crs);
};


#endif //QGSUTIL_H
