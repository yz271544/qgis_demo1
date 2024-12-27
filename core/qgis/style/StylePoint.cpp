
#include "StylePoint.h"


QgsFeatureRenderer* StylePoint::get2d_rule_based_renderer(QString& font_style, QJsonDocument& layer_style, QString& icon_path, qreal point_size) {
	std::string label_font_color = "#000000";
	float label_font_opacity = 1.0;
	std::pair<std::string, float> colorOpacity = ColorTransformUtil::str_rgba_to_hex(font_style.toStdString());
	label_font_color = colorOpacity.first;
	label_font_opacity = colorOpacity.second;
	std::string label_style_font_family = "SimSun";
	double label_style_font_size = 12;
	QgsSymbol* rule_symbol = QgsSymbol::defaultSymbol(Qgis::GeometryType::Point);
	QgsFontMarkerSymbolLayer* rule_font_marker = new QgsFontMarkerSymbolLayer(label_style_font_family.c_str());
	rule_font_marker->setSizeUnit(Qgis::RenderUnit::Millimeters);
	rule_font_marker->setSize(QgsUtil::d300_pixel_to_mm(label_style_font_size));
	rule_font_marker->setColor(QColor(label_font_color.c_str()));
	rule_font_marker->setDataDefinedProperty(QgsSymbolLayer::Property::Character, QgsProperty::fromExpression("name"));
	rule_font_marker->setOffset(QPointF(0, -5));

	QgsRasterMarkerSymbolLayer* rule_raster_marker = new QgsRasterMarkerSymbolLayer(icon_path);
	rule_raster_marker->setSizeUnit(Qgis::RenderUnit::Millimeters);
	rule_raster_marker->setSize(QgsUtil::d300_pixel_to_mm(point_size));
	rule_symbol->changeSymbolLayer(0, rule_raster_marker);
	rule_symbol->appendSymbolLayer(rule_font_marker);

	QgsRuleBasedRenderer::Rule* root_rule = new QgsRuleBasedRenderer::Rule(nullptr);
	QgsRuleBasedRenderer::Rule* rule = new QgsRuleBasedRenderer::Rule(rule_symbol);
	rule->setFilterExpression("ELSE");
	root_rule->appendChild(rule);
	QgsRuleBasedRenderer* rule_renderer = new QgsRuleBasedRenderer(root_rule);

	if (!ENABLE_POINT_CLUSTER) {
		return rule_renderer;
	}

	rule_symbol->appendSymbolLayer(rule_font_marker);
	//QgsSymbol* cluster_symbol = QgsSymbol::defaultSymbol(Qgis::GeometryType::Point);
	QgsMarkerSymbol* cluster_symbol = new QgsMarkerSymbol();
	QgsFontMarkerSymbolLayer* font_marker = new QgsFontMarkerSymbolLayer(label_style_font_family.c_str());
	font_marker->setSize(QgsUtil::d300_pixel_to_mm(label_style_font_size));
	font_marker->setColor(QColor(label_font_color.c_str()));
	font_marker->setDataDefinedProperty(QgsSymbolLayer::Property::Character, QgsProperty::fromExpression("concat('(', @cluster_size, ')')"));
	font_marker->setOffset(QPointF(0, -5));

	QgsRasterMarkerSymbolLayer* raster_marker = new QgsRasterMarkerSymbolLayer(icon_path);
	raster_marker->setSize(QgsUtil::d300_pixel_to_mm(point_size));
	cluster_symbol->changeSymbolLayer(0, raster_marker);
	cluster_symbol->appendSymbolLayer(font_marker);

	QgsPointClusterRenderer* cluster_renderer = new QgsPointClusterRenderer();
	cluster_renderer->setTolerance(10);
	cluster_renderer->setToleranceUnit(Qgis::RenderUnit::Millimeters);
	cluster_renderer->setClusterSymbol(cluster_symbol);
	cluster_renderer->setEmbeddedRenderer(rule_renderer);

	return cluster_renderer;
}
