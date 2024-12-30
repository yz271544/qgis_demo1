//
// Created by Lyndon on 2024/12/12.
//
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#if defined(_WIN32)
#include <windows.h>
#endif
#include <fstream>
#include <vector>
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include "qgspluginlayerregistry.h"

#include <complex>
#include <execution>
#include <QCoreApplication>
#include <QTextCodec>
#include <QUrl>
#include <QUrlQuery>
#include <QFileInfo>
#include <qgis.h>
#include <qgstiledscenelayer.h>
#include <qgstiledscenelayer3drenderer.h>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>
#include <qgsgeometry.h>
#include <qgslayertree.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsmapcanvas.h>
#include <qgsprojectviewsettings.h>
#include <qgsvectorfilewriter.h>
#include <qgsprovidermetadata.h>
#include <qgs3d.h>
#include <qgs3dmapcanvas.h>
#include <qgswkbtypes.h>
#include <qgslayoutmanager.h>
#include "config.h"

#include <QDebug>

QgsPointXY validateAndTransformPoint(const QgsPointXY& point, const QgsCoordinateTransform& transformer) {
	double lat = point.y();
	if (lat > 85.06) {
		lat = 85.06;
	}
	else if (lat < -85.06) {
		lat = -85.06;
	}

	QgsPointXY validPoint(point.x(), lat);
	return transformer.transform(validPoint);
}

int main(int argc, char* argv[]) {
	// Initialize QGIS application
	QgsApplication app(argc, argv, true);
	QgsApplication::initQgis();

	// Define source and target CRS
	QgsCoordinateReferenceSystem crs_4326("EPSG:4326");
	QgsCoordinateReferenceSystem crs_3857("EPSG:3857");

	// Create a coordinate transform object
	QgsCoordinateTransform transformer(crs_4326, crs_3857, QgsProject::instance());

	// Example point with invalid latitude
	// QgsPointXY point(12410132.385677, 4971751.968104);
	QgsPointXY point(111.482116, 40.724241);
	// Validate and transform the point
	QgsPointXY transformedPoint = validateAndTransformPoint(point, transformer);

	qDebug() << "Transformed point:" << transformedPoint.x() << transformedPoint.y();

	// Clean up
	QgsApplication::exitQgis();
	return 0;
}