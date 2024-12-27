#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#if defined(_WIN32)
#include <windows.h>
#endif
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QString>
#include <QtTest>

class TestJson : public QObject
{
	Q_OBJECT
private slots:
	void testCase1()
	{
		QVERIFY(2 + 2 == 4);
	}

	void testCase2() {
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

};

QTEST_MAIN(TestJson)
#include "test_json.moc"