//
// Created by Lyndon on 2025/1/8.
//

#ifndef JSONTOMAP_H
#define JSONTOMAP_H
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>

class JsonUtil {
public:
    static QVariantMap jsonObjectToVariantMap(const QJsonObject& jsonObject);
};



#endif //JSONTOMAP_H
