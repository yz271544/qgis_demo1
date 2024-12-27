//
// Created by etl on 12/26/24.
//

#ifndef STYLEPOINT_H
#define STYLEPOINT_H
#include <qgsrenderer.h>

class StylePoint {
public:
    static QgsFeatureRenderer* get2d_rule_based_renderer(QString font_style, QString icon_path, qreal point_size);
};



#endif //STYLEPOINT_H
