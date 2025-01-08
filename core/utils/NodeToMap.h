//
// Created by Lyndon on 2025/1/7.
//

#ifndef NODETOMAP_H
#define NODETOMAP_H
#include <QVariantMap>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <qdebug.h>

class NodeToMap {
public:
    static QVariantMap mapToVariantMap(const YAML::Node& config);

    static QVariant nodeToVariant(const YAML::Node& node);

    static QVariantList sequenceToVariantList(const YAML::Node& node);

    static QVariant scalarToVariant(const YAML::Node& node);
};



#endif //NODETOMAP_H
