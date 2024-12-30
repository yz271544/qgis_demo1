
#include "ColorTransformUtil.h"

// 将 rgba 字符串转换为整数元组
std::tuple<int, int, int, float> ColorTransformUtil::str_rgba_to_tuple_int(const QString& rgba) {
	std::string str = rgba.toStdString();
	// 移除 "rgba(" 和 ")"
	size_t start = str.find("rgba(");
	if (start != std::string::npos) {
		str = str.substr(start + 5);
	}
	size_t end = str.find(")");
	if (end != std::string::npos) {
		str = str.substr(0, end);
	}
	// 按逗号分割字符串
	QVector<QString> parts;
	size_t pos = 0;
	while ((pos = str.find(',')) != std::string::npos) {
		parts.push_back(QString::fromStdString(str.substr(0, pos)));
		str.erase(0, pos + 1);
	}
	parts.push_back(QString::fromStdString(str));
	// 解析并转换为相应类型
	int r = parts[0].toInt();
	int g = parts[1].toInt();
	int b = parts[2].toInt();
	float a = parts[3].toFloat();
	return std::make_tuple(r, g, b, a);
}

// 将 RGB 元组转换为十六进制字符串
QString ColorTransformUtil::rgb_to_hex(const std::tuple<int, int, int>& rgb) {
	return QString("#%1%2%3")
		.arg(std::get<0>(rgb), 2, 16, QLatin1Char('0'))
		.arg(std::get<1>(rgb), 2, 16, QLatin1Char('0'))
		.arg(std::get<2>(rgb), 2, 16, QLatin1Char('0'));
}

std::pair<QString, float> ColorTransformUtil::str_rgba_to_hex(const QString& rgba) {
	auto rgb = str_rgba_to_tuple_int(rgba);
	float capacity = std::get<3>(rgb);
	QString hex = rgb_to_hex(std::make_tuple(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb)));
	return std::make_pair(hex, capacity);
}

// 将十六进制字符串转换为 RGB 元组
std::tuple<int, int, int> ColorTransformUtil::hex_to_rgb(const QString& hex_str) {
	std::string str = hex_str.toStdString();
	int r = std::stoi(str.substr(1, 2), nullptr, 16);
	int g = std::stoi(str.substr(3, 2), nullptr, 16);
	int b = std::stoi(str.substr(5, 2), nullptr, 16);
	return std::make_tuple(r, g, b);
}

// 比较两种颜色
bool ColorTransformUtil::compare_color(const QVector<QString>& color1, const QVector<QString>& color2) {
	if (color1.size() < 3 || color2.size() < 3) {
		return false;
	}
	return color1[0] == color2[0] && color1[1] == color2[1] && color1[2] == color2[2];
}

// 合并颜色
QString ColorTransformUtil::merge_color(const QVector<QString>& color) {
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
QVector<QString> ColorTransformUtil::split_color(const QString& merged_color) {
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
QHash<QString, int> ColorTransformUtil::multi_color_group(const QVector<QVector<QString>>& style_colors) {
	QHash<QString, int> color_dict;
	for (const auto& style_color : style_colors) {
		QString style_color_merged = merge_color(style_color);
		if (color_dict.contains(style_color_merged)) {
			color_dict[style_color_merged]++;
		}
		else {
			color_dict[style_color_merged] = 1;
		}
	}
	return color_dict;
}
