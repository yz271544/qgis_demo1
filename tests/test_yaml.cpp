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
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#if defined(_WIN32)
#include <windows.h>
#endif

class TestYaml : public QObject
{
	Q_OBJECT
private slots:
	static void test_read_name()
	{
#if defined(_WIN32)
		YAML::Node config = YAML::LoadFile("D:/iProject/cpath/qgis_demo1/config/settings.yaml");
#elif defined(__linux__)
        YAML::Node config = YAML::LoadFile("/lyndon/iProject/cpath/qgis_demo1/config/settings.yaml");
#endif

		if (config["specification"]) {
			std::cout << "specification in: " << config["specification"][0]["name"] << "\n";
			qDebug() << "name in: " << QString(config["specification"][0]["name"].as<std::string>().c_str());
			qDebug() << "local in: " << QString(config["specification"][0]["local"].as<std::string>().c_str());
			qDebug() << "title_font_color in: " << QString(config["specification"][0]["title_font_color"].as<std::string>().c_str());
			qDebug() << "title_font_size in: " << config["specification"][0]["title_font_size"].as<int>();
			qDebug() << "title_is_bold in: " << config["specification"][0]["title_is_bold"].as<bool>();
			qDebug() << "title_letter_spacing in: " << config["specification"][0]["title_letter_spacing"].as<double>();
		}
	}

	static void test_loop_specification()
	{
#if defined(_WIN32)
        YAML::Node config = YAML::LoadFile("D:/iProject/cpath/qgis_demo1/config/settings.yaml");
#elif defined(__linux__)
        YAML::Node config = YAML::LoadFile("/lyndon/iProject/cpath/qgis_demo1/config/settings.yaml");
#endif

		if (config["specification"])
		{
			for (YAML::const_iterator it = config["specification"].begin(); it != config["specification"].end(); ++it)
			{
				std::cout << "name: " << (*it)["name"] << "\n";
				qDebug() << "name: " << QString((*it)["name"].as<std::string>().c_str());
				qDebug() << "local: " << QString((*it)["local"].as<std::string>().c_str());
				qDebug() << "title_font_color: " << QString((*it)["title_font_color"].as<std::string>().c_str());
				qDebug() << "title_font_size: " << (*it)["title_font_size"].as<int>();
				qDebug() << "title_is_bold: " << (*it)["title_is_bold"].as<bool>();
				qDebug() << "title_letter_spacing: " << (*it)["title_letter_spacing"].as<double>();
			}
		}
	}

	static void test_shared_ptr_specification()
	{
#if defined(_WIN32)
        YAML::Node config = YAML::LoadFile("D:/iProject/cpath/qgis_demo1/config/settings.yaml");
#elif defined(__linux__)
        YAML::Node config = YAML::LoadFile("/lyndon/iProject/cpath/qgis_demo1/config/settings.yaml");
#endif

		std::shared_ptr<YAML::Node> config_ptr = std::make_shared<YAML::Node>(config);

		if ((*config_ptr)["specification"])
		{
			for (YAML::const_iterator it = (*config_ptr)["specification"].begin(); it != (*config_ptr)["specification"].end(); ++it)
			{
				std::cout << "name: " << (*it)["name"] << "\n";
				qDebug() << "name: " << QString((*it)["name"].as<std::string>().c_str());
				qDebug() << "local: " << QString((*it)["local"].as<std::string>().c_str());
				qDebug() << "title_font_color: " << QString((*it)["title_font_color"].as<std::string>().c_str());
				qDebug() << "title_font_size: " << (*it)["title_font_size"].as<int>();
				qDebug() << "title_is_bold: " << (*it)["title_is_bold"].as<bool>();
				qDebug() << "title_letter_spacing: " << (*it)["title_letter_spacing"].as<double>();
			}
		}


	}


};

QTEST_MAIN(TestYaml)
#include "test_yaml.moc"