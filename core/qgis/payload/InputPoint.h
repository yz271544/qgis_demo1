//
// Created by etl on 12/27/24.
//

#ifndef INPUTPOINT_H
#define INPUTPOINT_H
#include <qgspoint.h>
#include <QString>


class InputPoint {
private:
    QString name;
    QgsPoint point;

public:
    InputPoint();

    ~InputPoint();

    InputPoint(const QString &name, const QgsPoint &point);

    QString getName();

    QgsPoint getPoint();

    void setName(const QString &name);

    void setPoint(const QgsPoint &point);

};



#endif //INPUTPOINT_H
