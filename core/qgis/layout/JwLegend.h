//
// Created by Lyndon on 2025/1/7.
//

#ifndef JWLEGEND_H
#define JWLEGEND_H
#include <QgsLayoutItemLegend.h>
#include <QgsVectorLayer.h>
#include <qgsmaplayerlegend.h>
#include <qgssymbollayer.h>
#include <qgsmarkersymbol.h>
#include <qgssymbol.h>
#include <qgsproject.h>
#include <QPair>

#include <../../../config.h>
#include <../../../core/utils/FontUtil.h>

class JwLegend {
public:

    JwLegend(QString& legendTitle, QgsProject* project) : legendTitle(legendTitle), project(project) {};

    ~JwLegend() = default;

    QPair<double, double> customize(
        QgsLayoutItemLegend* legend,
        QMap<QString, QVariant> imageSpec,
        double legendWidth,
        int32_t legendHeight,
        const QSet<QString>& filteredLegendItems);

    static QString get_type(QgsVectorLayer* layer);
private:
    QString legendTitle;
    QgsProject* project;
};



#endif //JWLEGEND_H
