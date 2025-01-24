
#include "ColorTransformUtil.h"

std::tuple<int, int, int, float> ColorTransformUtil::strRgbaToTupleInt(const QString& rgba) {
	if (rgba.isEmpty()) {
		qCritical() << "Input string is empty";
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	QString cleanedRgba = rgba.trimmed(); // 去除前后空格
	// qDebug() << "Original rgba:" << rgba;
	// qDebug() << "Cleaned rgba:" << cleanedRgba;

	if (!cleanedRgba.startsWith("rgba(") || !cleanedRgba.endsWith(")")) {
		qCritical() << "Invalid rgba format: must start with 'rgba(' and end with ')'";
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	// 去掉 'rgba(' 和 ')'
	cleanedRgba = cleanedRgba.mid(5, cleanedRgba.length() - 6).trimmed();
	// qDebug() << "After removing 'rgba(' and ')':" << cleanedRgba;

	// 按逗号分割
	QStringList parts = cleanedRgba.split(",");
	if (parts.size() != 4) {
		qCritical() << "Invalid rgba format: expected 4 components, got" << parts.size();
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	// 转换每个部分
	bool ok;
	int r = parts[0].trimmed().toInt(&ok);
	if (!ok) {
		qCritical() << "Invalid red component:" << parts[0];
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	int g = parts[1].trimmed().toInt(&ok);
	if (!ok) {
		qCritical() << "Invalid green component:" << parts[1];
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	int b = parts[2].trimmed().toInt(&ok);
	if (!ok) {
		qCritical() << "Invalid blue component:" << parts[2];
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	float a = parts[3].trimmed().toFloat(&ok);
	if (!ok) {
		qCritical() << "Invalid alpha component:" << parts[3];
		return std::make_tuple(0, 0, 0, 0.0f);
	}

	return std::make_tuple(r, g, b, a);
}

// 将 RGB 元组转换为十六进制字符串
QString ColorTransformUtil::rgbToHex(const std::tuple<int, int, int>& rgb) {
	return QString("#%1%2%3")
		.arg(std::get<0>(rgb), 2, 16, QLatin1Char('0'))
		.arg(std::get<1>(rgb), 2, 16, QLatin1Char('0'))
		.arg(std::get<2>(rgb), 2, 16, QLatin1Char('0'));
}

std::pair<QString, float> ColorTransformUtil::strRgbaToHex(const QString& rgba) {
	auto rgb = strRgbaToTupleInt(rgba);
	float capacity = std::get<3>(rgb);
	QString hex = rgbToHex(std::make_tuple(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb)));
	return std::make_pair(hex, capacity);
}

// 将十六进制字符串转换为 RGB 元组
std::tuple<int, int, int> ColorTransformUtil::hexToRgb(const QString& hex_str) {
	std::string str = hex_str.toStdString();
	int r = std::stoi(str.substr(1, 2), nullptr, 16);
	int g = std::stoi(str.substr(3, 2), nullptr, 16);
	int b = std::stoi(str.substr(5, 2), nullptr, 16);
	return std::make_tuple(r, g, b);
}

// 比较两种颜色
bool ColorTransformUtil::compareColor(const QVector<QString>& color1, const QVector<QString>& color2) {
	if (color1.size() < 3 || color2.size() < 3) {
		return false;
	}
	return color1[0] == color2[0] && color1[1] == color2[1] && color1[2] == color2[2];
}

// 合并颜色
QString ColorTransformUtil::mergeColor(const QVector<QString>& color) {
	QString result;
	for (int i = 0; i < color.size(); ++i) {
		result += color[i];
		if (i < color.size() - 1) {
			result += "-";
		}
	}
	return result;
}

// 拆分颜色
QVector<QString> ColorTransformUtil::splitColor(const QString& merged_color) {
	QVector<QString> result;
	int start = 0;
	int pos = 0;
	while ((pos = merged_color.indexOf('-', start)) != -1) {
		result.push_back(merged_color.mid(start, pos - start));
		start = pos + 1;
	}
	result.push_back(merged_color.mid(start));
	return result;
}


// 对颜色进行分组统计
QHash<QString, int> ColorTransformUtil::multiColorGroup(const QVector<QVector<QString>>& style_colors) {
	QHash<QString, int> color_dict;
	for (const auto& style_color : style_colors) {
		QString style_color_merged = mergeColor(style_color);
		if (color_dict.contains(style_color_merged)) {
			color_dict[style_color_merged]++;
		}
		else {
			color_dict[style_color_merged] = 1;
		}
	}
	return color_dict;
}
