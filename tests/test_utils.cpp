#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QString>
#include <QtTest>
#include "../core/utils/ColorTransformUtil.h"
#include "../core/utils/ImageUtil.h"

#if defined(_WIN32)
#include <windows.h>
#endif

class TestUtils : public QObject
{
	Q_OBJECT
private slots:
	static void test_str_rgba_to_tuple_int()
	{
		std::tuple<int, int, int, float> rgb_capacity = ColorTransformUtil::strRgbaToTupleInt("rgba(255, 255, 255, 0.5)");
		QCOMPARE(std::get<0>(rgb_capacity), 255);
		QCOMPARE(std::get<1>(rgb_capacity), 255);
		QCOMPARE(std::get<2>(rgb_capacity), 255);
		QCOMPARE(std::get<3>(rgb_capacity), 0.5f);
	}

	static void test_rgb_to_hex() {
		QString rgb_hex = ColorTransformUtil::rgbToHex(std::make_tuple(255, 255, 255));
		QCOMPARE(rgb_hex, "#ffffff");
	}

	static void test_str_rgba_to_hex() {
		std::pair<QString, float> hex_capacity = ColorTransformUtil::strRgbaToHex("rgba(255, 255, 255, 0.5)");
		QCOMPARE(hex_capacity.first, "#ffffff");
		QCOMPARE(hex_capacity.second, 0.5f);
	}

	static void test_hex_to_rgb() {
		std::tuple<int, int, int> rgb = ColorTransformUtil::hexToRgb("#ffffff");
		QCOMPARE(std::get<0>(rgb), 255);
		QCOMPARE(std::get<1>(rgb), 255);
		QCOMPARE(std::get<2>(rgb), 255);
	}

	static void test_compare_color() {
		QVector<QString> color1 = { "#ff4040", "#00cd52", "#2f99f3" };
		QVector<QString> color2 = { "#ff4040", "#00cd52", "#2f99f3" };
		QVERIFY(ColorTransformUtil::compareColor(color1, color2));
	}

	static void test_merge_color() {
		QVector<QString> color = { "#ff4040", "#00cd52", "#2f99f3" };
		QString merged_color = ColorTransformUtil::mergeColor(color);
		QCOMPARE(merged_color, "#ff4040-#00cd52-#2f99f3");
	}

	static void tset_split_color() {
		QVector<QString> color = { "#ff4040", "#00cd52", "#2f99f3" };
		QString merged_color = ColorTransformUtil::mergeColor(color);
		QVector<QString> split_color = ColorTransformUtil::splitColor(merged_color);
		QCOMPARE(split_color.size(), 3);
		QCOMPARE(split_color[0], "#ff4040");
		QCOMPARE(split_color[1], "#00cd52");
		QCOMPARE(split_color[2], "#2f99f3");
	}

	static void test_multi_color_group() {
		QVector<QVector<QString>> style_colors = {
			{ "#ff4040", "#00cd52", "#2f99f3" },
			{ "#ff4040", "#00cd52", "#2f99f3" },
			{ "#1c6ad6", "#00cd52", "#cbc829" }
		};
		QHash<QString, int> color_dict = ColorTransformUtil::multiColorGroup(style_colors);
		QCOMPARE(color_dict.size(), 2);
		QCOMPARE(color_dict["#ff4040-#00cd52-#2f99f3"], 2);
		QCOMPARE(color_dict["#1c6ad6-#00cd52-#cbc829"], 1);
	}

	static void test_base64_case1() {
		QString save_qgis_project_path = "/lyndon/iProject/cpath/qgis_demo1/common/png";
		QString layer_name = "民警";
		QString icon_base64 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAAolBMVEUAAAC1xPmIn/SKn//n6/3e5PzS2vvN1vusvPeUqfWNpPUAMub////z9f6mt/by9P1Ia+2MovP6+/72+P7AzPiUqfQrVOoFNuYgTOnp7fzi5/xOcO2dr/UuV+p6lPISQOisvPejtPZ/l/IcSOje5Ptaeu9EaOybrvV3kfFTdO7X3vvI0vm9yfi6x/iDm/Jvi/BphvBVdu5Nb+05X+uPpPSOpPN4UAcsAAAAC3RSTlMA0y0M/fny7sF6UvKXmSMAAAGJSURBVDjLhZNXYoMwDEAhkGnZmGFICZBA9l7t/a9WRx6h6Qfvx0g22nLeeGN32Ke0P3THnvOfnhtAkLAwZIn8cHsf194ojgqeESTjRRSPvD+/D4BNSYspg0HLyMSnM/LBjPoT+7+fp6icX9liwa5zFMrc1za8AcX7VFBYheEKqEC5pAMVxwjQ/gYkBec1SDboBUboIGbKfiQvdpwX8ohUxCx+OXG1REJ5E+y+X++EziVyZQTBlijm+ZfGX2pVEXjOGDjRnEFzNhoOY8cNMiOKphF1LapdaDRZ4DrDhBiOAP7hkAMcrSoZOn1mJQEaYVWs79DQSiVoSqsKKT4wLABZkNYD5cImCpJ8TlouMEjLxpbZBolpWu4gWWtBp/kuVNqcAH5kHKcmbRXKCwqVQqISWOORlKbUplmPGJD9XZ3xQzXLtHtJQfF8goIusd1mYNag2e9Bs8aBMSOXVStAqkqdqyZL1cjZoV3y20XUVVWLy41L+6ka2u6x714cu3rb9+pt7ep1L2/n+v8CysQxDA9OhyUAAAAASUVORK5CYII=";
		qDebug() << "icon_base64:" << icon_base64;
		QString icon_path = QString().append(save_qgis_project_path).append("/").append(layer_name).append(".png");
		qDebug() << "icon_path:" << icon_path;
		if (!icon_base64.isEmpty())
		{
			std::pair<QString, QByteArray> base64_image = ImageUtil::parse_base64_image(icon_base64);
			qDebug() << "parsed the extension is:" << base64_image.first;
			qDebug() << "parsed the bytearray is:" << base64_image.second;
			QFile iconFile(icon_path);
			if (iconFile.open(QIODevice::WriteOnly)) {
				iconFile.write(base64_image.second);
				iconFile.close();
			}
			else {
				qWarning() << "Failed to open file for writing:" << icon_path;
			}
		}
	}

};

QTEST_MAIN(TestUtils)
#include "test_utils.moc"