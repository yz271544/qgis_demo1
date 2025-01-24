//
// Created by Lyndon on 2024/12/27.
//

#include "QgsUtil.h"
#include "FileUtil.h"

QgsTextFormat* QtFontUtil::create_font(
	const QString& font_family,
	int8_t font_size,
	const QString& font_color,
	bool is_bold,
	bool is_italic,
	Qgis::TextOrientation orientation,
	double spacing)
{
	QgsTextFormat* text_format = new QgsTextFormat();
	QFont font(font_family);
	font.setPointSizeF(font_size);
	font.setBold(is_bold);
	font.setItalic(is_italic);
	text_format->setFont(font);
	text_format->setSize(font_size);
	text_format->setColor(QColor(font_color));
	text_format->setOrientation(orientation);
	QgsTextBufferSettings buffer_settings;
	buffer_settings.setEnabled(true);
	buffer_settings.setSize(spacing);
	text_format->setBuffer(buffer_settings);
	return text_format;
}

void QgsUtil::show_layer_label(QgsVectorLayer* layer, const QString& style) {
	layer->setLabelsEnabled(true);
	layer->setDisplayExpression("name");
	// Create label settings
	QgsPalLayerSettings label_settings;
	label_settings.fieldName = "name";
	// label_settings.placement = QgsPalLayerSettings::OverPoint;

	// Create text format for the labels
	QgsTextFormat* text_format = QtFontUtil::create_font(style, 12, QString("#000000"), false, false,
		Qgis::TextOrientation::Horizontal, 0.0);

	// Apply text format to label settings
	label_settings.setFormat(*text_format);

	// Apply label settings to the layer
	QgsVectorLayerSimpleLabeling* labeling = new QgsVectorLayerSimpleLabeling(label_settings);
	layer->setLabeling(labeling);
}

QgsVectorLayerSimpleLabeling*
QgsUtil::get_layer_label(const QString& style, const std::string& label_of_field_name) {
	QgsPalLayerSettings label_settings;
	label_settings.fieldName = label_of_field_name.c_str();
	// label_settings.placement = QgsPalLayerSettings::OverPoint;

	// Create text format for the labels
	QgsTextFormat* text_format = QtFontUtil::create_font(style, 12, QString("#000000"), false, false,
		Qgis::TextOrientation::Horizontal, 0.0);

	// Apply text format to label settings
	label_settings.setFormat(*text_format);

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
	qDebug() << "Number of features in layer: " << layer->featureCount();
	// QgsFeatureIterator it = layer->getFeatures();
	// QgsFeature feature;
	// while (it.nextFeature(feature)) {
	// 	qDebug() << "Feature ID:" << feature.id();
	// 	qDebug() << "Geometry:" << feature.geometry().asWkt();
	// 	qDebug() << "Attributes:" << feature.attributes();
	// }

	QString file_prefix = QString().append(project_dir).append("/").append(layer_name);
	QString file_path = QString().append(file_prefix).append(".geojson");
	QString temp_file_path = QString().append(file_prefix).append(".tmp");

	qDebug() << "GeoJSON file path: " << file_path;

	// 删除旧文件
	if (QFile::exists(file_path)) {
		if (!QFile::remove(file_path)) {
			qDebug() << "Failed to delete existing file: " << file_path;
			return nullptr;
		}
	}

	// 设置写入选项
	QgsVectorFileWriter::SaveVectorOptions options;
	options.driverName = "GeoJSON";
	options.fileEncoding = "UTF-8";
	options.includeZ = true;
	options.overrideGeometryType = qgs_wkb_type;

	// 写入临时文件
	QgsVectorFileWriter* writer = QgsVectorFileWriter::create(
		temp_file_path,
		fields,
		qgs_wkb_type,
		crs,
		cts,
		options
	);

	if (writer->hasError() != QgsVectorFileWriter::NoError) {
		std::cerr << "Error creating file writer: " << writer->errorMessage().toStdString() << std::endl;
		delete writer;
		return nullptr;
	}

	// 写入所有要素
	QgsFeatureIterator it = layer->getFeatures();
	QgsFeature feature;
	while (it.nextFeature(feature)) {
		if (writer->addFeature(feature) != QgsVectorFileWriter::NoError) {
			qDebug() << "Failed to write feature ID:" << feature.id();
		}
	}

	writer->flushBuffer();
	delete writer;

	// 确保资源释放
	QThread::msleep(100);

	// QgsVectorFileWriter::writeAsVectorFormatV3(
	// 	layer,                    // 原始图层
	// 	file_path,                // 目标文件路径
	// 	cts,                      // 坐标转换上下文
	// 	options                   // 写入选项
	// );

	// 重命名临时文件
	QString temp_real_file_path = QString(temp_file_path).append(".geojson");
	if (!QFile::rename(temp_real_file_path, file_path)) {
		qDebug() << "Failed to rename temp file to target file.";
		qDebug() << "Temp file path:" << temp_file_path;
		qDebug() << "Target file path:" << file_path;
		return nullptr;
	}

	qDebug() << "Successfully wrote GeoJSON file: " << file_path << " baseName:" << layer_name;
	return new QgsVectorLayer(file_path, layer_name, "ogr");
}
