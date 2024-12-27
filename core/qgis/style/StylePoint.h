//
// Created by etl on 12/26/24.
//

#ifndef STYLEPOINT_H
#define STYLEPOINT_H
#include <qgsrenderer.h>
#include <qgis.h>
#include "qgssymbol.h"
#include "qgsmarkersymbol.h"
#include "qgsmarkersymbollayer.h"
#include "qgspointdistancerenderer.h"
#include "qgsrulebasedrenderer.h"
#include "qgspointclusterrenderer.h"
#include "qgsproperty.h"
#include "qgsunittypes.h"
#include "qgswkbtypes.h"

#include "../../../config.h"
#include "../../../core/utils/QgsUtil.h"
#include "../../utils/ColorTransformUtil.h"


class StylePoint {
public:
	static QgsFeatureRenderer* get2d_rule_based_renderer(QString& font_style, QJsonDocument& layer_style, QString& icon_path, qreal point_size);
};



#endif //STYLEPOINT_H
