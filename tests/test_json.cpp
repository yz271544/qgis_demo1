#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QString>
#include <QtTest>

#include "../../../core/utils/JsonUtil.h"

#if defined(_WIN32)
#include <windows.h>
#endif

class TestJson : public QObject
{
	Q_OBJECT
private slots:
	static void testCase1()
	{
		QVERIFY(2 + 2 == 4);
	}

	static void testCase2() {
		QString jsonStr = R"({
            "name": "Alice",
            "age": 25,
            "hobbies": ["reading", "swimming"],
            "address": {
                "city": "New York",
                "street": "Broadway"
            }
        })";

		// 将JSON字符串转换为QJsonDocument
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
		if (jsonDoc.isObject()) {
			QJsonObject jsonObject = jsonDoc.object();

			// 遍历JSON对象的键值对
			QJsonObject::const_iterator it;
			for (it = jsonObject.begin(); it != jsonObject.end(); ++it) {
				QString key = it.key();
				QJsonValue value = it.value();
				if (value.isString()) {
					qDebug() << key << ": " << value.toString();
				}
				else if (value.isDouble()) {
					qDebug() << key << ": " << value.toDouble();
				}
				else if (value.isArray()) {
					qDebug() << key << " (array): ";
					QJsonArray jsonArray = value.toArray();
					for (int i = 0; i < jsonArray.size(); ++i) {
						qDebug() << "    - " << jsonArray.at(i).toString();
					}
				}
				else if (value.isObject()) {
					qDebug() << key << " (object): ";
					QJsonObject subObject = value.toObject();
					QJsonObject::const_iterator subIt;
					for (subIt = subObject.begin(); subIt != subObject.end(); ++subIt) {
						qDebug() << "    " << subIt.key() << ": " << subIt.value().toString();
					}
				}
			}
		}
		qDebug() << "test success";
	}

	static void test_parse_topic_payload() {
		QFile file("/lyndon/iProject/cpath/qgis_demo1/common/input/topicMap.json");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qCritical() << "Could not open file";
		}
		QByteArray jsonData = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(jsonData);
		if (doc.isNull()) {
			qCritical() << "Failed to parse JSON document";
		}


		if (doc.isObject()) {
			QJsonObject obj = doc.object();
			// 访问对象中的键值对
			if (obj.contains("data")) {
				QJsonValue data = obj.value("data");
				if (data.isArray()) {
					auto data1 = data[0];
					if (data1.isObject()) {
						QJsonObject dataObj = data1.toObject();
						if (dataObj.contains("name")) {
							QString name = dataObj.value("name").toString();
							qDebug() << "name: " << name;
						}
						if (dataObj.contains("plottings")) {
							QJsonValue plottings = dataObj.value("plottings");
							if (plottings.isArray()) {
								QJsonArray plottingsArray = plottings.toArray();
								QJsonValue json_value = plottingsArray.at(0);
								if (json_value.isObject()) {
									QJsonObject json_obj = json_value.toObject();
									if (json_obj.contains("name")) {
										QString name = json_obj.value("name").toString();
										qDebug() << "element name: " << name;
									}
									if (json_obj.contains("styleInfoJson")) {
										QJsonValue styleInfoJson = json_obj.value("styleInfoJson");
										if (styleInfoJson.isObject()) {
											QJsonObject styleInfoJsonObj = styleInfoJson.toObject();
											if (styleInfoJsonObj.contains("color")) {
												QJsonValue color = styleInfoJsonObj.value("color");
												if (color.isString()) {
													qDebug() << "color: " << color;
												}
											}
										}
										else {
											qDebug() << "styleInfoJson type:" << styleInfoJson.type();
										}
										if (styleInfoJson.isString()) {
											qDebug() << "111";
											QJsonDocument style = QJsonDocument::fromJson(QString(styleInfoJson.toString()).toUtf8());
											if (style.isObject()) {
												QJsonObject styleObj = style.object();
												if (styleObj.contains("fontStyle")) {
													qDebug() << "222 fontStyle type:" << styleObj["fontStyle"].type();
													QJsonObject fontStyle = styleObj["fontStyle"].toObject();
													QJsonObject::const_iterator it;
													for (it = fontStyle.begin(); it != fontStyle.end(); ++it) {
														QString key = it.key();
														QJsonValue value = it.value();
														if (value.isString()) {
															qDebug() << key << ": " << value.toString();
														}
														else if (value.isDouble()) {
															qDebug() << key << ": " << value.toDouble();
														}
														else if (value.isArray()) {
															qDebug() << key << " (array): ";
															QJsonArray jsonArray = value.toArray();
															for (int i = 0; i < jsonArray.size(); ++i) {
																qDebug() << "    - " << jsonArray.at(i).toString();
															}
														}
														else if (value.isObject()) {
															qDebug() << key << " (object): ";
															QJsonObject subObject = value.toObject();
															QJsonObject::const_iterator subIt;
															for (subIt = subObject.begin(); subIt != subObject.end(); ++subIt) {
																qDebug() << "    " << subIt.key() << ": " << subIt.value().toString();
															}
														}
													}
												}
												//fontStyle
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	static void test_parse_json2() {
		//        QString font_style_payloads = R"({"borderColor":"rgba(255,255,255,1)\","loadFlag":true,"x":0,"y":35,"fontSize":60,"fontColor":"rgba(237, 233, 26, 1)\","fontFlag":true})";
		QString styleInfoJson = "{\"color\":\"rgba(255,255,255,1)\",\"bim\":\"\",\"num\":2,\"loadFlag\":false,\"fontStyle\":{\"borderColor\":\"rgba(255,255,255,1)\",\"loadFlag\":true,\"x\":0,\"y\":35,\"fontSize\":60,\"fontColor\":\"rgba(237, 233, 26, 1)\",\"fontFlag\":true},\"radius\":100}";
		QJsonObject styleInfoJsonObj = QJsonDocument::fromJson(styleInfoJson.toUtf8()).object();
		QJsonObject::const_iterator it;
		for (it = styleInfoJsonObj.begin(); it != styleInfoJsonObj.end(); ++it) {
			QString key = it.key();
			QJsonValue value = it.value();
			qDebug() << key << ": " << value;
		}

		if (styleInfoJsonObj.contains("fontStyle")) {
			QJsonObject fontStyleObj = styleInfoJsonObj["fontStyle"].toObject();
			qDebug() << "borderColor:" << fontStyleObj["borderColor"];
			qDebug() << "fontColor:" << fontStyleObj["fontColor"];
			qDebug() << "fontFlag:" << fontStyleObj["fontFlag"];
			qDebug() << "loadFlag:" << fontStyleObj["loadFlag"];
			qDebug() << "x:" << fontStyleObj["x"];
			qDebug() << "y:" << fontStyleObj["y"];
		}
	}

	static void test_parse_layerStyle_json() {
		QString layerStyle = "{\"scale\":0.8}";
		QJsonObject layerStyleObj = QJsonDocument::fromJson(layerStyle.toUtf8()).object();
		QJsonObject::const_iterator it;
		for (it = layerStyleObj.begin(); it != layerStyleObj.end(); ++it) {
			QString key = it.key();
			QJsonValue value = it.value();
			qDebug() << key << ": " << value;
		}
	}

	static void test_plotting_web_json_to_map()
	{
		QString plottingWeb = "{\"selectPath\":true,\"path\":\"健康谷正射\",\"tileIndex\":[[846789,407356],[856152,402089]],\"sceneId\":\"1847168269595754497\",\"sceneName\":\"test\",\"topicCategory\":\"\",\"geojson\":{\"type\":\"Feature\",\"geometry\":{\"type\":\"Polygon\",\"coordinates\":[[[111.45614558807182,40.718542891344214],[111.45614558807182,40.73911269545787],[111.51314153018527,40.73911269545787],[111.51314153018527,40.718542891344214],[111.45614558807182,40.718542891344214]]]},\"properties\":{}},\"savePath\":\"C:/Users/Administrator/Desktop\",\"sceneType\":\"01\",\"layinfo\":{\"title\":{\"text\":\"test郑州二期警务部署图\",\"position\":[],\"borderColor\":\"rgba(0,0,0,1)\",\"fillColor\":\"rgba(255,0,0,1)\",\"fontSize\":28,\"color\":\"rgba(0,0,0,1)\",\"fontBorderColor\":\"\"},\"subTitle\":{\"text\":\"右侧索引标题\",\"color\":\"rgba(0,0,0,1)\",\"fontSize\":16},\"remark\":[{\"text\":\"指挥: 这里填写指挥信息\",\"position\":[0,0,28,10],\"borderColor\":\"\",\"fillColor\":\"rgba(0,151,233,1)\",\"fontSize\":18,\"color\":\"rgba(0,0,0,1)\",\"url\":\"C:/security2.0/ToDWG/tmp/0.png\"},{\"text\":\"备注: 这里填写备注信息\",\"position\":[0,90,28,10],\"borderColor\":\"\",\"fillColor\":\"rgba(0,51,133,1)\",\"fontSize\":18,\"color\":\"rgba(0,0,0,1)\",\"url\":\"C:/security2.0/ToDWG/tmp/0.png\"}],\"north\":{\"position\":[97,0,2,8],\"rotate\":30},\"arrows\":[{\"position\":[80,80,10,2],\"rotate\":30}],\"scaleBar\":true},\"paper\":\"a3\",\"pictureUnit\":\"制图单位：xxx 制\",\"mapType\":{\"map\":true,\"electron\":false}}";
		QJsonObject layerStyleObj = QJsonDocument::fromJson(plottingWeb.toUtf8()).object();
		// QJsonObject::const_iterator it;
		// for (it = layerStyleObj.begin(); it != layerStyleObj.end(); ++it) {
		// 	QString key = it.key();
		// 	QJsonValue value = it.value();
		// 	qDebug() << key << ": " << value;
		// }
		QVariantMap variants = JsonUtil::jsonObjectToVariantMap(layerStyleObj);
		QVariantMap::const_iterator it;
		for (it = variants.begin(); it != variants.end(); ++it) {
			QString key = it.key();
			QVariant value = it.value();
			qDebug() << key << ": " << value;
		}


	}


};

QTEST_MAIN(TestJson)
#include "test_json.moc"