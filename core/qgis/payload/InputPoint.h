//
// Created by etl on 12/27/24.
//

#ifndef INPUTPOINT_H
#define INPUTPOINT_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#if defined(_WIN32)
#include <windows.h>
#endif
#include <qgspoint.h>
#include <QString>


class InputPoint {
private:
	QString name;
	QgsPoint point;

public:
	InputPoint();

	~InputPoint();

	InputPoint(const QString& name, const QgsPoint& point);

	QString getName();

	QgsPoint getPoint();

	void setName(const QString& name);

	void setPoint(const QgsPoint& point);

};



#endif //INPUTPOINT_H
