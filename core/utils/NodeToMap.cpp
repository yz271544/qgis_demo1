//
// Created by Lyndon on 2025/1/7.
//

#include "NodeToMap.h"


QVariantMap* NodeToMap::YamlNodeToMap(YAML::Node config)
{
    // 将 YAML::Node 转换为 QVariantMap
    QVariantMap* nodeMap = new QVariantMap();
    for (YAML::const_iterator nodeIt = config.begin(); nodeIt != config.end(); ++nodeIt)
    {
        QString key = QString(nodeIt->first.as<std::string>().c_str());
        QVariant value;

        // 根据 YAML 节点的类型转换为 QVariant
        if (nodeIt->second.IsScalar())
        {
            value = QString(nodeIt->second.as<std::string>().c_str());
        }
        else if (nodeIt->second.IsSequence())
        {
            QVariantList list;
            for (const auto& item : nodeIt->second)
            {
                list.append(QString(item.as<std::string>().c_str()));
            }
            value = list;
        }
        else if (nodeIt->second.IsMap())
        {
            QVariantMap map;
            for (YAML::const_iterator subIt = nodeIt->second.begin(); subIt != nodeIt->second.end(); ++subIt)
            {
                map[QString(subIt->first.as<std::string>().c_str())] = QString(
                    subIt->second.as<std::string>().c_str());
            }
            value = map;
        }

        nodeMap[key] = value;
    }
    return nodeMap;
}
