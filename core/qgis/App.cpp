//
// Created by Lyndon on 2025/1/7.
//

#include "App.h"

App::App(): qgis(qgis), pageSizeRegistry(pageSizeRegistry)
{
    project = new QgsProject();
    canvas = new QgsMapCanvas();
    mapSettings = new QgsMapSettings();
    projectDir = "";
    transformContext = new QgsCoordinateTransformContext();
    available_papers = new QList<PaperSpecification>();
}

App::~App() {
    delete project;
    delete canvas;
    delete mapSettings;
    delete transformContext;
    delete available_papers;
}
