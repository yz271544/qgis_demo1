//
// Created by etl on 12/27/24.
//

#include "InputPoint.h"

InputPoint::InputPoint() = default;

InputPoint::~InputPoint() = default;

InputPoint::InputPoint(const QString &name, const QgsPoint &point) {
    this->name = name;
    this->point = point;
}

QString InputPoint::getName() {
    return this->name;
}

QgsPoint InputPoint::getPoint() {
    return this->point;
}

void InputPoint::setName(const QString &name) {
    this->name = name;
}

void InputPoint::setPoint(const QgsPoint &point) {
    this->point = point;
}

