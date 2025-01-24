//
// Created by Lyndon on 2025/1/7.
//

#include "NodeToMap.h"



// 辅助函数：将 YAML 的 Scalar 转换为 QVariant
QVariant NodeToMap::scalarToVariant(const YAML::Node& node) {
    if (node.IsScalar()) {
        try {
            // 对于不同的 Scalar 类型，尝试不同的转换
            // 首先尝试转换为 bool
            try {
                return QVariant(node.as<bool>());
            } catch (const YAML::BadConversion&) {
                // 转换为 bool 失败，继续尝试下一个类型
            }
            try {
                return QVariant(node.as<int>());
            } catch (const YAML::BadConversion&) {
                // 转换为 int 失败，继续尝试下一个类型
            }
            try {
                return QVariant(node.as<double>());
            } catch (const YAML::BadConversion&) {
                // 转换为 double 失败，继续尝试下一个类型
            }
            try {
                return QVariant(QString::fromStdString(node.as<std::string>()));
            } catch (const YAML::BadConversion&) {
                // 转换为 string 失败，返回一个空的 QVariant
                return QVariant();
            }
        } catch (const YAML::Exception& e) {
            qCritical() << "Error converting scalar: " << e.what();
            throw;
        }
    }
    return QVariant();
}


// 辅助函数：将 YAML 的 Sequence 转换为 QVariantList
QVariantList NodeToMap::sequenceToVariantList(const YAML::Node& node) {
    QVariantList result;
    for (const auto& subNode : node) {
        result.append(nodeToVariant(subNode));
    }
    return result;
}


// 辅助函数：将 YAML 的 Map 转换为 QVariantMap
QVariantMap NodeToMap::mapToVariantMap(const YAML::Node& node) {
    QVariantMap result;
    for (const auto& it : node) {
        result[QString::fromStdString(it.first.as<std::string>())] = nodeToVariant(it.second);
    }
    return result;
}


// 主要的转换函数，将 YAML 的 Node 转换为 QVariant
QVariant NodeToMap::nodeToVariant(const YAML::Node& node) {
    if (node.IsScalar()) {
        return scalarToVariant(node);
    } else if (node.IsSequence()) {
        return QVariant(sequenceToVariantList(node));
    } else if (node.IsMap()) {
        return QVariant(mapToVariantMap(node));
    }
    return QVariant();
}
