//
// Created by Lyndon on 2025/1/7.
//

#ifndef FONTUTIL_H
#define FONTUTIL_H
#include <QtMath>
#include <QString>

#include "../../config.h"

class FontUtil {
public:
    static int getTextFontWidth(const QString& data, int fontSize, float letterSpacing);

    static int getSingleTextSize(float fontSize);
};



#endif //FONTUTIL_H
