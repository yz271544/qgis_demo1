
#include "ColorTransformUtil.h"

std::tuple<int, int, int, float> ColorTransformUtil::str_rgba_to_tuple_int(const std::string& rgba) {
	std::string str = rgba;
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
	std::vector<std::string> parts;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(',')) != std::string::npos) {
		token = str.substr(0, pos);
		parts.push_back(token);
		str.erase(0, pos + 1);
	}
	parts.push_back(str);
	// 解析并转换为相应类型
	int r = std::stoi(parts[0]);
	int g = std::stoi(parts[1]);
	int b = std::stoi(parts[2]);
	float a = std::stof(parts[3]);
	return std::make_tuple(r, g, b, a);
}

// 将 RGB 元组转换为十六进制字符串
std::string ColorTransformUtil::rgb_to_hex(const std::tuple<int, int, int>& rgb) {
	std::stringstream ss;
	ss << "#";
	ss << std::hex << std::setfill('0') << std::setw(2) << std::get<0>(rgb);
	ss << std::hex << std::setfill('0') << std::setw(2) << std::get<1>(rgb);
	ss << std::hex << std::setfill('0') << std::setw(2) << std::get<2>(rgb);
	return ss.str();
}

std::pair<std::string, float> ColorTransformUtil::str_rgba_to_hex(const std::string& rgba) {
	auto rgb = ColorTransformUtil::str_rgba_to_tuple_int(rgba);
	float capacity = std::get<3>(rgb);
	std::string hex = ColorTransformUtil::rgb_to_hex(std::make_tuple(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb)));
	return std::make_pair(hex, capacity);
}

// 将十六进制字符串转换为 RGB 元组
std::tuple<int, int, int> ColorTransformUtil::hex_to_rgb(const std::string& hex_str) {
	int r = std::stoi(hex_str.substr(1, 2), nullptr, 16);
	int g = std::stoi(hex_str.substr(3, 2), nullptr, 16);
	int b = std::stoi(hex_str.substr(5, 2), nullptr, 16);
	return std::make_tuple(r, g, b);
}

// 比较两种颜色
bool ColorTransformUtil::compare_color(const std::vector<std::string>& color1, const std::vector<std::string>& color2) {
	return color1[0] == color2[0] && color1[1] == color2[1] && color1[2] == color2[2];
}

// 合并颜色
std::string ColorTransformUtil::merge_color(const std::vector<std::string>& color) {
	std::string result;
	for (size_t i = 0; i < color.size(); ++i) {
		result += color[i];
		if (i < color.size() - 1) {
			result += "-";
		}
	}
	return result;
}

// 拆分颜色
std::vector<std::string> ColorTransformUtil::split_color(const std::string& merged_color) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t pos = 0;
	while ((pos = merged_color.find('-', start)) != std::string::npos) {
		result.push_back(merged_color.substr(start, pos - start));
		start = pos + 1;
	}
	result.push_back(merged_color.substr(start));
	return result;
}


// 对颜色进行分组统计
std::unordered_map<std::string, int> ColorTransformUtil::multi_color_group(const std::vector<std::vector<std::string>>& style_colors) {
	std::unordered_map<std::string, int> color_dict;
	for (const auto& style_color : style_colors) {
		std::string style_color_merged = merge_color(style_color);
		if (color_dict.count(style_color_merged) > 0) {
			color_dict[style_color_merged]++;
		}
		else {
			color_dict[style_color_merged] = 1;
		}
	}
	return color_dict;
}