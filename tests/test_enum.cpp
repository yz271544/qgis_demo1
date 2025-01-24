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
	static void test_paper_specification_list()
	{
		auto layoutPaperList = PaperSpecification::getLayoutPaperList();
		for (const auto& paper : layoutPaperList) {
			qDebug() << "Paper:" << paper.getPaperName() << ", Size:"
					 << paper.getPaperSize().first << "x" << paper.getPaperSize().second;
		}
	}

	static void test_paper_info()
	{
		PaperSpecification::Type a3 = PaperSpecification::Type::A3;
		PaperSpecification paperSpec(a3);

		QString paperName = paperSpec.getPaperName();
		QPair<int, int> paperSize = paperSpec.getPaperSize();

		qDebug() << "Paper Name:" << paperName;
		qDebug() << "Paper Size:" << paperSize.first << "x" << paperSize.second;
	}

	static void test_paper_from_string()
	{
		PaperSpecification* a3 = new PaperSpecification("A3");

		QString paperName = a3->getPaperName();
		QPair<int, int> paperSize = a3->getPaperSize();

		qDebug() << "Paper Name:" << paperName;
		qDebug() << "Paper Size:" << paperSize.first << "x" << paperSize.second;
	}

};

QTEST_MAIN(TestEnum)
#include "test_enum.moc"
