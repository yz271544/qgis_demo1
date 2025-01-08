//
// Created by Lyndon on 2025/1/8.
//

#include "JsonUtil.h"


QVariantMap JsonUtil::jsonObjectToVariantMap(const QJsonObject& jsonObject) {
    QVariantMap variantMap;
    for (const QString& key : jsonObject.keys()) {
        QJsonValue value = jsonObject.value(key);
        if (value.isBool()) {
            variantMap[key] = value.toBool();
        } else if (value.isDouble()) {
            variantMap[key] = value.toDouble();
        } else if (value.isString()) {
            variantMap[key] = value.toString();
        } else if (value.isArray()) {
            QJsonArray array = value.toArray();
            QVariantList variantList;
            for (const QJsonValue& arrayValue : array) {
                if (arrayValue.isBool()) {
                    variantList.append(arrayValue.toBool());
                } else if (arrayValue.isDouble()) {
                    variantList.append(arrayValue.toDouble());
                } else if (arrayValue.isString()) {
                    variantList.append(arrayValue.toString());
                }
            }
            variantMap[key] = variantList;
        } else if (value.isObject()) {
            variantMap[key] = jsonObjectToVariantMap(value.toObject());
        }
    }
    return variantMap;
}
