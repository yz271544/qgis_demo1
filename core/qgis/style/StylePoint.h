//
// Created by etl on 12/26/24.
//

#ifndef STYLEPOINT_H
#define STYLEPOINT_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#if defined(_WIN32)
#include <windows.h>
#endif
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
#include <qgspoint3dsymbol.h>
#include <qgsphongmaterialsettings.h>
#include <qgsrulebased3drenderer.h>

#include "../../../config.h"
#include "../../../core/utils/QgsUtil.h"
#include "../../utils/ColorTransformUtil.h"


class StylePoint {
public:
	static QgsFeatureRenderer* get2d_rule_based_renderer(
		QJsonObject& font_style,
		QJsonObject& layer_style,
		QString& icon_path,
		qreal point_size);

	static QgsAbstract3DRenderer* get3d_single_symbol_renderer(
		QgsVectorLayer& point_layer,
		QJsonObject& font_style,
		QJsonObject& layer_style,
		QString& icon_path,
		qreal point_size);

	static QgsAbstract3DRenderer* get3d_single_raster_symbol_renderer(
		QgsVectorLayer& point_layer,
		QJsonObject& font_style,
		QJsonObject& layer_style,
		QString& icon_path,
		qreal point_size);

	static QgsRuleBased3DRenderer* get3d_rule_renderer(
		QgsVectorLayer& point_layer,
		QJsonObject& font_style,
		QJsonObject& layer_style,
		QString& icon_path,
		qreal point_size);
};



#endif //STYLEPOINT_H
