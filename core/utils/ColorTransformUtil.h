//
// Created by etl on 12/27/24.
//
#ifndef COLORTRANSFORMUTIL_H
#define COLORTRANSFORMUTIL_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QDebug>
#include <QString>
#include <QVector>
#include <QHash>
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <cctype>


#if defined(_WIN32)
#include <windows.h>
#endif
class ColorTransformUtil {
public:
	/**
	* 将 rgba 字符串转换为整数元组
	* \param rgba rgba 字符串 rgb(47,153,243,1)
	* \returns 整数元组 tuple[int, int, int]
	*/
	static std::tuple<int, int, int, float> strRgbaToTupleInt(const QString& rgba);

	/**
	* 将 RGB 元组转换为十六进制字符串
	* \param rgb RGB 元组 tuple(47,153,243)
	* \returns 十六进制字符串 #2f99f3
	*/
	static QString rgbToHex(const std::tuple<int, int, int>& rgb);

	/**
	* 将 rgba 字符串转换为十六进制字符串
	* \param rgba rgba 字符串 rgb(47,153,243,1)
	* \returns pair 十六进制字符串和透明度
	* std::cout << "Hex: " << result.first << ", Capacity: " << result.second << std::endl;
	*/
	static std::pair<QString, float> strRgbaToHex(const QString& rgba);

	/**
	* 将十六进制字符串转换为 RGB 元组
	* \param hex_str 十六进制字符串 #2f99f3
	* \returns RGB 元组 tuple(47,153,243)
	*/
	static std::tuple<int, int, int> hexToRgb(const QString& hex_str);

	/**
	* 比较两种颜色
	* \param color1 颜色1
	* \param color2 颜色2
	* \returns 是否相等
	*/
	static bool compareColor(const QVector<QString>& color1, const QVector<QString>& color2);

	/**
	* 合并颜色
	* \param color 颜色 ['#ff4040', '#00cd52', '#2f99f3']
	* \returns 合并后的颜色 '#ff4040-#00cd52-#2f99f3'
	*/
	static QString mergeColor(const QVector<QString>& color);

	/**
	* 拆分颜色
	* \param merged_color 合并后的颜色 '#ff4040-#00cd52-#2f99f3'
	* \returns 拆分后的颜色 ['#ff4040', '#00cd52', '#2f99f3']
	*/
	static QVector<QString> splitColor(const QString& merged_color);


	/**
	* 对颜色进行分组统计
	* \param style_colors 颜色列表 [['#ff4040', '#00cd52', '#2f99f3'], ['#ff4040', '#00cd52', '#2f99f3'], ['#1c6ad6', '#00cd52', '#cbc829']]
	* \returns 分组统计结果 { '#ff4040-#00cd52-#2f99f3': 2, '#1c6ad6-#00cd52-#cbc829': 1 }
	*/
	static QHash<QString, int> multiColorGroup(const QVector<QVector<QString>>& style_colors);
};

#endif // COLORTRANSFORMUTIL_H