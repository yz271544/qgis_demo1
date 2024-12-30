//
// Created by Lyndon on 2024/12/27.
//

#include "QgsUtil.h"
#include "FileUtil.h"

QgsTextFormat QtFontUtil::create_font(const std::string& font_family, double font_size, const std::string& font_color,
	bool is_bold, bool is_italic, Qgis::TextOrientation orientation, double spacing) {
	QgsTextFormat text_format;
	QFont font(font_family.c_str());
	font.setPointSizeF(font_size);
	font.setBold(is_bold);
	font.setItalic(is_italic);
	text_format.setFont(font);
	text_format.setSize(font_size);
	text_format.setColor(QColor(font_color.c_str()));
	text_format.setOrientation(orientation);
	QgsTextBufferSettings buffer_settings;
	buffer_settings.setEnabled(true);
	buffer_settings.setSize(spacing);
	text_format.setBuffer(buffer_settings);
	return text_format;
}

void QgsUtil::show_layer_label(QgsVectorLayer* layer, const std::string& style) {
	layer->setLabelsEnabled(true);
	layer->setDisplayExpression("name");
	// Create label settings
	QgsPalLayerSettings label_settings;
	label_settings.fieldName = "name";
	// label_settings.placement = QgsPalLayerSettings::OverPoint;

	// Create text format for the labels
	QgsTextFormat text_format = QtFontUtil::create_font(style, 12.0, "#000000", false, false,
		Qgis::TextOrientation::Horizontal, 0.0);

	// Apply text format to label settings
	label_settings.setFormat(text_format);

	// Apply label settings to the layer
	QgsVectorLayerSimpleLabeling* labeling = new QgsVectorLayerSimpleLabeling(label_settings);
	layer->setLabeling(labeling);
}

QgsVectorLayerSimpleLabeling*
QgsUtil::get_layer_label(const std::string& style, const std::string& label_of_field_name) {
	QgsPalLayerSettings label_settings;
	label_settings.fieldName = label_of_field_name.c_str();
	// label_settings.placement = QgsPalLayerSettings::OverPoint;

	// Create text format for the labels
	QgsTextFormat text_format = QtFontUtil::create_font(style, 12.0, "#000000", false, false,
		Qgis::TextOrientation::Horizontal, 0.0);

	// Apply text format to label settings
	label_settings.setFormat(text_format);

	// Apply label settings to the layer
	return new QgsVectorLayerSimpleLabeling(label_settings);
}

float QgsUtil::d300_pixel_to_mm(float pixel_size) {
	// 假设Web页面上的字体大小是10像素
	// pixel_size = 10
	float web_dpi = 96;
	float target_dpi = 300;

	// 转换为毫米
	float font_size_mm = (pixel_size / web_dpi) * 25.4 * 2; // 先转换为毫米
	// 缩小比例
	float shrink_ratio = web_dpi / target_dpi;
	return font_size_mm * shrink_ratio; // 缩小字体
}

QgsCoordinateTransform QgsUtil::coordinate_transformer_4326_to_3857(QgsProject* project) {
	// Set coordinate transform
	QgsCoordinateReferenceSystem crs_4326("EPSG:4326"); // 假设 4326 是对应的EPSG代码
	QgsCoordinateReferenceSystem crs_3857("EPSG:3857"); // 假设 3857 是对应的EPSG代码
	return QgsCoordinateTransform(crs_4326, crs_3857, project);
}

QgsVectorLayer* QgsUtil::write_persisted_layer(const QString& layer_name,
	QgsVectorLayer* layer,
	const QString& project_dir,
	const QgsFields& fields,
	Qgis::WkbType qgs_wkb_type,
	const QgsCoordinateTransformContext& cts,
	const QgsCoordinateReferenceSystem& crs) {

	qDebug() << "CRS: " << crs.toWkt();
	// file path
	QString file_prefix = QString().append(project_dir).append("/").append(layer_name);
	QString file_path = QString().append(file_prefix).append(".geojson");

	// Delete existing GeoJSON file if it exists
	qDebug() << "delete geojson file:" << file_path;
	QFile::remove(file_path);
	//FileUtil::delete_file(file_path);


	// options
	QgsVectorFileWriter::SaveVectorOptions options;
	options.driverName = "GeoJSON";
	options.fileEncoding = "UTF-8";
	options.includeZ = true;
	options.overrideGeometryType = qgs_wkb_type;
	// options.layerName = layer_name;
	qDebug() << "prepare writer file:" << file_path;
	// Create a new vector file writer
	// std::unique_ptr<QgsVectorFileWriter> writer(QgsVectorFileWriter::create(
	// 	file_path,
	// 	fields,
	// 	qgs_wkb_type,
	// 	crs,
	// 	cts,
	// 	options
	// ));

	QgsVectorFileWriter *writer = QgsVectorFileWriter::create(
	    file_path,
	    fields,
	    qgs_wkb_type,
	    crs,
	    cts,
	    options
	);
	// assert
	if (writer->hasError() != QgsVectorFileWriter::NoError) {
		std::cerr << "Error creating file writer: " << writer->errorMessage().toStdString() << std::endl;
		return nullptr;
	}
	// write
	qDebug() << "writting: " << file_path << " ...";
	writer->writeAsVectorFormatV3(layer, file_path, cts, options);
	qDebug() << "writted the layer to geojson file";
	// flush disk
	writer->flushBuffer();
	delete writer;
	// delete gpkg
	// 假设 FileUtil 中的 delete_file 函数已经实现
	QString gpkg_file = QString().append(file_prefix).append(".gpkg");
	qDebug() << "delete file:" << gpkg_file;
	FileUtil::delete_file(gpkg_file);
	// return persistence layer
	return new QgsVectorLayer(file_path, layer_name, "ogr");
}
