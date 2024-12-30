


#include "StylePoint.h"


QgsFeatureRenderer* StylePoint::get2d_rule_based_renderer(QJsonObject& font_style, QJsonObject& layer_style, QString& icon_path, qreal point_size = 5.0) {
	QMap<QString, QVariant>* label_style = new QMap<QString, QVariant>();
	if (font_style.contains("fontColor")) {
		QString font_color = font_style["fontColor"].toString();
		qDebug() << "origin font_color:" << font_color;
		std::pair<QString, float> colorOpacity = ColorTransformUtil::str_rgba_to_hex(font_color);
		qDebug() << "insert fontColor:" << colorOpacity.first << " fontOpacity:" << colorOpacity.second;
		label_style->insert("fontColor", colorOpacity.first);
		label_style->insert("fontOpacity", colorOpacity.second);
	}
	else {
		qDebug() << "default fontColor:" << "#000000" << " fontOpacity" << 1.0;
		label_style->insert("fontColor", "#000000");
		label_style->insert("fontOpacity", 1.0);
	}
	if (font_style.contains("fontFamily")) {
		qDebug() << "insert fontFamily:" << font_style["fontFamily"].toString();
		label_style->insert("fontFamily", font_style["fontFamily"].toString());
	}
	else {
		qDebug() << "default fontFamily:" << "SimSun";
		label_style->insert("fontFamily", "SimSun");
	}
	if (font_style.contains("fontSize")) {
		qDebug() << "insert fontSize:" << font_style["fontSize"].toInt();
		label_style->insert("fontSize", font_style["fontSize"].toInt());
	}
	else {
		qDebug() << "default fontSize:" << 12;
		label_style->insert("fontSize", 12);
	}
	label_style->insert("is_bold", true);
	label_style->insert("is_italic", false);
	label_style->insert("spacing", 0.0);

	QgsSymbol* rule_symbol = QgsSymbol::defaultSymbol(Qgis::GeometryType::Point);
	qDebug() << "set rule_font_marker fontFamily: " << (*label_style)["fontFamily"].toString();
	QgsFontMarkerSymbolLayer* rule_font_marker = new QgsFontMarkerSymbolLayer((*label_style)["fontFamily"].toString());

	qDebug() << "set rule_font_marker fontSize: " << (*label_style)["fontSize"].toInt();
	rule_font_marker->setSizeUnit(Qgis::RenderUnit::Millimeters);
	rule_font_marker->setSize(QgsUtil::d300_pixel_to_mm((*label_style)["fontSize"].toInt()));

	qDebug() << "set rule_font_marker fontColor: " << (*label_style)["fontColor"].toString();
	rule_font_marker->setColor(QColor((*label_style)["fontColor"].toString()));
	rule_font_marker->setFillColor(QColor((*label_style)["fontColor"].toString()));
	//    rule_font_marker->setColor(QColor(237, 233, 26));
	//    rule_font_marker->setFillColor(QColor(237, 233, 26));
	//    rule_font_marker->setColor(QColor("#EDE91A"));
	//    rule_font_marker->setFillColor(QColor("#EDE91A"));
	//    rule_font_marker->setColor(QColor("#ede91a"));
	//    rule_font_marker->setFillColor(QColor("#ede91a"));

	rule_font_marker->setDataDefinedProperty(QgsSymbolLayer::Property::Character, QgsProperty::fromExpression("name"));
	rule_font_marker->setOffset(QPointF(0, -5));

	QgsRasterMarkerSymbolLayer* rule_raster_marker = new QgsRasterMarkerSymbolLayer(icon_path);
	rule_raster_marker->setSizeUnit(Qgis::RenderUnit::Millimeters);

	float raster_marker_size = QgsUtil::d300_pixel_to_mm(point_size);
	qDebug() << "point_size:" << point_size << " --> raster_marker_size: " << raster_marker_size;
	rule_raster_marker->setSize(raster_marker_size);
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
	//QgsSymbol* cluster_symbol = QgsSymbol::defaultSymbol(Qgis::GeometryType::Point);
	QgsMarkerSymbol* cluster_symbol = new QgsMarkerSymbol();
	QgsFontMarkerSymbolLayer* font_marker = new QgsFontMarkerSymbolLayer((*label_style)["fontFamily"].toString());
	font_marker->setSize(QgsUtil::d300_pixel_to_mm((*label_style)["fontSize"].toInt()));
	font_marker->setColor(QColor((*label_style)["fontColor"].toString()));
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

QgsAbstract3DRenderer* StylePoint::get3d_single_symbol_renderer(
	QgsVectorLayer& point_layer,
	QJsonObject& font_style,
	QJsonObject& layer_style,
	QString& icon_path,
	qreal point_size) {
	QgsPoint3DSymbol* symbol = new QgsPoint3DSymbol();
	QgsPhongMaterialSettings* material_settings = new QgsPhongMaterialSettings();

	QString font_color;
	if (font_style.contains("fontColor")) {
		font_color = font_style["fontColor"].toString();
	}
	else {
		font_color = "#000000";
	}

	std::pair<QString, float> colorOpacity = ColorTransformUtil::str_rgba_to_hex(font_color);
	QString material_font_color = colorOpacity.first;
	float material_font_opacity = colorOpacity.second;

	material_settings->setOpacity(material_font_opacity);

	material_settings->setDiffuse(QColor(font_color));
	material_settings->setAmbient(QColor(font_color));
	symbol->setMaterialSettings(material_settings);

	QgsVectorLayer3DRenderer* renderer = new QgsVectorLayer3DRenderer();
	renderer->setLayer(&point_layer);
	renderer->setSymbol(symbol);
	return renderer;
}

QgsAbstract3DRenderer* StylePoint::get3d_single_raster_symbol_renderer(
	QgsVectorLayer& point_layer,
	QJsonObject& font_style,
	QJsonObject& layer_style,
	QString& icon_path,
	qreal point_size) {
	QMap<QString, QVariant>* label_style = new QMap<QString, QVariant>();

	if (font_style.contains("fontColor")) {
		QString font_color = font_style["fontColor"].toString();
		std::pair<QString, float> colorOpacity = ColorTransformUtil::str_rgba_to_hex(font_color);
		label_style->insert("fontColor", colorOpacity.first);
		label_style->insert("fontOpacity", colorOpacity.second);

	}
	else {
		label_style->insert("fontColor", "#000000");
		label_style->insert("fontOpacity", 1.0);
	}
	if (font_style.contains("fontFamily")) {
		label_style->insert("fontFamily", font_style["fontFamily"].toString());
	}
	else {
		label_style->insert("fontFamily", "SimSun");
	}
	if (font_style.contains("fontSize")) {
		label_style->insert("fontSize", font_style["fontSize"].toInt());
	}
	else {
		label_style->insert("fontSize", 12);
	}
	label_style->insert("is_bold", true);
	label_style->insert("is_italic", false);
	label_style->insert("spacing", 0.0);


	float point_size_mm = QgsUtil::d300_pixel_to_mm(point_size);
	// 创建一个嵌入式规则渲染器
	QgsRasterMarkerSymbolLayer* raster_marker = new QgsRasterMarkerSymbolLayer(icon_path);

	raster_marker->setSize(point_size_mm);
	raster_marker->setSizeUnit(Qgis::RenderUnit::Millimeters);
	raster_marker->setOpacity(1.0);
	raster_marker->setAngle(0.0);
	raster_marker->setOffset(QPointF(0, 0));
	raster_marker->setVerticalAnchorPoint(QgsMarkerSymbolLayer::VerticalAnchorPoint::VCenter);
	raster_marker->setHorizontalAnchorPoint(QgsMarkerSymbolLayer::HorizontalAnchorPoint::HCenter);

	QgsPoint3DSymbol* symbol = new QgsPoint3DSymbol();
	symbol->setShape(Qgis::Point3DShape::Billboard);
	QgsMarkerSymbol* marker_symbol = symbol->billboardSymbol();
	marker_symbol->changeSymbolLayer(0, raster_marker);

	QgsVectorLayer3DRenderer* renderer = new QgsVectorLayer3DRenderer();
	renderer->setSymbol(symbol);
	return renderer;
}

QgsRuleBased3DRenderer* StylePoint::get3d_rule_renderer(
	QgsVectorLayer& point_layer,
	QJsonObject& font_style,
	QJsonObject& layer_style,
	QString& icon_path,
	qreal point_size) {
	// Create the root rule
	QgsRuleBased3DRenderer::Rule* root_rule = new QgsRuleBased3DRenderer::Rule(nullptr);

	QMap<QString, QVariant> label_style;

	if (font_style.contains("fontColor")) {
		QString font_color = font_style["fontColor"].toString();
		std::pair<QString, float> colorOpacity = ColorTransformUtil::str_rgba_to_hex(font_color);
		label_style.insert("fontColor", colorOpacity.first);
		label_style.insert("fontOpacity", colorOpacity.second);

	}
	else {
		label_style.insert("fontColor", "#000000");
		label_style.insert("fontOpacity", 1.0);
	}
	if (font_style.contains("fontFamily")) {
		label_style.insert("fontFamily", font_style["fontFamily"].toString());
	}
	else {
		label_style.insert("fontFamily", "SimSun");
	}
	if (font_style.contains("fontSize")) {
		label_style.insert("fontSize", font_style["fontSize"].toInt());
	}
	else {
		label_style.insert("fontSize", 12);
	}
	label_style.insert("is_bold", true);
	label_style.insert("is_italic", false);
	label_style.insert("spacing", 0.0);


	float point_size_mm = QgsUtil::d300_pixel_to_mm(point_size);
	// 创建一个嵌入式规则渲染器
	QgsMarkerSymbol* mark_symbol = new QgsMarkerSymbol();
	mark_symbol->setSize(point_size_mm);
	mark_symbol->setSizeUnit(Qgis::RenderUnit::Millimeters);
	mark_symbol->setOpacity(1.0);
	mark_symbol->setAngle(0.0);

	QgsRasterMarkerSymbolLayer* raster_marker = new QgsRasterMarkerSymbolLayer(icon_path);
	raster_marker->setSize(point_size_mm);
	raster_marker->setSizeUnit(Qgis::RenderUnit::Millimeters);
	raster_marker->setOpacity(1.0);
	raster_marker->setAngle(0.0);
	raster_marker->setOffset(QPointF(0, 0));
	raster_marker->setVerticalAnchorPoint(QgsMarkerSymbolLayer::VerticalAnchorPoint::VCenter);
	raster_marker->setHorizontalAnchorPoint(QgsMarkerSymbolLayer::HorizontalAnchorPoint::HCenter);

	mark_symbol->changeSymbolLayer(0, raster_marker);
	mark_symbol->setFlags(Qgis::SymbolFlag::AffectsLabeling);

	QgsPoint3DSymbol* symbol = new QgsPoint3DSymbol();
	QgsNullMaterialSettings* null_material_settings = new QgsNullMaterialSettings();
	symbol->setMaterialSettings(null_material_settings);
	symbol->setShape(Qgis::Point3DShape::Billboard);

	QgsRuleBased3DRenderer::Rule* rule = new QgsRuleBased3DRenderer::Rule(nullptr);

	rule->setSymbol(symbol);
	root_rule->appendChild(rule);

	QgsRuleBased3DRenderer* renderer3d = new QgsRuleBased3DRenderer(root_rule);

	return renderer3d;
}