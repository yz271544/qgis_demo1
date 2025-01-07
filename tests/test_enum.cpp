//
// Created by Lyndon on 2025/1/7.
//

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
#include "../core/enums/PaperSpecification.h"

#if defined(_WIN32)
#include <windows.h>
#endif

class TestEnum : public QObject
{
	Q_OBJECT
private slots:
	static void test_PaperSpecification()
	{
		auto layoutPaperList = PaperSpecification::getLayoutPaperList();
		for (const auto& paper : layoutPaperList) {
			qDebug() << "Paper:" << paper.getPaperName() << ", Size:"
					 << paper.getPaperSize().first << "x" << paper.getPaperSize().second;
		}
	}

};

QTEST_MAIN(TestEnum)
#include "test_enum.moc"
