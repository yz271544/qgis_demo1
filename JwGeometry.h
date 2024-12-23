//
// Created by etl on 24-12-21.
//

#ifndef JWGEOMETRY_H
#define JWGEOMETRY_H

#include <iostream>
#include <vector>
#include <math.h>
#include <qgis_core.h>
#include <qgscoordinatetransform.h>
#include <qgsgeometry.h>
#include <qgspoint.h>

class QgsCoordinateTransform;
// 定义命名空间，方便使用QGIS相关类，可根据实际情况调整

class JwGeometry {
public:
    // 对应Python中的transform_point函数
    QgsPoint* transform_point(const std::vector<double>& point, const QgsCoordinateTransform& transformer) {
        auto transformed_point = transformer.transform(QgsPointXY(point[0], point[1]));
        return new QgsPoint(transformed_point.x(), transformed_point.y(), point[2]);
    }

    // 对应Python中的transform_polygon函数
    QgsGeometry transform_polygon(const std::vector<std::vector<QgsPoint>>& transformed_polygon) {
        std::string wkt_polygon = "POLYGON((";
        for (const auto& e : transformed_polygon) {
            for (size_t index = 0; index < e.size(); ++index) {
                const QgsPoint& point = e[index];
                wkt_polygon += std::to_string(point.x()) + " " + std::to_string(point.y()) + " " + std::to_string(point.z());
                if (index < e.size() - 1) {
                    wkt_polygon += ", ";
                }
            }
            wkt_polygon += std::to_string(e[0].x()) + " " + std::to_string(e[0].y()) + " " + std::to_string(e[0].z());
            wkt_polygon += ")";
        }
        wkt_polygon += ")";
        return QgsGeometry::fromWkt(wkt_polygon.c_str());
    }

    // 对应Python中的paint_circle_geometry_3d函数
    QgsGeometry paint_circle_geometry_3d(int num_segments, const QgsPoint& center_transformed, double radius) {
        std::vector<QgsPoint> points;
        for (int i = 0; i < num_segments; ++i) {
            double angle = 2 * M_PI * i / num_segments;
            double x = center_transformed.x() + radius * cos(angle);
            double y = center_transformed.y() + radius * sin(angle);
            double z = center_transformed.z();
            points.push_back(QgsPoint(x, y, z));
        }
        points.push_back(points[0]);  // 闭合多边形

        std::string wkt_polygon = "POLYGON((";
        for (size_t index = 0; index < points.size(); ++index) {
            const QgsPoint& point = points[index];
            wkt_polygon += std::to_string(point.x()) + " " + std::to_string(point.y()) + " " + std::to_string(point.z());
            if (index < points.size() - 1) {
                wkt_polygon += ", ";
            }
        }
        wkt_polygon += "))";

        return QgsGeometry::fromWkt(wkt_polygon.c_str());
    }

    // 对应Python中的paint_circle_geometry_2d函数
    QgsGeometry paint_circle_geometry_2d(int num_segments, const QgsPoint& center_transformed, double radius) {
        QVector<QgsPointXY> points;
        // std::vector<QgsPointXY> points;
        for (int i = 0; i < num_segments; ++i) {
            double angle = 2 * M_PI * i / num_segments;
            double x = center_transformed.x() + radius * cos(angle);
            double y = center_transformed.y() + radius * sin(angle);
            points.push_back(QgsPointXY(x, y));
        }
        points.push_back(points[0]);  // 闭合多边形

        QVector<QgsPolylineXY> polyline;
        polyline.push_back(points);
        return QgsGeometry::fromPolygonXY(polyline);
    }
};



#endif //JWGEOMETRY_H
