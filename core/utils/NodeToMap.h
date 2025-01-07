//
// Created by Lyndon on 2025/1/7.
//

#ifndef NODETOMAP_H
#define NODETOMAP_H
#include <QVariantMap>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

class NodeToMap {
public:
    static QVariantMap* YamlNodeToMap(YAML::Node config);
};



#endif //NODETOMAP_H
