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

class JwGeometry
{
public:
	// 对应Python中的transform_point函数
	QgsPoint* transform_point(const std::vector<double>& point, const QgsCoordinateTransform& transformer);

	// 对应Python中的transform_polygon函数
	QgsGeometry transform_polygon(const std::vector<std::vector<QgsPoint>>& transformed_polygon);

	// 对应Python中的paint_circle_geometry_3d函数
	QgsGeometry paint_circle_geometry_3d(int num_segments, const QgsPoint& center_transformed, double radius);

	// 对应Python中的paint_circle_geometry_2d函数
	QgsGeometry paint_circle_geometry_2d(int num_segments, const QgsPoint& center_transformed, double radius);
};
#endif //JWGEOMETRY_H
