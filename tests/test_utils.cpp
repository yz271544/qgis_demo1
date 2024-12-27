#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QString>
#include <QtTest>
#include "../core/utils/ColorTransformUtil.h"



class TestUtils : public QObject
{
	Q_OBJECT
private slots:
	void test_str_rgba_to_tuple_int()
	{
		std::tuple<int, int, int, float> rgb_capacity = ColorTransformUtil::str_rgba_to_tuple_int("rgba(255, 255, 255, 0.5)");
		QCOMPARE(std::get<0>(rgb_capacity), 255);
		QCOMPARE(std::get<1>(rgb_capacity), 255);
		QCOMPARE(std::get<2>(rgb_capacity), 255);
		QCOMPARE(std::get<3>(rgb_capacity), 0.5f);
	}

	void test_rgb_to_hex() {
		std::string rgb_hex = ColorTransformUtil::rgb_to_hex(std::make_tuple(255, 255, 255));
		QCOMPARE(rgb_hex, "#ffffff");
	}

	void test_str_rgba_to_hex() {
		std::pair<std::string, float> hex_capacity = ColorTransformUtil::str_rgba_to_hex("rgba(255, 255, 255, 0.5)");
		QCOMPARE(hex_capacity.first, "#ffffff");
		QCOMPARE(hex_capacity.second, 0.5f);
	}

	void test_hex_to_rgb() {
		std::tuple<int, int, int> rgb = ColorTransformUtil::hex_to_rgb("#ffffff");
		QCOMPARE(std::get<0>(rgb), 255);
		QCOMPARE(std::get<1>(rgb), 255);
		QCOMPARE(std::get<2>(rgb), 255);
	}

	void test_compare_color() {
		std::vector<std::string> color1 = { "#ff4040", "#00cd52", "#2f99f3" };
		std::vector<std::string> color2 = { "#ff4040", "#00cd52", "#2f99f3" };
		QVERIFY(ColorTransformUtil::compare_color(color1, color2));
	}

	void test_merge_color() {
		std::vector<std::string> color = { "#ff4040", "#00cd52", "#2f99f3" };
		std::string merged_color = ColorTransformUtil::merge_color(color);
		QCOMPARE(merged_color, "#ff4040-#00cd52-#2f99f3");
	}

	void tset_split_color() {
		std::vector<std::string> color = { "#ff4040", "#00cd52", "#2f99f3" };
		std::string merged_color = ColorTransformUtil::merge_color(color);
		std::vector<std::string> split_color = ColorTransformUtil::split_color(merged_color);
		QCOMPARE(split_color.size(), 3);
		QCOMPARE(split_color[0], "#ff4040");
		QCOMPARE(split_color[1], "#00cd52");
		QCOMPARE(split_color[2], "#2f99f3");
	}

	void test_multi_color_group() {
		std::vector<std::vector<std::string>> style_colors = {
			{ "#ff4040", "#00cd52", "#2f99f3" },
			{ "#ff4040", "#00cd52", "#2f99f3" },
			{ "#1c6ad6", "#00cd52", "#cbc829" }
		};
		std::unordered_map<std::string, int> color_dict = ColorTransformUtil::multi_color_group(style_colors);
		QCOMPARE(color_dict.size(), 2);
		QCOMPARE(color_dict["#ff4040-#00cd52-#2f99f3"], 2);
		QCOMPARE(color_dict["#1c6ad6-#00cd52-#cbc829"], 1);
	}

};

QTEST_MAIN(TestUtils)
#include "test_utils.moc"